#include "sys/param.h"
#include "sys/systm.h"
#include "sys/user.h"
#include "sys/proc.h"
#include "sys/file.h"
#include "sys/inode.h"
#include "sys/vm.h"
#include "sys/pte.h"
#include "sys/mtpr.h"

#define SQSIZE 0100	/* Must be power of 2 */
#define HASH(x)	(( (int) x >> 5) & (SQSIZE-1))
struct proc *slpque[SQSIZE];

/*
 * Give up the processor till a wakeup occurs
 * on chan, at which time the process
 * enters the scheduling queue at priority pri.
 * The most important effect of pri is that when
 * pri<=PZERO a signal cannot disturb the sleep;
 * if pri>PZERO signals will be processed.
 * Callers of this routine must be prepared for
 * premature return, and check that the reason for
 * sleeping has gone away.
 */
sleep(chan, pri)
caddr_t chan;
{
	register struct proc *rp, **hp;
	register s;

	rp = u.u_procp;
	s = spl6();
	if (chan==0 || rp->p_stat != SRUN || rp->p_rlink)
		panic("sleep");
	rp->p_wchan = chan;
	rp->p_slptime = 0;
	rp->p_pri = pri;
	hp = &slpque[HASH(chan)];
	rp->p_link = *hp;
	*hp = rp;
	if(pri > PZERO) {
		if(rp->p_sig && issig()) {
			if (rp->p_wchan)
				unsleep(rp);
			rp->p_stat = SRUN;
			(void) spl0();
			goto psig;
		}
		if (rp->p_wchan == 0)
			goto out;
		rp->p_stat = SSLEEP;
		(void) spl0();
		swtch();
		if(rp->p_sig && issig())
			goto psig;
	} else {
		rp->p_stat = SSLEEP;
		(void) spl0();
		swtch();
	}
out:
	splx(s);
	return;

	/*
	 * If priority was low (>PZERO) and
	 * there has been a signal,
	 * execute non-local goto to
	 * the qsav location.
	 * (see trap1/trap.c)
	 */
psig:
	longjmp(u.u_qsav);
	/*NOTREACHED*/
}

/*
 * Sleep on chan at pri.
 * Return in no more than the indicated number of seconds.
 * (If seconds==0, no timeout implied)
 * Return	TS_OK if chan was awakened normally
 *		TS_TIME if timeout occurred
 *		TS_SIG if asynchronous signal occurred
 */
tsleep(chan, pri, seconds)
caddr_t chan;
{
	label_t lqsav;
	register n, rval;
	register struct proc *pp = u.u_procp;

	n = spl7();
	bcopy((caddr_t)u.u_qsav, (caddr_t)lqsav, sizeof (label_t));
	pp->p_tsleep = seconds;
	if (setjmp(u.u_qsav))
		rval = TS_SIG;
	else {
		sleep(chan, pri);
		if (pp->p_flag&STIMO)
			rval = TS_TIME;
		else
			rval = TS_OK;
	}
	pp->p_flag &= ~STIMO;
	bcopy((caddr_t)lqsav, (caddr_t)u.u_qsav, sizeof (label_t));
	splx(n);
	return(rval);
}

/*
 * Remove a process from its wait queue
 */
unsleep(p)
register struct proc *p;
{
	register struct proc **hp;
	register s;

	s = spl6();
	if (p->p_wchan) {
		hp = &slpque[HASH(p->p_wchan)];
		while (*hp != p)
			hp = &(*hp)->p_link;
		*hp = p->p_link;
		p->p_wchan = 0;
	}
	splx(s);
}

/*
 * Wake up all processes sleeping on chan.
 */
wakeup(chan)
register caddr_t chan;
{
	register struct proc *p, **q, **h;
	int s;

	s = spl6();
	h = &slpque[HASH(chan)];
restart:
	for (q = h; p = *q; ) {
		if (p->p_rlink || p->p_stat != SSLEEP && p->p_stat != SSTOP)
			panic("wakeup");
		if (p->p_wchan==chan) {
			p->p_wchan = 0;
			*q = p->p_link;
			p->p_slptime = 0;
			p->p_tsleep = 0;
			if (p->p_stat == SSLEEP) {
				/* OPTIMIZED INLINE EXPANSION OF setrun(p) */
				p->p_stat = SRUN;
				if (p->p_flag & SLOAD)
					setrq(p);
				if(p->p_pri < curpri) {
					runrun++;
					aston();
				}
				if ((p->p_flag&SLOAD) == 0) {
					if (runout != 0) {
						runout = 0;
						wakeup((caddr_t)&runout);
					}
					wantin++;
				}
				/* END INLINE EXPANSION */
				goto restart;
			}
		} else
			q = &p->p_link;
	}
	splx(s);
}

