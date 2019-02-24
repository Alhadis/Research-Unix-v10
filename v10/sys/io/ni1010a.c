/*
 * Interlan NI1010A Ethernet interface.
 * Each hardware device has eight minor devices starting at N*8;
 * the different channels may be caused to receive different
 * Ethernet protocols via ENIOTYPE.
 * Packets read or written have the Ethernet header in front.
 */
#include "sys/param.h"
#include "sys/stream.h"
#include "sys/conf.h"
#include "sys/ubaddr.h"
#include "sys/enio.h"
#include "sys/ethernet.h"
#include "sys/ni1010a.h"

/*
 * hardware registers
 */
struct ildevice {
	unsigned short il_csr;
	short il_bar;
	short il_bcr;
};

/*
 * il_csr
 */
#define	IL_EUA		0xc000	/* buffer address high bits */
#define	IL_CDONE	0x0080	/* command done */
#define	IL_CIE		0x0040	/* command intr enable */
#define	IL_RDONE	0x0020	/* receive data arrived */
#define	IL_RIE		0x0010	/* receive intr enable */
#define	IL_STATUS	0x000f	/* mask for command status */

/*
 * commands, already shifted into place
 */
#define	ILC_ONLINE	0x0900	/* speak to the network */
#define	ILC_STAT	0x1800	/* return statistics and address */
#define	ILC_RCV		0x2000	/* here's a receive buffer */
#define	ILC_LDXMIT	0x2800	/* take this data */
#define	ILC_XMIT	0x2900	/* take this data, start sending */
#define	ILC_RESET	0x3f00	/* reset */

/*
 * status values (some)
 */
#define	ILERR_SUCCESS	0	/* ok */
#define	ILERR_RETRIES	1	/* ok, but retried */
/* other values mean it didn't work */

/*
 * status after diagnostics
 */
#define	ILDIAG_SUCCESS	0	/* ok */

/*
 * frame status bits in received packet
 */
#define	FS_LOST		04	/* some earlier packet was lost */
#define	FS_ALIGN	02	/* alignment error */
#define	FS_CRC		01	/* CRC error */

#define	CRCSIZE	4		/* size of the pointless CRC on received pkt */
#define	MAXRBUFS 16		/* max number of rcv buffers allowed */

/*
 * enormous statistics record
 * we get it only for the ethernet address address
 */
struct	il_stats {
	short	ils_fill1;
	short	ils_length;		/* Length (should be 62) */
	char	ils_addr[6];		/* Ethernet Address */
	short	ils_frames;		/* Number of Frames Received */
	short	ils_rfifo;		/* Number of Frames in Receive FIFO */
	short	ils_xmit;		/* Number of Frames Transmitted */
	short	ils_xcollis;		/* Number of Excess Collisions */
	short	ils_frag;		/* Number of Fragments Received */
	short	ils_lost;		/* Number of Times Frames Lost */
	short	ils_multi;		/* Number of Multicasts Accepted */
	short	ils_rmulti;		/* Number of Multicasts Rejected */
	short	ils_crc;		/* Number of CRC Errors */
	short	ils_align;		/* Number of Alignment Errors */
	short	ils_collis;		/* Number of Collisions */
	short	ils_owcollis;		/* Number of Out-of-window Collisions */
	short	ils_fill2[8];
	char	ils_module[8];		/* Module ID */
	char	ils_firmware[8];	/* Firmware ID */
};

extern int ilcnt;
extern struct il il[];
extern struct ubaddr iladdr[];

/*
 * il.flags
 */

#define	CMDACT	01	/* some command active -- can't issue another yet */
#define	INITDONE 02	/* finished init */
#define	INITING	04	/* halfway through init; just wakeup on intr */

#define	NEXTCH(i)	(((i)+1)%NILCHAN)

#define	ETHERMAXTU	1500	/* max packet size */
#define	ILRBYTES	(ETHERMAXTU*2)	/* desired receive buffer size */
#define	ILRSIZE		1024	/* preferred receive block size */

