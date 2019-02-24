/*
 * files-11 subroutines,
 * mostly concerned directly with
 * filesystem structures
 */

#include <time.h>
#include <ctype.h>
#include <rf.h>
#include "files11.h"
#include "f11.h"

#define	NULL	0

/*
 * convert files-11 ascii time to unix binary time
 * 13 bytes: DDMMMYYhhmmss
 */

static char *months[] = {
	"JAN", "FEB", "MAR", "APR", "MAY", "JUN",
	"JUL", "AUG", "SEP", "OCT", "NOV", "DEC", 0
};

static int i2val();

long
htoutime(ht)
register char *ht;
{
	struct tm tm;
	register int mon;

	if (*ht == 0)
		return (0);
	tm.tm_yday = 0;
	tm.tm_mday = i2val(ht);
	ht += 2;
	for (mon = 0; months[mon]; mon++)
		if (strncmp(ht, months[mon], 3) == 0)
			break;
	tm.tm_mon = mon;
	ht += 3;
	tm.tm_year = i2val(ht);
	ht += 2;
	tm.tm_hour = i2val(ht);
	ht += 2;
	tm.tm_min = i2val(ht);
	ht += 2;
	tm.tm_sec = i2val(ht);
	return (timelocal(&tm, (char *)NULL));
}

static int
i2val(s)
register char *s;
{
	if (!isdigit(s[0]) || !isdigit(s[1]))
		return (0);
	return ((s[0]-'0') * 10 + (s[1]-'0'));
}

/*
 * convert f11 protection to unix permissions
 * information is lost, most importantly the delete flag
 */
int
htouperm(hp)
register int hp;
{
	register int up;
	register int i;

	hp >>= 4;		/* discard `system' field */
	up = 0;
	for (i = 0; i < 3; i++) {	/* and do owner, group, world */
		up <<= 3;
		if ((hp & 1) == 0)	/* read */
			up |= 4;
		if ((hp & 2) == 0)	/* write */
			up |= 2;
		if ((hp & 4) == 0)	/* execute */
			up |= 1;
		hp >>= 4;
	}
	return (up);
}

/*
 * compute the file size
 * use the FCS attributes if they look valid,
 * else just count the blocks
 */

long
hfilesize(h)
register struct header *h;
{
	register int i;
	register long sz;
	register Bmap *bp;

	if (h->h_fcs.f_rtyp != 0) {
		sz = plong(h->h_fcs.f_efbk) - 1;
		sz *= BLKSIZ;
		sz += h->h_fcs.f_ffby;
		return (sz);
	}
	for (sz = 0, i = 0; bp = getmap(&h->h_map, i); i++)
		sz += bp->count;
	return (sz * BLKSIZ);
}

/*
 * search a directory
 * return the file id
 */

int
f11walk(df, name)
Rfile *df;
char *name;
{
	struct filnam fn;
	register struct directory *dp;
	register struct filnam *fp;
	register unsigned short bestfid, bestver;
	char buf[BLKSIZ];
	long bno;

	if (utohname(name, &fn) == 0) {
		fserrno = RFEINVAL;
		return (0);
	}
	bestfid = 0;
	bestver = 0;
	fp = &fn;
	for (bno = 0; f11rblk(df, bno, buf) > 0; bno++) {
		dp = (struct directory *)buf;
		for (; dp < (struct directory *)&buf[BLKSIZ]; dp++) {
			if (dp->d_fid.f_num == 0)
				continue;
			if (dp->d_fname.f_nam[0] != fp->f_nam[0]
			||  dp->d_fname.f_nam[1] != fp->f_nam[1]
			||  dp->d_fname.f_nam[2] != fp->f_nam[2]
			||  dp->d_fname.f_typ    != fp->f_typ)
				continue;
			if (dp->d_fname.f_ver == fp->f_ver)
				return (dp->d_fid.f_num);	/* exact match */
			if (fp->f_ver)
				continue;		/* exact wanted */
			if (dp->d_fname.f_ver > bestver) {
				bestver = dp->d_fname.f_ver;
				bestfid = dp->d_fid.f_num;
			}
		}
	}
	if (bestfid == 0)
		fserrno = RFENOENT;
	return (bestfid);
}

/*
 * convert an ascii filename into a files-11 one
 * unspecified parts are left 0
 * returns 0 for bad filename, 1 for ok
 */

