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

#define	TRACE	0	/* turn on to provoke tracing */

/*
 * hardware stuff
 */

struct device {
	char ctl;	/* control -- written by host */
	unsigned char sts;	/* status -- written by device */
	short xlo;	/* xmit descriptor, low address */
	short rlo;	/* rcv descriptor, low address */
	char xhi, rhi;	/* high addresses and flags */
};

/*
 * control bits
 */
#define	CXIE	01	/* transmit interrupt enable */
#define	CRIE	02	/* receive interrupt enable */

/*
 * status bits
 */
#define	SERROR	0200	/* some error, usually NXM */

/*
 * flags in xhi and rhi
 */
#define	CNOADDR	0200	/* OK to give another descriptor address */
#define	CNODESC	0100	/* no descriptor being processed */
#define	CXA	014	/* high address bits */
#define	CXSHIFT	2	/* shift high bits this much */

/*
 * descriptor flags
 */
#define	DVALID	0200	/* valid descriptor for device */
#define	DCHAIN	020	/* `buffer' is a fresh descriptor address */
#define	DXA	014	/* high buffer address bits */
#define	DSCTL	01	/* first char of buffer is a control char */

#define	DXSHIFT	2	/* shift high addr bits this much */

/*
 * per-channel stuff
 */
struct	kb kb[];
extern int kbcnt;
char	kbstate[];

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

#define	RBSIZE	1024	/* desired receive buffer size */
#define	MINRBYTES	4096	/* desired total receive buffer */

#define	XNEXT(i)	((i)>=XRING-1?0:(i)+1)
#define	RNEXT(i)	((i)>=RRING-1?0:(i)+1)

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
	struct device *reg;
	uaddr_t ua;
	extern kbintr(), kbreset();

	kk->kno = KNO;
	kk->ubno = kmcaddr[KNO].ubno;	/* cheat */
	kp = &kmc[kk->kno];
	if ((reg = kp->k_addr) == 0)
		return (0);
	if ((kk->ringmap = ubmalloc(kk->ubno, sizeof(kk->rings), 0)) == 0) {
		printf("kb init no map\n");
		return (0);
	}
	ua = ubmaddr(kk->ubno, (caddr_t)&kk->rings, sizeof(kk->rings), kk->ringmap);
	kk->xrua = ua + ((caddr_t)kk->xring - (caddr_t)&kk->rings);
	kk->rrua = ua + ((caddr_t)kk->rring - (caddr_t)&kk->rings);
	kp->k_rint = kbintr;
	kp->k_xint = kbintr;
	kp->k_reset = kbreset;
	kk->addr = reg;
	kbreset(KNO);
	return (1);
}

/*
 * called when, e.g., kmc is reloaded
 */
