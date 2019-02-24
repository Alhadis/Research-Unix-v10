/*
 * primitives dealing in stream blocks
 */

#include "sys/param.h"
#include "sys/stream.h"
#include "sys/mtpr.h"
#include "sys/buf.h"

#define	M_HIPRI	127			/* for use by putbq */

extern struct block	cblock[];
extern struct buf	*cblkbuf[];
extern struct queue	queue[];
extern int		blkcnt, blkbcnt, queuecnt;
struct queue		*qhead;		/* head of queues to run */
struct queue		*qtail;		/* last queue */

/*
 * blocks are kept in handful of free lists,
 * each containing blocks of a particular size.
 * there are MAXLEVEL sizes, each SPLITSIZE times bigger than the last.
 * the system starts out with blkbcnt blocks of the largest size
 * (which are in fact buffers from bio)
 * and blkcnt-blkbcnt blocks of the smallest size.
 * if there are no blocks of the requested size,
 * larger ones are split to make some.
 * the smallest blocks (level 0) are a special case:
 * the data lives in the block header,
 * and there's no point splitting larger blocks to make more
 * because there are no headers to use after splitting.
 */

#define	MAXLEVEL	6
#define	SPLITSIZE	4

int	rbsize[MAXLEVEL] = { 4, 16, 64, 256, 1024, 4096 }; /* real block sizes */
int	bsize[MAXLEVEL] = { 4, 16, 64, 256, 256, 256 };	/* size for q limits */
struct	block *qfreelist[MAXLEVEL];
short	blkfree[MAXLEVEL];
short	blkall[MAXLEVEL];
short	blkmax[MAXLEVEL];

/*
 * hooks for debugging
 * turn this on only if you need it;
 * it's quite slow
 */
#if STDEBUG

static int qiniting;
#define	BADBP(bp)	((bp)!=&cblock[(bp)-cblock])

static
badfreelist(lev)
	register lev;
{
	register i = 0;
	register struct block *bp;

	/* look for block that is already on the free list */
	for(bp=qfreelist[lev];bp!=0;bp=bp->next)
		i++;
	if(i!=blkfree[lev]){
		printf("blkfree[%d]==%d i==%d\n", lev, blkfree[lev], i);
		panic("bad free list 1");
	}
}

static
duplicateblock(lev, bp)
	register lev;
	register struct block *bp;
{
	register i = 0;
	register struct block *nbp;

	/* look for block that is already on the free list */
	for(nbp=qfreelist[lev];nbp!=0;nbp=nbp->next) {
		if(nbp==bp)
			panic("duplicate free");
		i++;
	}
	if(i!=blkfree[lev]){
		printf("blkfree[%d]==%d i==%d\n", lev, blkfree[lev], i);
		panic("bad free list 2");
	}
}
#endif

/*
 * more debugging: trace the owner of a block
 * not so slow, but very VAX-dependent.
 */
#if BLKOWNER
#define	MAXOWNER	2000	/* this many blocks will be traced */
#define	CALLER(x)	*(&x+5)	/* x is an int, and is first auto in function */
int blkowner[MAXOWNER];
#endif

qinit()
{
	register i;
	register struct buf *bp;

#if STDEBUG
	qiniting=1;
#endif
	for (i = 0; i<blkbcnt; i++) {
		if ((bp = geteblk()) == NULL)
			break;		/* probably not best */
		cblock[i].class = MAXLEVEL-1;
		cblock[i].base = (u_char *)&bp->b_un.b_addr[0];
		cblock[i].lim = (u_char *)&bp->b_un.b_addr[bp->b_bcount];
		cblock[i].rbufix = i;
		cblkbuf[i] = bp;
		freeb(&cblock[i]);
	}
	blkall[MAXLEVEL-1] = 0;
	for ( ; i<blkcnt; i++) {
		cblock[i].class = 0;
		cblock[i].base = &cblock[i].data[0];
		cblock[i].lim = &cblock[i].data[sizeof(cblock[0].data)];
		cblock[i].rbufix = NOBUFIX;
		freeb(&cblock[i]);
	}
	blkall[0] = 0;
#if STDEBUG
	qiniting=0;
#endif
}

static bsplit();

