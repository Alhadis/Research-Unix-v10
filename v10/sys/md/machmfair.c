/*
 * machine-specific hardware routines,
 * these for MicroVAX III
 */

#include "sys/param.h"
#include "sys/pte.h"
#include "sys/mtpr.h"
#include "sys/vm.h"
#include "sys/qbio.h"
#include "sys/clock.h"

#define	CADR	37		/* first-level cache control */
#define	IORESET	55		/* reset all IO connections */
#define	CEN	020		/* cacr: cache enable */
#define	CAEN	0360		/* CADR: enable both halves, both I and D */

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
int delayfact = 3;		/* factor for DELAY macro */

/*
 * adjust physical top of memory to useful top of memory:
 * preserve memory bitmap (one bit per 512-byte page)
 * and Q-bus map, which steals memory as well
 * the console is meant to have left it all at the top
 * but we may not have found the top, if intermediate bits are broken
 */
machmem(hi)
register int hi;
{
	register int qm;

	qm = *(int *)0x20080010;	/* addr of Q-bus map in mem */
	qm -= qm/(NBPG*NBBY);		/* bitmap */
	if (hi < qm)			/* in case memory has holes */
		return (hi);
	return (qm);
}

/*
 * miscellaneous machine-dependent initialization
 * called just after mapping turned on
 *
 * - make instruction emulation code accessible from user space
 * - reset and enable IO
 * - enable caches
 *
 * eventually some of the init stuff moves to machreset
 */
machinit()
{
	register int *p;		/* pun; really struct pte */
	register char *e;
	register struct iomfair *q;
	register int i;
	extern char _emulbeg, _emulend;

	e = &_emulbeg;
	p = (int *)&Sysmap[btop((int)e & ~KSTART)];
	do {
		*p = (*p &~ PG_PROT) | PG_URKR;
		p++;
		e += NBPG;
	} while (e < &_emulend);
	mtpr(IORESET, 0);
	q = (struct iomfair *)iospace;
	for (i = 0; i < CACHESIZE; i++)
		q->d.cd[i] = 0;		/* flush second-level cache */
	q->d.cacr |= CEN;	/* and enable it */
	mtpr(CADR, mfpr(CADR)|CAEN);
	machreset();
	mcrinit();
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
	return (sizeof(struct iomfair));
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
	*p++ = PG_V|PG_KW|btop(0x20140400);	/* NVRAM */
	*p++ = PG_V|PG_KW|btop(0x20084000);	/* cache register */
	b = btop(0x10000000);			/* cache diagnostic space */
	for (i = 0; i < btop(CACHESIZE*sizeof(long)); i++)
		*p++ = PG_V|PG_KW|b++;
	b = btop(0x20088000);			/* Q-bus map */
	for (i = 0; i < btop(NQMREG*sizeof(long)); i++)
		*p++ = PG_V|PG_KW|b++;
	b = btop(0x20000000);			/* Q-bus io regs */
	for (i = 0; i < btop(8192); i++)
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
	return ((caddr_t)&((struct iomfair *)iospace)->u[u]);
}

/*
 * arrange for a restart on halt
 * -- it would be slightly preferable
 * to fall back to a boot if the restart fails;
 * alas, to do that on MicroVAX II or III,
 * you must disable console halts
 */

setrestart()
{
	register short *p;

	p = &((struct iomfair *)iospace)->w.cpmbx;
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
		p = &((struct iomfair *)iospace)->w.cpmbx;
	else
		p = (short *)0x20140400;
	*p &=~ (BOOTIH|RSTIH|HALTACT);
	*p |= HMBOOT;		/* halt mode `boot' */
}

/*
 * fetch/set time-of-year clock
 */

gettodr()
{
	return (mfpr(TODR));
}

settodr(t)
long t;
{
	mtpr(TODR, t);
}
