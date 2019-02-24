/*
 * DEQNA Ethernet Communications Controller interface,
 * vaguely derived from the driver in Ultrix.
 * Each DEQNA has eight minor devices starting at N*8;
 * the different channels may be caused to receive different
 * Ethernet protocols via ENIOTYPE.
 * Packets read or written have the Ethernet header in front.
 */

#include "sys/param.h"
#include "sys/buf.h"
#include "sys/stream.h"
#include "sys/ubaddr.h"
#include "sys/conf.h"
#include "sys/enio.h"
#include "sys/deqna.h"
#include "sys/ethernet.h"

struct qedevice {
	unsigned short qe_sta_addr[2];	/* ethernet addr; 6 shorts, overlaid */
	unsigned short qe_rcvlist_lo;	/* rcv list addr */
	unsigned short qe_rcvlist_hi;
	unsigned short qe_xmtlist_lo;	/* xmt list addr */
	unsigned short qe_xmtlist_hi;
	unsigned short qe_vector;
	unsigned short qe_csr;
};

/*
 * qe_csr
 */
#define QE_RCV_ENABLE	0x0001		/* receiver enable */
#define QE_RESET	0x0002		/* controller reset */
#define QE_NEX_MEM_INT	0x0004		/* non-existent memory */
#define QE_XL_INVALID	0x0010		/* nothing left to transmit */
#define QE_RL_INVALID	0x0020		/* no buffers left to receive */
#define QE_INT_ENABLE	0x0040		/* interrupt enable */
#define QE_XMIT_INT	0x0080		/* intr because xmit finished */
#define QE_ILOOP 	0x0100		/* internal loopback disable */
#define QE_ELOOP	0x0200		/* external loopback enable */
#define QE_RCV_INT	0x8000		/* intr because rcv finished */

/*
 * bits to shove into qe_csr for normal operation
 */
int qebits = QE_RCV_ENABLE|QE_INT_ENABLE|QE_ILOOP;
/*
 * interrupt request bits:
 * after an interrupt, write the ones that are set
 * to allow the next interrupt
 */
#define	INTFLAGS	(QE_XMIT_INT|QE_RCV_INT)

/*
 * buffer descriptor
 * flag is who owns it;
 * the rest is where the data is, and how big
 * they may be a list, or a chain.
 * we just use a ring; the DEQNA thinks it's a list,
 * but the chain flag is used to sew the ring together that the end
 */

struct qe_ring	{
	short flag;
	short hiaddr;	/* and descriptor type flags */
	short loaddr;
	short len;	/* negative length */
	unsigned short status1;
	unsigned short status2;
};

/*
 * flags: just descriptor ownership
 */

#define	FREADY	0x8000	/* descriptor ready, port not using yet */

/*
 * descriptor type flags, in hiaddr
 */

#define	DVALID	0x8000	/* valid descriptor */
#define	DCHAIN	0x4000	/* chain: address is that of next descriptor */
#define	DEND	0x2000	/* end of message; push output packet */
#define	DSETUP	0x1000	/* this is a setup packet */
#define	DENDODD	0x80	/* buffer ends on odd address */
#define	DBEGODD	0x40	/* buffer begins on odd address */

/*
 * status1 when we write it
 */
#define	NOTYET	0x8000	/* descriptor ready, but DEQNA hasn't taken it */
/*
 * status1 when DEQNA is through
 */
#define	NOTLAST	0x8000	/* rcv: not the last descriptor of a packet */
#define	ERROR	0x4000	/* either: some error happened */
#define	ESETUP	0x2000	/* rcv: setup packet acknowledgement */
#define	RHILEN	0x0700	/* rcv: high bits of received packet length */

/*
 * status2 when we write it
 */
#define	S2MAGIC	1	/* write this there to make descriptor ready.  why? */
/*
 * status2 when DEQNA is through
 */
#define RLOLEN	0x00ff	/* rcv: low bits of received length */

#define	RMINLEN	60	/* add this to get real received packet length */

/*
 *  operating condition set-up bits
 */
#define ALLM	0x1	/* match all multicast addresses */
#define PROM	0x2	/* match all addresses (promiscuous mode) */

