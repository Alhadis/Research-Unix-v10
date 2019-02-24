/*
 * file service routines for FILES-11 ODS-1
 * in its modern instance, with subdirectories
 */

#include <rf.h>
#include "files11.h"
#include "f11.h"

#define	NULL	0
#define	HUGE	0x7fffffff

int diskfd;
struct homeblock home;
Fsfile indexf;
unsigned short dirtyp;

static htorf();
static char *itoa();
static int binchk();
char *r50toa();
char *malloc();

/*
 * permission hacks
 */
#define	UXOTHER	1
static Idmap gmap[] = { "other", UXOTHER, 0 };

/*
 * init:
 * install the root
 *
 * argv[1] == the file containing our filesystem
 */
Rfile *
fsinit(argc, argv)
int argc;
char **argv;
{
	register Rfile *f;
	register long firsthdr;
	char *p;

	p = "dir";
	dirtyp = ator50(&p);
	if (argc <= 1)
		rfpanic("no files-11 filesystem specified\n");
	if ((diskfd = open(argv[1], 0)) < 0)	/* readonly for now */
		rfpanic("%s: cannot open\n", argv[1]);
	if (lseek(diskfd, (long)HOMEBLK * BLKSIZ, 0) < 0)
		rfpanic("can't seek for home block\n");
	if (read(diskfd, (char *)&home, sizeof(home)) != sizeof(home))
		rfpanic("can't read homeblock\n");
	if ((f = (Rfile *)malloc(sizeof(Rfile))) == NULL
	||  (f->fs = malloc(sizeof(Fsfile))) == NULL)
		rfpanic("no mem for root\n");
	firsthdr = plong(home.H_iblb)+home.H_ibsz;
	lseek(diskfd, (firsthdr + fidtob(FINDEX)) * BLKSIZ, 0);
	if (read(diskfd, (char *)&indexf.h, sizeof(indexf.h)) != sizeof(indexf.h))
		rfpanic("can't read index header\n");
	indexf.lbase = HUGE;
	if (gethdr(FROOT, &fsp(f)->h) == 0)
		rfpanic("can't read root header\n");
	htorf(f, &fsp(f)->h);
	fsp(f)->parent = FROOT;
	fsp(f)->lbase = HUGE;
	fsp(f)->flags = FBIN;
	fsp(f)->tboff = fsp(f)->tbno = fsp(f)->tuoff = 0;
	rfgidmap = gmap;
	return (f);
}

/*
 * access a file
 */
Rfile *
fswalk(df, name)
Rfile *df;
char *name;
{
	register Rfile *f;
	unsigned short fid;
	int binary;

	binary = binchk(name);
	if (strcmp(name, ".") == 0)
		return (df);
	else if (strcmp(name, "..") == 0) {
		if (df->ino == FROOT) {
			fserrno = 0;		/* magic: popped out */
			return (NULL);
		}
		fid = fsp(df)->parent;
	} else if ((fid = f11walk(df, name)) == 0)
		return (NULL);
	if ((f = (Rfile *)malloc(sizeof(Rfile))) == NULL) {
		rflog("no mem\n");
		fserrno = RFEINVAL;
		return (NULL);
	}
	if ((f->fs = malloc(sizeof(Fsfile))) == NULL) {
		free((char *)f);
		rflog("no mem\n");
		fserrno = RFEINVAL;
		return (NULL);
	}
	fsp(f)->lbase = HUGE;
	if (gethdr(fid, &fsp(f)->h) == 0) {
		free((char *)f);
		fserrno = RFEIO;
		return (NULL);
	}
	if (fsp(f)->h.h_ident.i_fnam.f_typ == dirtyp)
		fsp(f)->parent = fsp(df)->h.h_fnum;
	htorf(f, &fsp(f)->h);
	fsp(f)->tboff = fsp(f)->tbno = fsp(f)->tuoff = 0;
	if (binary)
		fsp(f)->flags = FBIN;
	else if (fsp(f)->h.h_fcs.f_rtyp == RTVAR && fsp(f)->h.h_fcs.f_ratt & RTCR)
		fsp(f)->flags = 0;
	else
		fsp(f)->flags = FBIN;
	return (f);
}

