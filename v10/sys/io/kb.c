/*
 * simple two-stream kmc datakit driver
 * -- assumes it is the first kmc, and only one allowed for now
 *	(the latter not severe, as 256 channels are permitted)
 */
#include "sys/param.h"
#include "sys/stream.h"
#include "sys/dkio.h"
#include "sys/ubaddr.h"
#include "sys/conf.h"
#include "sys/kb.h"
#include "sys/kmc.h"
#include "sys/dkstat.h"
#include "sys/dkmod.h"

/*
 * hardware stuff
 */

struct device {
	short	sts;	/* written by KMC */
	short	cmd;	/* command and transmit chan -- written by host */
	short	ba;	/* low bus address -- written by host */
	short	bc;	/* negative buffer size -- written by host */
};

/*
 * status info
 */
#define	SCOUNT	0xfff	/* receive buffer resid count */
#define	SRRDY	(020<<8)	/* receive ready */
#define	SOK	(040<<8)	/* init ok */
#define	SERROR	(0100<<8)	/* transfer or other error */
#define	SXRDY	(0200<<8)	/* transmit ready */

/*
 * commands
 */
#define	CGO	(01<<8)		/* turned off when command read */
#define	CRCV	(01<<8)		/* here is a receive buffer */
#define	CSEND	(03<<8)		/* send this buffer of data */
#define	CSCTL	(043<<8)	/* send this data, first char is ctl */
#define	CIE	(020<<8)	/* interrupt after this command */
#define	CXA	(014<<8)	/* high address bits */
#define	XASHIFT	10		/* shift high address bits this much */

#define	STALL	10000		/* max time to stall waiting for ready */
/*
 * per-channel stuff
 */
extern struct kb kb[];
extern int kbcnt;
extern char kbstate[];

/*
 * kb flags
 */
#define	DKXCL	01		/* exclusive open */
#define	DKXWANT	02		/* output pending this channel */

/*
 * per-controller stuff;
 * always just one for now
 */

struct kbkmc kbkmc[1];
#define	KBNO	0	/* always this kb for now */

/*
 * kbkmc flags
 */
#define	INIT	01

/*
 * illicit linkage to other datakit code
 */

struct	dkstat	dkstat;

/*
 * illicit linkage to kmc driver
 */

extern struct kmc kmc[];
extern struct ubaddr kmcaddr[];
extern int kmccnt;
#define	KNO	0		/* kmc0 is ours, by fiat */

#define	DKISIZE	1024		/* preferred input buffer size */
#define	MAXOBUF	4096		/* max allowed output buffer -- for unibus map */

long kbopen();
int kbclose(), kbput(), kbosrv();

static	struct qinit kbrinit = { nodev, NULL, kbopen, kbclose, 0, 0 };
	struct qinit kbwinit = { kbput, NULL, kbopen, kbclose, 1500, 600 };
struct	streamtab kbinfo = { &kbrinit, &kbwinit };
struct	cdevsw kbcdev = cstrinit(&kbinfo);

/*
 * open DK channel
 */
long
kbopen(q, dev)
register struct queue *q;
register dev_t dev;
{
	register struct kb *dkp;
	register struct kbkmc *kk;
	register chan;

	chan = minor(dev);
	if (chan<=0 || chan>=kbcnt)
		return(0);
	kk = &kbkmc[KBNO];
	if ((kk->flags & INIT) == 0) {
		if ((kk->modp = dkmodall(dev, 0, kbcnt)) == NULL)
			return (0);
		kk->modp->dkstate = kbstate;
		if (kbinit(kk) == 0)
			return (0);
		kk->flags |= INIT;
	}
	dkp = &kb[chan];
	if (kbstate[chan] != DKCLOSED) {	/* already open */
		if (dkp->flag & DKXCL)
			return(0);
		if (kbstate[chan] != DKOPEN)
			return(0);	/* closing channels can't reopen */
		return(1);
	}
	dkp->dkrq = q;
	q->ptr = (caddr_t)dkp;
	WR(q)->ptr = (caddr_t)dkp;
	WR(q)->flag |= QNOENB|QBIGB;
	dkp->flag = DKXCL;
	dkp->chan = chan;
	kbstate[chan] = DKOPEN;
	return(1);
}

