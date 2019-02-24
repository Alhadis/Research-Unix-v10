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
#define T_CALL	'0'
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
long	adkopen();
int	adkclose(), adkiput(), adkoput(), adkosrv(), adkisrv();
static	struct qinit adkrinit = { adkiput, adkisrv, adkopen, adkclose, 72, 36};
static	struct qinit adkwinit = { adkoput, adkosrv, adkopen, nulldev, 72, 36};
struct	streamtab adkstream = { &adkrinit, &adkwinit };

extern	struct	dkstat dkstat;
extern	struct	dkmodule dkmod[];
extern	int dkmodcnt;

#define	NDKRAW	32
struct	adk {
	struct	queue *q;
	struct	dkmodule *module;
	short	chan;
} adk[NDKRAW];

long
adkopen(q, dev)
register struct queue *q;
{
	static timer_on = 0;
	register struct adk *dkp, *edkp;
	register struct dkmodule *dkm;

	if ((dkm = getdkmod(dev)) == NULL)
		return (0);
	edkp = NULL;
	for (dkp=adk; dkp < &adk[NDKRAW]; dkp++) {
		if (dkp->q == q) {
			if (dkp->chan != minor(dev) - dkm->lo || WR(q)->ptr==NULL) {
				printf("q %x dkp %x\n", q, dkp);
				panic("adkopen");
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
		adktimer();
		timer_on = 1;
	}
	return(1);
}

adkclose(q)
register struct queue *q;
{
	register struct block *bp;
	register struct adk *dkp = (struct adk *)WR(q)->ptr;

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
adkoput(q, bp)
register struct queue *q;
register struct block *bp;
{
	register struct adk *dkp = (struct adk *)q->ptr;
	register struct dkmodule *modp = dkp->module;
	register i;

	switch (bp->type) {

	case M_IOCTL:
		switch (stiocom(bp)) {

		/* hang everybody up */
		case DIOCHUP:
			/* must be on listener chan */
			if (q == modp->listnrq) {
				adkmesg(modp, T_LSTNR, 0, 0, 0, 1);
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
				RD(q)->flag |= QNOENB;
				modp->type = CMCLD;
				adkmesg(modp, T_LSTNR, 0, 0, 0, 0);
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

adkosrv(q)
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
adkiput(q, bp)
register struct queue *q;
register struct block *bp;
{
	register struct adk *dkp = (struct adk *)WR(q)->ptr;
	register int delim;

	switch (bp->type) {

	case M_DATA:
		if (WR(q) == dkp->module->listnrq) {
			delim = bp->class & S_DELIM;
			putq(q, bp);
			if (delim)
				qenable(q);
			return;
		}
		if (q->next->flag&QFULL) { /* you lose */
			freeb(bp);
			return;
		}
	/*	bp->class |= S_DELIM; always push through; this seems wrong */
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
				adkmesg(dkp->module, T_CHG, D_ISCLOSED, 0, bp->rptr[1], 0);
			else
				adkmesg(dkp->module, T_CHG, D_CLOSE, 0, bp->rptr[1], 0);
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
adkmesg(modp, type, srv, p0, p1, p2)
register struct dkmodule *modp;
{
	register struct dialout *dp;
	register struct block *bp;

	if (modp->listnrq==NULL || modp->listnrq->next->flag&QFULL)
		return;
	if ((bp = allocb(sizeof(struct dialout))) == NULL)
		return;		/* hope it will be retried later */
	dp = (struct dialout *)bp->wptr;
	dp->chan = ((struct adk *)modp->listnrq->ptr)->chan;
	dp->d.type = type;
	dp->d.srv = srv;
	dp->d.param0 = p0;
	dp->d.param1 = p1;
	dp->d.param2 = p2;
	bp->wptr += sizeof(struct dialout);
	bp->class |= S_DELIM;
	(*modp->listnrq->next->qinfo->putp)(modp->listnrq->next, bp);
}


/* try to construct a struct dialin from the message */
struct block *
adkdialin(q, bp)
register struct queue *q;
register struct block *bp;
{
	register struct block *nbp;
	register int len;
	register int delim;
	register int n;
	register int over;

	if ((len = bp->wptr - bp->rptr) == sizeof(struct dialin))
		return bp;
	n = sizeof(struct dialin) - len;
	delim = bp->class & S_DELIM;

	if (bp->lim - bp->rptr >= sizeof(struct dialin))
		nbp = bp;
	else if ((nbp = allocb(sizeof(struct dialin))) == NULL) {
		freeb(bp);
		return NULL;
	} else {
		bcopy(bp->rptr, nbp->wptr, len);
		nbp->wptr += len;
		freeb(bp);
	}

	over = 0;
	while (!delim && (bp = getq(q))) {
		len = bp->wptr - bp->rptr <= n ? bp->wptr - bp->rptr : (over = 1, n);
		bcopy(bp->rptr, nbp->wptr, len);
		nbp->wptr += len;
		n -= len;
		delim = bp->class & S_DELIM;
		freeb(bp);
	}
	if (n == 0 && !over && delim)
		return nbp;
	freeb(nbp);
	return NULL;
}

/*
 * Receive message for listener
 */
adkisrv(q)
register struct queue *q;
{
	register struct adk *dkp = (struct adk *)WR(q)->ptr;
	struct dkmodule *modp = dkp->module;
	register struct queue *listnrq = modp->listnrq;
	register struct block *bp;
	register struct dialin *dialp;
	register int i;

	if (WR(q) != modp->listnrq)
		return;

	while (bp = getq(q)) {
	
		switch (*bp->rptr) {
	
		case T_CALL:
			bp->rptr++;
			if (bp->wptr - bp->rptr <= 0 &&
			   ((bp->class & S_DELIM) == 0 || (bp = getq(q)) == NULL))
				return;
			do {
				i = bp->class & S_DELIM;
				if (q->next->flag & QFULL)
					free(bp);
				else
					(*q->next->qinfo->putp)(q->next, bp);
			} while (i == 0 && (bp = getq(q)));
			break;
		case T_CHG:	
			if ((bp = adkdialin(q, bp)) == NULL)
				break;
			dialp = (struct dialin *)bp->rptr;
			i = dialp->param1;
			if (i <= 0 || i >= modp->hi - modp->lo) {
				dkstat.chgstrange++;
				if (dialp->srv)
					adkmesg(modp, T_CHG, D_ISCLOSED, 0, i, 0);
				freeb(bp);
				break;
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
					adkmesg(modp, T_CHG, D_ISCLOSED, 0, i, 0);
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
				adkmesg(modp, T_CHG, D_ENQANS, 0, i, modp->dkstate[i]);
				break;
			}
			freeb(bp);
			break;
	
		case T_REPLY:	/* CMC sends reply; find, and hand to process */
			if ((bp = adkdialin(q, bp)) == NULL)
				break;
			dialp = (struct dialin *)bp->rptr;
			i = dialp->param0;
			if (i < 0 || i >= modp->hi - modp->lo) {
				free(bp);
				break;
			}
			for (dkp=adk; dkp<&adk[NDKRAW]; dkp++) {
				if (dkp->module==modp && dkp->chan==i) {
					bp->class |= S_DELIM;
					(*dkp->q->next->qinfo->putp)(dkp->q->next, bp);
					break;
				}
			}
			if (dkp == &adk[NDKRAW])
				free(bp);
			break;
	
		default:
			free(bp);
			break;
		}
	}
}

/*
 * 15-second timer
 * -- should look at dkmod->type, so different
 * listeners can coexist
 */
adktimer()
{
	register i;
	register struct dkmodule *dkp;

	for (dkp = dkmod; dkp < &dkmod[dkmodcnt]; dkp++) {
		if (dkp->listnrq && dkp->type == CMCLD) {
			adkmesg(dkp, T_LSTNR, 0, 0, 0, 0);
			for (i=dkp->hi - dkp->lo - 1; i >= 0; --i)
				if (dkp->dkstate[i] == DKLCLOSE)
					adkmesg(dkp, T_CHG, D_CLOSE, 0, i, 0);
		}
	}
	timeout(adktimer, (caddr_t)NULL, 15*HZ);
}
