/*
 * network filesystem protocol
 *
 * messages are character arrays,
 * encoding structures of characters,
 * two-byte shorts, and four-byte longs.
 * shorts and longs have a specific byte order,
 * and should be accessed through the macros below.
 */

/*
 * general numbers
 */
#define	NETB	2	/* protocol version */
#define	NBMAXMSG	(5*1024)	/* max message size */
#define	NBROOTTAG	0	/* i_tag assumed for the root */


/*
 * turn network numbers into host numbers
 * unsigned char *p
 */
#define	frnetlong(p, off)	(p[off+0]+(p[off+1]<<8)+((long)p[off+2]<<16)+((long)p[off+3]<<24))
#define	frnetshort(p, off)	(p[off+0]+(p[off+1]<<8))
#define	frnetchar(p, off)	p[off]

/*
 * turn host numbers to network numbers
 */

#define	tonetlong(p, off, l)	(p[off+0]=(l), p[off+1]=(l)>>8, p[off+2]=(l)>>16, p[off+3]=(l)>>24)
#define	tonetshort(p, off, s)	(p[off+0]=(s), p[off+1]=(s)>>8)
#define	tonetchar(p, off, c)	(p[off]=c)

/*
 * messages from client to server
 */

/*
 * general header
 */

#define	SNB_VERSION	0	/* (char) version */
#define	SNB_CMD		1	/* (char) command; see below */
#define	SNB_FLAGS	2	/* (char) mostly for nami */
/* one byte of padding */
#define	SNB_TRANNUM	4	/* (long) unique transaction ID */
#define	SNB_LEN		8	/* (long) including header and any data */
#define	SNB_TAG		12	/* (long) which file this is about */
#define	SNB_UID		16	/* (short) who wants to do IO */
#define	SNB_GID		18	/* (short) ditto */
/* four more bytes of junk */
#define	SNBSIZE		24

/*
 * commands
 */
#define	NBPUT	1	/* put file */
/* 2 was NAGET */
#define	NBUPD	3	/* update attributes */
#define	NBREAD	4	/* read data */
#define	NBWRT	5	/* write data */
#define	NBNAMI	6	/* translate name (with many side effects) */
#define	NBSTAT	7	/* read file status */
#define	NBIOCTL	8	/* ioctl */
#define	NBTRNC	9	/* truncate */
#define	NBDIR	10	/* directory read */

/*
 * flags, for namei sub-function code,
 * are defined in inode.h
 * should they be here too?
 */

/*
 * additional data for each command
 */

/*
 * update
 */
#define	SUP_MODE	(SNBSIZE+0)	/* (short) new mode */
/* two bytes for raw device (mknod) */
/* four bytes of junk */
#define	SUP_ATIME	(SNBSIZE+8)	/* (long) access time */
#define	SUP_MTIME	(SNBSIZE+12)	/* (long) mod time */
#define	SUPSIZE		(SNBSIZE+16)

/*
 * read, dirread
 */
#define	SRD_LEN		(SNBSIZE+0)	/* (long) how much to read */
#define	SRD_OFFSET	(SNBSIZE+4)	/* (long) file offset */
#define	SRDSIZE		(SNBSIZE+8)

/*
 * write
 */
#define	SWR_LEN		(SNBSIZE+0)	/* (long) how much to write */
#define	SWR_OFFSET	(SNBSIZE+4)	/* (long) where to write it */
#define	SWRSIZE		(SNBSIZE+8)
/* data to be written follows immediately */

/*
 * namei
 */
#define	SNM_MODE	(SNBSIZE+0)	/* (short) mode for creat */
#define	SNM_DEV		(SNBSIZE+2)	/* (short) device (mknod); used? */
#define	SNM_INO		(SNBSIZE+4)	/* (long) i-number, for link */
#define	SNMSIZE		(SNBSIZE+8)
/* name to be translated follows immediately */

/*
 * SNB_FLAGS: namei function codes
 * these must match the numbers in inode.h;
 * they are repeated here so the protocol
 * is all defined in one place
 */