struct block *
allocb(size)
{
#if BLKOWNER
	int x;
#endif
	register struct block *bp;
	register lev;
	register s;

	for (lev = 0; lev < MAXLEVEL; lev++) {
		if (size <= rbsize[lev])
			break;
	}
	if (lev >= MAXLEVEL)
		panic("allocb bad size");
	s = spl6();
#if STDEBUG
	badfreelist(lev);
#endif

	/* check for free block of right size */
	if ((bp = qfreelist[lev]) == NULL) {
		bsplit(lev+1);		/*  else try to split larger block */
		if ((bp = qfreelist[lev]) == NULL) {
			if (size != QBSIZE) {
				splx(s);
				return (allocb(QBSIZE)); /* last chance */
			}
			panic("allocb no block");
		}
	}
	qfreelist[lev] = bp->next;
	blkall[lev]++;
	if (blkall[lev] > blkmax[lev])
		blkmax[lev] = blkall[lev];
	blkfree[lev]--;
#if BLKOWNER
	if ((x = bp-cblock) < MAXOWNER)
		blkowner[x] = CALLER(x);
#endif
	splx(s);
	bp->type = M_DATA;
	bp->next = NULL;
	bp->rptr = bp->base;
	bp->wptr = bp->base;
	bp->bufix = bp->rbufix;
	bp->class &= ~S_DELIM;
#if STDEBUG
	if (BADBP(bp))
		panic("allocb");
#endif
	return(bp);
}

static
bsplit(lev)
register lev;
{
	register struct block *bp, *bp1;
	register i, size;

	if (lev>=MAXLEVEL)
		return;
	if ((bp = qfreelist[lev]) == NULL) {
		bsplit(lev+1);
		if ((bp = qfreelist[lev]) == NULL)
			return;
	}
	qfreelist[lev] = bp->next;
	blkfree[lev]--;
	size = (bp->lim - bp->base)/SPLITSIZE;
	lev--;			/* the blocks we're creating */
	for (i=1; i<SPLITSIZE; i++) {
		if ((bp1 = qfreelist[0]) == NULL)
			return;
		qfreelist[0] = bp1->next;
		blkfree[0]--;
		bp1->base = bp->base;
		bp->base += size;
		bp1->lim = bp1->base+size;
		bp1->class = lev;
		bp1->rbufix = bp->rbufix;
		bp1->next = qfreelist[lev];
		qfreelist[lev] = bp1;
		blkfree[lev]++;
	}
	bp->class = lev;
	bp->next = qfreelist[lev];
	qfreelist[lev] = bp;
	blkfree[lev]++;
}

/*
 *  if this is a large block and its data can be squeezed
 *  into a smaller block, do it.
 */
struct block *
cramb(bp)
	register struct block *bp;
{
	register struct block *nbp;
	register int n;

	n = bp->wptr - bp->rptr;
	if((bp->class&CL_MASK) < 3 || n > rbsize[(bp->class&CL_MASK)-1])
		return bp;
	nbp = allocb(n);
	if(nbp==NULL)
		return bp;
	if(nbp->lim-nbp->wptr < n) {
		freeb(nbp);
		return bp;
	}
	bcopy(bp->rptr, nbp->wptr, n);
	nbp->wptr += n;
	nbp->type = bp->type;
	if (bp->class & S_DELIM)
		nbp->class |= S_DELIM;
	freeb(bp);
	return nbp;
}

/*
 * make a block that points to the same data
 * as some existing block
 */
struct block *
dupb(bp)
register struct block *bp;
{
#if BLKOWNER
	int x;
#endif
	register struct block *nbp;

	if ((nbp = allocb(0)) == NULL)
		return (NULL);
	nbp->type = bp->type;
	nbp->rptr = bp->rptr;
	nbp->wptr = bp->wptr;
	nbp->bufix = bp->bufix;
	nbp->class |= bp->class&S_DELIM;
#if BLKOWNER
	if ((x = nbp-cblock) < MAXOWNER)
		blkowner[x] = CALLER(x);
#endif
	return (nbp);
}
	
freeb(bp)
register struct block *bp;
{
	register s = spl6();
	register lev;

	lev = bp->class&CL_MASK;
#if STDEBUG
	if (BADBP(bp))
		panic("freeb");
	duplicateblock(lev, bp);
	if (blkall[lev] <= 0 && !qiniting)
		panic("freeb excess free");
#endif
	bp->next = qfreelist[lev];
	qfreelist[lev] = bp;
	blkall[lev]--;
	blkfree[lev]++;
	splx(s);
}

struct block *
getq(q)
register struct queue *q;
{
#if BLKOWNER
	int x;
#endif
	register struct block *bp;
	register s = spl6();

	if ((bp = q->first) == NULL) {
		if ((q->flag&QENAB) == 0)
			q->flag |= QWANTR;
	} else {
		if ((q->first = bp->next) == NULL)
			q->last = NULL;
		q->count -= bsize[bp->class&CL_MASK];
		if (q->count < q->qinfo->limit)
			q->flag &= ~QFULL;
		q->flag &= ~QWANTR;
#if BLKOWNER
		if ((x = bp-cblock) < MAXOWNER)
			blkowner[x] = CALLER(x);
#endif
	}
	if (q->count<=q->qinfo->lolimit && q->flag&QWANTW && OTHERQ(q)->next) {
		register struct queue *bq = backq(q);
		if (bq->qinfo->srvp)
			qenable(bq);
		q->flag &= ~QWANTW;
	}
	splx(s);
#if STDEBUG
	if(bp && BADBP(bp))
		panic("getq");
#endif
	return(bp);
}