/*
 * setup packet:
 * lists the addresses we may receive
 * each column (sic) of the array contains an address
 * e.g. setup[1][0..5]
 * setup[0][*] and setup[*][6..7] are junk
 */

struct setup {
	char d[16][8];
};

/*
 * data accessed directly by the DEQNA:
 * the buffer descriptors and the setup packet
 */

struct qeio {
	struct qe_ring rring[QENRCV+1];
	struct qe_ring xring[QENXMT+1];
	struct setup setup;
};

/*
 * This constant should really be 60 because the qna adds 4 bytes of crc.
 * However when set to 60 our packets are ignored by deuna's , 3coms are
 * okay ??????????????????????????????????????????
 */
#define ETHERMINTU 64
#define ETHERMAXTU 1564
#define MAX_RCV_BYTES (3*ETHERMAXTU)	/* maximum bytes in receive ring */
int	nRCV = QENRCV;
int	nXMT = QENXMT;

struct qechan *qefindchan();

extern int qecnt;
extern struct qe qe[];
extern struct ubaddr qeaddr[];

#define NEXTCH(c) (((c)+1)%QENCHAN)	/* next channel */

/* flag settings */
#define ATTACHED	0x4
#define SETUPQD		0x8

/* handy macros for ring management */
#define NFREE(s) ((s)->xindex>=(s)->oxindex?(QENXMT-((s)->xindex-(s)->oxindex)-1):\
					    ((s)->oxindex-(s)->xindex-1))
#define NEXTRCV(c) (((c)+1)%QENRCV)
#define NEXTXMT(c) (((c)+1)%QENXMT)

long	qeopen();
int	qeclose(), qeput();
static struct qinit qerinit = { noput, NULL, qeopen, qeclose, 0, 0 };
static struct qinit qewinit = { qeput, NULL, qeopen, qeclose, 4*ETHERMAXTU, 64 };
static struct streamtab qesinfo = { &qerinit, &qewinit };
struct cdevsw qecdev = cstrinit(&qesinfo);

/* open a channel */
long
qeopen(q, dev)
register struct queue *q;
register dev_t dev;
{
	register struct qechan *cp;
	register struct qe *qp;
	int unit, chan;

	dev = minor(dev);
	chan = dev % QENCHAN;
	unit = dev / QENCHAN;
	if(unit >= qecnt)
		return(0);
	if (qeinit(unit) == 0)
		return (0);
	qp = &qe[unit];
	cp = &qp->chan[chan];
	if(cp->rq)
		return(0);
	cp->unit = unit;
	cp->type = 0;
	q->ptr = (caddr_t)cp;
	WR(q)->ptr = (caddr_t)cp;
	WR(q)->flag |= QDELIM|QBIGB;
	q->flag |= QDELIM;
	cp->rq = q;
	return(1);
}

qeclose(q)
register struct queue *q;
{
	register struct qechan *cp;

	cp = (struct qechan *)q->ptr;
	cp->rq = 0;
	cp->type = 0;
}

/*
 * init a controller--
 * grab and map a buffer for qeio
 * reset the device and send it a setup packet
 */
qeinit(unit)
int unit;
{
	register struct qedevice *addr;
	register struct qe *qp;
	register uaddr_t ua;

	qp = &qe[unit];
	if (qp->addr)
		return (1);
	if ((addr = (struct qedevice *)ubaddr(&qeaddr[unit])) == 0
	||  badaddr(&addr->qe_sta_addr[0], sizeof(short))) {
		printf("deqna %d absent\n", unit);
		return (0);
	}
	qp->intvec = qeaddr[unit].vec;
	qp->ubno = qeaddr[unit].ubno;
	if (qp->iobuf == NULL) {
		qp->iobuf = geteblk();
		qp->iobm = ubmbuf(qp->ubno, qp->iobuf, 0);
		ua = ubadbuf(qp->ubno, qp->iobuf, qp->iobm);
		qp->rring = ((struct qeio *)(qp->iobuf->b_un.b_addr))->rring;
		qp->xring = ((struct qeio *)(qp->iobuf->b_un.b_addr))->xring;
		qp->setup = &((struct qeio *)(qp->iobuf->b_un.b_addr))->setup;
		qp->raddr = (uaddr_t)((struct qeio *)ua)->rring;
		qp->xaddr = (uaddr_t)((struct qeio *)ua)->xring;
		qp->setupaddr = (uaddr_t)&((struct qeio *)ua)->setup;
	}
	qp->addr = addr;
	addr->qe_csr = QE_RESET;
	addr->qe_csr = 0;	/* clear RESET to make reset happen */
	qemkrings(qp);
	qemksetup(qp);
	qeenable(qp);
	return (1);
}

