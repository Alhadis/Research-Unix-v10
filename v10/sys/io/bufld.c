/*
 * buffering line discipline.
 *  transparent, but saves up characters for a while.
 */

#include "sys/param.h"
#include "sys/stream.h"
#include "sys/conf.h"
#include "sys/bufld.h"

extern int bufldcnt;
extern struct bufld bufld[];

int	buftimo();
long	bufopen();
int	bufput(), bufsrv(), bufclose();
long	bufldact;
#define	MAXINDEX 32	/* this is a bit silly */

static	struct qinit bufrinit = { bufput, bufsrv, bufopen, bufclose, 1024, 512 };
static	struct qinit bufwinit = { bufput, bufsrv, bufopen, bufclose, 128, 64 };
struct streamtab bufldstream = { &bufrinit, &bufwinit };

long
bufopen(q, dev)
register struct queue *q;
{
	register i;
	register struct bufld *fp;

	if (q->ptr)
		return(1);
	for (i=0; bufld[i].queue!=0 && i<bufldcnt; i+= 2)
		;
	if (i >= bufldcnt || i >= MAXINDEX)
		return(0);
	fp = &bufld[i];
	fp->queue = q;
	(fp+1)->queue = WR(q);
	fp->index = i;
	(fp+1)->index = i+1;
	fp->nclick = 3;
	(fp+1)->nclick = 3;
	fp->nchar = 16;
	(fp+1)->nchar = 16;
	fp->rnchar = 0;
	(fp+1)->rnchar = 0;
	fp->btime = 0;
	(fp+1)->btime = 0;
	q->flag |= q->next->flag & QDELIM;
	WR(q)->flag |= WR(q)->next->flag & QDELIM;
	q->ptr = (caddr_t)fp;
	WR(q)->ptr = (caddr_t)(fp+1);
	q->flag |= QNOENB;
	WR(q)->flag |= QNOENB;
	return(1);
}

bufclose(q)
register struct queue *q;
{

	bufunload(q);
	bufunload(WR(q));
	((struct bufld *)q->ptr)->btime = 0;
	((struct bufld *)WR(q)->ptr)->btime = 0;
	((struct bufld *)q->ptr)->queue = 0;
	((struct bufld *)WR(q)->ptr)->queue = 0;
}

bufput(q, bp)
register struct queue *q;
register struct block *bp;
{
	register struct bufld *fp = (struct bufld *)q->ptr;
	register s = spl6();
	register n, t;

	t = bp->type;
	n = bp->wptr - bp->rptr;
	putq(q, bp);
	fp->rnchar += n;
	if ((t!=M_DATA) || fp->rnchar >= fp->nchar)
		bufunload(q);
	if (q->first) {
		/* n/2 ~ about 1200 cps */
		fp->btime = fp->nclick + n/2;
		if (bufldact == 0)
			timeout(buftimo, (caddr_t)0, 1);
		bufldact |= 1 << fp->index;
	} else  {
		fp->rnchar = 0;
		fp->btime = 0;
	}
	splx(s);
}

bufsrv(q)
register struct queue *q;
{
	register struct bufld *fp = (struct bufld *)q->ptr;

	if (q->first && fp->btime == 0)
		bufunload(q);
}

bufunload(q)
register struct queue *q;
{
	register struct block *bp;
	register struct bufld *fp = (struct bufld *)q->ptr;

	while ((q->next->flag&QFULL)==0 && (bp = getq(q))) {
		fp->rnchar -= bp->wptr - bp->rptr;
		(*q->next->qinfo->putp)(q->next, bp);
	}
}

buftimo()
{
	register long bact;
	register struct bufld *fp;
	register ish;

	bact = bufldact;
	for (fp = bufld, ish = 1; bact!=0; fp++, ish <<= 1) {
		if (bact & ish) {
			bact &= ~ish;
			if (fp->btime) {
				fp->btime--;
				if (fp->btime == 0 && fp->queue
				 && fp->queue->first) {
					qenable(fp->queue);
					bufldact &= ~ish;
				}
			}
		}
	}
	if (bufldact)
		timeout(buftimo, (caddr_t)0, 1);
}