putq(q, bp)
register struct queue *q;
register struct block *bp;
{
	int s;

#if STDEBUG
	if (BADBP(bp))
		panic("putq");
#endif
	if (bp->type==M_FLUSH)
		flushq(q, 0);
	s = spl6();
	if (q->first==NULL) {			/* empty, just tack on */
		q->first = bp;
		q->last = bp;
		bp->next = NULL;
	} else if (bp->type<QPCTL || q->last->type>=QPCTL) {	/* put at end */
		register struct block *lastp = q->last;
		register n = bp->wptr - bp->rptr;
		if (bp->type==M_DATA && lastp->type==M_DATA
		 && (lastp->class&S_DELIM)==0
		 && n <= lastp->lim-lastp->wptr && lastp->wptr>=lastp->base) {
			bcopy(bp->rptr, lastp->wptr, n);
			lastp->wptr += n;
			if (bp->class&S_DELIM)
				lastp->class |= S_DELIM;
			freeb(bp);
			bp = NULL;
		} else {
			lastp->next = bp;
			q->last = bp;
			bp->next = NULL;
		}
	} else {				/* pri, put after any others */
		register struct block *nbp = q->first;
		if (nbp->type < QPCTL) {
			bp->next = q->first;
			q->first = bp;
		} else {
			while (nbp->next->type>=QPCTL)
				nbp = nbp->next;
			bp->next = nbp->next;
			nbp->next = bp;
		}
	}
	if (bp) {
		q->count += bsize[bp->class&CL_MASK];
		if (bp->type >= QPCTL && bp->type!= M_HIPRI)
			q->flag |= QWANTR;
	}
	if (q->count >= q->qinfo->limit)
		q->flag |= QFULL|QWANTW;
	if ((q->flag & (QWANTR|QENAB|QNOENB)) == QWANTR && q->qinfo->srvp)
		qenable(q);
	splx(s);
}

/*
 * Put stuff back at beginning of Q
 * (but after any priority msgs)
 */
putbq(q, bp)
register struct queue *q;
register struct block *bp;
{
	register savetype = bp->type;
	register s = spl6();

	bp->type = M_HIPRI;		/* fake priority, to force to start */
	putq(q, bp);
	bp->type = savetype;
	splx(s);
}

/*
 * empty a queue.  Leave any non-data messages, unless flag is 1.
 */
flushq(q, flag)
register struct queue *q;
{
	register struct block *bp, *nbp;
	register s = spl6();

	bp = q->first;
	q->first = NULL;
	if (q->last)
		q->last->next = NULL;
	q->last = NULL;
	q->count = 0;
	q->flag &= ~QFULL;
	while (bp) {
		nbp = bp->next;
		if (bp->type != M_DATA && bp->type != M_DELIM
		 && bp->type != M_CTL && bp->type != M_DELAY
		 && bp->type != M_FLUSH && !flag)
			putq(q, bp);
		else {
			if (bp->type == M_PASS)
				printf("flushing PASS %x\n",*(int *)(bp->rptr));
			freeb(bp);
		}
		bp = nbp;
	}
	if (q->flag&QWANTW && OTHERQ(q)->next) {
		q->flag &= ~QWANTW;
		qenable(backq(q));
	}
	splx(s);
}

/*
 * allocate a pair of queues
 */
struct queue *
allocq()
{
	register struct queue *qp;
	static struct queue zeroR =
	  { NULL,NULL,NULL,NULL,NULL,NULL,0,QUSE|QREADR};
	static struct queue zeroW =
	  { NULL,NULL,NULL,NULL,NULL,NULL,0,QUSE};

	for (qp = queue; qp < &queue[queuecnt]; qp += 2) {
		if ((qp->flag & QUSE) == 0) {
			*qp = zeroR;
			*WR(qp) = zeroW;
			return(qp);
		}
	}
	return(NULL);
}

/*
 * put routine that shouldn't be called
 */
noput(q, c)
struct queue *q;
{
	panic("noput");
}

/*
 * Put one data char on a queue, using f
 */
putd(f, q, c)
int (*f)();
register struct queue *q;
{
	register struct block *bp;
	register s = spl6();

	if (f==putq && (bp = q->last) && bp->type==M_DATA && bp->wptr<bp->lim
	 && (bp->class&S_DELIM)==0) {
		*bp->wptr++ = c;
		splx(s);
	 } else {
		splx(s);
		if ((bp = allocb(16)) == NULL)
			return(0);
		*bp->wptr++ = c;
		(*f)(q, bp);
	}
	return(1);
}

