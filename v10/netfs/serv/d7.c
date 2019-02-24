/*
 * read a V7 PDP-11 dump tape as a filesystem
 */
#include <stdio.h>
#include <rf.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>

/*
 * PDP-11 integers
 */
#define	p11s(c)	((c)[0]|((c)[1]<<8))
#define	p11l(c)	((c)[2]|((c)[3]<<8)|((c)[0]<<16)|((c)[1]<<24))	/* wretched FP-11 */
#define	P11S	2	/* size of PDP-11 short */
#define	P11L	4	/* size of PDP-11 long */

/*
 * PDP-11 disk inode offsets
 */

#define	DI_MODE	0	/* (short) */
#define	DI_NLINK 2	/* (short) */
#define	DI_UID	4	/* (short) */
#define	DI_GID	6	/* (short) */
#define	DI_SIZE	8	/* (long) */
#define	DI_ADDR	12	/* 40 bytes */
#define	DI_ATIME 52	/* (long) */
#define	DI_MTIME 56	/* (long) */
#define	DI_CTIME 60	/* (long) */
#define	DILEN	64

/*
 * PDP-11 directory entry
 */

#define	D_INO	0	/* (short) */
#define	D_NAME	2	/* 14 characters */
#define	DNSIZE	14
#define	DLEN	16

/*
 * dump records
 */

#define	TBSIZE	512

#define	MAGIC	(int)60011

#define	TS_TAPE	1
#define	TS_INODE 2
#define	TS_BITS	3
#define	TS_ADDR	4
#define	TS_END	5
#define	TS_CLRI	6

struct spcl11 {		/* header as found on the tape */
	unsigned char c_type[P11S];
	unsigned char c_date[P11L];
	unsigned char c_ddate[P11L];
	unsigned char c_volume[P11S];
	unsigned char c_tapea[P11L];
	unsigned char c_inumber[P11S];
	unsigned char c_magic[P11S];
	unsigned char c_checksum[P11S];
	unsigned char c_dinode[DILEN];
	unsigned char c_count[P11S];
	unsigned char c_addr[TBSIZE];
};

struct spcl {		/* header we use internally */
	int c_type;
	int c_inumber;
	unsigned char c_dinode[DILEN];
	int c_count;
	char c_addr[TBSIZE];
};

/*
 * our data
 */

typedef unsigned int taddr_t;

#define	MAXINO	10000	/* dynamic would be better */
static taddr_t iaddr[MAXINO];	/* offset of TS_INODE for this file */

static int devfd;
static Rfile *root;
int fserrno;

struct spcl *getspcl();
taddr_t curtblock();
Rfile *getino();

char *malloc();
long lseek();

/*
 * init
 */
Rfile *
fsinit(argc, argv)
int argc;
char **argv;
{

	if (argc <= 1)
		rfpanic("no device\n");
	if ((devfd = open(argv[1], 0)) < 0)
		rfpanic("%s: cannot open\n", argv[1]);
	scantape();
	if ((root = getino(2)) == NULL)
		rfpanic("no root\n");
	return (root);
}

/*
 * look up a file
 */
Rfile *
fswalk(df, name)
Rfile *df;
char *name;
{
	int ino;

	if ((ino = dsearch(df, name)) == 0) {
		fserrno = ENOENT;
		return (NULL);
	}
	if (df == root) {	/* "." and ".." magic */
		if (strcmp(name, ".") == 0)
			return (df);
		if (strcmp(name, "..") == 0) {
			fserrno = 0;	/* pseudo-error */
			return (NULL);
		}
	}
	return (getino(ino));
}

/*
 * all done with a file
 */
fsdone(f)
Rfile *f;
{
	free(f->fs);
	free((char *)f);
	return (0);
}

/*
 * read data
 */
int
fsread(f, off, buf, len)
register Rfile *f;
long off;
char *buf;
int len;
{
	char blk[TBSIZE];
	int rest;
	long bno;

	switch (f->mode & S_IFMT) {
	case S_IFREG:
	case S_IFDIR:
		break;

	default:
		return (0);
	}
	if (off >= f->size)
		return (0);
	if (off + len > f->size)
		len = f->size - off;
	bno = off / TBSIZE;
	if (getblk(f, bno, blk) == 0)
		return (-1);
	rest = (bno + 1)*TBSIZE - off;
	if (len > rest)
		len = rest;
	memcpy(buf, blk + (off % TBSIZE), len);
	return (len);
}

