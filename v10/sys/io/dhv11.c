/*
 * DHV11 driver for Ninth Edition UNIX
 * by Andrew Hume (loosely based on dh driver from toronto)
 */

#include "sys/param.h"
#include "sys/systm.h"
#include "sys/stream.h"
#include "sys/ttyio.h"
#include "sys/ubaddr.h"
#include "sys/conf.h"
#include "sys/dhv11.h"

#define NPER	8		/* lines per board */

/* bits in dhv[]->state */
#define	ISOPEN	0x0002
#define	WOPEN	0x0004
#define	TIMEOUT	0x0008
#define	CARRIER	0x0010
#define	DHSTOP	0x0020
#define	HPCL	0x0040
#define	BRKING	0x0080
#define BUSY	0x0100
#define FLUSH	0x0200

#define SSPEED	B9600	/* reasonable default nowadays */
#define DHPRI	30

/* hardware structure */
struct dhvreg
{
	unsigned short csr;
	short rbuf;		/* coincides with txchar (not used) */
	unsigned short lpr;	/* line param */
	unsigned short lstat;	/* line status */
	unsigned short lctl;	/* line control */
	unsigned short addr1;
	unsigned short addr2;
	unsigned short cnt;
};

struct hilo	/* for utterly wretched appalling hardware */
{
	char low;
	char high;
};
#define	LOW(ptr)	((struct hilo *)&(ptr))->low
#define	HIGH(ptr)	((struct hilo *)&(ptr))->high

/* csr bits */
#define	TXACT		0x8000
#define	TXIE		0x4000
#define	DIAGFAIL	0x2000
#define	TXERR		0x1000
#define	TXLINE(csr)	(((csr)>>8)&0xF)
#define	RXAVAIL		0x0080
#define	RXIE		0x0040
#define	MRESET		0x0020
#define	POINT(r, l)	LOW((r)->csr) = (((l)&0xF) | RXIE)

/* rbuf */
#define	VALID		0x8000
#define	OERR		0x4000
#define	FERR		0x2000
#define	PERR		0x1000
#define	LINE(rbuf)	(((rbuf)>>8)&0xF)
#define	ISMODEM(rbuf)	(((rbuf)&0x7000)==0x7000)
#define	DCD		0x10	/* note that it is shifted left 8 bits in lstat */

/* lpr bits */
#define	BITS5		0x00
#define	BITS6		0x08
#define	BITS7		0x10
#define	BITS8		0x18
#define	PENAB		0x20
#define	EPARITY		0x40
#define	STOP2		0x80

/* lctl bits */
#define	TXABORT		0x0001
#define	RXENABLE	0x0004
#define	BREAK		0x0008
#define	MODEM		0x0100
#define	DTR		0x0200
#define	RTS		0x1000

/* addr2 */
#define	TXEN		0x8000
#define	TXSTART		0x0080

#define	RESETSECS	5	/* seconds to wait for master reset */
#define	RESETJUNK	8	/* number of bytes reset puts in the fifo */
#define	RSDIAG	0200		/* some sort of diag message */
#define	RSNULL	0201		/* ok self-test status */
#define	RSSKIP	0203		/* self-test skipped */

char dhvsp[16] =
{
	0, 0, 1, 2, 3, 4 ,0, 5, 6, 7, 8, 10, 11, 13, 14, 0
};

/*
 * interface with i/o system
 */
long	dhvopen();
int	dhvclose(), dhvoput();
static	struct qinit dhvrinit = { noput, NULL, dhvopen, dhvclose, 0, 0 };
static	struct qinit dhvwinit = { dhvoput, NULL, dhvopen, dhvclose, 200, 100 };
static	struct streamtab dhvinfo = { &dhvrinit, &dhvwinit };
struct	cdevsw dhvcdev = cstrinit(&dhvinfo);

extern struct ubaddr dhvaddr[];
extern struct dhv dhv[];
extern int dhvcnt;

/*
 * misc private data
 */
int	dhvoverrun;
int	dhvmiss;			/* chars lost due to q full */