/*
 * see if filename is `binary': trailing % in name
 * stamp it out if so
 */
static
binchk(name)
register char *name;
{
	name += strlen(name);
	if (*--name == '%') {
		*name = 0;
		return (1);
	}
	return (0);
}

/*
 * discard a file reference
 */
int
fsdone(f)
Rfile *f;
{

	free(f->fs);
	free((char *)f);
	return (0);
}

/*
 * return file status
 */
fsstat(f)
register Rfile *f;
{

	if (gethdr(fsp(f)->h.h_fnum, &fsp(f)->h) == 0) {
		fserrno = RFEINVAL;
		return (-1);
	}
	htorf(f, &fsp(f)->h);
	return (0);
}

/*
 * read
 */
int
fsread(f, off, buf, len)
Rfile *f;
long off;
char *buf;
int len;
{

	if (fsp(f)->flags & FBIN)
		return (binread(f, off, buf, len));
	else
		return (textread(f, off, buf, len));
}

int
binread(f, off, buf, len)
Rfile *f;
long off;
char *buf;
int len;
{
	char bbuf[BLKSIZ];
	int boff;
	long bno;
	int nread, r;
	int ncpy;

	nread = 0;
	bno = off / BLKSIZ;
	boff = off % BLKSIZ;
	/*
	 * contract this loop later
	 */
	r = 0;
	while (len > 0) {
		if ((r = f11rblk(f, bno, bbuf)) <= 0)
			break;
		ncpy = BLKSIZ - boff;
		ncpy = (len > ncpy) ? ncpy : len;
		memcpy(buf, bbuf+boff, ncpy);
		len -= ncpy;
		buf += ncpy;
		nread += ncpy;
		boff = 0;
		bno++;
	}
	if (nread || r >= 0)
		return (nread);
	fserrno = RFEIO;
	return (-1);
}

int
textread(f, off, buf, len)
Rfile *f;
long off;
char *buf;
int len;
{
	unsigned char bbuf[BLKSIZ];
	int boff;
	long bno;
	int nread, r;
	int ncpy;
	int rlen;
	unsigned char *p;

	nread = 0;
	if (off == fsp(f)->tuoff) {
		bno = fsp(f)->tbno;
		boff = fsp(f)->tboff;
	} else if (off == 0) {
		bno = 0;
		boff = 0;
	} else {				/* wrong, but so what? */
		bno = off / BLKSIZ;
		boff = off % BLKSIZ;
	}
	if ((r = f11rblk(f, bno, bbuf)) <= 0)
		goto out;
	p = bbuf + boff;
	while (len > 0) {
		if (boff & 01) {
			p++;
			boff++;
		}
		if ((bno * BLKSIZ) + boff >= f->size)
			break;
		rlen = p[0] + (p[1]<<8);
		if (rlen == -1) {	/* skip to next block */
			if ((r = f11rblk(f, ++bno, bbuf)) <= 0)
				goto out;
			p = bbuf;
			boff = 0;
			continue;
		}
		if (rlen + 1 > len)	/* +1 for newline */
			/* set a particular error here? */
			break;
		p += 2;
		for (; rlen > 0; rlen -= ncpy) {
			ncpy = &bbuf[BLKSIZ] - p;
			if (ncpy > rlen)
				ncpy = rlen;
			memcpy(buf, p, ncpy);
			p += ncpy;
			boff = p - bbuf;
			nread += ncpy;
			buf += ncpy;
			len -= ncpy;
			if (ncpy < rlen) {	/* next block */
				if ((r = f11rblk(f, ++bno, bbuf)) <= 0)
					goto out;
				p = bbuf;
				boff = 0;
			}
		}
		*buf++ = '\n';
		nread++;
		len--;
		if (boff > BLKSIZ - 2) {	/* about to need new block */
			if ((r = f11rblk(f, ++bno, bbuf)) <= 0)
				goto out;
			p = bbuf;
			boff = 0;
		}
	}
out:
	if (boff == BLKSIZ) {
		boff = 0;
		bno++;
	}
	if (nread) {
		fsp(f)->tuoff = off + nread;
		fsp(f)->tbno = bno;
		fsp(f)->tboff = boff;
	}
	if (nread || r >= 0)
		return (nread);
	fserrno = RFEIO;
	return (-1);
}

