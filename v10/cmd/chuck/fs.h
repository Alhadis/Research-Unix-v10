#include "sys/param.h"
#include "sys/filsys.h"
#include "sys/ino.h"
#include "sys/inode.h"
#include "sys/dir.h"
struct	filsys sblk;
long	ninode,	/* first illegal inode number */
	fblk,	/* first legal data block number */
	lblk,	/* first illegal data block number */
	pblk,	/* first illegal physical block number */
	inopb;	/* dinodes per block */
typedef struct {	/* one per file system block */
	char type;
	long ino;
	int bnum;
} bm;
enum bm_type {Unk, Sblock, Inode, Free, Data, Ind, Ind2, Ind3,
		First, Other, Bits, Ioerr, Boot, Bad};
bm *bmap;
typedef struct {	/* one per inode */
	unsigned char type, seen;
	unsigned short nrefs;	/* how many dirs am i found in? */
	long last;	/* last legal block number */
	long dotdot;	/* .. for dirs */
	long parent;	/* a directory i'm found in */
	int ptr;	/* where dir entries start in dmap, after qsort */
} im;
enum im_type {Unalloc, Dir, Lnk, Chr, Blk, Reg, Weird};
im *imap;
typedef struct {
	long dino, ino;	/* directory summary */
} dm;
dm *dmap;
int dmaplen;	/* in case we have to realloc */
int dmapptr;
char flags[128];
char *file;	/* the name of the file system */
int fd;		/* fd for reading it */
int	bsize;	/* block size in file system */
int	exitcode;
char *freeb;	/* bit map blocks for large file systems */
int	tfree, tinode;	/* for the super block */
char *itype(), *btype(),*prname(), *prino();
char mbuf[4096], xbuf[];	/* to format output messages */
char *buf;	/* where the inode blocks get read (first 2 are extra bufs) */
int buflen;	/* how much is allocated */
enum e_type {Elinksize = 1, Ebadaddr, Efirst, Ebadread, Efirstio, Edirio,
		Enotdot, Edotino, Enotdotdot, Ebadino, Ebadname, Enullable,
		Elinkcnt, Emulti, Ebadparent, Einvalid, Esuperfree,
		Esuperino, Efreelist, Edup, Eshort, Ehole, Etriple, Eshortdir,
		Eattach, Efakeroot, };
typedef struct {
	enum e_type type;
	long a, b;
	char done;
} er;
er *erlist;
int erptr, erlen;
int errcnts[32];	/* at least all the e_types! */
int blkcnts[32];
int *iarg, *barg;	/* argument I and B */
int ilen, iptr, blen, bptr;