/*
 * Initialize the (doubly-linked) run queues
 * to be empty.
 */
rqinit()
{
	register int i;

	for (i = 0; i < NQS; i++)
		qs[i].ph_link = qs[i].ph_rlink = (struct proc *)&qs[i];
}

/*
 * Set the process running;
 * arrange for it to be swapped in if necessary.
 */
setrun(p)
register struct proc *p;
{
	register s;

	s = spl6();
	switch (p->p_stat) {
	default:
		panic("setrun");

	case SSTOP:
	case SSLEEP:
		unsleep(p);		/* e.g. when sending signals */
		break;

	case SIDL:
		break;
	}
	p->p_stat = SRUN;
	if (p->p_flag & SLOAD)
		setrq(p);
	splx(s);
	if(p->p_pri < curpri) {
		runrun++;
		aston();
	}
	if ((p->p_flag&SLOAD) == 0) {
		if(runout != 0) {
			runout = 0;
			wakeup((caddr_t)&runout);
		}
		wantin++;
	}
}

/*
 * Set user priority.
 * The rescheduling flag (runrun)
 * is set if the priority is better
 * than the currently running process.
 */
setpri(pp)
register struct proc *pp;
{
	register int p;

	p = pp->p_cpu/4;
	p += PUSER + 2*(pp->p_nice - NZERO);
	if (pp->p_rssize > pp->p_maxrss && freemem < desfree)
		p += 2*4;	/* i.e. nice(4) */
	if (p > PRIMAX)
		p = PRIMAX;
	if(p < curpri) {
		runrun++;
		aston();
	}
	pp->p_usrpri = p;
	return(p);
}

/*
 * Create a new process--
 * the internal version of sys fork.
 * supply a process slot and a pid.
 * returns 1 in the new process, 0 in the old.
 */
newproc(rpp, pid)
register struct proc *rpp;
int pid;
{
	register struct proc *rip;
	register int n;

	/*
	 * init the proc entry
	 */
	if (rpp->p_stat != 0)
		panic("newproc");
	rip = u.u_procp;
	bzero((caddr_t)rpp, sizeof(*rpp));
	rpp->p_stat = SIDL;
	rpp->p_flag = SLOAD | (rip->p_flag & (SSEXEC|SPAGI|STRC|SPROCTR));
	rpp->p_uid = rip->p_uid;
	rpp->p_pgrp = rip->p_pgrp;
	rpp->p_nice = rip->p_nice;
	rpp->p_textp = rip->p_textp;
	rpp->p_pid = pid;
	rpp->p_ppid = rip->p_pid;
	rpp->p_pptr = rip;
	rpp->p_siga0 = rip->p_siga0;
	rpp->p_siga1 = rip->p_siga1;
	/* take along any pending signals, like stops? */
	rpp->p_tsize = rip->p_tsize;
	rpp->p_dsize = rip->p_dsize;
	rpp->p_ssize = rip->p_ssize;
	rpp->p_szpt = rip->p_szpt;
	rpp->p_maxrss = rip->p_maxrss;
	/*
	 * duplicate file references and so on
	 */
	for(n=0; n<NOFILE; n++)
		if(u.u_ofile[n] != NULL)
			u.u_ofile[n]->f_count++;
	u.u_cdir->i_count++;
	if (u.u_rdir)
		u.u_rdir->i_count++;
	/*
	 * make the copy
	 * child returns 1 when made runnable
	 * parent makes child runnable and returns 0
	 */
	rip->p_flag |= SKEEP;	/* prevent parent from being swapped */
	if (procdup(rpp))
		return (1);
	(void) spl6();
	rpp->p_stat = SRUN;
	setrq(rpp);
	(void) spl0();
	/* SSWAP NOT NEEDED IN THIS CASE AS u.u_pcb.pcb_sswap SUFFICES */
	/* rpp->p_flag |= SSWAP; */
	rip->p_flag &= ~SKEEP;
	return (0);
}