#define	ISCHAIN(l)	(((l)&07) == 0)
#define	MKCHAIN(l)	((l)&~07)	/* force length to allow rbuf chaining */
#define	MKTRUNC(l)	(MKCHAIN(l)-2)	/* force to disallow */

long ilopen();
int ilclose(), ilput();
static struct qinit ilrinit = { noput, NULL, ilopen, ilclose, 0, 0 };
static struct qinit ilwinit = { ilput, NULL, ilopen, ilclose, 4*ETHERMAXTU, 64 };
static struct streamtab ilsinfo = { &ilrinit, &ilwinit };
struct cdevsw ilcdev = cstrinit(&ilsinfo);

long
ilopen(q, dev)
register struct queue *q;
register dev_t dev;
{
	register struct ilchan *cp;
	register struct il *is;
	int unit;

	dev = minor(dev);
	unit = dev / NILCHAN;
	if (unit >= ilcnt || ilinit(unit) == 0)
		return(0);
	is = &il[unit];
	cp = &is->chan[dev%NILCHAN];
	if(cp->rq)
		return(0);
	cp->rq = q;
	q->ptr = (caddr_t)cp;
	WR(q)->ptr = (caddr_t)cp;
	WR(q)->flag |= QDELIM|QBIGB;
	q->flag |= QDELIM;
	cp->unit = unit;		/* needed? */
	cp->type = 0;
	return(1);
}

/*
 * init the hardware
 */
static struct il_stats ilstats;
static char ilsbusy;

ilinit(dev)
int dev;
{
	register struct il *is;
	register struct ildevice *addr;
	ubm_t ubm;
	uaddr_t uad;
	register int sts;
	int s;

	is = &il[dev];
	s = spl6();
	while (is->flags & INITING)
		tsleep((caddr_t)is, PZERO+1, 5);
	splx(s);
	if (is->flags & INITDONE)
		return (1);
	is->flags |= INITING;
	if ((addr = (struct ildevice *)ubaddr(&iladdr[dev])) == NULL
	||  badaddr(&addr->il_csr, sizeof(short))) {
		printf("ni1010a %d absent\n", dev);
		is->flags &=~ INITING;
		return (0);
	}
	is->ubno = iladdr[dev].ubno;
	is->addr = addr;
	if ((sts = ilincmd(is, ILC_RESET)) == 0
	||  (sts & IL_STATUS) != ILDIAG_SUCCESS) {
		printf("ni1010a %d: reset failed csr %x\n", dev, is->lastcsr);
		is->flags &=~ INITING;
		return (0);
	}
	s = spl6();
	while (ilsbusy)
		tsleep((caddr_t)&ilsbusy, PZERO, 5);
	ilsbusy = 1;
	splx(s);
	ubm = ubmalloc(is->ubno, sizeof(ilstats), USLP);
	uad = ubmaddr(is->ubno, (char *)&ilstats, sizeof(ilstats), ubm);
	addr->il_bar = uad;
	addr->il_bcr = sizeof(ilstats);
	sts = ilincmd(is, (int)((uad >> 2) & IL_EUA) | ILC_STAT);
	ubmfree(is->ubno, ubm);
	if (sts == 0 || (sts & IL_STATUS) != ILERR_SUCCESS) {
		ilsbusy = 0;
		wakeup((caddr_t)&ilsbusy);
		printf("ni1010a %d: stat failed csr %x\n", dev, is->lastcsr);
		is->flags &=~ INITING;
		return (0);
	}
	bcopy(ilstats.ils_addr, is->enaddr, sizeof(is->enaddr));
	ilsbusy = 0;
	wakeup((caddr_t)&ilsbusy);
	if ((sts = ilincmd(is, ILC_ONLINE)) == 0
	||  (sts & IL_STATUS) != ILERR_SUCCESS) {
		printf("ni1010a %d: online failed csr %x\n", dev, sts);
		is->flags &=~ INITING;
		return (0);
	}
	is->flags &=~ INITING;
	is->flags |= INITDONE;
	wakeup((caddr_t)is);		/* in case someone is waiting */
	s = spl6();
	ilrcvbufs(is);
	splx(s);
	return (1);
}

