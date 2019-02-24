/*
 * stream pipe
 */
#include "sys/param.h"
#include "sys/stream.h"
#include "sys/conf.h"

extern struct	queue	*spipes[];
extern int spcnt;

int	spsrv(), spclose();
long	spopen();
static struct qinit sprinit = { putq, spsrv, spopen, spclose, 128, 65 };
struct streamtab spinfo = { &sprinit, &sprinit };
struct cdevsw spcdev = cstrinit(&spinfo);

long
spopen(q, dev)
register struct queue *q;
{
	register d = minor(dev);
	register struct queue *oq;

	if (d > spcnt)
		return (0);
	if (q->ptr) {			/* already open */
		if (d&01)
			return(0);	/* reopening master */
		else
			return(1);	/* reopening slave */
	}
	q->flag |= QDELIM;
	if (oq = spipes[d^01]) {
		if (d&01)
			return(0);	/* master with old lingering slave */
		WR(q)->ptr = (caddr_t)oq;
		WR(oq)->ptr = (caddr_t)q;
	} else if ((d&01) == 0)
		return(0);		/* slave opened first */
	q->ptr = (caddr_t)dev;
	spipes[d] = q;
	return(1);
}

spclose(q)
register struct queue *q;
{
	register struct queue *oq;
	register struct block *bp;

	if (oq = (struct queue *)WR(q)->ptr) {
		while (bp = getq(WR(q)))
			(*oq->next->qinfo->putp)(oq->next, bp);
		putctl(oq->next, M_HANGUP);
		WR(oq)->ptr = NULL;
	}
	spipes[minor((int)q->ptr)] = NULL;
}

spsrv(q)
register struct queue *q;
{
	register struct block *bp;
	register struct queue *oq;

	if (q->flag&QREADR) {
		oq = (struct queue *)WR(q)->ptr;
		if (oq && (q->next->flag & QFULL) == 0)
			qenable(WR(oq));
		return;
	}
	oq = (struct queue *)q->ptr;
	while (bp = getq(q)) {
		if (oq == NULL) {
			freeb(bp);
			continue;
		}
		if (oq->next->flag & QFULL && bp->type < QPCTL) {
			putbq(q, bp);
			return;
		}
		(*oq->next->qinfo->putp)(oq->next, bp);
	}
}
