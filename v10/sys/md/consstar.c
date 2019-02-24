/*
 * VAX 11/780 console driver
 * multiple ID stuff is mostly to aid floppy
 */
#include "sys/param.h"
#include "sys/stream.h"
#include "sys/ttyio.h"
#include "sys/cons.h"
#include "sys/mtpr.h"
#include "sys/conf.h"

#define	NCONS	16	/* possible console IDs */

#define	DATA	0xff
#define	IDMASK	0xff
#define	IDSHIFT	8

#define	CONPRI	(PZERO+5)	/* something interruptible */

/*
 * state bits
 */
#define	TTSTOP	01
#define	TIMEOUT	04

/*
 * cnbusy bits
 */
#define	TXBUSY	01
#define	IOBUSY	02	/* cninc/cnoutc busy */

static char cnbusy;
static char cnonext;
static char cnstate[NCONS];
static struct queue *cnrq[NCONS];
static struct queue *cnwq[NCONS];

/*
 * for internal io without device open
 */
struct cnbuf {
	int count;
	char *buf;
};
static struct cnbuf cnibuf[NCONS];
static struct cnbuf cnobuf[NCONS];
static short cnlastin;		/* debugging */

long	cnopen();
int	cnclose(), cnoput();

static struct qinit cnrinit = { nulldev, NULL, cnopen, cnclose, 0, 0 };
static struct qinit cnwinit = { cnoput, NULL, cnopen, cnclose, 200, 100 };
struct streamtab cnstream = { &cnrinit, &cnwinit };
struct cdevsw cncdev = cstrinit(&cnstream);

long
cnopen(qp, dev)
struct queue *qp;
dev_t dev;
{
	register int i;

	i = minor(dev);
	if (i >= NCONS)
		return (1);
	cnrq[i] = qp;
	cnwq[i] = WR(qp);
	qp->ptr = (caddr_t)i;
	mtpr(RXCS, mfpr(RXCS)|RXCS_IE);
	return(1);
}

cnclose(qp)
struct queue *qp;
{
	register int i;

	i = (int)qp->ptr;
	cnrq[i] = NULL;
	cnwq[i] = NULL;
}

/*
 * Console write put routine
 */
cnoput(q, bp)
register struct queue *q;
register struct block *bp;
{
	register struct ttydevb *sp;
	register int dev;

	dev = (int)OTHERQ(q)->ptr;
	switch(bp->type) {
	case M_IOCTL:		/* just acknowledge */
		sp = (struct ttydevb *)stiodata(bp);
		switch (stiocom(bp)) {
		case TIOCGDEV:
			sp->ispeed = sp->ospeed = B9600;
			bp->wptr = bp->rptr + sizeof(struct ttydevb) + STIOCHDR;
			bp->type = M_IOCACK;
			qreply(q, bp);
			return;

		case TIOCSDEV:
			bp->wptr = bp->rptr;
			bp->type = M_IOCACK;
			qreply(q, bp);
			return;

		default:
			bp->type = M_IOCNAK;
			bp->wptr = bp->rptr;
			qreply(q, bp);
			return;
		}

	case M_STOP:
		cnstate[dev] |= TTSTOP;
		break;

	case M_START:
		cnstate[dev] &= ~TTSTOP;
		cnstart();
		break;

	case M_FLUSH:
		flushq(q, 0);
		break;

	case M_DELAY:
	case M_DATA:
		if (bp->rptr >= bp->wptr)
			break;
		putq(q, bp);
		cnstart();
		return;
	
	default:		/* not handled; just toss */
		break;
	}
	freeb(bp);
}

/*
 * Console receive interrupt
 */

cnrint()
{
	register int c, dev;
	register struct queue *qp;
	register struct cnbuf *cp;

	c = mfpr(RXDB);
	dev = (c >> IDSHIFT) & IDMASK;
	cp = &cnibuf[dev];
	if (cp->count) {
		*cp->buf++ = c;
		if (--cp->count == 0)
			wakeup((caddr_t)&cp->count);
		return;
	}
	if ((qp = cnrq[dev])!=NULL) {
		if ((qp->next->flag & QFULL) == 0)
			putd(qp->next->qinfo->putp, qp->next, c);
		return;
	}
	cnlastin = c;		/* for debugging */
}