int
ilincmd(is, csr)
register struct il *is;
register short csr;
{
	register int s;

	s = spl6();
	is->flags |= CMDACT;
	is->addr->il_csr = csr|IL_CIE;
	while (is->flags & CMDACT)
		if (tsleep((caddr_t)is, PZERO, 5) != TS_OK) {
			is->flags &=~ CMDACT;
			splx(s);
			return (0);
		}
	csr = is->lastcsr;	/* probably unnecessary */
	splx(s);
	return (csr);
}

ilclose(q)
struct queue *q;
{
	struct ilchan *cp;

	cp = (struct ilchan *)q->ptr;
	cp->rq = 0;
}

/*
 * stash data
 */
ilput(q, bp)
struct queue *q;
register struct block *bp;
{
	register struct ilchan *cp;
	register int s;

	cp = (struct ilchan *)q->ptr;
	switch (bp->type) {
	case M_DATA:
		cp->xlast = bp;
		putq(q, bp);
		if (bp->class&S_DELIM)
			break;
		return;

	case M_IOCTL:
		ilioctl(q, bp);
		return;

	default:
		freeb(bp);
		return;
	}
	/*
	 * S_DELIM: end of packet
	 */
	if (cp->xlast == NULL)	/* empty record */
		return;
	cp->xlast = NULL;
	cp->ndelims++;
	s = spl6();
	ilsendpkt(&il[cp->unit]);
	splx(s);
}

/*
 * pick a channel with a packet to send, and start sending it
 * first block goes to the device here;
 * interrupt code will feed it more
 * adjust ethernet header in first block --
 * interlan expects no source address
 */
ilsendpkt(is)
register struct il *is;
{
	register struct ilchan *cp;
	register struct block *bp;
	register int i;
	register struct etherpup *ep;

	if (is->flags & CMDACT)
		return;
again:
	for (i = NEXTCH(is->lastch); ; i = NEXTCH(i)) {
		cp = &is->chan[i];
		if (cp->ndelims)
			break;
		if (i == is->lastch)
			return;
	}
	is->lastch = i;
	if ((bp = getq(WR(cp->rq))) == NULL)
		panic("ilsendpkt");
	cp->ndelims--;
	if (bp->wptr - bp->rptr < sizeof(struct etherpup)) {
		/* should probably try to pull packets together here */
		printf("ni1010a %d: short header\n", cp->unit);
		for (; (bp && bp->class&S_DELIM)==0; bp = getq(WR(cp->rq)))
			freeb(bp);
		goto again;
	}
	ep = (struct etherpup *)(bp->rptr);
	bcopy(ep->dhost, ep->shost, sizeof(ep->dhost));
	bp->rptr += sizeof(ep->dhost);
	cp->xcur = bp;
	ildebug(bp, 1, 0);	/* how can we generate length cheaply? */
	ilsendblock(is, bp);
}

/*
 * command done interrupt
 * if in init code, just wakeup
 * if need receive buffers, make one
 * else finish any pending transmit
 */
