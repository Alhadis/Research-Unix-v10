/*
 * Structure of the super-block
 */
struct	filsys
{
	unsigned short s_isize;		/* size in blocks of i-list */
	daddr_t	s_fsize;   		/* size in blocks of entire volume */
	short  	s_ninode;  		/* number of i-nodes in s_inode */
	ino_t  	s_inode[NICINOD];	/* free i-node list */
	char   	s_flock;   		/* lock during free list manipulation */
	char   	s_ilock;   		/* lock during i-list manipulation */
	char   	s_fmod;    		/* super block modified flag */
	char   	s_ronly;   		/* mounted read-only flag */
	time_t 	s_time;    		/* last super block update */
	daddr_t	s_tfree;   		/* total free blocks*/
	ino_t  	s_tinode;  		/* total free inodes */
	short	s_dinfo[2];		/* interleave stuff */
#define	s_m	s_dinfo[0]	/* only mkfs.c? */
#define	s_n	s_dinfo[1]
#define	s_cylsize s_dinfo[0]
#define	s_aspace s_dinfo[1]
	char   	s_fsmnt[14];		/* ordinary file mounted on */
	ino_t	s_lasti;		/* start place for circular search */
	ino_t	s_nbehind;		/* est # free inodes before s_lasti */
	union {
		struct {
			short  	S_nfree;/* number of addresses in s_free */
			daddr_t	S_free[NICFREE];/* free block list */
		} R;
		struct {
			char	S_valid;/* 1 on disk means bit map valid */
			char	S_flag;
#define BITMAP	961
			long	S_bfree[BITMAP];/* bit map for free data blocks */
		} B;
		struct {
			char	S_valid;
			char	S_flag;	/* 1 means bitmap not in S_bfree */
			long	S_bsize;/* how big the bitmap blocks are */
			struct buf *	S_blk[BITMAP-1];
		} N;
	} U;
};
#define s_nfree U.R.S_nfree
#define s_free  U.R.S_free
#define s_valid U.B.S_valid
#define s_bfree U.B.S_bfree

/*
 * fiddle bits in free block bitmap
 * assume NBBY*sizeof(long) is a power of 2
 */
#define	BITCELL	(NBBY*sizeof(long))
#define	BITISFREE(p, i)	(p[(i)/BITCELL]&(1<<((i)&(BITCELL-1))))
#define	BITFREE(p, i)	(p[(i)/BITCELL]|=(1<<((i)&(BITCELL-1))))
#define	BITALLOC(p, i)	(p[(i)/BITCELL]&=~(1<<((i)&(BITCELL-1))))

#ifdef KERNEL
struct	filsys *getfs();
#endif