/*
 * Transmitter interrupt
 */
cnxint()
{
	cnbusy &=~ TXBUSY;
	mtpr(TXCS, mfpr(TXCS)&~TXCS_IE);
	cnstart();
}

cntime(dev)
caddr_t dev;
{
	cnstate[(int)dev] &= ~TIMEOUT;
	cnstart();
}

#define	NEXT(i)	(((i)+1)%NCONS)

cnstart()
{
	register struct block *bp;
	register int i;
	register struct queue *q;
	register struct cnbuf *cp;
	register s, j;

	s = spl6();
	if (cnbusy & TXBUSY) {
		splx(s);
		return;
	}
	i = cnonext;
	for (j = 0; j < NCONS && (cnbusy & TXBUSY) == 0; i = NEXT(i), j++) {
		cp = &cnobuf[i];
		if (cp->count) {
			mtpr(TXDB, (*cp->buf++&0377) | (i<<IDSHIFT));
			mtpr(TXCS, mfpr(TXCS)|TXCS_IE);
			cnbusy |= TXBUSY;
			if (--cp->count == 0)
				wakeup((caddr_t)&cp->count);
			continue;
		}
		if ((q = cnwq[i]) == NULL)
			continue;
		if ((cnstate[i] & (TIMEOUT|TTSTOP)) || q->count == 0)
			continue;
		bp = getq(q);
		switch (bp->type) {
		case M_DATA:
			mtpr(TXDB, (i<<IDSHIFT)|*bp->rptr++);
			mtpr(TXCS, mfpr(TXCS)|TXCS_IE);
			cnbusy |= TXBUSY;
			if (bp->rptr >= bp->wptr)
				freeb(bp);
			else
				putbq(q, bp);
			break;

		case M_DELAY:
			timeout(cntime, (caddr_t)i, (int)*bp->rptr);
			cnstate[i] |= TIMEOUT;
		default:	/* flow through */
			freeb(bp);
			break;
		}
	}
	cnonext = i;
	splx(s);
}

/*
 * Print a character on console; for printf.
 * try to preserve things; wait a bit for console to come ready.
 * all fairly hopeless.
 */
cnputc(c)
register c;
{
	register s, timo;

	timo = 3000000;
	while((mfpr(TXCS)&TXCS_RDY) == 0)
		if(--timo == 0)
			return;
	if(c == 0)
		return;
	s = mfpr(TXCS);
	mtpr(TXCS, 0);
	mtpr(TXDB, c&DATA);
	if(c == '\n')
		cnputc('\r');
	cnputc(0);
	mtpr(TXCS, s);
}

/*
 * primitives for internal console I/O
 * p = cniwrite(id, buf, count)
 * p = cniread(id, buf, count)
 * p is a pointer to an integer.
 * when the count is satisfied, *p will be zero and p will be awakened
 * *p = 0 aborts the I/O
 * cniwait(p, s) waits for at most s seconds, then aborts
 *
 * only one I/O may be outstanding per id;
 * a new one aborts the previous one, quietly
 *
 * buf had best be static; stacks move, and the process waiting for io
 * will almost certainly go out of context.
 * hence the KSTART checks.
 */

int *
cniread(id, buf, count)
int id, count;
char *buf;
{
	register struct cnbuf *cp;

	if (id < 0 || id >= NCONS || (int)buf < KSTART)
		panic("cniread");
	cp = &cnibuf[id];
	cp->buf = buf;
	cp->count = count;
	mtpr(RXCS, mfpr(RXCS)|RXCS_IE);
	return (&cp->count);
}

int *
cniwrite(id, buf, count)
int id, count;
char *buf;
{
	register struct cnbuf *cp;

	if (id < 0 || id >= NCONS || (int)buf < KSTART)
		panic("cniwrite");
	cp = &cnobuf[id];
	cp->buf = buf;
	cp->count = count;
	cnstart();
	return (&cp->count);
}

cniwait(p, t)
register int *p;
int t;
{
	register int s;
	register int sh;

	s = spl6();
	while (*p)
		if (tsleep((caddr_t)p, CONPRI, t) != TS_OK) {
			sh = *p;
			*p = 0;
			splx(s);
			return (sh);
		}
	splx(s);
	return (0);
}
