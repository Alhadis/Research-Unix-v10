/*
 * DR11C for Mergenthaler 202
 */
#include "sys/param.h"
#include "sys/conf.h"
#include "sys/stream.h"
#include "sys/ubaddr.h"
#include "sys/mg.h"

/*
 * hardware
 */
struct device {
	u_short csr;
	u_short wbuf;
	u_short rbuf;
};

#define CSR0 1
#define CSR1 2
#define REQA 0200
#define REQB 0100000
#define INTA 0100
#define INTB 040

/*
 * stream glue
 */
long mgopen();
int nodev(), mgclose(), mgoput();

static struct qinit mgrinit = {
	nodev, NULL, mgopen, mgclose, 0, 0
};
static struct qinit mgwinit = {
	mgoput, NULL, mgopen, mgclose, 200, 100
};
struct streamtab mginfo = {
	&mgrinit, &mgwinit
};

/*
 * config glue
 */
extern struct mg mg[];			/* one per device */
extern struct ubaddr mgaddr[];		/* one per device */
extern int mgcnt;			/* one per device or what? */
struct cdevsw mgcdev = cstrinit(&mginfo);

long
mgopen(q, d)
register struct queue *q;
{
	register struct mg *mp;
	register dev, s;

	if((dev = (minor(d) >> 1)) >= mgcnt)
		return 0;
	mp = &mg[dev];
	if((mp->addr = (struct device *)ubaddr(&mgaddr[dev])) == 0
	  || ubbadaddr(mgaddr[dev].ubno, mp->addr, sizeof(u_short))) {
		printf("mg%d absent\n", d);
		return 0;
	}
	mp->addr->csr |= CSR0 | CSR1;
	if(d & 01) {
		if(mp->wq != NULL)
			return 0;
		mp->addr->csr |= INTA;
		mp->wq = WR(q);
	}
	else {
		if(mp->rq != NULL)
			return 0;
		s = mp->addr->rbuf;
		mp->addr->csr |= INTB;
		mp->rq = q;
	}
	WR(q)->ptr = q->ptr = (caddr_t) minor(d);
	return 1;
}

mgclose(q)
register struct queue *q;
{
	register struct mg *mp = &mg[((int)q->ptr) >> 1];

	if(((int)q->ptr) & 01) {
		mp->wq = NULL;
	}
	else {
		mp->addr->csr &= (~INTB);
		flushq(WR(q), 1);
		mp->rq = NULL;
	}
}

mg1int(dev)
{
	register struct mg *mp = &mg[dev];
	register c;

	c = mp->addr->rbuf;
	if(mp->rq && (mp->rq->flag & QFULL) == 0)
		putd(mp->rq->next->qinfo->putp, mp->rq->next, c);
}

mg0int(dev)
{
	register struct mg *mp = &mg[dev];

	mp->busy = 0;
	mgstart(mp);
}

mgoput(q, bp)
register struct queue *q;
register struct block *bp;
{
	switch(bp->type) {

	case M_IOCTL:
		bp->type = M_IOCNAK;
		bp->wptr = bp->rptr;
		qreply(q, bp);
		return;

	case M_FLUSH:
		flushq(q, 0);
		break;

	case M_DATA:
		putq(q, bp);
		mgstart(&mg[((int)q->ptr) >> 1]);
		return;

	default:
		break;
	}
	freeb(bp);
}

mgstart(mp)
register struct mg *mp;
{
	register struct block *bp;
	register s;

	if(mp->wq == NULL)
		return;
	s = spl6();
	if((mp->busy == 0) && mp->wq->count) {
		bp = getq(mp->wq);
		switch(bp->type) {

		case M_DATA:
			mp->addr->wbuf = *bp->rptr++;
			mp->busy = 1;
			if(bp->rptr >= bp->wptr)
				freeb(bp);
			else
				putbq(mp->wq, bp);
			break;

		default:
			freeb(bp);
			break;
		}
	}
	splx(s);
}
