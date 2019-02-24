/*
 * machine-dependent init stuff;
 * specific to VAX but general to all models
 */

#include "sys/param.h"
#include "sys/systm.h"
#include "sys/user.h"
#include "sys/proc.h"
#include "sys/map.h"
#include "sys/reg.h"
#include "sys/mtpr.h"
#include "sys/clock.h"
#include "sys/pte.h"
#include "sys/vm.h"
#include "sys/psl.h"
#include "sys/buf.h"
#include "sys/reboot.h"
#include "sys/conf.h"
#include "sys/text.h"
#include "sys/cmap.h"
#include "sys/msgbuf.h"

/*
 * table of little bits of page table needed
 * by various bits of code
 * the right way to do this would be
 * to have a pool, and hand space out dynamically
 */

extern char *CADDR1, *CADDR2, *CADDR3;
extern struct pte *CMAP1, *CMAP2, *CMAP3, *mmap;
extern char *vmmap;
extern char *priobuf;
extern struct pte *Prbufmap;

struct {
	int npages;
	struct pte **pmap;
	caddr_t *pmem;
} mmlist[] = {
	UPAGES, &Swapmap, (caddr_t *)&swaputl,
	UPAGES, &Forkmap, (caddr_t *)&forkutl,
	UPAGES, &Xswapmap, (caddr_t *)&xswaputl,
	UPAGES, &Xswap2map, (caddr_t *)&xswap2utl,
	UPAGES, &Pushmap, (caddr_t *)&pushutl,
	UPAGES, &Prusrmap, (caddr_t *)&prusrutl,
	CLSIZE, &Prbufmap, (caddr_t *)&priobuf,
	CLSIZE, &msgbufmap, (caddr_t *)&msgbuf,
	USRPTSIZE, &Usrptmap, (caddr_t *)&usrpt,
	1,	&CMAP1, (caddr_t *)&CADDR1,
	1,	&CMAP2, (caddr_t *)&CADDR2,
	1,	&CMAP3, (caddr_t *)&CADDR3,
	1,	&mmap, (caddr_t *)&vmmap,
	-1
};

struct pte *physspt;
struct pte *Sysmap;
struct pte *Usrptmap;
struct pte *usrpt;

/*
 * init the memory map
 * called early on, well before main
 * memory management is off
 *
 * the system page table is allocated here, out of free memory
 * the theory is:
 *	map the static kernel first
 *	then any pseudo-dynamic data structures
 *	then the buffers
 *	then lay down the system page table
 *	then map io space
 *
 * subtlety: we generally want end and etext to be physical addresses.
 * (long)&end & ~KSTART seems to be the only expression that is constant
 * with and without the c optimizer.
 */

mmapinit(hiaddr)
long hiaddr;
{
	extern int end, etext;
	extern char *iospace;
	register int i;
	register long *p;	/* pun; really struct pte * */
	register long mtop;	/* highest real memory address used */
	register long ptop;	/* highest virtual address used */
	register long v;

	maxmem = physmem = btoc(hiaddr);
	mtop = (long)&end & ~KSTART;
	/*
	 * dynamic things: add to memtop, set addrs along the way
	 */
	if (nbuf == 0) {
		nbuf = 32 * physmem / btoc(1024*1024);
		if (nbuf < 32)
			nbuf = 32;
	}
	buf = (struct buf *)(KSTART+mtop);
	mtop += nbuf * sizeof(struct buf);
	mtop = (mtop + NBPG - 1) & ~PGOFSET;
	buffers = (char *)(KSTART+mtop);
	mtop += nbuf * BUFSIZE;
	/*
	 * cmaps; we'll slightly overestimate; too bad
	 */
	cmap = (struct cmap *)(KSTART+mtop);
	ncmap = (physmem * NBPG - mtop)/(CLSIZE*NBPG + sizeof(struct cmap));
	mtop += ncmap * sizeof(struct cmap);
	ecmap = (struct cmap *)(KSTART+mtop);
	/*
	 * clear all the memory we just handed out
	 */
	p = (long *)((long)&end & ~KSTART);
	while (p < (long *)mtop)
		*p++ = 0;
	/*
	 * now the things that aren't memory
	 * maps are relative to beginning of spt
	 */
	mtop = (mtop + NBPG - 1) & ~PGOFSET;
	ptop = mtop;
	for (i = 0; mmlist[i].npages >= 0; i++) {
		*mmlist[i].pmem = (caddr_t)(ptop + KSTART);
		*mmlist[i].pmap = (struct pte *)(btoc(ptop) * sizeof(struct pte));
		ptop += ctob(mmlist[i].npages);
	}
	iospace = (char *)(ptop + KSTART);
	ptop += mchiopsize();
	Sysmap = (struct pte *)(ptop + KSTART);
	physspt = (struct pte *)mtop;
	p = (long *)mtop;
	i = (int)&etext & ~KSTART;	/* not - KSTART; optimizer gets it wrong */
	for (v = 0; v < mtop; v += NBPG)	/* kernel text + data */
		if (v < i)
			*p++ = PG_V|PG_KR|btoc(v);
		else
			*p++ = PG_V|PG_KW|btoc(v);
	for (; v < ptop; v += NBPG)		/* mystery maps */
		*p++ = 0;
	ptop += (btoc(ptop) + btoc(btoc(ptop)*sizeof(struct pte)))*sizeof(struct pte);
	for (i = btoc(mtop); v < ptop; v += NBPG, i++)		/* spt */
		*p++ = PG_V|PG_KW|i;
	firstfree = i + 1;
	/*
	 * fix the random maps
	 */
	for (i = 0; mmlist[i].npages >= 0; i++)
		*(long *)mmlist[i].pmap += (long)Sysmap;
	mchiopinit(physspt + btoc(iospace - KSTART));
	p = (long *)(physspt + (msgbufmap - Sysmap));
	for (i = 0; i < btoc(sizeof(struct msgbuf)); i++)
		*p++ = PG_V|PG_KW|(physmem - btoc(sizeof(struct msgbuf)) + i);
	maxmem -= CLSIZE;
	mtpr(SBR, mtop);
	mtpr(SLR, btoc(ptop));
}

