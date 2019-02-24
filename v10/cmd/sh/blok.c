/*	@(#)blok.c	1.4	*/
/*
 *	UNIX shell
 *
 *	Bell Telephone Laboratories
 *
 */

#include	"defs.h"


/*
 *	storage allocator
 *	(circular first fit strategy)
 */

#ifdef	CRAY
#ifdef	CRAY2
#define	BUSY	020000000000
#else
#define	BUSY	040000000
#endif
#else
#define BUSY 01
#endif

#define	mkbusy(x)	(Rcheat(x) | BUSY)
#define	mknbusy(x)	(Rcheat(x) & ~BUSY)
#define busy(x)		(Rcheat((x)->word) & BUSY)

unsigned	brkincr = BRKINCR;
struct blk *blokp;			/*current search pointer*/
struct blk *bloktop;		/* top of arena (last blok) */
struct blk *blokbot;		/* first blok */

char		*brkbegin;
char		*setbrk();

char *
shalloc(nbytes)
	unsigned nbytes;
{
	register unsigned rbytes = round(nbytes+BYTESPERWORD, BYTESPERWORD);

	for (;;)
	{
		int	c = 0;
		register struct blk *p = blokp;
		register struct blk *q;

		do
		{
			if (!busy(p))
			{
				while (!busy(q = p->word)) {
					p->word = q->word;
				}
				if ((char *)q - (char *)p >= rbytes)
				{
					blokp = (struct blk *)((char *)p + rbytes);
					if (q > blokp)
						blokp->word = p->word;
					p->word = (struct blk *)mkbusy(blokp);
					return((char *)(p + 1));
				}
			}
			q = p;
			p = (struct blk *)mknbusy(p->word);
		} while (p > q || (c++) == 0);
		addblok(rbytes);
	}
}

addblok(reqd)
	unsigned reqd;
{
	if (stakbot == 0)
	{
		brkbegin = setbrk(3 * BRKINCR);
		bloktop = blokbot = (struct blk *)brkbegin;
	}

	if (stakbas != staktop)
	{
		register char *rndstak;
		register struct blk *blokstak;

		pushstak(0);
#ifdef	CRAY
		rndstak = (char *)sround(staktop, BYTESPERWORD);
#else
		rndstak = (char *)round(staktop, BYTESPERWORD);
#endif
		blokstak = (struct blk *)(stakbas) - 1;
		blokstak->word = stakbsy;
		stakbsy = blokstak;
		bloktop->word = (struct blk *)mkbusy(rndstak);
		bloktop = (struct blk *)(rndstak);
	}
	reqd += brkincr;
	reqd &= ~(brkincr - 1);
	blokp = bloktop;
	bloktop = bloktop->word = bloktop + (reqd/BYTESPERWORD);
	while ((char *)bloktop >= brkend) {
		setbrk(round((char *)bloktop - brkend + 1, BRKINCR));
	}
	bloktop->word = (struct blk *)mkbusy(brkbegin);
	{
		register char *stakadr = (char *)(bloktop + 2);

		if (stakbot != staktop)
			staktop = movstr(stakbot, stakadr);
		else
			staktop = stakadr;

		stakbas = stakbot = stakadr;
	}
}

shfree(ap)
	struct blk *ap;
{
	register struct blk *p;

	if ((p = ap) && p < bloktop && p >= blokbot)
	{
#ifdef DEBUG
		chkbptr(p);
#endif
		--p;
		p->word = (struct blk *)mknbusy(p->word);
		p->word = (struct blk *)(Rcheat(p->word) & ~BUSY);
	}


}


#ifdef DEBUG

chkbptr(ptr)
	struct blk *ptr;
{
	int	exf = 0;
	register struct blk *p = (struct blk *)brkbegin;
	register struct blk *q;
	int	us = 0, un = 0;

	for (;;)
	{
		q = (struct blk *)(Rcheat(p->word) & ~BUSY);

		if (p+1 == ptr)
			exf++;

		if (q < (struct blk *)brkbegin || q > bloktop)
			abort(3);

		if (p == bloktop)
			break;

		if (busy(p))
			us += q - p;
		else
			un += q - p;

		if (p >= q)
			abort(4);

		p = q;
	}
	if (exf == 0) {
		abort(1);
	}
}

#endif

chkmem()
{
	register struct blk *p = (struct blk *)brkbegin;
	register struct blk *q;
	int	us = 0, un = 0;

	for (;;)
	{
		q = (struct blk *)(Rcheat(p->word) & ~BUSY);

		if (q < (struct blk *)brkbegin || q > bloktop)
			abort(3);

		if (p == bloktop)
			break;

		if (busy(p))
			us += q - p;
		else
			un += q - p;

		if (p >= q)
			abort(4);

		p = q;
	}

	prs("un/used/avail ");
	prn(un);
	blank();
	prn(us);
	blank();
	prn((char *)bloktop - brkbegin - (un + us));
	newline();

}