/*
 * make sure kmc is alive;
 * init data structures once
 */
kbinit(kk)
register struct kbkmc *kk;
{
	register struct kmc *kp;
	register struct device *reg;
	register int i;
	extern kbintr(), kbreset();

	kk->kno = KNO;
	kk->ubno = kmcaddr[KNO].ubno;	/* cheat */
	kp = &kmc[kk->kno];
	if ((reg = kp->k_addr) == 0)
		return (0);
	if ((reg->sts & SOK) == 0)
		return (0);
	if ((kk->omap = ubmalloc(kk->ubno, MAXOBUF, UBDP)) == 0)
		return (0);
	kp->k_rint = kbintr;
	kp->k_xint = kbintr;
	kp->k_reset = kbreset;
	kk->addr = reg;
	i = spl5();
	kbistart(kk);
	splx(i);
	return (1);
}

/*
 * called when, e.g., kmc is reloaded
 */
kbreset(kno)
int kno;
{
	register struct kbkmc *kk;
	register int s;

	kk = &kbkmc[KNO];
	if (kk->obuf) {
		freeb(kk->obuf);
		kk->obuf = NULL;
	}
	if (kk->imap) {
		ubmfree(kk->ubno, kk->imap);
		kk->imap = 0;
	}
	if (kk->ibuf) {
		freeb(kk->ibuf);
		kk->ibuf = NULL;
	}
	s = spl5();
	kbistart(kk);
	splx(s);
}

/*
 * close DK channel
 */
kbclose(q)
register struct queue *q;
{
	register struct kb *dkp;
	register struct kbkmc *kk;

	kk = &kbkmc[KBNO];
	dkp = (struct kb *)q->ptr;
	if (dkp == NULL)
		panic("kbclose");
	dkp->dkrq = NULL;
	dkp->flag = 0;
	if (kbstate[dkp->chan] == DKRCLOSE || kk->modp->listnrq==NULL)
		kbstate[dkp->chan] = DKCLOSED;
	else if (kbstate[dkp->chan] == DKOPEN)
		kbstate[dkp->chan] = DKLCLOSE;
	if (kk->modp->listnrq)
		putctl2(RD(kk->modp->listnrq), M_PRICTL, DKMCLOSE, dkp->chan);
}

/*
 * interrupt -- receiver only
 * try to short-circuit interrupts;
 * data often comes in a large piece, followed by two small ones (URP trailer)
 */

kbintr(dev)
{
	register struct kbkmc *kk;
	register struct block *bp;
	register struct queue *q;
	register int i;

	kk = &kbkmc[KBNO];
	if (kk->addr == NULL)
		return;
	i = kk->addr->sts;
	if ((bp = kk->ibuf) == NULL) {	/* shouldn't happen */
		kbistart(kk);
		return;
	}
	ubmfree(kk->ubno, kk->imap);
	kk->ibuf = NULL;
	kbistart(kk);
	i |= ~SCOUNT;	/* crypto-sign-extend */
	if ((i & SCOUNT) == 0)
		i = 0;	/* overflow just means buffer full */
	bp->wptr = bp->lim + i;		/* lim - residue */
	i = *bp->rptr;			/* channel */
	bp->rptr++;
	if (*bp->rptr)			/* ctl flag */
		bp->type = M_CTL;
	bp->rptr++;
	if (i == 0 || i >= kbcnt || (q = kb[i].dkrq) == NULL) {
		if (i == 0)
			dkstat.pack0++;
		else if (i >= kbcnt)
			dkstat.packstrange++;
		else
			dkstat.closepack++;
		freeb(bp);
		return;
	}
	dkstat.input += bp->wptr - bp->rptr;
	if ((q->next->flag & QFULL) == 0)
		(*q->next->qinfo->putp)(q->next, bp);
	else
		freeb(bp);
}

/*
 * start the next input buffer
 */
