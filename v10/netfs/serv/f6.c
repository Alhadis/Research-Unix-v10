/*
 * read an old (V6 and before) PDP-11 Unix filesystem
 * quick, cheap hack; runs only on VAXes
 */

/*
 * miscellaneous filesystem definitions
 * some are magic numbers here
 */

#include <stdio.h>
#include <sys/types.h>
#include <sys/inode.h>
#include <sys/dir.h>	/* hack: pdp11 dirs == vax dirs */

#define	BLSIZE	512

/*
 * v6 disk inode
 */
#define	V6NADDR	8

struct v6dinode {
	short flags;
	unsigned char nlinks;
	unsigned char uid;
	unsigned char gid;
	unsigned char hisize;
	unsigned short losize;
	unsigned short addr[V6NADDR];
	unsigned short atime[2];	/* pdp-11 order */
	unsigned short mtime[2];	/* pdp-11 order */
};

/*
 * type part of mode
 */
#define	V6FMT	0160000
#define	V6IFREG	0100000
#define	V6IFDIR	0140000
#define	V6IFCHR	0120000
#define	V6IFBLK	0160000
#define	V6MODE	07777
#define	V6LARGE	010000

#define	V6SUPERB	1
#define	V6ROOT		1	/* root inode */

/*
 * local file data
 */
typedef struct Fsfile {
	long addr[V6NADDR];
	int large;
} Fsfile;

#define	fsp(f)	((Fsfile *)((f)->fs))

#include "rf.h"
#include <errno.h>
#include <libc.h>

int fserrno;
static int devfd;
static Rfile *root;

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
	char *passwd, *group;

	if (argc <= 1)
		rfpanic("no device specified\n");
	if ((devfd = open(argv[1], 0)) < 0)
		rfpanic("%s: cannot open\n", argv[1]);
	if (argc > 2)
		passwd = argv[2];
	else
		passwd = "/etc/passwd";
	if (argc > 3)
		group = argv[3];
	else
		group = "/etc/group";
	rfuidmap = rfmkidmap(passwd, (Namemap *)0);
	rfgidmap = rfmkidmap(group, (Namemap *)0);
	/* never mind the super-block */
	if ((f = (Rfile *)malloc(sizeof(Rfile))) == NULL)
		rfpanic("no mem for root\n");
	if ((f->fs = malloc(sizeof(Fsfile))) == NULL)
		rfpanic("no mem for root\n");
	f->ino = V6ROOT;
	fsstat(f);
	root = f;
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
		n = sprintf(one, "%d\t%.14s", de->d_ino, de->d_name);
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

#define	LINOPB	(BLSIZE/sizeof(struct v6dinode))
int
getino(f)
register Rfile *f;
{
	char buf[BLSIZE];
	register struct v6dinode *dp;
	register int ioff;
	register int i;
	int mode;

	ioff = f->ino - 1;
	lseek(devfd, (long)BLSIZE*(ioff/LINOPB + V6SUPERB + 1), 0);
	if (read(devfd, buf, BLSIZE) != BLSIZE) {
		/* print error */
		return (0);
	}
	dp = ((struct v6dinode *)buf) + (ioff%LINOPB);
	switch (dp->flags & V6FMT) {
	case V6IFREG:
		mode = IFREG;
		break;

	case V6IFDIR:
		mode = IFDIR;
		break;

	case V6IFCHR:
		mode = IFCHR;
		break;

	case V6IFBLK:
		mode = IFBLK;
		break;

	default:
		return (0);	/* unalloc or illegal */
	}
	for (i = 0; i < V6NADDR; i++)
		fsp(f)->addr[i] = dp->addr[i];
	f->dev = 0;		/* all the same device */
	f->rdev = fsp(f)->addr[0];
	f->mode = mode | (dp->flags & V6MODE);
	fsp(f)->large = (dp->flags & V6LARGE) != 0;
	f->nlink = dp->nlinks;
	f->uid = dp->uid;
	f->gid = dp->gid;
	f->size = (dp->hisize << 16) + dp->losize;
	f->tm = (dp->mtime[0]<<16) + dp->mtime[1];
	f->ta = (dp->atime[0]<<16) + dp->atime[1];
	f->tc = f->tm;
	return (1);
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
#define	LNINDIR	(BLSIZE/sizeof(unsigned short))

daddr_t
bmap(f, bno)
register Rfile *f;
daddr_t bno;
{
	unsigned short indbuf[LNINDIR];

	if (fsp(f)->large == 0) {
		if (bno < V6NADDR)
			return (fsp(f)->addr[bno]);
		return (0);
	}
	if (bno < V6NADDR*LNINDIR) {
		lseek(devfd, fsp(f)->addr[bno/LNINDIR]*BLSIZE, 0);
		if (read(devfd, (char *)indbuf, BLSIZE) != BLSIZE)
			return (0);
		return (indbuf[bno%LNINDIR]);
	}
	return (0);
}
