#include "sys/param.h"
#include "sys/systm.h"
#include "sys/meter.h"
#include "sys/callout.h"
#include "sys/user.h"
#include "sys/proc.h"
#include "sys/psl.h"
#include "sys/vm.h"
#include "sys/buf.h"
#include "sys/text.h"
#include "sys/vlimit.h"
#include "sys/mtpr.h"
#include "sys/clock.h"

int	queueflag;

/*
 * trivial kernel profiling; quite expensive in space
 */
#define	KMAX	(200*1024)	/* max kernel text */
#define	KPSIZE	(50*256)	/* number of buckets */
#define	KPROF	1
#if KPROF
long kprof[KPSIZE];
#endif


/*
 * Hardclock is called straight from
 * the real time clock interrupt.
 * We limit the work we do at real clock interrupt time to:
 *	reloading clock
 *	decrementing time to callouts
 *	recording cpu time usage
 *	modifying priority of current process
 *	arrange for soft clock interrupt
 *	kernel pc profiling
 *
 * At software (softclock) interrupt time we:
 *	implement callouts
 *	maintain date
 *	lightning bolt wakeup (every second)
 *	alarm clock signals
 *	jab the scheduler
 *
 * On the vax softclock interrupts are implemented by
 * software interrupts.  Note that we may have multiple softclock
 * interrupts compressed into one (due to excessive interrupt load),
 * but that hardclock interrupts should never be lost.
 */
/*ARGSUSED*/
hardclock(pc, ps)
	caddr_t pc;
{
	register struct callout *p1;
	register struct proc *pp;
	register int s, cpstate;

	/*
	 * reprime clock
	 */
	clkreld();

	/*
	 * update callout times
	 */
	for (p1 = calltodo.c_next; p1 && p1->c_time <= 0; p1 = p1->c_next)
		;
	if (p1)
		p1->c_time--;
	pp = u.u_procp;
	if (USERMODE(ps)) {
		u.u_vm.vm_utime++;
		if (pp->p_nice > NZERO)
			cpstate = CP_NICE;
		else
			cpstate = CP_USER;
		s = 1;
	} else {
#if KPROF
		s = (long)pc - KSTART;
		if (s < 0)
			s = 0;	/* shouldn't */
		else if (s >= KMAX)
			s = KMAX - 1;
		s /= KMAX/KPSIZE;
		kprof[s]++;
#endif
		cpstate = CP_SYS;
		if (queueflag) {
			cpstate = CP_QUEUE;
			s = 0;
		} else if (noproc) {
			if (BASEPRI(ps) == 0)
				cpstate = CP_IDLE;
			s = 0;
		} else {
			u.u_vm.vm_stime++;
			s = 1;
		}
	}
	cp_time[cpstate]++;
	/*
	 * Adjust priority of current process.
	 */
	if (s) {
		pp->p_cpticks++;
		if(++pp->p_cpu == 0)
			--pp->p_cpu;
	}
	++lbolt;
	setsoftclock();
}

#define cpuave(a,b) ((int)(((int)((a)*(b)))/((b)+1)))
extern double avenrun[];
/*
 * Constant for decay filter for cpu usage field
 * in process table (used by ps au).
 */
static float ccpu = 0.9512294245;		/* exp(-1/20) */

/*
 * Software clock interrupt.
 * This routine runs at lower priority than device interrupts.
 *
 * Processes have their (32-bit) priority depend on their owner's ``normalised
 * usage'' of resources. However, V9's low-level scheduler only has 127 priorities,
 * (in fact, only 32, so that someone could use an 'ffs' instruction),
 * so we normalise this ``sharepri'' into the 7-bit ``usrpri''. Note that
 * the 'ffs' hack means that (after PUSER) there are only 20 real priorities
 * for processes to run in, and that defined kernel priorities should differ
 * by more than 4 to be meaningful.
 */