il1int(unit)
{
	register struct il *is;
	register struct ilchan *cp;
	register int type;

	is = &il[unit];
	if (is->addr == NULL) {
		printf("ni1010a %d: spurious interrupt\n", unit);
		return;
	}
	is->lastcsr = is->addr->il_csr;
	if ((is->flags & CMDACT) == 0) {
		printf("ni1010a %d: stray cmd interrupt, csr %x\n", unit, is->lastcsr);
		return;
	}
	is->flags &=~ CMDACT;
	if (is->flags & INITING) {
		wakeup((caddr_t)is);
		return;
	}
	if (is->rbytes < ILRBYTES)
		if (ilrcvbufs(is))
			return;
	cp = &is->chan[is->lastch];
	if (cp->xcur) {			/* sending something */
		type = cp->xcur->class;
		freeb(cp->xcur);
		cp->xcur = NULL;
		if ((type&S_DELIM)==0
		&&  (cp->xcur = getq(WR(cp->rq))) != NULL) {
			ilsendblock(is, cp->xcur);
			return;
		}
		switch (is->lastcsr & IL_STATUS) {
		case ILERR_RETRIES:
			is->collisions++;
		case ILERR_SUCCESS:
			is->opackets++;
			break;
	
		default:
			is->oerrors++;
			printf("ni1010a %d: xmt error 0x%x\n", unit, is->lastcsr&IL_STATUS);
			break;
		}
	}
	ilsendpkt(is);
}

/*
 * feed a block to the controller
 */
ilsendblock(is, bp)
struct il *is;
register struct block *bp;
{
	register struct ildevice *addr;
	uaddr_t uad;
	register int i;

	addr = is->addr;
	uad = ubadrptr(is->ubno, bp, ubmblk(is->ubno, bp, 0));
	addr->il_bar = uad;
	addr->il_bcr = bp->wptr - bp->rptr;
	is->flags |= CMDACT;
	i = ((uad>>2)&IL_EUA)|IL_RIE|IL_CIE;
	if ((bp->class&S_DELIM)==0)	/* not last piece of packet */
		addr->il_csr = i | ILC_LDXMIT;
	else
		addr->il_csr = i | ILC_XMIT;
}

/*
 * add receive buffer space if needed
 * -- let it be an ordinary command, interrupt when complete, for now.
 * this is probably too slow
 * returns nonzero if a command was started
 */
ilrcvbufs(is)
register struct il *is;
{
	register struct block *bp;
	register struct ildevice *addr;
	register int len;
	register uaddr_t uad;

	if (is->flags & CMDACT)
		return (0);
	if (is->rbytes >= ILRBYTES || is->rbufs >= MAXRBUFS)
		return (0);
	if ((bp = allocb(ILRSIZE)) == NULL)
		return (0);
	bp->next = NULL;
	if (is->rfirst == NULL)
		is->rfirst = bp;
	else
		is->rlast->next = bp;
	is->rlast = bp;
	len = bp->lim - bp->wptr;
	/* assume at least eight bytes in bp */
	len = MKCHAIN(len);
	is->rbytes += len;
	bp->wptr = bp->rptr + len;
	is->rbufs++;
	*(long *)bp->rptr = 0x80818283;	/* debuggery */
	uad = ubadrptr(is->ubno, bp, ubmblk(is->ubno, bp, 0));
	is->flags |= CMDACT;
	addr = is->addr;
	addr->il_bar = uad;
	addr->il_bcr = len;
	addr->il_csr = ((uad>>2)&IL_EUA)|ILC_RCV|IL_RIE|IL_CIE;
	return (1);
}

/*
 * receive done interrupt
 * -- (is->rfirst, is->rlast) is the set of buffers involved in receiving
 * is->rnext, if non-empty, points to the next one we expect data in
 * hence (is->rfirst, is->rnext) is the current incomplete packet
 * is->rcur is the number of bytes not yet received in the current packet
 */