/*
	use tsleep so user can interrupt without wrecking accounting
*/
long
dhvopen(q, d)
	register struct queue *q;
	dev_t d;
{
	register int dev;
	register struct dhv *dhvp;
	register int s;

	dev = minor(d);
	if(dev >= dhvcnt)
		return(0);
	dhvp = &dhv[dev];
	q->ptr = (caddr_t)dhvp;
	WR(q)->ptr = (caddr_t)dhvp;
	/*
		If this is first open, initialise tty state to default.
	 */
	if(((dhvp->state&ISOPEN)==0) || ((dhvp->state&CARRIER)==0)){
		if (dhvinit(dev) == 0)		/* reset the hardware */
			return(0);
		dhvp->flags = ODDP|EPARITY;
		dhvp->ispeed = dhvp->ospeed = SSPEED;
		dhvp->lctl = MODEM|DTR|RTS|RXENABLE;
		dhvp->state = 0;
		s = spl5();
		dhvparam(dhvp);
		while(!(dhvp->state & CARRIER))
			if(tsleep((caddr_t)dhvp, DHPRI, 0) != TS_OK){
				splx(s);
				return(0);
			}
		dhvp->rdq = q;
		dhvp->oblock = 0;
		dhvp->state |= ISOPEN;
		splx(s);
	}
	return 1;
}

/*
 * reset the DHV hardware
 * -- be sure address is correct for first line of board
 */
dhvinit(dev)
int dev;
{
	register struct dhvreg *regs;
	register struct ubaddr *ap;
	register int bd, i, c;
	int s, bad;

	bd = dev / NPER;
	ap = &dhvaddr[bd];
	bd *= NPER;		/* first line of this board */
	if (dhv[bd].regs) {
		dhv[dev].regs = dhv[bd].regs;
		dhv[dev].adno = ap->ubno;
		dhv[dev].line = dev%NPER;
		return (1);		/* already set up */
	}
	if ((regs = (struct dhvreg *)ubaddr(ap)) == 0
	||  ubbadaddr(ap->ubno, &regs->csr, sizeof(short))) {
		printf("dhv11 %d absent\n", bd/NPER);
		return (0);
	}
	s = spl5();
	regs->csr = MRESET;
	for (i = 0; i < RESETSECS; i++) {
		sleep((caddr_t)&lbolt, PZERO);
		if ((regs->csr & MRESET) == 0)
			break;
	}
	splx(s);
	if (regs->csr & (MRESET|DIAGFAIL)) {
		printf("dhv11 %d: bad reset: csr %o\n", bd/NPER, regs->csr);
		return (0);
	}
	bad = 0;
	for (i = 0; i < RESETJUNK; i++) {
		c = regs->rbuf & 0377;
		if ((c & RSDIAG) == 0)		/* just a ROM version */
			continue;
		if (c == RSNULL || c == RSSKIP)
			continue;
		bad++;
		printf("dhv11 %d: diag err %o\n", bd/NPER, c);
	}
	if (bad)
		return (0);
	dhv[dev].regs = regs;
	dhv[dev].adno = ap->ubno;
	dhv[dev].line = dev%NPER;
	if (dev != bd) {
		dhv[bd].regs = regs;
		dhv[bd].adno = ap->ubno;
	}
	return (1);
}

/*
 * Close a DHV11 line.
 */
dhvclose(q)
	register struct queue *q;
{
	register struct dhv *dhvp;
	int s = spl5();

	dhvp = (struct dhv *)q->ptr;
	if(dhvp->oblock){
		freeb(dhvp->oblock);
		dhvp->oblock = 0;
	}
	flushq(WR(q), 1);
	dhvp->rdq = NULL;
	POINT(dhvp->regs, dhvp->line);
	dhvp->regs->lctl = 0;
	dhvp->state = 0;
	splx(s);
}


/*
 * dhv11 write put routine
 */
