#include	<errno.h>

#define		SMAGIC		0x21746967
#define		DMAGIC		0x3A746967

typedef struct Superblock
{
	long magic;		/* magic number for Superblock */
	unsigned short blocksize;	/* physical size of blocks */
	short version;		/* type of superblock */
	long nblocks;		/* number of blocks on device */
	long zero;		/* first logical data block */
	long nfree;		/* number of free blocks */
	long ninodes;		/* number of inodes */
	long ninochars;		/* number of bytes of inode names */
	long binodes;		/* start of inodes */
	long nextffree;		/* next free file block */
	long nextsb;		/* next superblock */
	short fd;		/* fildes for device (in core) */
	char vol_id[128];	/* name the disk can be mounted as */
	char comment[128];	/* comments */
	long myblock;		/* where this superblock is */
	long nF;		/* bytes for .F (VBTREE) */
	long nT;		/* bytes for .T (VBTREE) */
	long ctime;		/* create time for this superblock */
} Superblock;

typedef struct Inode
{
	long magic;		/* magic number for Dirent */
	long block;		/* starting block of file */
	long nbytes;		/* bytes in file */
	long ctime;		/* creation time */
	union {
		char *n;	/* core - name */
		long o;		/* disk - offset into chars block */
	} name;			/* filename */
	long pad1;		/* to 32 bytes */
	short mode;		/* a la stat */
	short uid;		/* owner */
	short gid;		/* owner */
	short pad2;		/* to 32 bytes */
} Inode;
#define		IPERB		(s->blocksize/sizeof(Inode))

extern char *openinode(), *lkwri(), *lkwsb();
extern char *mapdev();
extern Inode *(*inodefn)();
extern void (*traversefn)();
extern long numinodes;
extern long numnamechars;
extern char *timenow();

#define	inodeof(s)	(*inodefn)(s)
#define	inodetraverse(fn)	(*traversefn)(fn)

#define	VLINK	1	/* linked list version */
#define	VBTREE	2	/* cbt */

#define	NBLKS(s, x)	(long)(((s)->blocksize-1+(x))/(s)->blocksize)

/*
	flags for openinode
*/
#define	DO_INODE	1
#define	SPIN_DOWN	2

#define	BIGBLOCK	(60*1024L)	/* max read/write */

/*
	in.c declarations
*/

extern int bad;
extern long nbytes;
extern long blkdone;
extern long nfiles;
extern char *argout;