/*
 * make the setup packet
 * receive from our address, and from the broadcast address
 */
qemksetup(qp)
	register struct qe *qp;
{
	register struct setup *sp;
	register int i, j;

	sp = qp->setup;
	/*
	 * fetch our Ethernet address into column 1
	 * toss the broadcast address into column 2
	 * copy our address into the rest of the first half
	 * duplicate first half into second
	 * columns 0 and 8 are junk
	 */
	for (i = 0; i < ETHERALEN; i++)
		sp->d[i][1] = qp->addr->qe_sta_addr[i];
#ifndef NOTDEF
	for (i = 0; i < ETHERALEN; i++)
		sp->d[i][2] = 0xff;		/* the broadcast addr */
	for (i = 0; i < ETHERALEN; i++)
		for (j = 3; j < 8; j++)
			sp->d[i][j] = sp->d[i][1];
#else
	for (i = 0; i < ETHERALEN; i++)
		for (j = 2; j < 8; j++)
			sp->d[i][j] = sp->d[i][1];
#endif
	bcopy(&sp->d[0][0], &sp->d[8][0], 8*8);
}

/*
 * output put routine
 * treasure up data till a DELIM comes,
 * then send it all as one packet
 */
qeput(q, bp)
	register struct queue *q;
	struct block *bp;
{
	register struct qechan *cp=(struct qechan *)q->ptr;
	int s;

	switch (bp->type) {
	case M_IOCTL:
		qeioctl(q, bp);
		return;

	case M_DATA:
		putq(q, bp);
		if (bp->class&S_DELIM)
			break;
		return;

	default:
		freeb(bp);
		return;
	}
	s = spl6();
	qestagepacket(cp);
	while (qexmtpacket(&qe[cp->unit])==0)
		;
	splx(s);
}

qeioctl(q, bp)
register struct queue *q;
register struct block *bp;
{
	register struct qechan *cp;
	int i;
	char *ap;

	cp = (struct qechan *)q->ptr;
	bp->type = M_IOCACK;
	switch(stiocom(bp)){
	case ENIOTYPE:
		cp->type = *((int *)stiodata(bp));
		break;

	case ENIOPROM:
		qexmtsetup(&qe[cp->unit], PROM, 1);
		break;

	case ENIOADDR:
		for (ap=stiodata(bp), i=0; i<ETHERALEN; i++) 
			*ap++ = qe[cp->unit].addr->qe_sta_addr[i];
		bp->wptr = bp->rptr + ETHERALEN + STIOCHDR;
		break;

	default:
		bp->type = M_IOCNAK;
		break;
	}
	qreply(q, bp);
}

/*
 * primitives for sending/receiving packets
 */

/*
 * make a descriptor invalid
 */
qeinvaldesc(rp)
	register struct qe_ring *rp;
{
	bzero((caddr_t)rp, sizeof(struct qe_ring));
}

/*
 * set up a buffer descriptor
 * tflags are the ones in hiaddr,
 * like `setup packet'
 */
qefilldesc(rp, addr, len, tflags)
	register struct qe_ring *rp;
	uaddr_t addr;
	int len;
	int tflags;
{

	rp->hiaddr = tflags;
	if (addr & 1) {
		len++;
		addr--;
		rp->hiaddr |= DBEGODD;
	}
	if (len & 1) {
		len++;
		rp->hiaddr |= DENDODD;
	}
	rp->len = -(len/2);
	rp->loaddr = addr;
	rp->hiaddr |= addr >> 16;
	rp->status2 = S2MAGIC;		/* something meaningless */
	rp->status1 = NOTYET;
	rp->flag = FREADY;
	rp->hiaddr |= DVALID;
}