/*
 * read a piece of a directory
 * -- cheap out for now: just return one
 */
int
fsdirread(f, off, buf, len, offp)
register Rfile *f;
long off;
char *buf;
int len;
long *offp;
{
	int stlen;
	register unsigned char *de;
	unsigned char blk[TBSIZE];
	unsigned char one[TBSIZE];
	int n, ino;

	if (off % DLEN) {
		fserrno = EINVAL;
		return (-1);
	}
	stlen = len;
	de = &blk[TBSIZE];
	for (; off < f->size; de += DLEN, off += DLEN) {
		if (de >= &blk[TBSIZE]) {
			if (getblk(f, off/TBSIZE, blk) == 0)
				break;
			de = &blk[off%TBSIZE];
		}
		ino = p11s(&de[D_INO]);
		if (ino == 0)
			continue;
		n = sprintf(one, "%d\t%.14s", ino, &de[D_NAME]);
		n++;	/* need the NUL too */
		if (n > len)
			break;
		memcpy(buf, one, n);
		len -= n;
		buf += n;
	}
	*offp = off;
	return (stlen - len);
}

/*
 * search a directory
 */
int
dsearch(f, name)
Rfile *f;
char *name;
{
	unsigned char dbuf[TBSIZE];
	register unsigned char *de;
	register int ino;
	register long b, size;

	for (b = 0, size = f->size; size > 0; b++, size -= TBSIZE) {
		if (getblk(f, b, dbuf) == 0)
			continue;
		for (de = dbuf; de < &dbuf[TBSIZE]; de += DLEN) {
			ino = p11s(&de[D_INO]);
			if (ino == 0)
				continue;
			if (strncmp(&de[D_NAME], name, DNSIZE) == 0)
				return (ino);
		}
	}
	return (0);
}

/*
 * retrieve file data
 */
Rfile *
getino(i)
int i;
{
	register Rfile *f;
	register struct spcl *sp;
	long nblocks;

	if (i >= MAXINO || i < 0 || iaddr[i] == 0) {
		rflog("%d: bad i-number\n", i);
		fserrno = EINVAL;
		return (NULL);
	}
	lseek(devfd, (long)iaddr[i] * TBSIZE, 0);
	if ((sp = getspcl()) == NULL
	||  sp->c_type != TS_INODE) {
		rflog("%d: can't find header\n", i);
		fserrno = EIO;
		return (NULL);
	}
	if (sp->c_inumber != i) {
		rflog("%d: found header for %d instead\n",
			i, sp->c_inumber);
		fserrno = EIO;
		return (NULL);
	}
	if ((f = (Rfile *)malloc(sizeof(Rfile))) == NULL) {
		rflog("%d: no memory for header\n", i);
		fserrno = ENOMEM;
		return (NULL);
	}
	f->ino = i;
	f->dev = 0;
	f->mode = p11s(&sp->c_dinode[DI_MODE]);	/* understood same bits */
	f->nlink = p11s(&sp->c_dinode[DI_NLINK]);
	f->uid = p11s(&sp->c_dinode[DI_UID]);
	f->gid = p11s(&sp->c_dinode[DI_GID]);
	f->rdev = 0;
	f->size = p11l(&sp->c_dinode[DI_SIZE]);
	f->ta = p11l(&sp->c_dinode[DI_ATIME]);
	f->tm = p11l(&sp->c_dinode[DI_MTIME]);
	f->tc = p11l(&sp->c_dinode[DI_CTIME]);
	nblocks = (f->size + TBSIZE - 1)/TBSIZE;
	if ((f->fs = malloc(nblocks * sizeof(taddr_t))) == NULL) {
		rflog("%d: no mem for %d block addresses\n", i, nblocks);
		fserrno = ENOMEM;
		free((char *)f);
		return (NULL);
	}
	scanfile(sp, (taddr_t *)f->fs, nblocks);
	return (f);
}

/*
 * read a TBSIZE block from file
 */