kbistart(kk)
register struct kbkmc *kk;
{
	register struct device *reg;
	register int sts;
	register struct block *bp;
	register int n;
	register uaddr_t ua;

	if ((reg = kk->addr) == NULL)
		return;
	if (kk->ibuf)
		return;
	for (sts = 0, n = 0; n < STALL; n++) {
		if ((reg->cmd & CGO) == 0
		&&  (sts = reg->sts) & SRRDY)
			break;
		/* delay to free up unibus? */
	}
	if ((sts & SRRDY) == 0) {
		printf("kb rcv not ready, sts %o cmd %o\n", reg->sts, reg->cmd);
		panic("kbistart");
		return;
	}
	if (sts & SERROR)
		printf("kb err, sts %o\n", sts&0177777);
	if ((bp = allocb(DKISIZE)) == NULL
	||  (kk->imap = ubmblk(kk->ubno, bp, 0)) == 0) {
		panic("kbistart can't alloc\n");	/* later printf */
		if (bp)
			freeb(bp);
		return;
	}
	kk->ibuf = bp;
	if ((int)bp->rptr & 01)
		bp->rptr++;
	kk->iaddr = ua = ubadrptr(kk->ubno, bp, kk->imap);
	reg->bc = bp->rptr - bp->lim;	/* sic - negative count */
	reg->ba = ua;
	n = (ua >> 16) << XASHIFT;
	n &= CXA;
	n |= CIE|CRCV;
	reg->cmd = n;
}

/*
 * put procedure for output
 */
kbput(q, bp)
struct queue *q;
register struct block *bp;
{
	register struct kb *dkp;
	register struct kbkmc *kk;
	register int n;
	register struct device *reg;
	register int sts;
	int s;

	dkp = (struct kb *)q->ptr;
	kk = &kbkmc[KBNO];
	switch (bp->type) {

	case M_IOCTL:
		switch (stiocom(bp)) {
		case DIOCNXCL:
			dkp->flag &=~ DKXCL;
			bp->wptr = bp->rptr;
			bp->type = M_IOCACK;
			break;

		case KIOCINIT:
			/* eventually, reset things here */

		default:
			bp->type = M_IOCNAK;
			break;
		}
		qreply(q, bp);
		return;

	case M_CTL:
	case M_DATA:
		if (bp->rptr == bp->wptr) {
			freeb(bp);
			return;
		}
		dkstat.output += bp->wptr - bp->rptr;
		s = spl5();
		reg = kk->addr;
		for (sts = 0, n = 0; n < STALL; n++) {
			if ((reg->cmd & CGO) == 0
			&&  (sts = reg->sts) & SXRDY)
				break;
			DELAY(40);	/* get off the UNIBUS */
		}
		if ((sts & SXRDY) == 0) {	/* n >= STALL */
			printf("kb xmit not ready, sts %o, cmd %o\n", sts, reg->cmd);
			splx(s);
			freeb(bp);
			return;
		}
		if (sts & SERROR)
			printf("kb err, sts %o\n", sts&0177777);
		ubmflush(kk->ubno, ubmpath(kk->omap));
		if (kk->obuf) {
			freeb(kk->obuf);
			kk->obuf = NULL;
		}
		n = bp->wptr - bp->rptr;
		kk->oaddr = ubmaddr(kk->ubno, bp->rptr, n, kk->omap);
		reg->bc = -n;
		reg->ba = kk->oaddr;
		n = (kk->oaddr >> 16)<<XASHIFT;
		n &= CXA;
		n |= dkp->chan;
		if (bp->type == M_DATA)
			n |= CSEND;
		else
			n |= CSCTL;
		reg->cmd = n;
		kk->obuf = bp;
		splx(s);
		return;

	case M_PRICTL:
		switch (*bp->rptr) {
		case DKMCLOSE:
			n = bp->rptr[1];
			if (n < kbcnt) {
				if (kbstate[n] == DKOPEN) {
					kbstate[n] = DKRCLOSE;
					putctl(kb[n].dkrq->next, M_HANGUP);
				} else if (kbstate[n] == DKLCLOSE)
					kbstate[n] = DKCLOSED;
			}
			freeb(bp);
			return;

		case DKMXINIT:
			n = bp->rptr[1];
			if (n < kbcnt && kbstate[n] == DKOPEN)
				(*kb[n].dkrq->next->qinfo->putp)(kb[n].dkrq->next, bp);
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