/*
 * allocate a receive buffer
 * and give it to the DEQNA
 * if we're out of descriptors or are using too much memory,
 * return quietly
 */
qercvblock(qp)
	register struct qe *qp;
{
	register uaddr_t a;
	register int len;
	register struct block *bp;

	if (qp->rbytes>MAX_RCV_BYTES || NEXTRCV(qp->rindex)==qp->orindex)
		return -1;
	if ((bp = allocb(ETHERMAXTU))==NULL)
		panic("qercvblock");
	qp->rbp[qp->rindex] = bp;
	a = ubadwptr(qp->ubno, bp, ubmblk(qe->ubno, bp, 0));
	len = bp->lim - bp->wptr;
	qefilldesc(&qp->rring[qp->rindex], a, len, 0);
	qp->rbytes += len;
	if ((qp->addr->qe_csr&QE_RL_INVALID) &&
	    qp->rring[qp->rindex].status1 == NOTYET) {	/* wake up device */
		a = qp->raddr + qp->rindex*sizeof(struct qe_ring);
		qp->addr->qe_rcvlist_lo = a;
		qp->addr->qe_rcvlist_hi = a >> 16;
	}
	qp->rindex = NEXTRCV(qp->rindex);
	return 0;
}

/*
 * send a block
 */
qexmtblock(qp, bp, tflags)
	register struct qe *qp;
	struct block *bp;
	int tflags;
{
	register uaddr_t a;

	if (qp->xring[qp->xindex].status1 == NOTYET)
		panic("qexmitbp");
	qp->xbp[qp->xindex] = bp;
	a = ubadrptr(qp->ubno, bp, ubmblk(qe->ubno, bp, 0));
	qefilldesc(&qp->xring[qp->xindex], a, bp->wptr - bp->rptr, tflags);
	if ((qp->addr->qe_csr&QE_XL_INVALID) &&
	    qp->xring[qp->xindex].status1==NOTYET) {	/* wake up device */
		a = qp->xaddr + qp->xindex*sizeof(struct qe_ring);
		qp->addr->qe_xmtlist_lo = (short)a;
		qp->addr->qe_xmtlist_hi = (short)(a >> 16);
	}
	qp->xindex = NEXTXMT(qp->xindex);
}

/*
 * send the setup packet
 * -- knows it's the first packet ever sent
 */
qexmtsetup(qp, cmd, docmd)
	register struct qe *qp;
	int cmd;
	int docmd;
{
	register uaddr_t a;

	qefilldesc(&qp->xring[qp->xindex], qp->setupaddr,
		docmd ? (128+cmd) : sizeof(struct setup), DSETUP|DEND);
	if ((qp->addr->qe_csr&QE_XL_INVALID) &&
	    qp->xring[qp->xindex].status1==NOTYET) {	/* presumably always */
		a = qp->xaddr + qp->xindex*sizeof(struct qe_ring);
		qp->addr->qe_xmtlist_lo = a;
		qp->addr->qe_xmtlist_hi = a >> 16;
	}
	qp->xindex = NEXTXMT(qp->xindex);
}

/*
 * init the descriptor rings:
 * clear out any stale blocks
 * zap all the descriptors
 * point the last descriptor at the first,
 * forming the ring
 */
qemkrings(qp)
	register struct qe *qp;
{
	register int i, ch;

	for (i=0; i<QENRCV; i++)
		if (qp->rbp[i]) {
			freeb(qp->rbp[i]);
			qp->rbp[i] = NULL;
		}
	for (i=0; i<QENXMT; i++)
		if (qp->xbp[i]) {
			freeb(qp->xbp[i]);
			qp->xbp[i] = NULL;
		}
	for (ch=0; ch<QENCHAN; ch++) {
		qp->chan[ch].nxmt = 0;
		for (i=0; i<QENXMT; i++)
			if (qp->chan[ch].xmt[i]) {
				freeb(qp->chan[ch].xmt[i]);
				qp->chan[ch].xmt[i] = NULL;
			}
	}
	for (i = 0 ; i < QENRCV ; i++)
		qeinvaldesc(&qp->rring[i]);
	qefilldesc(&qp->rring[QENRCV], qp->raddr, 0, DCHAIN);
	for (i = 0 ; i < QENXMT ; i++)
		qeinvaldesc(&qp->xring[i]);
	qefilldesc(&qp->xring[QENRCV], qp->xaddr, 0, DCHAIN);
	qp->lastch = qp->orindex = qp->oxindex = qp->xindex = qp->rindex = 0;
	qp->rbytes = 0;
}