int
utohname(name, f)
char *name;
register struct filnam *f;
{
	char *n;
	register char *e;
	register int i;
	register int x;

	f->f_nam[0] = f->f_nam[1] = f->f_nam[2] = 0;
	f->f_typ = 0;
	f->f_ver = 0;
	n = name;
	for (e = n; *e && *e != '.'; e++)
		;
	if (e - n > 9)
		return (0);
	if (*e)
		*e++ = 0;
	for (i = 0; i < 3; i++) {
		if ((x = ator50(&n)) < 0)
			return (0);
		f->f_nam[i] = x;
	}
	n = e;
	for (; *e && *e != '.'; e++)
		;
	if (e - n > 3)
		return (0);
	if (*e)
		*e++ = 0;
	if ((x = ator50(&n)) < 0)
		return (0);
	f->f_typ = x;
	if (*e) {
		x = atoi(e);
		if (x < 0 || x > 65535)
			return (0);
		f->f_ver = x;
	}
	return (1);
}

/*
 * read a block from a file
 */
int
f11rblk(f, bno, buf)
Rfile *f;
long bno;
char *buf;
{
	register Bmap *bp;

	if ((bp = getblk(fsp(f), bno)) == NULL)
		return (0);
	lseek(diskfd, (long)bp->bno * BLKSIZ, 0);
	if (read(diskfd, buf, BLKSIZ) != BLKSIZ)
		return (-1);
	return (1);
}

/*
 * get the header for a file
 */

int
gethdr(fid, hp)
unsigned short fid;
struct header *hp;
{
	long bno;
	register Bmap *bp;

	bno = IBITMAP + home.H_ibsz + fidtob(fid);
	if ((bp = getblk(&indexf, bno)) == NULL)
		return (0);
	lseek(diskfd, (long)bp->bno * BLKSIZ, 0);
	if (read(diskfd, (char *)hp, sizeof(*hp)) != sizeof(*hp))
		return (0);
	if (hp->h_fnum != fid)
		fprint(2, "gethdr: wanted fid %d, got %d\n", fid, hp->h_fnum);
	return (1);
}

/*
 * get the map for a chunk of a file,
 * starting at block bno
 */

Bmap *
getblk(fsf, bno)
register Fsfile *fsf;
long bno;
{
	static Bmap b;
	long offset;
	int mi;
	long mbno;
	register Bmap *bp;

	offset = bno - fsf->lbase;
	if (offset > 0 && offset < fsf->lmap.count) {	/* within cached map */
		b.count = fsf->lmap.count - offset;
		b.bno = fsf->lmap.bno + offset;
		return (&b);
	}
	if (offset > 0) {
		mi = fsf->lmi + 1;
		mbno = fsf->lbase + fsf->lmap.count;
	} else {
		mi = 0;
		mbno = 0;
	}
	while ((bp = getmap(&fsf->h.h_map, mi)) != NULL) {
		if (mbno + bp->count < bno) {
			mbno += bp->count;
			mi++;
			continue;
		}
		fsf->lbase = mbno;
		fsf->lmi = mi;
		fsf->lmap = *bp;
		offset = bno - mbno;
		b.count = bp->count - offset;
		b.bno = bp->bno + offset;
		return (&b);
	}
	return (NULL);
}

/*
 * fetch the n'th retrieval pointer from a map
 * no hope for extended headers yet
 */

#define	RS(c, l)	((c << 8)|l)

Bmap *
getmap(m, n)
register struct map *m;
int n;
{
	static Bmap b;
	register unsigned char *p;
	register int off;

	off = m->m_ctsz + m->m_lbsz;
	off *= n;
	if (off >= m->m_use * sizeof(short))
		return (NULL);
	p = m->m_rtrvp + off;
	switch (RS(m->m_ctsz, m->m_lbsz)) {
	case RS(1, 3):
		b.count = p[1];
		b.bno = (p[0]<<24) | p[2] | (p[3]<<8);
		break;

	case RS(2, 2):
		b.count = p[0] | (p[1]<<8);
		b.bno = p[2] | (p[3]<<8);
		break;

	case RS(2, 4):
		b.count = p[0] | (p[1]<<8);
		b.bno = (p[3]<<16) | (p[4]<<24) | p[4] | (p[5]<<8);
		break;

	default:
		return (NULL);
	}
	b.count++;
	return (&b);
}