/*ARGSUSED*/
softclock(pc, ps)
	caddr_t pc;
{
	register struct callout *p1;
	register struct proc *pp;
	register int a, s;
	extern char *panicstr;

	/*
	 * Perform callouts (but not after panics)
	 */
	if (panicstr == 0) {
		for (;;) {
			register caddr_t arg;
			register int (*func)();

			s = spl7();
			if ((p1 = calltodo.c_next) == 0 || p1->c_time > 0) {
				splx(s);
				break;
			}
			calltodo.c_next = p1->c_next;
			arg = p1->c_arg;
			func = p1->c_func;
			p1->c_next = callfree;
			callfree = p1;
			(void) splx(s);
			(*func)(arg);
		}
	}

	/*
	 * If idling and processes are waiting to swap in,
	 * check on them.
	 */
	if (noproc && runin) {
		runin = 0;
		wakeup((caddr_t)&runin);
	}
	if (lbolt % (HZ/10) == 0) {
		runrun++;
		aston();
	}

	/*
	 * Lightning bolt every second:
	 *	sleep timeouts
	 *	process priority recomputation
	 *	process %cpu averaging
	 *	p_time and p_slptime for the swapper
	 *	kick swapper if processes want in
	 */
	if (lbolt >= HZ) {
		/* meaningless on VAX; meant for hardclock */
		if (BASEPRI(ps))
			return;
		time += lbolt / HZ;
		lbolt %= HZ;
		wakeup((caddr_t)&lbolt);
		for (pp = proc; pp < procNPROC; pp++)
		if ((a=pp->p_stat)!=0 && a!=SZOMB) {
			if (pp->p_time != 127)
				pp->p_time++;
			if (pp->p_clktim && --pp->p_clktim == 0)
				psignal(pp, SIGALRM);
			if (pp->p_tsleep && --pp->p_tsleep == 0) {
				s = spl6();
				switch (pp->p_stat) {	/* != a if interrupted */

				case SSLEEP:
					setrun(pp);
					break;

				case SSTOP:
					unsleep(pp);
					break;
				}
				pp->p_flag |= STIMO;
				splx(s);
			}
			if (a==SSLEEP || a==SSTOP)
				if (pp->p_slptime != 127)
					pp->p_slptime++;
			/*
			 * update silly numbers for ps to print
			 */
			if (pp->p_flag&SLOAD)
				pp->p_pctcpu = ccpu*pp->p_pctcpu +
				    (1.0 - ccpu) * (pp->p_cpticks/(float)HZ);
			pp->p_cpticks = 0;

			/*
			 * Update p_cpu for scheduling
			 */
			a = cpuave(pp->p_cpu, 2*avenrun[0]);
			if (a < 0)
				a = 0;
			if (a > 255)
				a = 255;
			pp->p_cpu = a;
			(void) setpri(pp);
			/*
			 * p_usrpri == new process priority
			 * now fix p_pri to match, carefully
			 */
			s = spl6();
			if (pp->p_pri >= PUSER && pp->p_pri != pp->p_usrpri) {
				if ((pp != u.u_procp || noproc) && pp->p_stat == SRUN &&
				    (pp->p_flag & SLOAD)) {
					remrq(pp);
					pp->p_pri = pp->p_usrpri;
					setrq(pp);
				} else
					pp->p_pri = pp->p_usrpri;
			}
			splx(s);
		}
		if (runin!=0) {
			runin = 0;
			wakeup((caddr_t)&runin);
		}
	}
	if (noproc)
		return;
	pp = u.u_procp;
	/*
	 * If trapped user-mode, give it a profiling tick.
	 */
	if (USERMODE(ps) && u.u_prof.pr_scale) {
		pp->p_flag |= SOWEUPC;
		aston();
	}
	/*
	 * random statistics, apparently computed just so
	 * that programs like ps can print them?  not a good reason
	 */
	s = pp->p_rssize;
	u.u_vm.vm_idsrss += s;
	if (pp->p_textp) {
		a = pp->p_textp->x_rssize;
		s += a;
		u.u_vm.vm_ixrss += a;
	}
	if (s > u.u_vm.vm_maxrss)
		u.u_vm.vm_maxrss = s;
	/*
	 * cpu time limit
	 */
	if ((u.u_vm.vm_utime+u.u_vm.vm_stime+1)/HZ > u.u_limit[LIM_CPU]) {
		psignal(pp, SIGXCPU);
		if (u.u_limit[LIM_CPU] < INFINITY - 5)
			u.u_limit[LIM_CPU] += 5;
	}
}

/*
 * Timeout is called to arrange that
 * fun(arg) is called in tim/HZ seconds.
 * An entry is linked into the callout
 * structure.  The time in each structure
 * entry is the number of HZ's more
 * than the previous entry.
 * In this way, decrementing the
 * first entry has the effect of
 * updating all entries.
 *
 * The panic is there because there is nothing
 * intelligent to be done if an entry won't fit.
 */
timeout(fun, arg, tim)
	int (*fun)();
	caddr_t arg;
{
	register struct callout *p1, *p2, *pnew;
	register int t;
	int s;

	t = tim;
	s = spl7();
	pnew = callfree;
	if (pnew == NULL)
		panic("timeout table overflow");
	callfree = pnew->c_next;
	pnew->c_arg = arg;
	pnew->c_func = fun;
	for (p1 = &calltodo; (p2 = p1->c_next) && p2->c_time < t; p1 = p2)
		t -= p2->c_time;
	p1->c_next = pnew;
	pnew->c_next = p2;
	pnew->c_time = t;
	if (p2)
		p2->c_time -= t;
	splx(s);
}

/*
 *      Delay goes to sleep on a unique address for a
 *      guaranteed minimum period ticks/HZ secs.
 *      Because a timeout() can't be cancelled, the process
 *      will be unkillable while asleep.  Beware of giving
 *      delay() an argument of more than a few hundred.
 *
 * used by one device driver and by sys nap.
 * do we really need it?
 */
delay(ticks)
{
        register int    x;
        extern wakeup();

        if (ticks<=0)
                return;
        x = spl7();
        timeout(wakeup, (caddr_t)u.u_procp+1, ticks);
        sleep((caddr_t)u.u_procp+1, PZERO-1);
        splx(x);
}

/*
 * init callouts
 * called from main
 */

callinit()
{
	register int i;

	callfree = callout;
	for (i = 1; i < calloutcnt; i++)
		callout[i-1].c_next = &callout[i];
}
