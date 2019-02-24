#define	QPCTL	0100		/* priority control message */
#define	QBSIZE	64		/* "standard" size of queue block*/

/*
 * data queue
 */
struct	queue {
	struct	qinit	*qinfo;		/* procs and limits for queue */
	struct	block	*first;		/* first data block */
	struct	block	*last;		/* last data block */
	struct	queue	*next;		/* Q of next stream */
	struct	queue	*link;		/* to next Q for scheduling */
	caddr_t	ptr;			/* to private data structure */
	short	count;			/* number of blocks on Q */
	u_short	flag;
};

/* Queue flags */
#define	QENAB	01			/* Queue is already enabled to run */
#define	QWANTR	02			/* Someone wants to read Q */
#define	QWANTW	04			/* Someone wants to write Q */
#define	QFULL	010			/* Q is considered full */
#define	QREADR	020			/* This is the reader (first) Q */
#define	QUSE	040			/* This queue in use (allocation) */
#define	QNOENB	0100			/* Don't enable Q via putq */
#define	QDELIM	0200			/* This queue generates delimiters */
#define	QBIGB	0400			/* This queue would like big blocks */

/*
 * queue information structure
 */
struct	qinit {
	int	(*putp)();		/* put procedure */
	int	(*srvp)();		/* service procedure */
	long	(*qopen)();		/* called on startup */
	int	(*qclose)();		/* called on finish */
	short	limit;			/* high water mark */
	short	lolimit;		/* low water mark */
};

#define	OTHERQ(q)	((q)->flag&QREADR? (q)+1: (q)-1)
#define	WR(q)		(q+1)
#define	RD(q)		(q-1)

/*
 * Queue data block
 */
struct	block {
	struct	block	*next;		/* next in chain */
	u_char	*rptr;			/* first unconsumed data */
	u_char	*wptr;			/* next unwritten data */
	u_char	*lim;			/* max for wptr */
	u_char	*base;			/* min for rptr/wptr */
	u_char	type;			/* type of block contents */
	u_char	class;			/* size class (plus S_DELIM flag) */
	u_char	bufix;			/* buffer index (for io maps) */
	u_char	rbufix;			/* real index for anon block */
	u_char	data[4];		/* data contents for tiny blocks */
};

#define	NOBUFIX	255
#define	S_DELIM	0200			/* This block is followed by a delimiter */
#define	CL_MASK	0177

/*
 * Header for a stream: interface to rest of system
 */
struct stdata {
	struct	queue *wrq;		/* write queue */
	struct	block *iocblk;		/* return block for ioctl */
	struct	inode *inode;		/* backptr, for hangups */
	struct	proc *wsel;		/* process write-selecting */
	struct	proc *rsel;		/* process read-selecting */
	short	pgrp;			/* process group, for signals */
	short	flag;
	char	count;			/* # processes in stream routines */
};
#define	IOCWAIT	01			/* Someone wants to do ioctl */
#define RSLEEP	02			/* Someone wants to read */
#define	WSLEEP	04			/* Someone wants to write */
#define	HUNGUP	010			/* Device has vanished */
#define	RSEL	020			/* read-select collision*/
#define	WSEL	040			/* write-select collision */
#define	EXCL	0100			/* exclusive-use (no opens) */
#define	STWOPEN	0200			/* waiting for 1st open */

#ifdef KERNEL
struct	block	*getq();
int	putq();
int	noput();
struct	block	*allocb();
struct	block	*cramb();
struct	block	*dupb();
struct	queue	*backq();
struct	queue	*allocq();
struct	inode	*stopen();
#endif

/*
 * Control messages (regular priority)
 */
#define	M_DATA	0		/* regular data (not ctl) */
#define	M_BREAK	01		/* line break */
#define	M_HANGUP 02		/* line disconnect */
#define	M_DELIM	03		/* data delimiter */
#define	M_IOCTL	06		/* ioctl; set/get params */
#define	M_DELAY 07		/* real-time xmit delay (1 param) */
#define	M_CTL	010		/* device-specific control message */
#define	M_PASS	011		/* pass file */
#define	M_YDEL	012		/* stream has started generating delims */
#define	M_NDEL	013		/* stream has stopped generating delims */

/*
 * Control messages (high priority; go to head of queue)
 */
#define	M_SIGNAL 0101		/* generate process signal */
#define	M_FLUSH	0102		/* flush your queues */
#define	M_STOP	0103		/* stop transmission immediately */
#define	M_START	0104		/* restart transmission after stop */
#define	M_IOCACK 0105		/* acknowledge ioctl */
#define	M_IOCNAK 0106		/* negative ioctl acknowledge */
#define	M_PRICTL 0107		/* high priority device-specific ctl */
#define	M_IOCWAIT 0110		/* stop ioctl timeout, ack/nak follows later */

#define	setqsched()	mtpr(SIRR, 0x3)

/*
 * ioctl message packet
 */

#define	STIOCSIZE	16
#define	STIOCHDR	4

struct stioctl {
	unsigned char com[STIOCHDR];	/* four-byte command, low order byte first */
	char data[STIOCSIZE];	/* depends on command */
};

#define	stiocom(bp)	(((struct stioctl *)bp->rptr)->com[0]|(((struct stioctl *)bp->rptr)->com[1]<<8))	/* two high bytes ignored for now */
#define	stiodata(bp)	(((struct stioctl *)bp->rptr)->data)

/*
 * for passing files across streams
 */
struct	kpassfd {
	union  {
		struct	file *fp;
		int	fd;
	} f;
	short	uid;
	short	gid;
	short	nice;
	char	logname[8];
};

/*
 * header for messages, see mesg.c
 */

struct mesg {
	char	type;
	u_char	magic;
	u_char	losize, hisize;
};

#define	MSGMAGIC	0345
#define	MSGHLEN	4	/* true length of struct mesg in bytes */
