/*
 *  DZ-11 Driver
 */

/* Modified 4/84 by J. Wolitzky to allow dialout operation with smart modems */

#define	TRC(c)

#include "sys/param.h"
#include "sys/stream.h"
#include "sys/ttyio.h"
#include "sys/ttyld.h"
#include "sys/ubaddr.h"
#include "sys/dz.h"
#include "sys/conf.h"

#define	LPB	8	/* lines per board */
#define	BOARD(d)	((d)>>3)
#define	LINE(d)		((d)&07)

#define BITS7	020
#define BITS8	030
#define TWOSB	040
#define PENABLE	0100
#define OPAR	0200
#define	CLR	020		/* Clear */
#define MSE	040		/* Master Scan Enable */
#define RIE	0100		/* Receiver Interrupt Enable */
#define	SAE	010000		/* Silo Alarm Enable */
#define TIE	040000		/* Transmit Interrupt Enable */
#define	TRDY	0100000		/* Transmitter ready */
#define DZ_IEN	(MSE+RIE+TIE)
#define PERROR	010000
#define FRERROR	020000
#define	OVERRUN	040000
#define SSPEED	B1200		/* std speed = 1200 baud */

#define	ISOPEN	02
#define	WOPEN	04
#define	TIMEOUT	010
#define	CARR_ON	020
#define	DZSTOP	040
#define	HPCL	0100
#define	BRKING	0200
#define	DIALOUT	0400	/* set when used as dialout with smart modems */

#define	DZPRI	30

struct device {
	short	dzcsr;
	short	dzrbuf;
#define	dzlpr	dzrbuf
	char	dztcr;
	char	dzdtr;
	char	dztbuf;
#define	dzrind	dztbuf
	char	dzbrk;
#define	dzmsr	dzbrk
};

extern struct	dz dz[];		/* one per line */
extern struct	ubaddr dzaddr[];	/* one per board */
extern	int dzcnt;		/* number of lines */
int	dzoverrun;

char	dz_speeds[] = {
	0, 020, 021, 022, 023, 024, 0, 025,
	026, 027, 030, 032, 034, 036, 037, 0
};

long	dzopen();
int	dzclose(), dzoput();

static	struct qinit dzrinit = { noput, NULL, dzopen, dzclose, 0, 0 };
	struct qinit dzwinit = { dzoput, NULL, dzopen, dzclose, 200, 100 };
struct	streamtab dzinfo = { &dzrinit, &dzwinit };
struct	cdevsw dzcdev = cstrinit(&dzinfo);

int	dzmiss;

long
dzopen(q, d)
register struct queue *q;
dev_t d;
{
	register dev;
	register struct dz *dzp;
	register int s;
	static int onceinit = 0;
 
	dev = minor(d);
	if (dev >= dzcnt)
		return(0);
	dzp = &dz[dev];
	if ((dzp->addr = (struct device *)ubaddr(&dzaddr[BOARD(dev)])) == NULL
	||  ubbadaddr(dzaddr[BOARD(dev)].ubno, dzp->addr, sizeof(short))) {
		printf("dz%d absent\n", BOARD(dev));
		return (0);
	}
	dz[BOARD(dev)*LPB].addr = dzp->addr;	/* see intr code */
	if (onceinit == 0) {
		onceinit++;
		dzscan((caddr_t)0);
	}
	dzp->board = BOARD(dev);
	dzp->line = LINE(dev);
	q->ptr = (caddr_t)dzp;
	WR(q)->ptr = (caddr_t)dzp;
	dzscan((caddr_t)1);			/* update CARR_ON */
	if ((dzp->state&ISOPEN)==0 || (dzp->state&CARR_ON)==0) {
		s = spl5();
		for (;;) {
			dzp->flags = F8BIT|ODDP|EVENP;
			dzp->speed = SSPEED;
			dzparam(dzp);
			if (dzp->state & CARR_ON)
				break;
#if NOTDEF
			dzflag = dzboard[BOARD(dev)]->ui_flags;
			/* ignore carrier? */
			if (dzflag & (1 << (LINE(dev)))) {
				dzp->state |= (DIALOUT | CARR_ON);
				break;
			}
#endif
			if (tsleep((caddr_t)dzp, DZPRI, 0) != TS_OK) {
				wakeup((caddr_t)dzp);
				dzp->speed = 0;
				dzparam(dzp);
				splx(s);
				return(0);
			}
		}
		dzp->rdq = q;
		dzp->state |= ISOPEN;
		splx(s);
	}
	TRC('o');
	return(1);
}

