/*
 *  Datakit driver
 */
#include "sys/param.h"
#include "sys/systm.h"
#include "sys/stream.h"
#include "sys/dkio.h"
#include "sys/ubaddr.h"
#include "sys/conf.h"
#include "sys/dk.h"
#include "sys/dkstat.h"
#include "sys/dkmod.h"

#define	SAFETY	1		/* turn off doubtful speed hack */

struct device {
	unsigned short csr;
	unsigned short dko;
	unsigned short dki;
};

struct	device	*DKADDR;	/* assume one addr for now */

extern struct dk dk[];
extern int dkcnt;
extern char dkstate[];

#define	DKXCL	01

int	dkalive;
struct	dkstat	dkstat;
extern	struct	dkmodule dkmod[];
struct	dkmodule *dkmodp;

/*
 * Hardware control bits
 */
#define	DKTENAB	0100
#define	DKRENAB	040
#define	ENABS	(DKRENAB)
#define	DKTDONE	0200
#define	DKRDONE	0100000
#define	D_OSEQ	0
#define	D_READ	01
#define	D_WRITE	02
#define	D_XPACK	03
#define	DKMARK	01000
#define	DKDATA	0400
#define	DKPARITY 0100000

#define	DKOPKT	16	/* biggest output packet */
#define	DKIPKT	16	/* biggest input packet */
#define	DKISML	16	/* smallest interesting allocation */
#define	DKITHRES 200

int	dkclose(), dkput();
long	dkopen();

static	struct qinit dkrinit = { noput, NULL, dkopen, dkclose, 0, 0 };
	struct qinit dkwinit = { dkput, NULL, dkopen, dkclose, 0, 0 };
struct	streamtab dkinfo = { &dkrinit, &dkwinit };
struct	cdevsw dkcdev = cstrinit(&dkinfo);
extern struct ubaddr dkaddr[];

/*
 * open DK channel
 */
long
dkopen(q, dev)
register struct queue *q;
register dev_t dev;
{
	register struct dk *dkp;
	static opened, badtime;
	register chan;

	chan = minor(dev);
	if (chan<=0 || chan>=dkcnt)
		return(0);
	if (!opened) {
		register i;

		if ((DKADDR = (struct device *)ubaddr(&dkaddr[0])) == NULL
		||  badaddr(DKADDR, 2)) {
			printf("dk0 absent\n");
			return (0);
		}
		DKADDR->csr = D_OSEQ;
		DKADDR->dko = 0;	/* Clear fifo's */
		i = 256;		/* sanity count */
		DELAY(1000);		/* wait for board to reset */
		do {
			if (DKADDR->csr & DKTDONE) {
				dkalive = 0;
				if (time > badtime + 300) {
					badtime = time;
					printf("DK interface bad\n");
				}
				return(0);
			}
		} while (--i);
		if ((dkmodp = dkmodall(dev, 0, dkcnt)) == NULL)
			return (0);
		dkmodp->dkstate = dkstate;
		dkalive = 1;
		for (dkp = dk, i = 0; i < dkcnt; dkp++, i++)
			dkp->chan = i;
		DKADDR->csr = ENABS;
		opened++;
		dkrecover(chan);
	}
	dkp = &dk[chan];
	if (dkstate[chan] != DKCLOSED) {	/* already open */
		if (dkp->flag & DKXCL) {
/* printf("DK XCL chan %d state %o flag %o\n", chan, dkstate[chan], dkp->flag); */
			return(0);
		}
		if (dkstate[chan] != DKOPEN) {
/* printf("DK XCL chan %d state %o flag %o\n", chan, dkstate[chan], dkp->flag); */
			return(0);	/* closing channels can't reopen */
		}
		return(1);
	}
	dkp->dkrq = q;
	q->ptr = (caddr_t)dkp;
	WR(q)->ptr = (caddr_t)dkp;
	dkp->flag = DKXCL;
	dkp->icnt = 0;
	dkp->isize = 0;
	dkp->ibuf = 0;
	dkp->ialloc = DKISML;
	dkstate[chan] = DKOPEN;
	return(1);
}

/*
 * Timer to recover from lost interrupt condition.
 */
dkrecover(dev)
dev_t	dev;
{
	register int	ps = spl5();

	if (DKADDR->csr & DKRDONE)
		dk1int(dev);
	splx(ps);

	timeout(dkrecover, (caddr_t)dev, HZ/2);
}

