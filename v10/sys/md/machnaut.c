/*
 * machine-specific hardware routines,
 * these for nautilus
 */

#include "sys/mtpr.h"
#include "sys/cons.h"

int delayfact = 5;	/* for DELAY macro */

#define	NICTRL	0x80	/* nmi interrupt enables: */
#define	MEMENB	0x20	/* memory/fault */
#define	NB1ENB	0x40	/* device 1 */
#define	NB0ENB	0x80	/* device 0 */
#define	COR	0x85	/* cache on */

static int nmienable = MEMENB;

/*
 * adjust physical top of memory to useful top of memory
 * (some vaxes put important console stuff there)
 * called with mapping off
 */
machmem(hi)
int hi;
{
	return (hi);
}

/*
 * miscellaneous machine-dependent initialization
 * called just after mapping turned on
 */
machinit()
{

	mtpr(COR, 1);		/* turn on the cache */
	mcrinit();
	mtpr(NICTRL, nmienable);
}

/*
 * enable interrupts from one of the NBIAs
 */

setnmi(dev)
int dev;
{
	nmienable |= dev ? NB1ENB : NB0ENB;
	mtpr(NICTRL, nmienable);	
}

/*
 * stray interrupt handling:
 * just decrypt it and return
 */

long passintr;

strayintr(v)
int v;
{
	if ((v & 0x1ff) == 0) {
		passintr++;
		return;
	}
	printf("stray interrupt at 0x%x == 0%o\n", v, v);	/* temp */
}

/*
 * read the time-of-year clock, which is buried in the wretched console
 */

#define	CMDID	0xf
#define	READTOY	0x8
#define	WRITTOY	0x9
#define	RTOYID	0xd
#define	WTOYID	0xd

int *cniread(), *cniwrite();
static char rtbusy, wtbusy;
long
gettodr()
{
	register int i;
	register int *p;
	static char rtoy = READTOY;
	static long toy;

	if (rtbusy)
		return (0);
	rtbusy++;
	for (i = 0; i < 2; i++) {
		p = cniread(RTOYID, (char *)&toy, 4);
		cniwrite(CMDID, &rtoy, 1);
		if (cniwait(p, 2) == 0) {
			rtbusy = 0;
			return (toy);
		}
	}
	rtbusy = 0;
	return (0);
}

/*
 * set time-of-year clock
 * 10% of the time, it is mysteriously set wrong;
 * try a couple of times
 */

settodr(toy)
unsigned long toy;
{
	register int i;
	register unsigned long ntoy;

	if (wtbusy)
		return;
	wtbusy++;
	for (i = 0; i < 4; i++) {
		csettodr(toy);
		ntoy = gettodr();
		if (ntoy >= toy && ntoy < toy + 100)
			break;
	}
	wtbusy = 0;
}

csettodr(toy)
long toy;
{
	static char wtoy = WRITTOY;
	static long tbuf;

	if (cniwait(cniwrite(CMDID, &wtoy, 1), 2))
		return;
	tbuf = toy;
	cniwait(cniwrite(WTOYID, (char *)&tbuf, 4));
}