/*
 * set up context for process 0:
 * first page of free memory becomes page table
 * next UPAGES become user block
 * init the process control block slightly
 * called before memory mapping is turned on
 */

uctinit(kpc)
long kpc;
{
	register long *p;	/* pun: struct pte * */
	register int i;
	register struct pcb *pp;

	p = (long *)ctob(firstfree);
	for (i = 0; i < (UPAGES+1)*NBPG; i += sizeof(long))
		*p++ = 0;
	p = (long *)(physspt + ((long)usrpt-KSTART)/NBPG);
	*p = PG_V|PG_KW|firstfree;	/* one page for first p0 page table */
	p = (long *)ctob(firstfree+1);
	for (i = UPAGES; i > 0; --i)
		*--p = PG_V|PG_URKW|firstfree+i;
	pp = (struct pcb *)ctob(firstfree+1);
	pp->pcb_p0br = usrpt;
	pp->pcb_p0lr = 0 + AST_NONE;	/* no p0 space mapped */
	pp->pcb_p1br = usrpt + NPTEPG - P1TOP;
	pp->pcb_p1lr = P1TOP - UPAGES;
	pp->pcb_ksp = KSTART;
	pp->pcb_esp = pp->pcb_ssp = -1;		/* something invalid */
	pp->pcb_usp = USRSTACK;
	pp->pcb_psl = 0;		/* mode (kernel,kernel), ipl 0 */
	pp->pcb_pc = kpc;
	firstfree += UPAGES + 1;
	mtpr(PCBB, pp);
}

/*
 * Machine-dependent startup code
 * some of this is excessively specific to the paging code
 */

int dmmin, dmmax, dmtext;

startup()
{
	register long maxsz;
	register unsigned i;
	register struct proc *p;
	extern struct map kernelmap[];
	extern int kernelcnt;
	extern char version[];

	setrestart();
	printf("%s\n", version);
	meminit(firstfree, maxmem);
	maxmem = freemem;
	printf("mem = %d\n", ctob(maxmem));
	rminit(kernelmap, kernelcnt, USRPTSIZE-1, 1);
	/*
	 * dmmin fixed for now
	 * dmmax == first power of 2 larger than max?size/(NDMAP-1)
	 */
	dmmin = DMMIN;
	maxsz = maxtsize / NXDAD;
	for (i = 0x80000000; i; i >>= 1)
		if (maxsz & i)
			break;
	i *= 2;
	if (i < dmmin)
		i = dmmin;
	dmtext = i;
	maxsz = maxdsize > maxssize ? maxdsize : maxssize;
	maxsz /= NDMAP-1;
	if (maxsz < dmtext)
		maxsz = dmtext;
	for (i = 0x80000000; i; i >>= 1)
		if (maxsz & i)
			break;
	i *= 2;
	if (i < dmmin)
		i = dmmin;
	dmmax = i;
	maxpgio *= nswdevt;
	/*
	 * set up vm aspects of first process
	 */
	p = &proc[0];
	p->p_p0br = (struct pte *)mfpr(P0BR);
	p->p_szpt = 1;
	p->p_addr = uaddr(p);
	setredzone(p->p_addr, (caddr_t)&u);
	u.u_pcb.pcb_szpt = CLSIZE;
}

/*
 * init unix time
 * base is some nearly correct number, e.g. from a filesystem.
 * try to get something better from the VAX time-of-year clock,
 * which contains the number of .01-second clicks in the current year
 */