/*
 * turn the receiver on:
 * enable the hardware, set up receive buffers
 * send a setup packet
 */

qeenable(qp)
register struct qe *qp;
{

	qp->addr->qe_vector = qp->intvec;
	qp->addr->qe_csr = qebits;
	while (qercvblock(qp)==0)
		;
	qexmtsetup(qp, 0, 0);
	/*printf("qe csr after enable: %x\n", qp->addr->qe_csr);*/
}

/*
 * find a staged packet and send it
 */
qexmtpacket(qp)
	register struct qe *qp;
{
	register int i;
	register struct qechan *cp;

	/* Look for a channel that has a packet that will fit */
	for(i=NEXTCH(qp->lastch); ; i=NEXTCH(i)) {
		cp = &qp->chan[i];
		if (cp->nxmt && cp->nxmt<=NFREE(qp))
			break;
		if (i==qp->lastch)
			return -1;
	}
	qp->lastch = i;
	qedebug(cp->xmt[0], 1, cp->nbytes);
	for (i=0; i<cp->nxmt-1; i++) {		/* send any partial pieces */
		qexmtblock(qp, cp->xmt[i], 0);	
		cp->xmt[i] = NULL;
	}
	qexmtblock(qp, cp->xmt[i], DEND);	/* send the last piece */
	cp->xmt[i] = NULL;
	cp->nxmt = 0;
	qestagepacket(cp);	/* stage the next packet for this channel */
	return 0;

}

/*
 * Here on any interrupt
 */
qe0int(unit)
	int unit;
{
	register struct qe *qp = &qe[unit];
	register int csr;

	csr = qp->addr->qe_csr;
	qp->addr->qe_csr = qebits|(csr&INTFLAGS);
	if( csr & QE_RCV_INT ) 
		qerint(qp);
	if( csr & QE_XMIT_INT )
		qexint(qp);
	if( csr & QE_NEX_MEM_INT )
		printf("deqna %d: nxm\n", unit);
}

/*
 * transmit interrupt:
 * free up descriptors and blocks
 * DEQNA doesn't hear its own broadcasts,
 * so loop them around by hand here
 */
qexint(qp)
	register struct qe *qp;
{
	register int last;		/* last block of packet */
	register int first;		/* first block of packet */
	register struct qechan *cp;

	/* loop once per packet */
	for(;;) {
		/* loop once per block */
		first = qp->oxindex;
		for (last=first; ; last=NEXTXMT(last)) {
			if (last==qp->xindex || qp->xring[last].status1==NOTYET){
				/*
				 * nothing left
				 */
				while (qexmtpacket(qp)==0)
					;
				return;
			}
			if (qp->xring[last].hiaddr & DEND)
				break;
		}
		/*
		 * first -> first descriptor for some packet
		 * last -> last descriptor for the same packet
		 */
		if ((qp->xring[last].status1&ERROR) == 0)
			qp->opackets++;
		else {
			printf("deqna %d: xmt err st1 %x st2 %x\n",
				qp - qe,
				qp->xring[last].status1,
				qp->xring[last].status2);
			qp->oerrors++;
		}
		if (qp->xring[first].hiaddr & DSETUP) {	/* skip over setup packet */
			qeinvaldesc(&qp->xring[first]);	/* just one desc */
			qp->oxindex = NEXTXMT(last);
			continue;
		}
		if (qebits&QE_ELOOP)	/* debuggery */
			cp = NULL;
		else
			cp = qefindchan(qp, qp->xbp[first], 1);

		/* loop once per block to free (or loop back) packet */
		for(last=NEXTXMT(last); first!=last; first=NEXTXMT(first)) {
			/*printf("qexint: removing %d to %x\n", first, cp);*/
			if (cp)
				(*cp->rq->next->qinfo->putp)(cp->rq->next,qp->xbp[first]);
			else
				freeb(qp->xbp[first]);
			qeinvaldesc(&qp->xring[first]);
			qp->xbp[first] = NULL;
		}
		qp->oxindex = last;
	}
}

