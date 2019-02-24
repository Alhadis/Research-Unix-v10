/*
 * minimal definitions of 4BSD `fast file system' format
 * just enough to read a disk volume
 */

#define	SBLOCK	8192	/* byte offset to (first) super-block */
#define	SBSIZE	8192	/* size of super-block */

/*
 * contents of super-block
 */

struct	fs
{
	struct	fs *fs_link;		/* linked list of file systems */
	struct	fs *fs_rlink;		/*     used for incore super blocks */
	long	fs_sblkno;		/* addr of super-block in filesys */
	long	fs_cblkno;		/* offset of cyl-block in filesys */
	long	fs_iblkno;		/* offset of inode-blocks in filesys */
	long	fs_dblkno;		/* offset of first data after cg */
	long	fs_cgoffset;		/* cylinder group offset in cylinder */
	long	fs_cgmask;		/* used to calc mod fs_ntrak */
	long 	fs_time;    		/* last time written */
	long	fs_size;		/* number of blocks in fs */
	long	fs_dsize;		/* number of data blocks in fs */
	long	fs_ncg;			/* number of cylinder groups */
	long	fs_bsize;		/* size of basic blocks in fs */
	long	fs_fsize;		/* size of frag blocks in fs */
	long	fs_frag;		/* number of frags in a block in fs */
/* these are configuration parameters */
	long	fs_minfree;		/* minimum percentage of free blocks */
	long	fs_rotdelay;		/* num of ms for optimal next block */
	long	fs_rps;			/* disk revolutions per second */
/* these fields can be computed from the others */
	long	fs_bmask;		/* ``blkoff'' calc of blk offsets */
	long	fs_fmask;		/* ``fragoff'' calc of frag offsets */
	long	fs_bshift;		/* ``lblkno'' calc of logical blkno */
	long	fs_fshift;		/* ``numfrags'' calc number of frags */
/* these are configuration parameters */
	long	fs_maxcontig;		/* max number of contiguous blks */
	long	fs_maxbpg;		/* max number of blks per cyl group */
/* these fields can be computed from the others */
	long	fs_fragshift;		/* block to frag shift */
	long	fs_fsbtodb;		/* fsbtodb and dbtofsb shift constant */
	long	fs_sbsize;		/* actual size of super block */
	long	fs_csmask;		/* csum block offset */
	long	fs_csshift;		/* csum block number */
	long	fs_nindir;		/* value of NINDIR */
	long	fs_inopb;		/* value of INOPB */
	long	fs_nspf;		/* value of NSPF */
	long	fs_optim;		/* optimization preference, see below */
	long	fs_sparecon[5];		/* reserved for future constants */
/* sizes determined by number of cylinder groups and their sizes */
	long fs_csaddr;		/* blk addr of cyl grp summary area */
	long	fs_cssize;		/* size of cyl grp summary area */
	long	fs_cgsize;		/* cylinder group size */
/* these fields should be derived from the hardware */
	long	fs_ntrak;		/* tracks per cylinder */
	long	fs_nsect;		/* sectors per track */
	long  	fs_spc;   		/* sectors per cylinder */
/* this comes from the disk driver partitioning */
	long	fs_ncyl;   		/* cylinders in file system */
/* these fields can be computed from the others */
	long	fs_cpg;			/* cylinders per group */
	long	fs_ipg;			/* inodes per group */
	long	fs_fpg;			/* blocks per group * fs_frag */
#if NOTDEF	/* what follows isn't interesting */
/* this data must be re-computed after crashes */
	struct	csum fs_cstotal;	/* cylinder summary information */
/* these fields are cleared at mount time */
	char   	fs_fmod;    		/* super block modified flag */
	char   	fs_clean;    		/* file system is clean flag */
	char   	fs_ronly;   		/* mounted read-only flag */
	char   	fs_flags;   		/* currently unused flag */
	char	fs_fsmnt[MAXMNTLEN];	/* name mounted on */
/* these fields retain the current block allocation info */
	long	fs_cgrotor;		/* last cg searched */
	struct	csum *fs_csp[MAXCSBUFS];/* list of fs_cs info buffers */
	long	fs_cpc;			/* cyl per cycle in postbl */
	short	fs_postbl[MAXCPG][NRPOS];/* head of blocks for each rotation */
	long	fs_magic;		/* magic number */
	u_char	fs_rotbl[1];		/* list of blocks for each rotation */
/* actually longer */
#endif
};

#define	INOSIZE	128	/* length of inode on disk */

#define	NDADDR	12
#define	NIADDR	3

/*
 * inode on disk
 */

struct 	icommon
{
	short	i_mode;		/*  0: mode and type of file */
	short	i_nlink;	/*  2: number of links to file */
	short	i_uid;		/*  4: owner's user id */
	short	i_gid;		/*  6: owner's group id */
	long	i_size;		/*  8: number of bytes in file */
	long	i_hisize;	/* 12: high part of 64-byte size */
	long	i_atime;	/* 16: time last accessed */
	long	i_atspare;
	long	i_mtime;	/* 24: time last modified */
	long	i_mtspare;
	long	i_ctime;	/* 32: last time inode changed */
	long	i_ctspare;
	long	i_db[NDADDR];	/* 40: disk block addresses */
	long	i_ib[NIADDR];	/* 88: indirect blocks */
	long	i_flags;	/* 100: status, currently unused */
	long	i_blocks;	/* 104: blocks actually held */
	long	i_spare[5];	/* 108: reserved, currently unused */
};

#define	ROOTINO	2

/*
 * directory format
 */

#define	MAXNAMLEN	255

struct	direct {
	long	d_ino;			/* inode number of entry */
	unsigned short	d_reclen;		/* length of this record */
	unsigned short	d_namlen;		/* length of string in d_name */
	char	d_name[MAXNAMLEN + 1];	/* name must be no longer than this */
};

#define DIRSIZ(dp) \
    ((sizeof (struct direct) - (MAXNAMLEN+1)) + (((dp)->d_namlen+1 + 3) &~ 3))
