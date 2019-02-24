/*
 * protocol for talking to the face server
 * defined as lengths and offsets for machine independence
 *
 * multiple byte quantites are stored little endian:
 * least significant byte in byte 0,
 * next least in byte 1,
 * and so on
 */

#define	frfshort(p, off)	((p)[(off)+0]+((p)[(off)+1]<<8))
#define	frflong(p, off)	((p)[(off)+0]+((p)[(off)+1]<<8)+((p)[(off)+2]<<16)+((p)[(off)+3]<<24))
#define	tofshort(p, off, n)	((p)[(off)+0]=n, (p)[(off)+1]=((n)>>8))
#define	toflong(p, off, n)	((p)[(off)+0]=n, (p)[(off)+1]=((n)>>8),\
				(p)[(off)+2]=((n)>>16), (p)[(off)+3]=((n)>>24))

#define	F_TYPE	0	/* one byte: message type */
/* one byte of pad */
#define	F_LEN	2	/* two bytes: length of accompanying data */
#define	F_P1	4	/* four bytes: first parameter */
#define	F_P2	8	/* four bytes: second parameter */

#define	F_DATA	12	/* associated data, if any */

/*
 * type codes
 */
#define	DOSTAT	1
#define	DOREAD	2

/*
 * for DOSTAT, associated data returned
 * is a representation of struct stat
 * it was first defined on a VAX,
 * so here, too, integers are little-endian
 */

#define	ST_DEV	0	/* (2) */
#define	ST_INO	2	/* (2) */
#define	ST_MODE	4	/* (2) */
#define	ST_NLINK 6	/* (2) */
#define	ST_UID	8	/* (2) */
#define	ST_GID	10	/* (2) */
#define	ST_RDEV	12	/* (2) */
/* two bytes of padding */
#define	ST_SIZE	16	/* (4) */
#define	ST_ATIME 20	/* (4) */
#define	ST_MTIME 24	/* (4) */
#define	ST_CTIME 28	/* (4) */
#define	STLEN	32	/* sizeof(struct stat) */

#define	STFMT	0170000	/* ST_MODE: type of file */
#define	STDIR	0040000	/* .. directory */

#define	FMAXPATH	128	/* max length of pathname */
#define	FMAXDATA	4096	/* max data read at once */

#define	FROOT	2	/* i-number of root directory */

/*
 * directories
 */
#define	FD_INO	0	/* (2 bytes) */
#define	FD_NAME	2
#define	FDLEN	16
