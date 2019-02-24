/*
 *  Raw line discipline and communications with unixp datakit controller
 */
#include "sys/param.h"
#include "sys/stream.h"
#include "sys/dkio.h"
#include "sys/conf.h"
#include "sys/dkstat.h"
#include "sys/dkmod.h"

/*
 * Unixp message structure
 */
struct dialin {
	char	type;
	char	srv;
	short	param0;
	short	param1;
	short	param2;
	short	param3;
	short	param4;
};

/*
 * Message codes
 */
#define	T_SRV	1
#define	T_REPLY	2
#define	T_CHG	3
#define	T_ALIVE	4
#define	T_RESTART 8
#define	D_CLOSE	1
#define	D_ISCLOSED 2
#define	D_CLOSEALL 3
#define	D_OK	1
#define	D_OPEN	2
#define D_FAIL	3
#define	D_RESTART 1
#define D_XINIT	7

/*
 * Raw-mode line discipline for DK (only)
 */
long	xpopen();
int	xpclose(), xpiput(), xpoput(), xposrv();
static	struct qinit xprinit = { xpiput, NULL, xpopen, xpclose, 72, 36};
static	struct qinit xpwinit = { xpoput, xposrv, xpopen, nulldev, 72, 36};
struct	streamtab xpstream = { &xprinit, &xpwinit };

extern	struct	dkstat dkstat;
extern struct dkmodule dkmod[];
extern	int dkmodcnt;

#define	NDKRAW	32
struct	xp {
	struct	queue *q;
	struct	dkmodule *module;
	short	chan;
} xp[NDKRAW];

long
xpopen(q, dev)
register struct queue *q;
{
	static timer_on = 0;
	register struct xp *dkp, *edkp;
	register struct dkmodule *dkm;

	if ((dkm = getdkmod(dev)) == NULL)
		return (0);
	edkp = NULL;
	for (dkp=xp; dkp < &xp[NDKRAW]; dkp++) {
		if (dkp->q == q) {
			if (dkp->chan != minor(dev) - dkm->lo || WR(q)->ptr==NULL) {
				printf("q %x dkp %x\n", q, dkp);
				panic("xpopen");
			}
			return(1);
		}
		if (dkp->q == NULL && edkp==NULL)
			edkp = dkp;
	}
	if (edkp==NULL)
		return(0);
	edkp->q = q;
	edkp->chan = minor(dev) - dkm->lo;
	edkp->module = dkm;
	q->flag |= QDELIM;
	q->ptr = 0;
	WR(q)->ptr = (caddr_t)edkp;
	if (timer_on==0) {
		xptimer();
		timer_on = 1;
	}
	return(1);
}

xpclose(q)
register struct queue *q;
{
	register struct block *bp;
	register struct xp *dkp = (struct xp *)WR(q)->ptr;

if (dkp==NULL) printf("null ptr; q %x wq %x\n", q, WR(q));
	if (WR(q)==dkp->module->listnrq) {
		dkp->module->listnrq = NULL;
		dkp->module->type = 0;
	}
	while (bp = getq(q))
		(*q->next->qinfo->putp)(q->next, bp);
	dkp->q = NULL;
	dkp->module = NULL;
}

/*
 * output put procedure.
 */
