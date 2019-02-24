/*
 * Interlan NI1010A Ethernet controller
 */

/*
 * one struct ilchan per minor device;
 * eight of these per controller,
 * embedded in struct il
 */

#define	NILCHAN	8

struct ilchan {
	char unit;
	char ndelims;	/* number of packets waiting */
	short type;	/* ethernet protocol # */
	struct queue *rq;
	struct block *xcur;	/* block now being sent */
	struct block *xlast;	/* last data block put */
};

/*
 * one struct il per controller
 */

struct il {
	char flags;
	char ubno;
	short rbytes;		/* buffer space now outstanding */
	short rbufs;		/* number of buffers outstanding */
	short lastcsr;		/* il_csr at last interrupt */
	int ipackets, opackets;
	int ierrors, oerrors;
	int collisions;
	int ilost;
	struct ildevice *addr;
	struct block *rfirst, *rlast;	/* receive buffer */
	struct block *rnext;		/* where next block goes */
	short rcur;			/* bytes left in current packet */
	char lastch;			/* current transmit channel */
	unsigned char enaddr[6];	/* my ethernet address */
	struct ilchan chan[NILCHAN];
};