/*
 * close DK channel
 */
dkclose(q)
register struct queue *q;
{
	register struct dk *dkp;
	register struct block *bp;

	dkp = (struct dk *)q->ptr;
	dkp->dkrq = NULL;
	dkp->flag = 0;
	if (dkstate[dkp->chan] == DKRCLOSE || dkmodp->listnrq==NULL)
		dkstate[dkp->chan] = DKCLOSED;
	else if (dkstate[dkp->chan] == DKOPEN)
		dkstate[dkp->chan] = DKLCLOSE;
	if (dkmodp->listnrq)
		putctl2(RD(dkmodp->listnrq), M_PRICTL, DKMCLOSE, dkp->chan);
	if (dkp->ibuf)
		freeb(dkp->ibuf);
	while ((bp = dkp->ofirst) != NULL) {
		dkp->ofirst = bp->next;
		freeb(bp);
	}
	dkp->ibuf = 0;
}

/*
 * DK receiver interrupt.
 */

#define	PUTIN(bp, q)	{\
	dkstat.input += (bp)->wptr - (bp)->rptr;\
	if (((q)->next->flag & QFULL) == 0)\
		(*(q)->next->qinfo->putp)(q->next, bp);\
	else\
		freeb(bp);\
}

#define	DKSIZUP(dkp)	{\
	if (dkp->isize > DKITHRES) {\
		dkp->ialloc = dkp->isize;\
		dkp->icnt = 3;\
	} else if (--dkp->icnt <= 0) {\
		dkp->icnt = 0;\
		dkp->ialloc = DKISML;\
	}\
	dkp->isize = 0;\
}

dk1int(dev)
{
	register struct queue *q;
	register struct block *bp;
	register struct dk *dkp;
	register struct device *reg;
	register c, nbytes;
	register sane, chan;
	register struct block **bpp;
	struct block *blist[DKIPKT+2];

	if ((reg = DKADDR) == NULL)
		return;
	c = 0;
	dkp = NULL;
	while (reg->csr & DKRDONE) {
		reg->csr = D_READ|ENABS;
		if ((c & DKMARK) == 0) {
			/* Search for channel number */
			sane = 256;
			do {
				c = reg->dki;
				if (c & DKMARK || --sane==0)
					break;
			} while (reg->csr & DKRDONE);
		}
		if ((c & DKMARK) == 0) {
			dkstat.markflt++;
			continue;
		}
		if ((c & DKPARITY) == 0) {
			dkstat.markparity++;
			c = 0;
			continue;
		}
		/* check channel */
		chan = c & 0777;
		if (dkp && dkp->chan != chan) {
			if ((bp = dkp->ibuf) != NULL)
				PUTIN(bp, q);
			DKSIZUP(dkp);
			dkp->ibuf = NULL;
		}
		if (chan == 0 || chan >= dkcnt || (q = dk[chan].dkrq)==NULL) {
			if (chan>0 && chan<dkcnt)
				dkstat.closepack++;
			else if (chan == 0)
				dkstat.pack0++;
			else
				dkstat.packstrange++;
			for (nbytes=0; nbytes<DKIPKT; nbytes++) {
				c = reg->dki;
				if (c & DKMARK)
					break;
			}
			continue;
		}
		dkp = (struct dk *)q->ptr;
		if ((bp = dkp->ibuf) == NULL
		&&  (bp = allocb(dkp->ialloc)) == NULL)
			return;
		bpp = blist;
		for (nbytes=0; nbytes<DKIPKT; nbytes++) {
#ifdef SAFETY
			if ((reg->csr&DKRDONE) == 0)
				break;
#endif
			c = reg->dki;
			if ((c&(DKPARITY|DKMARK|DKDATA)) == (DKPARITY|DKDATA)) {
				if (bp->wptr >= bp->lim) {
					*bpp++ = bp;
					if ((bp = allocb(DKISML)) == NULL)
						break;
				}
				*bp->wptr++ = c;
				dkp->isize++;
				continue;
			}
			/*
			 * control or error
			 */
			if (c & DKMARK) {
				dkstat.shortpack++;
				break;
			}
			if ((c & DKPARITY) == 0) {
				dkstat.parity++;
				break;
			}
			if ((c & 0377) == 0)	/* control-null */
				continue;
			if (bp->wptr > bp->rptr) {
				*bpp++ = bp;
				if ((bp = allocb(DKISML)) == NULL)
					break;
			}
			bp->type = M_CTL;
			*bp->wptr++ = c;
		}
		if (bpp == blist && bp->type == M_DATA) {
			dkp->ibuf = bp;
			continue;
		}
		*bpp++ = bp;
		*bpp = NULL;
		bpp = blist;
		while ((bp = *bpp++) != NULL)
			PUTIN(bp, q);
		DKSIZUP(dkp);
		dkp->ibuf = NULL;
	}
	if (dkp && (bp = dkp->ibuf) != NULL) {
		PUTIN(bp, q);
		DKSIZUP(dkp);
		dkp->ibuf = NULL;
	}
}

