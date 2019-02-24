/*
 *  Raw line discipline and listener stuff for talking to controller
 */
#include "sys/param.h"
#include "sys/stream.h"
#include "sys/dkio.h"
#include "sys/conf.h"
#include "sys/dkstat.h"
#include "sys/dkmod.h"

/*
 * CMC/Listener message structure
 */
struct dialin {
	char	type;
	char	srv;
	short	param0;
	short	param1;
	short	param2;
	short	param3;
	short	param4;
	short	param5;
};

struct dialout {
	short	chan;
	struct	dialin d;
};

/*
 * Message codes
 */
#define	T_LSTNR	4
#define	T_CHG	3
#define	T_REPLY	10
#define	D_CLOSE	1
#define	D_ISCLOSED 2
#define	D_ENQ	3
#define	D_ENQANS 4

/*
 * Raw-mode line discipline for DK (only)
 */
long	rdkopen();
int	rdkclose(), rdkiput(), rdkoput(), rdkosrv();
static	struct qinit rdkrinit = { rdkiput, NULL, rdkopen, rdkclose, 72, 36};
static	struct qinit rdkwinit = { rdkoput, rdkosrv, rdkopen, nulldev, 72, 36};
struct	streamtab rdkstream = { &rdkrinit, &rdkwinit };

extern	struct	dkstat dkstat;
extern	struct	dkmodule dkmod[];
extern	int dkmodcnt;

#define	NDKRAW	32
struct	rdk {
	struct	queue *q;
	struct	dkmodule *module;
	short	chan;
} rdk[NDKRAW];

long
rdkopen(q, dev)
register struct queue *q;
{
	static timer_on = 0;
	register struct rdk *dkp, *edkp;
	register struct dkmodule *dkm;

	if ((dkm = getdkmod(dev)) == NULL)
		return (0);
	edkp = NULL;
	for (dkp=rdk; dkp < &rdk[NDKRAW]; dkp++) {
		if (dkp->q == q) {
			if (dkp->chan != minor(dev) - dkm->lo || WR(q)->ptr==NULL) {
				printf("q %x dkp %x\n", q, dkp);
				panic("rdkopen");
			}
			return(1);
		}
		if (dkp->q == NULL && edkp==NULL)
			edkp = dkp;
	}
	if (edkp==NULL)
		return(0);
	edkp->q = q;
	edkp->module = dkm;
	edkp->chan = minor(dev) - dkm->lo;
	q->flag |= QDELIM;
	q->ptr = 0;
	WR(q)->ptr = (caddr_t)edkp;
	if (timer_on==0) {
		dktimer();
		timer_on = 1;
	}
	return(1);
}