clkinit(base)
	time_t base;
{
	register unsigned long todr;
	int year = YRREF;

	todr = gettodr();
	if (todr < TODRZERO) {	/* too small; TODR was restarted */
		time = base;
		clkset();	/* so fix it */
	}
	else {
		for (time = (todr-TODRZERO)/100; time < base-SECYR/2; time += SECYR) {
			if (LEAPYEAR(year))
				time += SECDAY;
			year++;
		}
	}
}

/*
 * once a day or so, set TOY clock to match system clock
 */

static time_t clknext;

clkcheck()
{

	if (clknext == 0)
		clknext = time + SECDAY;
	if (clknext > time)
		return;
	clknext = time + SECDAY;
	clkset();
}

/*
 * reset the TOY clock to match current unix time
 */

clkset()
{
	int year = YRREF;
	unsigned secyr;
	unsigned yrtime = time;

	for (;;) {
		secyr = SECYR;
		if (LEAPYEAR(year))
			secyr += SECDAY;
		if (yrtime < secyr)
			break;
		yrtime -= secyr;
		year++;
	}
	settodr(TODRZERO + yrtime*100);
}

/*
 * bias the clock to correct an error of `wrong' milliseconds
 * -- just tick slightly slowly or quickly for a while
 * MicroVAX has no NICR, but writing it is a no-op;
 * let the clock interrupt code do the work
 */

int clockbias;		/* number of ticks */
char clocksign;		/* direction to correct */

biasclock()
{
	register struct a {
		int wrong;
	} *uap;
	register long nticks;

	uap = (struct a *)u.u_ap;
	if (!suser())
		return;
	nticks = (long)uap->wrong * 1000;	/* microseconds */
	nticks /= CLKBIAS;			/* number of ticks to bias */
	spl7();
	if (nticks < 0) {
		mtpr(NICR, CLK60HZ-CLKBIAS);
		clockbias = -nticks;
		clocksign = -1;		/* -1 => clock must run slower */
	}
	else {
		mtpr(NICR, CLK60HZ+CLKBIAS);
		clockbias = nticks;
		clocksign = 1;
	}
	spl0();
}

/*
 * Return the difference (in microseconds)
 * between the  current time and a previous
 * time as represented  by the arguments.
 * If there is a pending clock interrupt
 * which has not been serviced due to high
 * ipl, return error code.
 */
vmtime(otime, olbolt, oicr)
	register int otime, olbolt, oicr;
{

	if (mfpr(ICCS)&ICCS_INT)
		return(-1);
	else
		return(((time-otime)*60 + lbolt-olbolt)*16667 + mfpr(ICR)-oicr);
}

/*
 * Send an interrupt to process
 *
 * SHOULD CHANGE THIS TO PASS ONE MORE WORK SO THAT ALL INFORMATION
 * PROVIDED BY HARDWARE IS AVAILABLE TO THE USER PROCESS.
 */
sendsig(p, n)
	int (*p)();
{
	register int *usp, *regs;

	regs = u.u_ar0;
	usp = (int *)regs[SP];
	usp -= 5;
	if ((int)usp <= USRSTACK - ctob(u.u_ssize))
		(void) grow((unsigned)usp);
	if (useracc((caddr_t)usp, 5*sizeof(int), B_WRITE) == 0)
		goto bad;
	*usp++ = n;
	if (n == SIGILL || n == SIGFPE) {
		*usp++ = u.u_code;
		u.u_code = 0;
	} else
		*usp++ = 0;
	*usp++ = (int)p;
	*usp++ = regs[PC];
	*usp++ = regs[PS];
	regs[SP] = (int)(usp - 5);
	regs[PS] &= ~(PSL_CM|PSL_FPD);
	regs[PC] = (int)u.u_pcb.pcb_sigc;
	return;

bad:
	/*
	 * Process has trashed its stack; give it an illegal
	 * instruction to halt it in its tracks.
	 */
	u.u_signal[SIGILL] = SIG_DFL;
	P_SETDFL(u.u_procp, SIGMASK(SIGILL));
	psignal(u.u_procp, SIGILL);
}

/*
 * Invalidate single all pte's in a cluster
 */
tbiscl(v)
	unsigned v;
{
	register caddr_t addr;
	register int i;

	addr = ptob(v);
	for (i = 0; i < CLSIZE; i++) {
		mtpr(TBIS, addr);
		addr += NBPG;
	}
}

/*
 * sys reboot
 * can this please go away?
 */

boot(howto)
	int howto;
{

	if (howto&RB_HALT)
		death();
	reboot(1);
}

/*
 * freeze.  can't just halt because
 * on some machines that would cause a boot
 */

death()
{
	splx(0x1f);
	printf("death\n");
	for (;;)
		;
}