xpoput(q, bp)
register struct queue *q;
register struct block *bp;
{
	register struct xp *dkp = (struct xp *)q->ptr;
	register struct dkmodule *modp = dkp->module;
	register i;

	switch (bp->type) {

	case M_IOCTL:
		switch (stiocom(bp)) {

		/* hang everybody up */
		case DIOCHUP:
			/* must be on low channels */
			if (dkp->chan < 5) {
				xpmesg(modp, T_ALIVE, D_RESTART, 0);
				bp->type = M_IOCACK;
				bp->rptr = bp->wptr;
				qreply(q, bp);
				return;
			}
			break;

		/* announce listener channel */
		case DIOCLHN:
			if (modp->listnrq == NULL) {
				modp->listnrq = q;
				modp->type = UNIXPLD;
				xpmesg(modp, T_ALIVE, 0, 0);
				bp->type = M_IOCACK;
				bp->rptr = bp->wptr;
				qreply(q, bp);
				return;
			}
			break;

		/* delay input */
		case DIOCSTOP:
			RD(q)->ptr = (caddr_t)1;
			bp->type = M_IOCACK;
			bp->rptr = bp->wptr;
			qreply(q, bp);
			return;

		/* suggest a free outgoing channel */
		case DIOCCHAN:
			for (i=modp->lo + 2; i<modp->hi; i++) {
				if (modp->dkstate[i]==DKCLOSED) {
					*(int *)stiodata(bp) = i;
					bp->wptr = bp->rptr+sizeof(int)+STIOCHDR;
					bp->type = M_IOCACK;
					break;
				}
			}
			qreply(q, bp);
			return;

		default:
			(*q->next->qinfo->putp)(q->next, bp);
			return;
		}
		bp->type = M_IOCNAK;
		bp->wptr = bp->rptr;
		qreply(q, bp);
		return;
	}
	putq(q, bp);
}

xposrv(q)
register struct queue *q;
{
	register struct block *bp;

	while ((q->next->flag&QFULL)==0 && (bp = getq(q)))
		(*q->next->qinfo->putp)(q->next, bp);
}

/*
 * input put procedure
 *   -- ignores almost all control bytes
 */
xpiput(q, bp)
register struct queue *q;
register struct block *bp;
{
	register struct xp *dkp = (struct xp *)WR(q)->ptr;

	switch (bp->type) {

	case M_DATA:
		if (WR(q)==dkp->module->listnrq && xplstnr(dkp->module, bp))
			return;
		if (q->next->flag&QFULL) { /* sorry, you lose */
			freeb(bp);
			return;
		}
		bp->class |= S_DELIM;
		if (q->ptr) {	/* input delayed */
			putq(q, bp);
			return;
		}
		(*q->next->qinfo->putp)(q->next, bp);
		return;

	case M_IOCACK:
	case M_IOCNAK:
	case M_HANGUP:
		(*q->next->qinfo->putp)(q->next, bp);
		return;

	case M_PRICTL:
		switch (*bp->rptr) {
		case DKMCLOSE:
			dkp->module->dkstate[bp->rptr[1]] = DKLCLOSE;
			xpmesg(dkp->module, T_CHG, D_CLOSE, bp->rptr[1]);
			freeb(bp);
			return;

		default:
			freeb(bp);
			return;
		}

	default:
		freeb(bp);
	}
}

/*
 * listener sends a message to CMC
 */
xpmesg(modp, type, srv, p0)
register struct dkmodule *modp;
{
	register struct dialin *dp;
	register struct block *bp;

	if (modp->listnrq==NULL || modp->listnrq->next->flag&QFULL)
		return;
	if ((bp = allocb(sizeof(struct dialin))) == NULL)
		return;		/* hope it will be retried later */
	dp = (struct dialin *)bp->wptr;
	dp->type = type;
	dp->srv = srv;
	dp->param0 = p0;
	dp->param1 = 0;
	dp->param2 = 0;
	dp->param3 = 0;
	dp->param4 = 0;
	bp->wptr += sizeof(struct dialin);
	bp->class |= S_DELIM;
	(*modp->listnrq->next->qinfo->putp)(modp->listnrq->next, bp);
}

/*
 * Receive message for listener
 */
xplstnr(modp, bp)
register struct block *bp;
struct dkmodule *modp;
{
	register struct dialin *dialp;
	register i;
	register struct queue *listnrq = modp->listnrq;
	register struct xp *dkp;
	int t;

	dialp = (struct dialin *)bp->rptr;
	i = dialp->param0;
	t = dialp->param1;

