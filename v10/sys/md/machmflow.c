/*
 * machine-specific hardware routines,
 * these for MicroVAX II
 */

#include "sys/param.h"
#include "sys/pte.h"
#include "sys/mtpr.h"
#include "sys/vm.h"
#include "sys/qbio.h"
#include "sys/clock.h"

#define	IORESET	0x37		/* reset all IO connections */
#define	PARENB	01		/* mser: enable parity error traps */

/*
 * bits in console mailbox
 */

#define	HALTACT	03	/* halt action */
#define	HMRBOOT	0	/* halt: restart, then boot (if halts disabled!) */
#define	HMRST	01	/* halt: restart regardless (but never boot) */
#define	HMBOOT	02	/* halt: boot */
#define	BOOTIH	04	/* boot inhibit */
#define	RSTIH	010	/* restart inhibit */

char *iospace;
int delayfact = 1;		/* factor for DELAY macro */

/*
 * adjust physical top of memory to useful top of memory:
 * preserve space for console program (1024 bytes)
 * and memory bitmap (one bit per 512-byte page; why do we save it?)
 */
machmem(hi)
int hi;
{
	return(hi - (hi/(NBPG*NBBY)) - 1024);
}

/*
 * miscellaneous machine-dependent initialization
 * called just after mapping turned on
 *
 * - make instruction emulation code accessible from user space
 * - reset and enable IO
 */
machinit()
{
	register int *p;		/* pun; really struct pte */
	register char *e;
	register struct iomflow *q;
	extern char _emulbeg, _emulend;

	e = &_emulbeg;
	p = (int *)&Sysmap[btop((int)e & ~KSTART)];
	do {
		*p = (*p &~ PG_PROT) | PG_URKR;
		p++;
		e += NBPG;
	} while (e < &_emulend);
	mtpr(IORESET, 0);
	q = (struct iomflow *)iospace;
	q->c.mser = q->c.mser | PARENB;		/* clear stale error bits */
}

/*
 * stray interrupt handling:
 * just decrypt it and return
 */
strayintr(v)
int v;
{
	if (v < 0x200)
		printf("stray interrupt at 0x%x\n", v);
	else
		printf("stray Q-bus interrupt at 0%o\n", v-0x200);
}

/*
 * how big is io space?
 */

mchiopsize()
{
	return (sizeof(struct iomflow));
}

/*
 * set up the page tables for iospace
 * called while the system page table is being assembled;
 * memory mapping is off
 * pt is the first page table of an area
 * mapping what mchiopsize returned
 */

mchiopinit(pt)
struct pte *pt;
{
	register long *p;	/* pun, for efficiency */
	register long b;
	register int i;

	p = (long *)pt;
	*p++ = PG_V|PG_KW|btop(0x20080000);	/* cpu regs */
	*p++ = PG_V|PG_KW|btop(0x200b8000);	/* watch chip regs */
	b = btop(0x20088000);			/* Q-bus map */
	for (i = 0; i < (8192*sizeof(long))/NBPG; i++)
		*p++ = PG_V|PG_KW|b++;
	b = btop(0x20000000);			/* Q-bus io regs */
	for (i = 0; i < 8192/NBPG; i++)
		*p++ = PG_V|PG_KW|b++;
}

/*
 * return the IO regs for a Q-bus adapter
 * (there's really only one, but only this code knows that)
 */

caddr_t
qbaaddr(u)
int u;
{
	if (u != 0)
		return (0);
	return ((caddr_t)&((struct iomflow *)iospace)->u[u]);
}

/*
 * arrange for a restart on halt
 * -- it would be slightly preferable
 * to fall back to a boot if the restart fails;
 * alas, to do that on MicroVAX II,
 * you must disable console halts
 */

setrestart()
{
	register short *p;

	p = &((struct iomflow *)iospace)->w.cpmbx;
	*p &=~ (BOOTIH|RSTIH|HALTACT);
	*p |= HMRST;		/* always just restart */
}

