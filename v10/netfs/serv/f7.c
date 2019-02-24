/*
 * read a V7 PDP-11 filesystem
 * quick hack: assumed to be on a VAX (byte order matters)
 */

/*
 * miscellaneous filesystem definitions
 * some are magic numbers here
 */

#include <sys/param.h>
#include <sys/filsys.h>
#include <sys/ino.h>
#include <sys/inode.h>
#include <sys/dir.h>

#define	BLSIZE	512

#include "rf.h"
#include <errno.h>
#include <libc.h>

int fserrno;
static int devfd;
static Rfile *root;

typedef struct Fsfile {
	daddr_t addr[NADDR];
} Fsfile;
#define	fsp(f)	((Fsfile *)((f)->fs))

/*
 * init:
 * open the device
 */

Rfile *
fsinit(argc, argv)
int argc;
char **argv;
{
	register Rfile *f;

	if (argc <= 1)
		rfpanic("no device specified\n");
	if ((devfd = open(argv[0], 0)) < 0)
		rfpanic("%s: cannot open\n", argv[1]);
	/* never mind the super-block */
	if ((f = (Rfile *)malloc(sizeof(Rfile))) == NULL)
		rfpanic("no mem for root\n");
	if ((f->fs = malloc(sizeof(Fsfile))) == NULL)
		rfpanic("no mem for root\n");
	f->ino = ROOTINO;
	fsstat(f);
	root = f;
	return (f);
}

/*
 * access a file
 */

Rfile *
fswalk(df, name)
register Rfile *df;
char *name;
{
	register Rfile *f;
	int ino;

	if ((ino = dsearch(df, name)) == 0) {
		fserrno = ENOENT;
		return (NULL);
	}
	if (df == root) {	/* "." and ".." magic */
		if (strcmp(name, ".") == 0)
			return (df);
		if (strcmp(name, "..") == 0) {
			fserrno = 0;	/* pseudo-error: popped out of root */
			return (NULL);
		}
	}
	if ((f = (Rfile *)malloc(sizeof(Rfile))) == NULL) {
		fserrno = ENOMEM;
		return (NULL);
	}
	if ((f->fs = malloc(sizeof(Fsfile))) == NULL) {
		free((char *)f);
		fserrno = ENOMEM;
		return (NULL);
	}
	f->ino = ino;
	fsstat(f);
	return (f);
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
int
fsstat(f)
Rfile *f;
{

	getino(f);
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
	char blk[BLSIZE];
	int rest;
	daddr_t bno;

	switch (f->mode & IFMT) {
	case IFREG:
	case IFDIR:
		break;

	default:
		return (0);
	}
	if (off >= f->size)
		return (0);
	if (off + len > f->size)
		len = f->size - off;
	bno = off / BLSIZE;
	if (getlblk(f, bno, blk) == 0)
		return (-1);
	rest = (bno + 1)*BLSIZE - off;
	if (len > rest)
		len = rest;
	memcpy(buf, blk + (off % BLSIZE), len);
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
	register struct direct *de;
	char blk[BLSIZE];
	char one[BLSIZE];
	int n;

	if (off % sizeof(struct direct)) {
		fserrno = EINVAL;
		return (-1);
	}
	stlen = len;
	de = (struct direct *)&blk[BLSIZE];
	for (; off < f->size; de++, off += sizeof(struct direct)) {
		if (de >= (struct direct *)&blk[BLSIZE]) {
			if (getlblk(f, off/BLSIZE, blk) == 0)
				break;
			de = (struct direct *)&blk[off%BLSIZE];
		}
		if (de->d_ino == 0)
			continue;
		n = sprint(one, "%d\t%.14s", de->d_ino, de->d_name);
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
 * fetch an i-node
 * -- no sanity check for now
 * -- magic inode-to-disk-block stuff here
 */

#define	LINOPB	(BLSIZE/sizeof(struct dinode))
int
getino(f)
register Rfile *f;
{
	char buf[BLSIZE];
	register struct dinode *dp;
	register unsigned int ioff;

	ioff = f->ino - 1;
	lseek(devfd, (long)BLSIZE*(ioff/LINOPB + SUPERB + 1), 0);
	if (read(devfd, buf, BLSIZE) != BLSIZE) {
		/* print error */
		return (0);
	}
	dp = ((struct dinode *)buf) + (ioff%LINOPB);
	switch (dp->di_mode & IFMT) {
	case IFREG:
	case IFDIR:
	case IFBLK:
	case IFCHR:
		break;

	default:
		return (0);	/* unalloc or illegal */
	}
	l11tol(fsp(f)->addr, dp->di_addr, NADDR);
	f->dev = 0;
	f->rdev = fsp(f)->addr[0];	/* perhaps wrong; who cares? */
	f->mode = dp->di_mode;
	f->nlink = dp->di_nlink;
	f->uid = dp->di_uid;
	f->gid = dp->di_gid;
	f->size = p11long(dp->di_size);
	f->tm = p11long(dp->di_mtime);
	f->ta = p11long(dp->di_atime);
	f->tc = p11long(dp->di_ctime);
	return (1);
}

l11tol(l3, l, n)
register unsigned char *l3, *l;
register int n;
{
	while (--n >= 0) {
		*l++ = l3[2];
		*l++ = l3[3];
		*l++ = l3[1];
		*l++ = 0;
		l3 += 3;
	}
}

long
p11long(l)
long l;
{
	return ((l<<16) | ((l>>16)&0xffff));
}

/*
 * look up a file
 */

#define	LNDPB	(BLSIZE/sizeof(struct direct))

int
dsearch(f, name)
Rfile *f;
char *name;
{
	struct direct dbuf[LNDPB];
	register struct direct *de;
	register int i;
	register long b, size;

	for (b = 0, size = f->size; size > 0; b++, size -= BLSIZE) {
		if (getlblk(f, b, (char *)dbuf) == 0)
			continue;
		for (i = 0, de = dbuf; i < LNDPB; i++, de++) {
			if (de->d_ino == 0)
				continue;
			if (strncmp(de->d_name, name, DIRSIZ) == 0)
				return (de->d_ino);
		}
	}
	return (0);
}

/*
 * read a block from a file
 */
daddr_t bmap();

getlblk(f, bno, buf)
Rfile *f;
daddr_t bno;
char *buf;
{
	daddr_t dbno;

	if ((dbno = bmap(f, bno)) == 0) {
		memset(buf, 0, BLSIZE);
		return (1);
	}
	lseek(devfd, dbno*BLSIZE, 0);
	if (read(devfd, buf, BLSIZE) != BLSIZE) {
		fserrno = errno;
		return (0);
	}
	return (1);
}

/*
 * logical to physical block
 * only singly-indirect files for now
 */
#define	LNINDIR	(BLSIZE/sizeof(daddr_t))

daddr_t
bmap(f, bno)
register Rfile *f;
daddr_t bno;
{
	daddr_t indbuf[LNINDIR];

	if (bno < NADDR - 3)
		return (fsp(f)->addr[bno]);
	bno -= NADDR - 3;
	if (bno >= LNINDIR)
		return (0);
	lseek(devfd, fsp(f)->addr[NADDR-3]*BLSIZE, 0);
	if (read(devfd, (char *)indbuf, BLSIZE) != BLSIZE)
		return (0);
	return (indbuf[bno]);
}
