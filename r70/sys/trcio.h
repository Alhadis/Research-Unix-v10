/*
 * stream tracer ioctls
 */
#define	TRCGNAME	(('T'<<8)|6)	/* get trace module name */
#define	TRCSNAME	(('T'<<8)|7)	/* set trace module name */
#define	TRCGMASK	(('T'<<8)|8)	/* get trace module mask */
#define	TRCSMASK	(('T'<<8)|9)	/* set trace module mask */

/*
 * stream tracer mask values
 */
#define	TR_DATA		0x1
#define	TR_BREAK	0x2
#define	TR_HANGUP	0x4
#define	TR_DELIM	0x8
#define	TR_ECHO		0x10
#define	TR_ACK		0x20
#define	TR_IOCTL	0x40
#define	TR_DELAY	0x80
#define	TR_CTL		0x100
#define	TR_SIGNAL	0x200
#define	TR_FLUSH	0x400
#define	TR_STOP		0x800
#define	TR_START	0x1000
#define	TR_IOCACK	0x2000
#define	TR_IOCNAK	0x4000
#define	TR_CLOSE	0x8000