/*
 * receiver interrupt:
 * hand packet to the channel
 * that wants that protocol
 */
qerint(qp)
	register struct qe *qp;
{
	register int last;		/* last block of packet */
	register int first;		/* first block of packet */
	register struct qechan *cp;
	struct qe_ring *rp;
	struct block *bp;
	int len;

	/* loop once per packet */
	for(;;) {
		/* loop once per block */
		first = qp->orindex;
		for (last=first; ; last=NEXTRCV(last)) {
			if (last==qp->rindex ||
			    qp->rring[last].status1==NOTYET) {
				/*
				 * nothing left
				 */
				while(qercvblock(qp)==0)
					;
				return;
			}
			if ((qp->rring[last].status1&NOTLAST) == 0)
				break;
		}
		/*
		 * first -> first block of received packet
		 * last -> last block of that packet
		 */
		if (qp->rring[last].status1&ERROR) {
#if NOTDEF		/* usually boring */
			printf("deqna %d: rcv err st1 %x st2 %x\n",
				qp - qe,
				qp->rring[last].status1,
				qp->rring[last].status2);
#endif
			cp = NULL;
			qp->ierrors++;
		} else if (qp->rring[last].status1&ESETUP) { 
			/* printf("qerint: setup\n");*/
			cp = NULL;
		} else {
			cp = qefindchan(qp, qp->rbp[first], 0);
			qp->ipackets++;
		}

		/* loop once per block to receive packet */
		if (cp) {
			rp = &qp->rring[last];
			len = ((rp->status1&RHILEN)|(rp->status2&RLOLEN))+RMINLEN;
			qedebug(qp->rbp[first], 0, len);
		}
		/* install delimiter */
if (qp->rbp[last] == NULL)
panic("qerint");
		qp->rbp[last]->class |= S_DELIM;
		for(last=NEXTRCV(last); first!=last; first=NEXTRCV(first)) {
			/*printf("qerint: received %d to %x\n", first, cp);*/
			bp = qp->rbp[first];
			qp->rbytes -= bp->lim - bp->wptr;
			if (cp) {
				rp = &qp->rring[first];
				if (rp->status1&NOTLAST) {
					len -= bp->lim-bp->wptr;
					bp->wptr = bp->lim;
				} else {
					bp->wptr += len;
					if (bp->wptr > bp->lim)
						panic("qerint: length");
				}
				bp = cramb(bp);
				if(cp->rq->next->flag & QFULL)
					freeb(bp);
				else
					(*cp->rq->next->qinfo->putp)(cp->rq->next, bp);
			} else
				freeb(bp);
			qeinvaldesc(&qp->rring[first]);
			qp->rbp[first] = NULL;
		}
		qp->orindex = last;
	}
}

/*
 * find the channel for this input packet
 * checklocal means check that the packet was
 * addressed to us; used for loopback
 */
struct qechan *
qefindchan(qp, bp, checklocal)
	register struct qe *qp;
	struct block *bp;
	int checklocal;
{
	register int i;
	register struct etherpup *ep=(struct etherpup *)bp->rptr;
	register struct qechan *ch;

	/*printf("qefindchan: %x,%x,%x,%x,%x,%x:%d\n", 
		ep->dhost[0], ep->dhost[1], ep->dhost[2],
		ep->dhost[3], ep->dhost[4], ep->dhost[5],
		ep->type);*/
	if (checklocal) {
		for (i=0; i<ETHERALEN; i++) {
			if (ep->dhost[i]!=qp->setup->d[i][1]
			&&  ep->dhost[i]!=0xff)		/* broadcast */
				return NULL;
		}
	}
	for (i=0, ch=qp->chan; i<QENCHAN; i++, ch++)
		if (ch->rq!=NULL && ch->type==ep->type)
			return (ch);
	return NULL;
}

/*
 * stage the next packet for this channel:
 * gather the blocks, make sure they look sensible
 * printfs are a bit silly
 */