/*
 * DK put procedure
 */
dkput(q, bp)
register struct queue *q;
register struct block *bp;
{
	register n;
	register bn;
	int s;
	register struct device *reg = DKADDR;
	register struct dk *dkp;

	dkp = (struct dk *)q->ptr;
	if (dkp == NULL) {
		freeb(bp);
		return;
	}
	switch (bp->type) {

	case M_IOCTL:
		bp->type = M_IOCACK;
		switch (stiocom(bp)) {
		case DIOCNXCL:
			dkp->flag &=~ DKXCL;
			bp->wptr = bp->rptr;
			bp->type = M_IOCACK;
			break;

		default:
			bp->type = M_IOCNAK;
			break;
		}
		qreply(q, bp);
		return;

	case M_CTL:
	case M_DATA:
		s = spl6();
		bp->next = NULL;
		if (dkp->ofirst == NULL)
			dkp->ofirst = bp;
		else
			dkp->olast->next = bp;
		dkp->olast = bp;
		if ((bp->class&S_DELIM)==0 || dkalive == 0) {
			splx(s);
			return;
		}
		/*
		 * S_DELIM:
		 * push out data
		 */
		n = DKOPKT;
		reg->csr = D_WRITE + ENABS;
		reg->dko = DKMARK + dkp->chan;
		while ((bp = dkp->ofirst) != NULL) {
			dkp->ofirst = bp->next;
			if (bp->wptr == bp->rptr) {
				freeb(bp);
				continue;
			}
			dkstat.output += bp->wptr - bp->rptr;
			if (n <= 0) {
				reg->csr = D_XPACK + ENABS;
				reg->dko = 0;
				n = DKOPKT;
				reg->csr = D_WRITE + ENABS;
				reg->dko = DKMARK + dkp->chan;
			}
			--n;
			reg->dko = *bp->rptr++ | ((bp->type==M_DATA)?DKDATA:0);
			while ((bn = bp->wptr - bp->rptr) > 0) {
				if (bn > n)
					bn = n;
				n -= bn;
				while (--bn >= 0)
					reg->dko = *bp->rptr++ | DKDATA;
				if (n <= 0) {
					reg->csr = D_XPACK + ENABS;
					reg->dko = 0;
					n = DKOPKT;
					reg->csr = D_WRITE + ENABS;
					reg->dko = DKMARK + dkp->chan;
				}
			}
			freeb(bp);
		}
		reg->csr = D_XPACK + ENABS;
		reg->dko = 0;
		splx(s);
		return;

	case M_PRICTL:
		switch (*bp->rptr) {
		case DKMCLOSE:
			n = bp->rptr[1];
			if (n < dkcnt) {
				if (dkstate[n] == DKOPEN) {
					dkstate[n] = DKRCLOSE;
					putctl(dk[n].dkrq->next, M_HANGUP);
				} else if (dkstate[n] == DKLCLOSE)
					dkstate[n] = DKCLOSED;
			}
			freeb(bp);
			return;

		case DKMXINIT:
			n = bp->rptr[1];
			if (n < dkcnt && dkstate[n] == DKOPEN)
				(*dk[n].dkrq->next->qinfo->putp)(dk[n].dkrq->next, bp);
			else
				freeb(bp);
			return;				

		default:
			freeb(bp);
			return;
		}

	default:
		freeb(bp);
		return;
	}
}

/*
 * transmit interrupt
 */

dk0int(dev)
int dev;
{
	printf("dk%d xmit stray\n", dev);
}

/*
 * unibus reset
 */
dkreset()
{
	if (DKADDR)
		DKADDR->csr = ENABS;
}