rdkclose(q)
register struct queue *q;
{
	register struct block *bp;
	register struct rdk *dkp = (struct rdk *)WR(q)->ptr;

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
rdkoput(q, bp)
register struct queue *q;
register struct block *bp;
{
	register struct rdk *dkp = (struct rdk *)q->ptr;
	register struct dkmodule *modp = dkp->module;
	register i;

	switch (bp->type) {

	case M_IOCTL:
		switch (stiocom(bp)) {

		/* hang everybody up */
		case DIOCHUP:
			/* must be on listener chan */
			if (q == modp->listnrq) {
				dkmesg(modp, T_LSTNR, 0, 0, 0, 1);
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
				modp->type = CMCLD;
				dkmesg(modp, T_LSTNR, 0, 0, 0, 0);
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
			for (i=modp->lo + 3; i<modp->hi; i+=2) {
				if (modp->dkstate[i]==DKCLOSED) {
					stiodata(bp)[0] = i;
					for (i = 1; i < sizeof(int); i++)
						stiodata(bp)[i] = 0;	/* silly */
					bp->wptr = bp->rptr+STIOCHDR+sizeof(int);
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

rdkosrv(q)
register struct queue *q;
{
	register struct block *bp;

	while ((q->next->flag&QFULL)==0 && (bp = getq(q)))
		(*q->next->qinfo->putp)(q->next, bp);
}

/*
 * input put procedure
 *   -- ignores all control bytes
 */
rdkiput(q, bp)
register struct queue *q;
register struct block *bp;
{
	register struct rdk *dkp = (struct rdk *)WR(q)->ptr;

	switch (bp->type) {

	case M_DATA:
		if (WR(q)==dkp->module->listnrq && dklstnr(dkp->module, bp))
			return;
		if (q->next->flag&QFULL) { /* you lose */
			freeb(bp);
			return;
		}
		bp->class |= S_DELIM;	/* always push through */
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
			if (dkp->module->dkstate[bp->rptr[1]] == DKRCLOSE)
				dkmesg(dkp->module, T_CHG, D_ISCLOSED, 0, bp->rptr[1], 0);
			else
				dkmesg(dkp->module, T_CHG, D_CLOSE, 0, bp->rptr[1], 0);
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
dkmesg(modp, type, srv, p0, p1, p2)
register struct dkmodule *modp;
{
	register struct dialout *dp;
	register struct block *bp;

	if (modp->listnrq==NULL || modp->listnrq->next->flag&QFULL)
		return;
	if ((bp = allocb(sizeof(struct dialout))) == NULL)
		return;		/* hope it will be retried later */
	dp = (struct dialout *)bp->wptr;
	dp->chan = ((struct rdk *)modp->listnrq->ptr)->chan;
	dp->d.type = type;
	dp->d.srv = srv;
	dp->d.param0 = p0;
	dp->d.param1 = p1;
	dp->d.param2 = p2;
	bp->wptr += sizeof(struct dialout);
	bp->class |= S_DELIM;
	(*modp->listnrq->next->qinfo->putp)(modp->listnrq->next, bp);
}

/*
 * Receive message for listener
 */
dklstnr(modp, bp)
register struct block *bp;
struct dkmodule *modp;
{
	register struct dialin *dialp;
	register i;
	register struct queue *listnrq = modp->listnrq;
	register struct rdk *dkp;

	dialp = (struct dialin *)bp->rptr;
	switch (dialp->type) {

	case T_CHG:
		i = dialp->param1;
		if (i <= 0 || i >= modp->hi - modp->lo) {
			dkstat.chgstrange++;
			if (dialp->srv)
				dkmesg(modp, T_CHG, D_ISCLOSED, 0, i, 0);
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
				dkmesg(modp, T_CHG, D_ISCLOSED, 0, i, 0);
				putctl2(listnrq->next, M_PRICTL, DKMCLOSE, i);
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

		case D_ENQ:
			dkmesg(modp, T_CHG, D_ENQANS, 0, i, modp->dkstate[i]);
			break;
		}
		freeb(bp);
		return(1);

	case T_REPLY:	/* CMC sends reply; find, and hand to process */
		i = dialp->param0;
		if (i < 0 || i >= modp->hi - modp->lo)
			return(0);
		for (dkp=rdk; dkp<&rdk[NDKRAW]; dkp++) {
			if (dkp->module==modp && dkp->chan==i) {
				bp->class |= S_DELIM;
				(*dkp->q->next->qinfo->putp)(dkp->q->next, bp);
				return(1);
			}
		}
		return(0);

	default:
		return(0);
	}
}

/*
 * 15-second timer
 * -- should look at dkmod->type, so different
 * listeners can coexist
 */
dktimer()
{
	register i;
	register struct dkmodule *dkp;

	for (dkp = dkmod; dkp < &dkmod[dkmodcnt]; dkp++) {
		if (dkp->listnrq && dkp->type == CMCLD) {
			dkmesg(dkp, T_LSTNR, 0, 0, 0, 0);
			for (i=dkp->hi - dkp->lo - 1; i >= 0; --i)
				if (dkp->dkstate[i] == DKLCLOSE)
					dkmesg(dkp, T_CHG, D_CLOSE, 0, i, 0);
		}
	}
	timeout(dktimer, (caddr_t)NULL, 15*HZ);
}