dhvoput(q, bp)
	register struct queue *q;
	register struct block *bp;
{
	register struct dhv *dhvp = (struct dhv *)q->ptr;
	register struct ttydevb *sp;
	register int s;
	int delaytime;

	switch(bp->type)
	{
	case M_IOCTL:
		sp = (struct ttydevb *)stiodata(bp);
		switch(stiocom(bp))
		{
		case TIOCSDEV:
			delaytime = 0;
			if(dhvp->ispeed != sp->ispeed)
				delaytime = 20;
			dhvp->ospeed = sp->ospeed;
			dhvp->ispeed = sp->ispeed;
			dhvp->flags = sp->flags;
			bp->type = M_IOCACK;
			bp->wptr = bp->rptr;
			qreply(q, bp);
			qpctl1(q, M_DELAY, delaytime);	/* wait a bit */
			qpctl(q, M_CTL);		/* means do dhvparam */
			dhvstart(dhvp);
			return;
		case TIOCGDEV:
			sp->ispeed = dhvp->ispeed;
			sp->ospeed = dhvp->ospeed;
			sp->flags = dhvp->flags & (F8BIT|EVENP|ODDP);
			bp->type = M_IOCACK;
			qreply(q, bp);
			return;
		default:
			bp->wptr = bp->rptr;
			bp->type = M_IOCNAK;
			qreply(q, bp);
			return;
		}
	case M_STOP:
		s = spl5();
		dhvp->state |= DHSTOP;
		freeb(bp);
		dhvstop(dhvp);
		splx(s);
		return;
	case M_START:
		dhvp->state &= ~DHSTOP;
		dhvstart(dhvp);
		break;
	case M_FLUSH:
		flushq(q, 1);
		freeb(bp);
		return;
	case M_BREAK:
		qpctl1(q, M_DELAY, 10);
		putq(q, bp);
		qpctl1(q, M_DELAY, 10);
		dhvstart(dhvp);
		return;
	case M_HANGUP:
		dhvp->state &= ~DHSTOP;
	case M_DELAY:
	case M_DATA:
		putq(q, bp);
		dhvstart(dhvp);
		return;
	default:		/* not handled; just toss */
		break;
	}
	freeb(bp);
}

/*
	Set parameters from open or stty into the DH hardware
	registers.
*/
dhvparam(dhvp)
	register struct dhv *dhvp;
{
	register struct dhvreg *regs;
	register int lpar;
	register s;

	regs = dhvp->regs;
	if(dhvp->ospeed)
		dhvp->lctl |= (DTR|RTS);
	else
		dhvp->lctl &= ~(DTR|RTS);
	lpar = (dhvsp[dhvp->ospeed]<<12) | (dhvsp[dhvp->ispeed]<<8);
	if((dhvp->ospeed) == B134)
		lpar |= BITS6|PENAB;
	else if (dhvp->flags & F8BIT)
		lpar |= BITS8;
	else if(((s = dhvp->flags&(EVENP|ODDP)) == (EVENP|ODDP)) || (s == 0))
		lpar |= BITS8;
	else {
		lpar |= BITS7|PENAB;
		if(dhvp->flags&EVENP)
			lpar |= EPARITY;
	}
	if ((dhvp->ospeed) == B110)	/* 110 baud (ugh!): 2 stop bits */
		lpar |= STOP2;
	dhvp->state &= ~CARRIER;
	POINT(regs, dhvp->line);
	regs->lpr = lpar;
	regs->lctl = dhvp->lctl;
	if(regs->lstat&(DCD<<8))
		dhvp->state |= CARRIER;
}

/*
	DHV11 receiver interrupt.
*/
dhv0int(dev)
	int dev;
{
	register struct block *bp;
	register struct dhvreg *regs;
	register struct dhv *dhvp;
	register int c;
	int hangup;

	if(dev/NPER >= dhvcnt) {
		printf("dhv%d: stray rcv intr\n", dev);
		return;
	}
	regs = dhv[dev*NPER].regs;
	/*
		get chars from the silo for this line
	*/
	while((c = regs->rbuf) < 0) { /* char present */
		dhvp = &dhv[dev*NPER + LINE(c)];
		hangup = 0;
		if(ISMODEM(c)){
			dhvp->state &= ~CARRIER;
			if((c&DCD) == 0)
				hangup = 1;
			else {
				dhvp->state |= CARRIER;
				wakeup((caddr_t)dhvp);
				continue;
			}
			c &= ~(OERR|FERR|PERR);
		}
		if(c&OERR){
			++dhvoverrun;
			continue;
		}
		if(dhvp->rdq == NULL)
			continue;
		if(dhvp->rdq->next->flag & QFULL) {
			dhvmiss++;	/* you lose */
			continue;
		}
		if((bp = allocb(16)) == NULL){
			printf("rint: out of space\n");
			continue;	/* out of space - you lose */
		}
		if(hangup)
			bp->type = M_HANGUP;
		else if(c&FERR)		/* frame error == BREAK */
			bp->type = M_BREAK;
		else
			*bp->wptr++ = c;
		(*dhvp->rdq->next->qinfo->putp)(dhvp->rdq->next, bp);
	}
}