getblk(f, bno, buf)
register Rfile *f;
long bno;
char *buf;
{

	if (bno * TBSIZE > f->size) {
		fserrno = 0;
		return (0);
	}
	lseek(devfd, (long)((taddr_t *)f->fs)[bno] * TBSIZE, 0);
	if (read(devfd, buf, TBSIZE) != TBSIZE) {
		rflog("%d: bno %ld: errno %d\n", f->ino, bno, errno);
		fserrno = errno;
		return (0);
	}
	return (1);
}

/*
 * scan the tape,
 * remember where all the file records are
 */

scantape()
{
	register struct spcl *sp;
	register int i;

	for (;;) {
		if ((sp = getspcl()) == NULL) {
			rflog("unexpected tape EOF\n");
			return;
		}
		switch (sp->c_type) {
		case TS_END:
			return;

		case TS_TAPE:
			continue;

		case TS_BITS:
		case TS_CLRI:
			lseek(devfd, (long)sp->c_count * TBSIZE, 1);
			continue;

		case TS_ADDR:
			rflog("unexpected TS_ADDR i %d\n", sp->c_inumber);
			continue;

		default:
			rflog("ill spcl type %d i %d\n",
				sp->c_type, sp->c_inumber);
			continue;

		case TS_INODE:
			i = sp->c_inumber;
			if (i <= 0 || i >= MAXINO)
				rflog("ill TS_FILE i %d\n", i);
			else
				iaddr[i] = curtblock() - 1;
			scanfile(sp, (taddr_t *)NULL,
				(p11l(&sp->c_dinode[DI_SIZE])+TBSIZE-1)/TBSIZE);
			continue;
		}
	}
}

/*
 * skip lightly through a file,
 * remembering where the blocks are
 * ip is an array of tape addresses;
 * n is the length of the array, if present,
 * which is the same as the number of blocks expected
 * call with the TS_INODE record,
 * since that contains the first list of blocks
 */

scanfile(sp, ip, n)
register struct spcl *sp;
register taddr_t *ip;
register int n;
{
	register int i;
	taddr_t tblock;

	tblock = curtblock();
	while (n) {
		for (i = 0; i < sp->c_count; i++) {
			if (sp->c_count == 0) {		/* hole */
				if (ip)
					*ip++ = 0;
			} else {			/* real */
				if (ip)
					*ip++ = tblock;
				tblock++;
			}
			if (--n <= 0)
				break;
		}
		lseek(devfd, (long)tblock*TBSIZE, 0);	/* past data */
		if (n <= 0)
			return;
		/*
		 * more: get TS_ADDR
		 */
		if ((sp = getspcl()) == NULL) {
			rflog("unexpected tape EOF\n");
			return;
		}
		if (sp->c_type != TS_ADDR) {
			rflog("wanted TS_ADDR, got type %d i %d\n",
				sp->c_type, sp->c_inumber);
			unspcl();
			return;
		}
		tblock++;		/* count the header we just read */
	}
}

struct spcl *
getspcl()
{
	char buf[TBSIZE];
	static struct spcl s;
	register struct spcl11 *tp;
	int nbad;

	nbad = 0;
	tp = (struct spcl11 *)buf;
	for (;;) {
		if (read(devfd, buf, TBSIZE) != TBSIZE) {
			rflog("tape scan read, errno %d\n", errno);
			return (NULL);
		}
		if (p11s(tp->c_magic) != MAGIC) {
			nbad++;
			continue;
		}
		s.c_type = p11s(tp->c_type);
		s.c_inumber = p11s(tp->c_inumber);
		s.c_count = p11s(tp->c_count);
		/* a sanity check or two would go well here */
		memcpy(s.c_dinode, tp->c_dinode, DILEN);
		memcpy(s.c_addr, tp->c_addr, s.c_count);
		if (nbad)
			rflog("skipped %d to type %d i %d\n",
				nbad, s.c_type, s.c_inumber);
		return (&s);
	}
}

/*
 * reread the current header: cheap hack
 */
unspcl()
{
	lseek(devfd, (long)-TBSIZE, 1);
}

taddr_t
curtblock()
{
	return (lseek(devfd, 0L, 1)/TBSIZE);
}