il0int(unit)
int unit;
{
	register struct il *is;
	register struct block *bp;
	register struct block *lbp;
	register struct ilchan *cp;
	register struct queue *nq;
	register int i;
	int proto;

	is = &il[unit];
	if (is->addr)
		is->lastcsr = is->addr->il_csr;
	if (is->addr == NULL || is->rfirst == NULL) {
		printf("ni1010a %d: spurious rcv intr csr %x\n", unit, is->lastcsr);
		return;
	}
	if ((lbp = is->rnext) == NULL) {
		lbp = is->rfirst;
		lbp->rptr += 2;		/* frame status, junk */
		is->rcur = *lbp->rptr++;
		is->rcur += *lbp->rptr++ << 8;
		/* assume header is all in first block */
		ildebug(lbp, 0, is->rcur - CRCSIZE);
	}
	is->rbytes -= lbp->wptr - lbp->base;	/* sic -- in case first block */
	is->rbufs--;
	ilrcvbufs(is);
	i = lbp->wptr - lbp->rptr;
	if (is->rcur < i)
		i = is->rcur;
	lbp->wptr = lbp->rptr + i;
	is->rcur -= i;
	if (is->rcur > 0) {	/* more expected */
		if ((is->rnext = lbp->next) == NULL)
			panic("il0int");	/* too draconian */
		return;
	}
	/*
	 * complete packet:
	 * first block is is->rfirst,
	 * last is is->rnext == lbp
	 */
	lbp->wptr -= CRCSIZE;	/* discard boring CRC */
	lbp->class |= S_DELIM;	/* make delimiter */
	if (lbp->wptr < lbp->rptr) {
		i = lbp->rptr - lbp->wptr;
		lbp->rptr = lbp->wptr;
		for (bp = is->rfirst; bp != lbp; bp = bp->next)
			if (bp->next == lbp) {
				bp->wptr -= i;	/* and assume it stops here */
				break;
			}
	}
	bp = is->rfirst;	/* first piece of this packet */
	if (bp->rptr[-4] & (FS_ALIGN|FS_CRC)) {	/* [-4] == frame status */
		is->ierrors++;
		cp = NULL;
	} else {
		is->ipackets++;
		if (bp->rptr[-4] & FS_LOST)
			is->ilost++;
		/* assume the whole ethernet header is in the first block */
		proto = ((struct etherpup *)(bp->rptr))->type;
		for (i = 0; i < NILCHAN; i++) {
			cp = &is->chan[i];
			if (cp->rq && cp->type == proto)
				break;
		}
		if (i >= NILCHAN)
			cp = NULL;
	}
	lbp = lbp->next;		/* one past the end */
	if (cp == NULL || cp->rq->next->flag & QFULL) {
		while (is->rfirst != lbp) {
			bp = is->rfirst;
			is->rfirst = bp->next;
			freeb(bp);
		}
	} else {
		nq = cp->rq->next;
		while (is->rfirst != lbp) {
			bp = is->rfirst;
			is->rfirst = bp->next;
			(*nq->qinfo->putp)(nq, bp);
		}
	}
	is->rnext = NULL;
}

ilioctl(q, bp)
register struct queue *q;
register struct block *bp;
{
	register struct ilchan *cp;

	cp = (struct ilchan *)q->ptr;
	bp->type = M_IOCACK;
	switch(stiocom(bp)){
	case ENIOTYPE:
		cp->type = *((int *)stiodata(bp));
		break;

	case ENIOADDR:
		bcopy(il[cp->unit].enaddr, stiodata(bp), ETHERALEN);
		bp->wptr = bp->rptr + ETHERALEN + STIOCHDR;
		break;

	default:
		bp->type = M_IOCNAK;
		break;
	}
	qreply(q, bp);
}

#define ILLDEBSIZE 64
struct ild {
	time_t	time;
	unsigned short code;
	struct etherpup pup;
	short len;
} ild[ILLDEBSIZE];

int ili = 0;

#include "sys/systm.h"	/* just for time */

ildebug(bp, code, len)
struct block *bp;
int code, len;
{
	register struct ild *ip;

	ip = &ild[ili];
	ip->time = time;
	ip->code = code;
	ip->len = len;
	bcopy(bp->rptr, &ip->pup, sizeof(struct etherpup));
	if (++ili >= ILLDEBSIZE)
		ili = 0;
}