	switch (dialp->type) {

	case T_CHG:
		if (dialp->srv == D_CLOSEALL) {
			printf("datakit closing all chans\n");
			for(i=modp->hi-modp->lo-1; i > 0; i--) {
				if(((struct xp *)listnrq->ptr)->chan == i)
					continue;
				switch (modp->dkstate[i]) {

				case DKRCLOSE:
					dkstat.notclosed++;
				case DKOPEN:
					printf("closing %d\n", i);
					putctl2(listnrq->next, M_PRICTL, DKMCLOSE, i);
					break;

				case DKLCLOSE:
					printf("closed %d\n", i);
				case DKCLOSED:
					modp->dkstate[i] = DKCLOSED;
					break;
				}
			}
			freeb(bp);
			return(1);
		}
		if (i <= 0 || i >= modp->hi - modp->lo) {
			dkstat.chgstrange++;
			if (dialp->srv)
				xpmesg(modp, T_CHG, D_CLOSE, i);
			freeb(bp);
			return(1);
		}
		switch (dialp->srv) {

		case D_CLOSE:		/* remote shutdown */
			switch (modp->dkstate[i]) {

			case DKRCLOSE:
				dkstat.notclosed++;
			case DKOPEN:
				putctl2(listnrq->next, M_PRICTL, DKMCLOSE, i);
				break;

			case DKLCLOSE:
			case DKCLOSED:
				xpmesg(modp, T_CHG, D_CLOSE, i);
				break;
			}
			break;
		
		case D_ISCLOSED:
			switch (modp->dkstate[i]) {

			case DKLCLOSE:
			case DKCLOSED:
				putctl2(listnrq->next, M_PRICTL, DKMCLOSE, i);
				break;

			case DKOPEN:
			case DKRCLOSE:
				dkstat.isclosed++;
				break;
			}
			break;
		}
		freeb(bp);
		return(1);

	case T_REPLY:	/* CMC sends reply; find, and hand to process */
		if (i < 0 || i >= modp->hi - modp->lo)
			return(0);
		for (dkp=xp; dkp<&xp[NDKRAW]; dkp++) {
			if (dkp->module==modp && dkp->chan==i) {
				(*dkp->q->next->qinfo->putp)(dkp->q->next, bp);
				return(1);
			}
		}
		return(0);

	case T_SRV:
		/* see if we've been spliced */
		if (dialp->srv == D_XINIT) {
			if (i < 0 || i >= modp->hi - modp->lo)
				return(0);
			freeb(bp);
			if((bp = allocb(4))==0)
				return(0);
			*bp->wptr++ = DKMXINIT;
			*bp->wptr++ = i;
			*bp->wptr++ = t>>8;
			*bp->wptr++ = t;
			bp->type = M_PRICTL;
			(*listnrq->next->qinfo->putp)(listnrq->next, bp);
			return(1);
		}
		return(0);

	case T_RESTART:
		/* datakit has rebooted, tell it what we think is closed */
		printf("datakit restart\n");
		for(i = modp->hi - modp->lo - 1; i > 0; --i) {
			switch (modp->dkstate[i]) {
			case DKLCLOSE:
				xpmesg(modp, T_CHG, D_CLOSE, i);
				break;
			}
		}
		freeb(bp);
		return(1);

	default:
		return(0);
	}
}

/*
 * 15-second timer
 */
xptimer()
{
	register i;
	register struct dkmodule *dkp;

	for (dkp = dkmod; dkp < &dkmod[dkmodcnt]; dkp++) {
		if (dkp->listnrq && dkp->type == UNIXPLD) {
			xpmesg(dkp, T_ALIVE, 0, 0);
			for (i=dkp->hi - dkp->lo - 1; i >= 0; --i)
				if (dkp->dkstate[i] == DKLCLOSE)
					xpmesg(dkp, T_CHG, D_CLOSE, i);
		}
	}
	timeout(xptimer, (caddr_t)NULL, 15*HZ);
}
