/*
 * The I node is the focus of all
 * file activity in unix. There is a unique
 * inode allocated for each active file,
 * each current directory, each mounted-on
 * file, text file, and the root. An inode is 'named'
 * by its dev/inumber pair. (iget/iget.c)
 * Data, from mode on, is read in
 * from permanent inode on volume.
 */

#define	NADDR	13

struct	inode
{
	short	i_flag;
	dev_t	i_dev;		/* device where inode resides */
	char	i_fstyp;	/* type of its filesystem */
	unsigned short i_count;	/* reference count */
	long	i_number;	/* i number, 1-to-1 with device address */
	unsigned short i_mode;
	short	i_nlink;	/* directory entries */
	short	i_uid;		/* owner */
	short	i_gid;		/* group of owner */
	off_t	i_size;		/* size of file */
	struct	inode *i_mroot;	/* if mount point, root inode */
	struct	inode *i_mpoint;	/* inode of mount point */
	struct	stdata *i_sptr;	/* stream associated with this inode */
	union {
		struct {
			daddr_t	I_addr[NADDR];	/* if normal file/directory */
			daddr_t	I_lastr;	/* last read (for read-ahead) */
			struct buf *I_bufp;	/* buffer for super-block */
		} i_f;
#define	i_addr	i_f.I_addr
#define	i_lastr	i_f.I_lastr
#define	i_bufp	i_f.I_bufp
		struct {
			daddr_t	I_rdev;		/* i_addr[0] */
		} i_d;
#define	i_rdev	i_d.I_rdev
		struct {
			long I_tag;
			struct inode *I_cip;	/* communications */
			int I_fsflags;		/* in root inode only */
			time_t I_atime;
			time_t I_mtime;
			time_t I_ctime;
			dev_t I_rdev;
		} i_a;		/* network filesystems et al */
#define i_tag	i_a.I_tag
#define i_cip	i_a.I_cip
#define	i_fsflags	i_a.I_fsflags
#define	i_netatime	i_a.I_atime
#define	i_netmtime	i_a.I_mtime
#define	i_netctime	i_a.I_ctime
#define	i_netrdev	i_a.I_rdev
		struct {
			struct proc *I_proc;	/* sanity checking */
			int	    I_sigmask;	/* signal trace mask */
		} i_p;
#define i_proc	i_p.I_proc
#define i_sigmask	i_p.I_sigmask
	} i_un;
	struct inode *i_hlink;	/* link in hash chain (iget/iput/ifind) */
};

#ifdef KERNEL
extern struct inode inode[];
extern struct inode *inodeNINODE;
extern int inodecnt;

struct	inode *rootdir;		/* pointer to inode of root directory */

struct	inode *ialloc();
struct	inode *ifind();
struct	inode *iget();
struct	inode *iuniq();
struct	inode *namei();
struct	inode *openi();
#endif

/* flags */
#define	ILOCK	01		/* inode is locked */
#define	IUPD	02		/* file has been modified */
#define	IACC	04		/* inode access time to be updated */
#define	IWANT	020		/* some process waiting on lock */
#define	ITEXT	040		/* inode is pure text prototype */
#define	ICHG	0100		/* inode has been changed */
#define	IOPEN	0200		/* has been opened; close on last iput */

/* modes */
#define	IFMT	0170000		/* type of file */
#define		IFDIR	0040000	/* directory */
#define		IFCHR	0020000	/* character special */
#define		IFBLK	0060000	/* block special */
#define		IFREG	0100000	/* regular */
#define		IFLNK	0120000 /* symbolic link to another file */
#define	ISUID	04000		/* set user id on execution */
#define	ISGID	02000		/* set group id on execution */
#define	IREAD	0400		/* read, write, execute permissions */
#define	IWRITE	0200
#define	IEXEC	0100
#define	ICONC	0001000		/* this file is protected for concurrent access */
#define	ICCTYP	0007000		/* type of concurrent access */
#define		ISYNC	0001000	/* 1 writer and n readers (synchronized access) */
#define		IEXCL	0003000	/* 1 writer or n readers (exclusive access) */

struct argnamei {	/* namei's flag argument */
	short flag;	/* type of request */
	short len;	/* length of last component buffer */
	union {
		short	mode;		/* mode for creating */
		struct inode *il;	/* for linking */
		caddr_t	buf;		/* where to copy last component for exec */
	} un;
};
#define NI_SEARCH 0	/* search only (0 value known to nilargnamei, beware) */
#define NI_DEL	1	/* unlink this file */
#define NI_CREAT 2	/* create it if it doesn't exits */
#define NI_NXCREAT 3	/* create it, error if it already exists */
#define NI_LINK	4	/* make a link */
#define NI_MKDIR 5	/* make a directory */
#define NI_RMDIR 6	/* remove a directory */

#ifdef KERNEL
extern struct argnamei nilargnamei;
#endif
struct nx {	/* arg to real nami */
	struct inode *dp;	/* current inode */
	char *cp;		/* current position in pathname */
	char *nbuf;		/* pathname being parsed */
	short nlen;		/* length of pathname buffer */
	short nlink;		/* symlinks seen so far */
};

/*
 * some inline subroutines to speed things up
 */

#ifdef KERNEL

#define	plock(ip) \
{ \
	while ((ip)->i_flag & ILOCK) { \
		(ip)->i_flag |= IWANT; \
		sleep((caddr_t)(ip), PINOD); \
	} \
	(ip)->i_flag |= ILOCK; \
}

#define	prele(ip) \
{ \
	(ip)->i_flag &= ~ILOCK; \
	if ((ip)->i_flag&IWANT) { \
		(ip)->i_flag &= ~IWANT; \
		wakeup((caddr_t)(ip)); \
	} \
}

#endif