/*
 * read directory --
 * return ascii records: decimal `i-number', tab, filename, NUL
 * filename is filname.typ.ver
 * *offp gets new file offset for directory
 */
#define	DENTSIZE	(6+9+4+6+1)	/* max len of a directory record */

int
fsdirread(f, off, buf, len, offp)
Rfile *f;
long off;
char *buf;
int len;
long *offp;
{
	char bbuf[BLKSIZ];
	register char *p;
	register struct directory *dp;
	struct directory *dend;
	char *op;
	int r;
	long bno;
	int boff;

	bno = off / BLKSIZ;
	if ((r = f11rblk(f, bno, bbuf)) <= 0) {
		fserrno = RFEIO;
		return (r);
	}
	boff = off % BLKSIZ;
	dp = (struct directory *)bbuf + (boff/sizeof(struct directory));
	dend = (struct directory *)&bbuf[BLKSIZ];
	p = buf;
	while (len >= DENTSIZE) {
		while (dp < dend && dp->d_fid.f_num == 0)
			dp++;
		if (dp >= dend) {
			bno++;
			dp = (struct directory *)bbuf;
			if ((r = f11rblk(f, bno, bbuf)) <= 0)
				break;
		}
		op = p;
		p = itoa(p, dp->d_fid.f_num);
		*p++ = '\t';
		p = r50toa(p, dp->d_fname.f_nam[0]);
		p = r50toa(p, dp->d_fname.f_nam[1]);
		p = r50toa(p, dp->d_fname.f_nam[2]);
		while (p[-1] == ' ')
			--p;
		*p++ = '.';
		p = r50toa(p, dp->d_fname.f_typ);
		while (p[-1] == ' ')
			--p;
		*p++ = '.';
		p = itoa(p, dp->d_fname.f_ver);
		*p++ = 0;
		len -= p - op;
		dp++;
	}
	if (p == buf && r < 0) {
		fserrno = RFEIO;
		return (-1);
	}
	*offp = (bno * BLKSIZ) + (char *)dp - bbuf;
	return (p - buf);
}

/*
 * f11 to rf file attributes
 * -- temporary permission hack:
 * turn apparent f11 other to asserted unix other group
 */
#define	F11OTHER (-1)

static
htorf(f, h)
register Rfile *f;
register struct header *h;
{
	f->ino = h->h_fnum;
	f->dev = 0;
	f->mode = htouperm(h->h_fpro);
	if (h->h_ident.i_fnam.f_typ != dirtyp)
		f->type = RFTREG;
	else {
		f->mode |= (f->mode & 0444)>>2;	/* copy `read' into `execute' */
		f->type = RFTDIR;
	}
	f->nlink = 1;
	f->uid = h->h_fown.u_prog;
	f->gid = h->h_fown.u_proj;
	if (f->gid == F11OTHER)
		f->gid = UXOTHER;
	f->rdev = 0;
	f->size = hfilesize(h);
	f->ta = 0;
	f->tm = htoutime(h->h_ident.i_rvdt);
	f->tc = htoutime(h->h_ident.i_crdt);	/* wrong, but let it stand */
	if (f->tm == 0)
		f->tm = f->tc;
}

/*
 * number to string
 */

static char digits[] = "0123456789";

static char *
itoa(s, n)
register char *s;
register unsigned int n;
{
	register int d;

	if ((d = n / 10) == 0)
		*s++ = digits[n];
	else {
		s = itoa(s, d);
		*s++ = digits[n % 10];
	}
	return (s);
}