dzclose(q)
register struct queue *q;
{
	register struct dz *dzp;
	register s;

	dzp = (struct dz *)q->ptr;
	s = spl5();
	if (dzp->oblock) {
		register struct block *bp = dzp->oblock;
		dzp->oblock = NULL;
		freeb(bp);
	}
	flushq(WR(q), 1);
	dzp->rdq = NULL;
	if (dzp->state&HPCL || (dzp->state&CARR_ON)==0) {
		dzp->speed = 0;
		dzparam(dzp);
	}
	dzp->state = 0;
	splx(s);
	TRC('c');
}

/*
 * DZ write put routine
 */
dzoput(q, bp)
register struct queue *q;
register struct block *bp;
{
	register struct dz *dzp = (struct dz *)q->ptr;
	register s;
	register struct ttydevb *tb;
	register delaytime;

	TRC('p');
	switch(bp->type) {

	case M_IOCTL:
		TRC('i');
		switch (stiocom(bp)) {

		case TIOCSDEV:
			tb = (struct ttydevb *)stiodata(bp);
			delaytime = 0;
			if (dzp->speed != tb->ispeed)
				delaytime = 20;
			dzp->speed = tb->ispeed;
			dzp->flags = tb->flags;
			bp->type = M_IOCACK;
			bp->wptr = bp->rptr;
			qreply(q, bp);
			qpctl1(q, M_DELAY, delaytime);	/* wait a bit */
			qpctl(q, M_CTL);		/* means do dzparam */
			dzstart(dzp);
			return;

		case TIOCGDEV:
			tb = (struct ttydevb *)stiodata(bp);
			tb->ispeed = tb->ospeed = dzp->speed;
			tb->flags = dzp->flags;
			bp->wptr = bp->rptr + sizeof(struct ttydevb) + STIOCHDR;
			bp->type = M_IOCACK;
			qreply(q, bp);
			return;

		case TIOCHPCL:
			dzp->state |= HPCL;
			bp->type = M_IOCACK;
			bp->wptr = bp->rptr;
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
		dzp->state |= DZSTOP;
		freeb(bp);
		if (bp = dzp->oblock) {
			dzp->oblock = NULL;
			putbq(q, bp);
		}
		splx(s);
		return;

	case M_START:
		dzp->state &= ~DZSTOP;
		dzstart(dzp);
		break;

	case M_FLUSH:
		flushq(q, 1);
		freeb(bp);
		s = spl5();
		if (bp = dzp->oblock) {
			dzp->oblock = NULL;
			freeb(bp);
		}
		splx(s);
		return;

	case M_BREAK:
		qpctl1(q, M_DELAY, 10);
		putq(q, bp);
		qpctl1(q, M_DELAY, 10);
		dzstart(dzp);
		return;

	case M_HANGUP:
		dzp->state &= ~DZSTOP;
	case M_DELAY:
	case M_DATA:
		putq(q, bp);
		TRC('d');
		dzstart(dzp);
		return;

	default:		/* not handled; just toss */
		break;
	}
	freeb(bp);
}
 
/*
 * Receive interrupt.  Argument is DZ board number
 */
dz0int(dev)
{
	register struct dz *dzp;
	register struct block *bp;
	register struct device *dzaddr;
	register int c;
 
	if (dev >= BOARD(dzcnt)) {
		printf("dz%d bad rcv int\n", dev);
		return;
	}
	dzaddr = dz[dev*LPB].addr;
	while ((c = dzaddr->dzrbuf) < 0) {	/* char present */
		dzp = &dz[dev*LPB + ((c>>8)&07)];
		if (c&(OVERRUN/*|PERROR*/)) {
			if (c&OVERRUN)
				dzoverrun++;
			continue;
		}
		if (dzp->rdq == NULL)
			continue;
		if (dzp->rdq->next->flag&QFULL) {
			dzmiss++;
			continue;
		}
		if ((bp = allocb(16)) == NULL)
			continue;			/* no space */
		if (c&FRERROR)			/* frame err (break) */
			bp->type = M_BREAK;
		else
			*bp->wptr++ = c;
		(*dzp->rdq->next->qinfo->putp)(dzp->rdq->next, bp);
	}
}
 
/*
 * set device parameters
 */
dzparam(dzp)
register struct dz *dzp;
{
	register short lpr;
	register struct device *dzaddr;

	dzaddr = dzp->addr; 
	dzaddr->dzcsr = DZ_IEN;
	if (dzp->speed == 0) {
		dzaddr->dzdtr &= ~(1 << dzp->line);	/* hang up */
		return;
	}
	dzaddr->dzdtr |= 1 << dzp->line;
	lpr = (dz_speeds[dzp->speed]<<8) | dzp->line;
	if (dzp->flags & F8BIT)
		lpr |= BITS8;
	else
		lpr |= (BITS7|PENABLE);
	if ((dzp->flags & EVENP) == 0)
		lpr |= OPAR;
	if (dzp->speed == 3)
		lpr |= TWOSB; 			/* 110 baud: 2 stop bits */
	dzaddr->dzlpr = lpr;
}
 
/*
 * Transmitter interrupt. dev is board number.
 */
dz1int(dev)
{
	register struct device *dzaddr;
	register struct dz *dzp;
	register struct block *bp;
	register sts;
 
	if (dev >= BOARD(dzcnt)) {
		printf("dz%d bad xmt int\n", dev);
		return;
	}
	dzaddr = dz[dev*LPB].addr;
	while ((sts = dzaddr->dzcsr) & TRDY) {
		dzp = &dz[dev*LPB + ((sts>>8)&07)];
		if (bp = dzp->oblock) {
			if (bp->rptr < bp->wptr) {
				dzaddr->dztbuf = *bp->rptr++;
				continue;
			}
			freeb(bp);
			dzp->oblock = NULL;
		}
		dzaddr->dztcr &= ~(1 << (dzp->line));
		dzstart(dzp);
	}
}

dztimo(dzp)
register struct dz *dzp;
{
	if (dzp->state&BRKING) {
		dzp->brking &= ~(1 << dzp->line);
		dzp->addr->dzbrk = dzp->brking;
	}
	dzp->state &= ~(TIMEOUT|BRKING);
	dzstart(dzp);
}

dzstart(dzp)
register struct dz *dzp;
{
	register s = spl5();
	register struct block *bp;
	register struct device *dzaddr;
 
	TRC('s');
again:
	if (dzp->state & (TIMEOUT|DZSTOP|BRKING) || dzp->oblock) {
		TRC('t');
		goto out;
	}
	if (dzp->rdq == NULL)
		goto out;
	if ((bp = getq(WR(dzp->rdq))) == NULL) {
		TRC('n');
		goto out;
	}
	switch (bp->type) {

	case M_DATA:
		dzp->oblock = bp;
		dzaddr = dzp->addr;
		dzaddr->dztcr |= 1 << (dzp->line);
		break;

	case M_BREAK:
		dzaddr = dzp->addr;
		dzp->brking |= 1 << dzp->line;
		dzaddr->dzbrk = dzp->brking;
		dzp->state |= BRKING|TIMEOUT;
		timeout(dztimo, (caddr_t)dzp, HZ/4);	/* about 250 ms */
		freeb(bp);
		break;

	case M_DELAY:
		dzp->state |= TIMEOUT;
		timeout(dztimo, (caddr_t)dzp, (int)*bp->rptr + 6);
		freeb(bp);
		break;

	case M_HANGUP:
		dzp->speed = 0;
	case M_CTL:
		freeb(bp);
		dzparam(dzp);
		goto again;

	}
out:
	splx(s);
}
 
dzscan(timo)
caddr_t timo;
{
	register struct device *dzaddr;
	register struct dz *dzp;
	register i;
 
	for (i=0, dzp=dz; i<dzcnt; dzp++, i++) {
		if (dzp->addr == 0)
			continue;
		dzaddr = dzp->addr;
		if (dzaddr->dzmsr & (1<<dzp->line)) {
			/* carrier present */
			if ((dzp->state & CARR_ON)==0)
				wakeup((caddr_t)dzp);
			dzp->state |= CARR_ON;
		} else if ((dzp->state & CARR_ON) && !(dzp->state & DIALOUT)) {
			/* carrier lost */
			if (dzp->state&ISOPEN) {
				dzaddr->dzdtr &= ~(1<<dzp->line);
				if (dzp->rdq)
					putctl(dzp->rdq->next,M_HANGUP);
			}
			dzp->state &= ~CARR_ON;
		}
	}
	if (timo == 0)
		timeout(dzscan, (caddr_t)0, 2*HZ);
}