#define NI_SEARCH 0	/* search only (0 value known to nilargnamei, beware) */
#define NI_DEL	1	/* unlink this file */
#define NI_CREAT 2	/* create it if it doesn't exits */
#define NI_NXCREAT 3	/* create it, error if it already exists */
#define NI_LINK	4	/* make a link */
#define NI_MKDIR 5	/* make a directory */
#define NI_RMDIR 6	/* remove a directory */

/*
 * stat
 */
#define	SST_TIME	(SNBSIZE+0)	/* (long) time, for synchronization */
/* four bytes of padding */
#define	SSTSIZE		(SNBSIZE+8)

/*
 * ioctl
 */
#define	SIO_CMD		(SNBSIZE+0)	/* (long) function code */
#define	SIO_FLAG	(SNBSIZE+4)	/* (short) file flags; silly? */
/* two bytes padding */
#define	SIOSIZE		(SNBSIZE+8)
#define	SIODATA		64		/* bytes of data follow */
/* does some ioctl data follow? */

/*
 * responses from server to client
 */

/*
 * general header
 */
#define	RNB_TRANNUM	0	/* (long) transaction ID */
#define	RNB_ERRNO	4	/* (short) error number; zero if OK */
#define	RNB_FLAGS	6	/* (char) see below */
/* one byte of pad */
#define	RNB_LEN		8	/* (long) including header and any data */
#define	RNB_FSIZE	12	/* (long) file size after write (why here?) */
#define	RNBSIZE		16

/*
 * flags
 */
#define	NBROOT	1	/* namei popped out of root */
#define	NBEND	2	/* last read was short, probably end of file */

/*
 * extra data for each type of response
 */

/*
 * read: no extra header info;
 * data follows at offset RNBSIZE
 */

/*
 * namei
 * contains an entire stat response
 * so sys stat sends one namei message and no stat message
 * not exactly a stat message, though: RNM_USED is in the middle
 */

#define	RNM_TAG		(RNBSIZE+0)	/* (long) new file */
#define	RNM_INO		(RNBSIZE+4)	/* (long) its i-number */
#define	RNM_DEV		(RNBSIZE+8)	/* (short) st_dev */
#define	RNM_MODE	(RNBSIZE+10)	/* (short) mode */
#define	RNM_USED	(RNBSIZE+12)	/* (long) filename chars consumed (NBROOT only) */
#define	RNM_NLINK	(RNBSIZE+16)	/* (short) */
#define	RNM_UID		(RNBSIZE+18)	/* (short) */
#define	RNM_GID		(RNBSIZE+20)	/* (short) */
#define	RNM_RDEV	(RNBSIZE+22)	/* (short) st_rdev; useful? */
#define	RNM_SIZE	(RNBSIZE+24)	/* (long) */
#define	RNM_ATIME	(RNBSIZE+28)	/* (long) */
#define	RNM_MTIME	(RNBSIZE+32)	/* (long) */
#define	RNM_CTIME	(RNBSIZE+36)	/* (long) */
#define	RNMSIZE		(RNBSIZE+40)

/*
 * stat
 */
#define	RST_INO		(RNBSIZE+0)	/* (long) i-number */
#define	RST_DEV		(RNBSIZE+4)	/* (short) st_dev */
#define	RST_MODE	(RNBSIZE+6)	/* (short) */
#define	RST_NLINK	(RNBSIZE+8)	/* (short) */
#define	RST_UID		(RNBSIZE+10)	/* (short) */
#define	RST_GID		(RNBSIZE+12)	/* (short) */
#define	RST_RDEV	(RNBSIZE+14)	/* (short) st_rdev; used? */
#define	RST_SIZE	(RNBSIZE+16)	/* (long) */
#define	RST_ATIME	(RNBSIZE+20)	/* (long) */
#define	RST_MTIME	(RNBSIZE+24)	/* (long) */
#define	RST_CTIME	(RNBSIZE+28)	/* (long) */
#define	RSTSIZE		(RNBSIZE+32)

/*
 * ioctl:
 * no header, just SIOSIZE bytes of ioctl data
 */

/*
 * dirread
 */
#define	RDI_USED	(RNBSIZE+0)	/* (long) advance file pointer this much */
/* four bytes of junk */
#define	RDISIZE		(RNBSIZE+8)
