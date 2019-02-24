/*
 * read a Berkeley `fast file system'
 * quick, cheap hack
 */

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>	/* for file modes; they happen to match */
#include "bsdffs.h"

#include "rf.h"
#include <errno.h>
#include <libc.h>

int fserrno;
static int devfd;
static Rfile *root;
static union {
	char buf[SBSIZE];
	struct fs super;
} super;
#define	sb	(super.super)
static long firstblk;	/* offset to beginning of filsys */
static char *blk;

typedef struct Fsfile {
	long dir[NDADDR];
	long ind[NIADDR];
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
	if ((devfd = open(argv[1], 0)) < 0)
		rfpanic("%s: cannot open\n", argv[1]);
	if (argc > 2)
		firstblk = atol(argv[2]) * 512;
	lseek(devfd, firstblk + SBLOCK, 0);
	if (read(devfd, super.buf, SBSIZE) != SBSIZE)
		rfpanic("can't read super block\n");
	if ((blk = malloc(sb.fs_bsize)) == NULL)
		rfpanic("can't alloc %d for block buf\n", sb.fs_bsize);
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
	bno = off / sb.fs_bsize;
	if (getlblk(f, bno, blk, f->size - bno*sb.fs_bsize) == 0)
		return (-1);
	rest = (bno + 1)*sb.fs_bsize - off;
	if (len > rest)
		len = rest;
	memcpy(buf, blk + (off % sb.fs_bsize), len);
	return (len);
}

/*
 * read a piece of a directory
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
	struct direct *endblk;
	char one[MAXNAMLEN+30];
	int n;

	stlen = len;
	endblk = de = (struct direct *)&blk[sb.fs_bsize];
	for (; off < f->size; off += de->d_reclen,
	     de = (struct direct *)&blk[off%sb.fs_bsize]) {
		if (de >= endblk) {
			if (getlblk(f, off/sb.fs_bsize, blk, f->size - off) == 0)
				break;
			de = (struct direct *)&blk[off%sb.fs_bsize];
		}
		if (de->d_reclen == 0)
			break;
		if (de->d_ino == 0)
			continue;
		sprintf(one, "%ld\t%s", de->d_ino, de->d_name);
		n = strlen(one)+1;
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

int
getino(f)
register Rfile *f;
{
	register struct icommon *dp;
	register int cg;
	register long ifrag;
	register long ino;

	cg = f->ino / sb.fs_ipg;
	ino = f->ino % sb.fs_ipg;
	ifrag = cg*sb.fs_fpg + sb.fs_cgoffset*(cg&~sb.fs_cgmask) + sb.fs_iblkno;
	ifrag += ino / (sb.fs_fsize/sizeof(struct icommon));
	lseek(devfd, firstblk + ifrag * sb.fs_fsize, 0);
	if (read(devfd, blk, sb.fs_fsize) != sb.fs_fsize) {
		/* print error */
		return (0);
	}
	dp = ((struct icommon *)blk) + (ino % (sb.fs_fsize/sizeof(struct icommon)));
	switch (dp->i_mode & S_IFMT) {
	case S_IFREG:
	case S_IFDIR:
	case S_IFBLK:
	case S_IFCHR:
		break;

	default:
		return (0);	/* unalloc or illegal */
	}
	memcpy((char *)fsp(f)->dir, (char *)dp->i_db, NDADDR*sizeof(long));
	memcpy((char *)fsp(f)->ind, (char *)dp->i_ib, NIADDR*sizeof(long));
	f->dev = 0;
	f->rdev = 0;	/* well ... */
	f->mode = dp->i_mode;
	f->nlink = dp->i_nlink;
	f->uid = dp->i_uid;
	f->gid = dp->i_gid;
	f->size = dp->i_size;
	f->tm = dp->i_mtime;
	f->ta = dp->i_atime;
	f->tc = dp->i_ctime;
	return (1);
}

/*
 * look up a file
 */

int
dsearch(f, name)
Rfile *f;
char *name;
{
	register struct direct *de;
	register struct direct *endblk;
	register long b, size;

	endblk = (struct direct *)(blk + sb.fs_bsize);
	for (b = 0, size = f->size; size > 0; b++, size -= sb.fs_bsize) {
		if (getlblk(f, b, blk, size) == 0)
			continue;
		for (de = (struct direct *)blk; de < endblk;
		     de = (struct direct *)((char *)de + de->d_reclen)) {
			if (de->d_reclen == 0)		/* safety */
				break;
			if (de->d_ino == 0)
				continue;
			if (strcmp(de->d_name, name) == 0)
				return (de->d_ino);
		}
	}
	return (0);
}

/*
 * read a block from a file
 */
long bmap();

getlblk(f, bno, buf, maxlen)
Rfile *f;
long bno;
char *buf;
int maxlen;
{
	long dbno;

	if (maxlen > sb.fs_bsize)
		maxlen = sb.fs_bsize;
	if ((dbno = bmap(f, bno)) == 0) {
		memset(buf, 0, maxlen);
		return (1);
	}
	lseek(devfd, firstblk + dbno*sb.fs_fsize, 0);
	if (read(devfd, buf, maxlen) != maxlen) {
		fserrno = errno;
		return (0);
	}
	return (1);
}

/*
 * logical to physical block
 * only singly-indirect files for now
 */

long
bmap(f, bno)
register Rfile *f;
long bno;
{

	if (bno < NDADDR)
		return (fsp(f)->dir[bno]);
	bno -= NDADDR;
	if (bno >= sb.fs_nindir)
		return (0);
	lseek(devfd, firstblk + fsp(f)->ind[0] * sb.fs_fsize, 0);
	if (read(devfd, blk, sb.fs_bsize) != sb.fs_bsize)
		return (0);
	return (((long *)blk)[bno]);
}