/*
 * arrange for a boot, now or on next halt
 * -- sometimes called with mapping disabled
 */

setboot()
{
	register short *p;

	if (mfpr(MAPEN))
		p = &((struct iomflow *)iospace)->w.cpmbx;
	else
		p = (short *)0x200b801c;
	*p &=~ (BOOTIH|HALTACT);
	*p |= HMBOOT;		/* halt mode `boot' */
}

/*
 * time-of-year clock:
 * in MicroVAX, it's a watch chip
 * just use it as a counter with funny digits
 */

#define	BYEAR	82	/* arbitrary non-leap-year base */
#define	TODRRES	100	/* TODR units per second */

#define	UIP	0200	/* csra - update in progress */
#define	AMAG	040	/* csra - magic clock modes */
#define	SET	0200	/* csrb - set clock */
#define	DM	04	/* csrb - binary (not bcd) */
#define	M24	02	/* csrb - 24-hour mode */
#define	VRT	0200	/* csrd - valid clock */

#define	BOFF	(SET|DM|M24)	/* stop clock and ready for setting */
#define	BON	(DM|M24)	/* let clock go again */

static char dmsize[] = {
	31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31,
	31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31,
};

struct ty {	/* just for convenience */
	char sec, min, hr, day, mon, yr;
};

gettodr()
{
	register int s, i;
	register struct watchregs *regs;
	struct ty ty;

	regs = &((struct iomflow *)iospace)->w;
	while(regs->csra & UIP)
		;
	/*
	 * we now have 244us to read the clock
	 */
	if ((regs->csrd & VRT) == 0) {	/* invalid time */
		regs->csrb = BOFF;
		regs->csra = AMAG;
		regs->yr = 0;		/* just make it wrong */
		regs->csrb = BON;
		return (0);		/* and return a wrong answer */
	}
	s = spl7();
	ty.sec = regs->sec;
	ty.min = regs->min;
	ty.hr = regs->hr;
	ty.day = regs->day;
	ty.mon = regs->mon;
	ty.yr = regs->yr;
	splx(s);
	if (ty.sec < 0 || ty.sec > 59
	||  ty.min < 0 || ty.min > 59
	||  ty.hr < 0 || ty.hr > 23
	||  ty.day < 1 || ty.day > 31
	||  ty.mon < 1 || ty.mon > 12
	||  ty.yr < BYEAR || ty.yr > BYEAR+1)
		return 0;
	if (ty.yr > BYEAR)
		ty.mon += 12;	/* overflow */
	s = 0;
	for(i = 0; i < ty.mon-1; i++)
		s += dmsize[i];
	s += ty.day-1;
	s = 24*s + ty.hr;
	s = 60*s + ty.min;
	s = 60*s + ty.sec;
	return (s*TODRRES+TODRZERO);
}

/*
 * This routine is used to set the MicroVAX-II toy register.
 */
settodr(tim)
{
	register int yd;
	register struct watchregs *regs;
	struct ty ty;

	regs = &((struct iomflow *)iospace)->w;
	tim = (tim - TODRZERO)/TODRRES;		/* truncated -- too bad */
	ty.sec = tim % 60;
	tim /= 60;
	ty.min = tim % 60;
	tim /= 60;
	ty.hr = tim % 24;
	tim /= 24;
	yd = tim % 365;
	tim /= 365;
	ty.yr = BYEAR + tim;
	for (ty.mon = 0; yd > dmsize[ty.mon]; ty.mon++)
		yd -= dmsize[ty.mon];
	ty.day = yd;
	regs->csrb = BOFF;
	regs->csra = AMAG;
	regs->sec = ty.sec;
	regs->min = ty.min;
	regs->hr = ty.hr;
	regs->day = ty.day+1;
	regs->mon = ty.mon+1;
	regs->yr = ty.yr;
	regs->csrb = BON;
}