/*
 * Put a block of type c on queue
 */
putctl(q, c)
struct queue *q;
{
	register struct block *bp;

	if ((bp = allocb(0)) == NULL)
		return(0);
	bp->type = c;
	(*q->qinfo->putp)(q, bp);
	return(1);
}

/*
 * Put a block of type c, delimited, on queue
 */
putctld(q, c)
struct queue *q;
{
	register struct block *bp;

	if ((bp = allocb(0)) == NULL)
		return(0);
	bp->type = c;
	bp->class |= S_DELIM;
	(*q->qinfo->putp)(q, bp);
	return(1);
}

/*
 * Block of type c, with one byte of data
 */
putctl1(q, c, p)
struct queue *q;
{
	register struct block *bp;

	if ((bp = allocb(1)) == NULL)
		return(0);
	bp->type = c;
	*bp->wptr++ = p;
	(*q->qinfo->putp)(q, bp);
	return(1);
}

/*
 * Block of type c, delimited, with one byte of data
 */
putctl1d(q, c, p)
struct queue *q;
{
	register struct block *bp;

	if ((bp = allocb(1)) == NULL)
		return(0);
	bp->type = c;
	bp->class |= S_DELIM;
	*bp->wptr++ = p;
	(*q->qinfo->putp)(q, bp);
	return(1);
}

/*
 * block with two parameters
 */
putctl2(q, c, p1, p2)
struct queue *q;
{
	register struct block *bp;

	if ((bp = allocb(2)) == NULL)
		return(0);
	bp->type = c;
	*bp->wptr++ = p1;
	*bp->wptr++ = p2;
	(*q->qinfo->putp)(q, bp);
	return(1);
}

/*
 * put control record, using putq instead of queue's putp
 */
qpctl(q, d)
register struct queue *q;
{
	register struct block *bp = allocb(1);

	if (bp) {
		bp->type = d;
		putq(q, bp);
	}
}

qpctl1(q, c, d)
register struct queue *q;
{
	register struct block *bp = allocb(1);

	if (bp) {
		bp->type = c;
		*bp->wptr++ = d;
		putq(q, bp);
	}
}

qpctld(q, d)
register struct queue *q;
{
	register struct block *bp = allocb(1);

	if (bp) {
		bp->type = d;
		bp->class |= S_DELIM;
		putq(q, bp);
	}
}

/*
 * Copy a literal record onto queue
 */
putcpy(q, cp, n)
register struct queue *q;
register char *cp;
{
	register struct block *bp;
	register nm;

	while (n) {
		if ((bp = allocb(n)) == NULL)	/* sorry */
			return;
		bp->type = M_DATA;
		nm = bp->lim - bp->wptr;
		if (nm > n)
			nm = n;
		bcopy(cp, bp->wptr, nm);
		cp += nm;
		bp->wptr += nm;
		n -= nm;
		(*q->qinfo->putp)(q, bp);
	}
}

/*
 * return the queue upstream from this one
 */
struct queue *
backq(q)
register struct queue *q;
{
	q = OTHERQ(q);
	if (q->next) {
		q = q->next;
		return(OTHERQ(q));
	}
	q = OTHERQ(q);
	printf("backq called with no back (Q %x)\n", q);
	panic("backq");
	return(NULL);
}

/*
 * Send a block back up the queue in reverse from this
 * one (e.g. to respond to ioctls)
 */
qreply(q, bp)
register struct queue *q;
struct block *bp;
{
	q = OTHERQ(q);
	(*q->next->qinfo->putp)(q->next, bp);
}

/*
 * Enable a queue: put it on list of those whose srvp's are
 * ready to run.
 */
qenable(q)
register struct queue *q;
{
	register s;

	s = spl6();
	if (q->flag & QENAB) {
		splx(s);
		return;
	}
	if (q->qinfo->srvp==NULL) {
		splx(s);
		return;
	}
	q->flag |= QENAB;
	q->link = NULL;
	if (qhead==NULL)
		qhead = q;
	else
		qtail->link = q;
	qtail = q;
	setqsched();
	splx(s);
}

/*
 * Run the srvp's of each enabled queue
 *	-- Should not be reentered
 */
queuerun()
{
	register struct queue *q;
	register s;
	extern int queueflag;
	extern char *panicstr;

	if (panicstr)
		return;		/* to minimize destruction */
	s = spl6();
	queueflag++;
	while (q = qhead) {
		if ((qhead = q->link) == NULL)
			qtail = NULL;
		q->flag &= ~QENAB;
		splx(s);
		if (q->qinfo->srvp != NULL)
			(*q->qinfo->srvp)(q);
		else
			printf("Q %x run with no srvp\n", q);
		spl6();
	}
	queueflag--;
	splx(s);
}
