/*
 * DEQNA Q-bus Ethernet board
 * QENCHAN channels per controller,
 * each for a different packet type
 * one struct qe per controller,
 * each containing QENCHAN struct qechans
 */

#define QENCHAN	8		/* minor devices per controller */
#define	QENXMT	30		/* size of transmit ring */
#define	QENRCV	30		/* size of receive ring */

struct qechan {
	short unit;		/* device unit number */
	unsigned short type;	/* ethernet protocol for this chan */
	struct queue *rq;	/* read queue for this chan */
	struct block *xmt[QENXMT]; /* next packet to send */
	int nxmt;		/* number of blocks in nxmt */
	int nbytes;
};

struct	qe {
	/* statistics */
	int	ierrors,oerrors;	/* input/output errors */
	int	ipackets,opackets;	/* input/output packets */

	/* hardware stuff */
	struct	qedevice *addr;		/* device addr */
	short	intvec;			/* interrupt vector */
	short	ubno;			/* unibus number */

	/* for xmit/rcv */
	char	rindex;			/* next descriptor for receiving */
	char	orindex;		/* earliest pending receive */
	char	xindex;			/* next descriptor for transmitting */
	char	oxindex;		/* earliest pending transmit */
	int	rbytes;			/* bytes in receive buffer */
	struct	block *rbp[QENRCV];	/* receive blocks */
	struct	block *xbp[QENXMT];	/* transmit blocks */
	struct	buf *iobuf;		/* buffer where qeio lives */
	ubm_t	iobm;			/* its map token */
	struct	qe_ring *rring;		/* receive ring */
	struct	qe_ring *xring;		/* transmit ring */
	struct	setup *setup;		/* setup packet */
	uaddr_t	raddr, xaddr, setupaddr;	/* and their Q-bus addrs */

	/* channel info */
	int	lastch;			/* next channel to scan */
	struct qechan chan[QENCHAN];
};
