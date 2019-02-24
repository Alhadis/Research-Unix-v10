/*
 * One file structure is allocated
 * for each open/creat/pipe call.
 * Main use is to hold the read/write
 * pointer associated with each open
 * file.
 */
struct	file
{
	short	f_flag;
	short	f_count;		/* reference count */
	struct inode *f_inode;		/* pointer to inode structure */
	llong_t	f_offset;		/* read/write character pointer */
};

#ifdef	KERNEL
extern struct file file[];
extern struct file *fileNFILE;
extern int filecnt;

struct	file *falloc();
struct	file *allocfile();

/*
 * inline to speed things up
 */
#define	getf(fd)	((unsigned)(fd)>=NOFILE ? NULL : u.u_ofile[fd])

#endif

/* flags */
#define	FREAD	01
#define	FWRITE	02
#define	FPIPE	04
#define	FHUNGUP 010
#define	FRNBLK	040	/* no block on read */
#define	FWNBLK	0100	/* no block on write */