/*
	DHV11 transmitter interrupt. Dev is board number.
	Restart each line which used to be active but has
	terminated transmission since the last interrupt.
*/
dhv1int(dev)
	int dev;
{
	register struct dhvreg *regs;
	register struct dhv *dhvp;
	register struct block *bp;
	register int unit;
	register short csr;

	regs = dhv[dev*NPER].regs;
	while((csr = regs->csr) < 0){
		unit = TXLINE(csr);
		dhvp = &dhv[unit + dev*NPER];
		dhvp->state &= ~BUSY;
		POINT(regs, unit);
		if((csr&(TXACT|TXERR)) == (TXACT|TXERR)){	/* somebody goofed */
			printf("dhv%d: txerr csr=0x%x addr2=0x%x addr1=0x%x\n",
				dev, csr, regs->addr2, regs->addr1);
			regs->cnt = 0;	/* allow progress */
		}
		if(bp = dhvp->oblock){
			bp->rptr = bp->wptr - regs->cnt;
			if(bp->rptr == bp->wptr){
				dhvp->oblock = 0;
				freeb(bp);
			}
		}
		dhvstart(dhvp);
	}
}

dhvtimo(dhvp)
	register struct dhv *dhvp;
{

	if(dhvp->state&BRKING) {
		int s = spl5();
		POINT(dhvp->regs, dhvp->line);
		dhvp->regs->lctl &= ~BREAK;
		splx(s);
	}
	dhvp->state &= ~(TIMEOUT|BRKING);
	dhvstart(dhvp);
}

/*
	Start (restart) transmission on the given DH11 line.
*/
dhvstart(dhvp)
register struct dhv *dhvp;
{
	register struct dhvreg *regs;
	register int unit;
	register int s = spl5();
	register struct block *bp;
	register uaddr_t addr;
	ubm_t um;

	unit = dhvp->line;
	if(dhvp->state & BUSY)
		goto done;
	regs = dhvp->regs;
again:
	if(dhvp->state&(TIMEOUT|DHSTOP|BRKING) || (dhvp->rdq == NULL))
		goto done;
	if((bp = dhvp->oblock) == NULL){
		if((bp = getq(WR(dhvp->rdq))) == NULL)
			goto done;
	}
	switch(bp->type)
	{
	case M_DATA:
		if (bp->wptr <= bp->rptr) {
			freeb(bp);
			dhvp->oblock = 0;
			break;
		}
		if ((um = ubmblk(dhvp->adno, bp, 0)) == 0) {	/* snh */
			freeb(bp);
			break;
		}
		addr = ubadrptr(dhvp->adno, bp, um);
		LOW(regs->csr) = RXIE | (unit&0xF);
		while(regs->addr2&TXSTART)
			printf("dhv: start set\n");
		if(regs->lctl&TXABORT)
			regs->lctl &= ~TXABORT;
		HIGH(regs->csr) = TXIE>>8;
		regs->addr1 = addr;
		regs->cnt = bp->wptr - bp->rptr;
		HIGH(regs->addr2) = TXEN>>8;
		LOW(regs->addr2) = (addr>>16) | TXSTART;
		dhvp->state |= BUSY;
		dhvp->oblock = bp;
		break;
	case M_BREAK:
		dhvp->state |= BRKING|TIMEOUT;
		timeout(dhvtimo, (caddr_t)dhvp, 15);	/* about 250 ms */
		freeb(bp);
		break;
	case M_DELAY:
		dhvp->state |= TIMEOUT;
		timeout(dhvtimo, (caddr_t)dhvp, (int)*bp->rptr + 6);
		freeb(bp);
		break;
	case M_HANGUP:
		dhvp->ispeed = dhvp->ospeed = 0;
		/* fall through */
	case M_CTL:
		freeb(bp);
		dhvparam(dhvp);
		goto again;

	}
done:
	splx(s);
}


/*
	Stop output on a line, e.g. for ^S/^Q or output flush (e.g. ^O).
*/
dhvstop(dhvp)
	register struct dhv *dhvp;
{
	register int s = spl5();

	if(dhvp->state & BUSY){
		/*
			just stop it, tint looks at the count
		*/
		POINT(dhvp->regs, dhvp->line);
		dhvp->regs->lctl |= TXABORT;
	}
	splx(s);
}