kbreset(kno)
int kno;
{
	register struct kbkmc *kk;
	register int i;
	register struct device *reg;

	kk = &kbkmc[kno];
	bzero((caddr_t)&kk->rings, sizeof(kk->rings));
	kk->rrp = 0;
	kk->rwp = 0;
	kk->xrp = 0;
	kk->xwp = 0;
	for (i = 0; i < XRING; i++) {
		if (kk->xmap[i]) {
			ubmfree(kk->ubno, kk->xmap[i]);
			kk->xmap[i] = 0;
		}
		if (kk->xblock[i]) {
			freeb(kk->xblock[i]);
			kk->xblock[i] = 0;
		}
	}
	kk->xring[XRING].loaddr = kk->xrua;
	kk->xring[XRING].flag = DVALID|DCHAIN|((kk->xrua>>16)<<DXSHIFT);
	for (i = 0; i < RRING; i++) {
		if (kk->rmap[i]) {
			ubmfree(kk->ubno, kk->rmap[i]);
			kk->rmap[i] = 0;
		}
		if (kk->rblock[i]) {
			freeb(kk->rblock[i]);
			kk->rblock[i] = 0;
		}
	}
	kk->rring[RRING].loaddr = kk->rrua;
	kk->rring[RRING].flag = DVALID|DCHAIN|((kk->rrua>>16)<<DXSHIFT);
	kk->rbytes = 0;
	reg = kk->addr;
	reg->ctl = CRIE;
	i = spl5();
	kbibufs(kk);
	reg->rlo = kk->rrua;	/* kk->rring[0] */
	reg->rhi = (kk->rrua>>16)<<CXSHIFT;
	splx(i);
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
 * interrupt
 * receive any blocks that have arrived;
 * reload receive buffers;
 * clean up finished transmit buffers
 * restart receiver or transmitter if needed
 */

int kbxrst;		/* debugging count */
int kbxnst;		/* debugging count */

kbintr(dev)
{
	register struct kbkmc *kk;
	register struct device *reg;
	register int i;
	register uaddr_t ua;

	kk = &kbkmc[dev];
	if ((reg = kk->addr) == NULL)
		return;
#if NOTDEF	/* straighten out protocol */
	i = reg->sts;
	if (i & SERROR) {
		reg->sts = 0;
		printf("kb sts %o\n", i);
	}
#endif
	for (i = kk->rrp; i != kk->rwp; i = RNEXT(i)) {
		if (kk->rring[i].flag & DVALID)
			break;
		kbrcv(kk, i);
	}
	if (kk->rbytes < MINRBYTES)
		kbibufs(kk);
	if ((reg->rhi & (CNOADDR|CNODESC)) == (CNOADDR|CNODESC)) {
		/*
		 * receiver has stopped, but may have finished
		 * another block or two since the loop above
		 */
		for (; i != kk->rwp; i = RNEXT(i)) {
			if (kk->rring[i].flag & DVALID)
				break;
			kbrcv(kk, i);
		}
		if (kk->rbytes < MINRBYTES)
			kbibufs(kk);
		if (kk->rwp != i) {	/* safety check */
			ua = kk->rrua + i * sizeof(struct kbdesc);
			reg->rlo = ua;
			reg->rhi = (ua>>16)<<CXSHIFT;
		}
	}
	kk->rrp = i;
	i = reg->xhi;
	if (kk->xrp != kk->xwp)
		kbxscan(kk);
	if ((i & (CNOADDR|CNODESC)) == (CNOADDR|CNODESC)
	&&  kk->xrp != kk->xwp) {
		ua = kk->xrua + kk->xrp * sizeof(struct kbdesc);
		reg->xlo = ua;
		reg->xhi = (ua>>16)<<CXSHIFT;
		kbxrst++;
	}
}

/*
 * receive a block
 */
kbrcv(kk, i)
register struct kbkmc *kk;
register int i;
{
	register struct kbdesc *dxp;
	register struct queue *q;
	register int chan;
	register struct block *bp;
	struct block *xbp;

	dxp = &kk->rring[i];
#if TRACE
	trace('i', (i<<24)|(dxp->chan<<16)|dxp->flag, -dxp->len,
		*(int *)(kk->rblock[i]->rptr));
#endif
	if (kk->rmap[i] == 0)
		panic("kbrcv");
	ubmfree(kk->ubno, kk->rmap[i]);
	kk->rmap[i] = 0;
	bp = kk->rblock[i];
	kk->rblock[i] = NULL;
	kk->rbytes -= bp->lim - bp->wptr;
	bp->wptr = bp->lim + dxp->len;	/* negative residual length */
	if (dxp->flag & DSCTL) {
		/*
		 * special hack to save big blocks
		 */
		if (bp->wptr == bp->rptr + 1
		&&  (xbp = allocb(1)) != NULL) {
			*xbp->wptr++ = *bp->rptr;
			freeb(bp);
			bp = xbp;
		}
		bp->type = M_CTL;
	}
	if (bp->wptr <= bp->base || bp->wptr > bp->lim) {
		printf("kb ch%d bad len %d\n", dxp->chan, -dxp->len);
		freeb(bp);
		return;
	}
	chan = dxp->chan;
	if (chan == 0 || chan >= kbcnt || (q = kb[chan].dkrq) == NULL) {
		if (chan == 0)
			dkstat.pack0++;
		else if (chan >= kbcnt)
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
 * refresh our supply of input buffers
 */
kbibufs(kk)
register struct kbkmc *kk;
{
	register struct block *bp;
	register int i;
	register struct kbdesc *dxp;
	register uaddr_t ua;

	i = kk->rwp;
	while (kk->rbytes < MINRBYTES) {
		if (RNEXT(i) == kk->rrp)	/* all descriptors used */
			break;
		if ((bp = allocb(RBSIZE)) == NULL)
			break;
		if ((kk->rmap[i] = ubmblk(kk->ubno, bp, 0)) == 0) {
			printf("kb no rcv map\n");
			freeb(bp);
			break;
		}
		kk->rblock[i] = bp;
		ua = ubadrptr(kk->ubno, bp, kk->rmap[i]);
		dxp = &kk->rring[i];
		dxp->loaddr = ua;
		dxp->len = bp->wptr - bp->lim;	/* negative length */
		dxp->flag = ((ua>>16)<<DXSHIFT)|DVALID;
		kk->rbytes += bp->lim - bp->wptr;
		i = RNEXT(i);
	}
	kk->rwp = i;
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
	register int i;
	register struct device *reg;
	register struct kbdesc *dxp;
	uaddr_t ua;
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
		s = spl5();
		i = kk->xwp - kk->xrp;
		if (i < 0)
			i += XRING;
		if (i > XRING/2)
			kbxscan(kk);
		if (XNEXT(kk->xwp) == kk->xrp) {	/* no desc available? */
			splx(s);
			printf("kb xmt ch%d lost block\n", dkp->chan);
			freeb(bp);
			return;
		}
		i = kk->xwp;
		kk->xwp = XNEXT(i);
		if ((kk->xmap[i] = ubmblk(kk->ubno, bp, 0)) == 0) {
			splx(s);
			printf("kb xmt ch%d no map\n", dkp->chan);
			freeb(bp);
			return;
		}
#if TRACE
		trace('x', (i<<24)|(dkp->chan<<16)|bp->type, bp->wptr - bp->rptr,
			*(int *)bp->rptr);
#endif
		kk->xblock[i] = bp;
		ua = ubadrptr(kk->ubno, bp, kk->xmap[i]);
		dxp = &kk->xring[i];
		dxp->loaddr = ua;
		dxp->len = bp->rptr - bp->wptr;	/* negative length */
		dxp->chan = dkp->chan;
		dxp->flag = (ua>>16)<<DXSHIFT;
		if (bp->type != M_DATA)
			dxp->flag |= DSCTL;
		reg = kk->addr;
		dxp->flag |= DVALID;
		if ((reg->xhi & (CNOADDR|CNODESC)) == (CNOADDR|CNODESC)) {
			if (i != kk->xrp)
				kbxscan(kk);
			if (kk->xwp != kk->xrp) {	/* maybe it finished it off -- unlikely */
				ua = kk->xrua + kk->xrp * sizeof(struct kbdesc);
				reg->xlo = ua;
				reg->xhi = ((ua>>16)<<CXSHIFT);
			}
		}
		else	/* debug */
			kbxnst++;	/* debug */
		dkstat.output += bp->wptr - bp->rptr;
		splx(s);
		return;

	case M_PRICTL:
		switch (*bp->rptr) {
		case DKMCLOSE:
			i = bp->rptr[1];
			if (i < kbcnt) {
				if (kbstate[i] == DKOPEN) {
					kbstate[i] = DKRCLOSE;
					putctl(kb[i].dkrq->next, M_HANGUP);
				} else if (kbstate[i] == DKLCLOSE)
					kbstate[i] = DKCLOSED;
			}
			freeb(bp);
			return;

		case DKMXINIT:
			i = bp->rptr[1];
			if (i < kbcnt && kbstate[i] == DKOPEN)
				(*kb[i].dkrq->next->qinfo->putp)(kb[i].dkrq->next, bp);
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
 * clean up finished transmit buffers
 * call at spl5
 */

kbxscan(kk)
register struct kbkmc *kk;
{
	register int i;

	for (i = kk->xrp; i != kk->xwp; i = XNEXT(i)) {
		if (kk->xring[i].flag & DVALID)
			break;
		if (kk->xblock[i] == NULL)
			panic("kbxscan");
		ubmfree(kk->ubno, kk->xmap[i]);
		kk->xmap[i] = 0;
		freeb(kk->xblock[i]);
		kk->xblock[i] = NULL;
	}
	kk->xrp = i;
}