qestagepacket(cp)
	register struct qechan *cp;
{
	register struct block *bp;
	register int blk;
	int nbytes;
	register struct etherpup *ep;
	register struct setup *sp;

	if (cp->nxmt)		/* already have one staged */
		return;
	blk = 0;
	nbytes = 0;
	while ((bp = getq(WR(cp->rq))) != NULL) {
		if (bp->type != M_DATA)
			panic("qestagepkt");
		if (blk >= QENXMT-1) {
			blk = qepullup(cp);
			if (blk >= QENXMT-1) {
				freeb(bp);	/* wrong! but too hard for now */
				continue;
			}
		}
		cp->xmt[blk++] = bp;
		nbytes += bp->wptr-bp->rptr;
		if ((bp->class & S_DELIM) == 0)
			continue;
		/*
		 * have a whole packet
		 */
		if (nbytes > ETHERMAXTU) {
			printf("deqna %d: packet too long\n", cp->unit);
			break;
		}
		if (cp->xmt[0]->wptr-cp->xmt[0]->rptr < sizeof(struct etherpup)) {
			printf("deqna %d: too fragmented\n", cp->unit);
			break;
		}
		if (nbytes < ETHERMINTU)	/* wrong, but do it anyway */
			cp->xmt[blk-1]->wptr += ETHERMINTU-nbytes;
		/*
		 * force our address into the packet
		 * why doesn't the hardware do this?
		 */
		ep = (struct etherpup *)cp->xmt[0]->rptr;
		ep->type = cp->type;
		cp->nxmt = blk;
		sp = qe[cp->unit].setup;
		for(blk=0; blk < ETHERALEN; blk++)
			ep->shost[blk] = sp->d[blk][1];
		/*printf("qestagepacket: staged %d\n", cp->nxmt);*/
		cp->nbytes = nbytes;
		return;
	}
	/*
	 * if we come here, something went wrong
	 * any useful packet has already been removed from the queue
	 */
	if (bp)
		freeb(bp);
	else if (blk!=0)
		printf("deqna %d: stage no delim\n", cp->unit);
	while (--blk >= 0)
		freeb(cp->xmt[blk]);
	cp->nxmt = 0;
}

/*
 * transmit packet came in too many pieces
 * to fit in staging area
 * allocate a big block,
 * and try to squeeze some pieces into it
 * returns the first slot now available for a block,
 * QENXMT-1 if no space could be made
 *
 * assertion: no block we replace is S_DELIM
 */

int
qepullup(cp)
register struct qechan *cp;
{
	register struct block *newbp, *bp;
	register int blk, rep, space;
	register int n;

	if ((newbp = allocb(ETHERMAXTU/2)) == NULL)
		return (QENXMT-1);
	space = newbp->lim - newbp->wptr;
	for (blk = 0; blk < QENXMT-1; blk++) {
		bp = cp->xmt[blk];
		if (bp->wptr - bp->rptr < space)
			break;
	}
	if (blk >= QENXMT - 1) {	/* no block small enough to fit */
		freeb(newbp);
		return (blk);
	}
	/*
	 * pack as much as possible into newbp
	 */
	rep = blk;
	for (; blk < QENXMT-1; blk++) {
		bp = cp->xmt[blk];
		n = bp->wptr - bp->rptr;
		if (n > space)
			break;
		bcopy(bp->rptr, newbp->wptr, n);
		freeb(bp);
		newbp->wptr += n;
		space -= n;
	}
	/*
	 * slide remaining blocks up
	 */
	cp->xmt[rep++] = newbp;
	for (; blk < QENXMT-1; blk++, rep++)
		cp->xmt[rep] = cp->xmt[blk];
	return (rep);
}

/*
 * tracing stuff
 */

#include "sys/systm.h"		/* for time */

#define QEDEBSIZE 64
struct {
	time_t	time;
	unsigned short code;
	struct etherpup pup;
	short len;
} qed[QEDEBSIZE];

int qei = 0;

qedebug(bp, code, len)
register struct block *bp;
{
	qed[qei].time = time;
	qed[qei].code = code;
	qed[qei].len = len;
	bcopy(bp->rptr, &qed[qei].pup, sizeof(qed[qei].pup));
	qei = (qei + 1) % QEDEBSIZE;
}
