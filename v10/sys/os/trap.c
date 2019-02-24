#include "sys/param.h"
#include "sys/systm.h"
#include "sys/user.h"
#include "sys/proc.h"
#include "sys/reg.h"
#include "sys/trap.h"
#include "sys/psl.h"
#include "sys/mtpr.h"
#include "sys/vmmeter.h"

#define	USER	040		/* user-mode flag added to type */

int	syscnt[MAXSYS];

/*
 * Called from the trap handler when a processor trap occurs.
 */
/*ARGSUSED*/
trap(sp, type, code, pc, psl)
unsigned code;
{
	register int *locr0 = ((int *)&psl)-PS;
	register int i;
	register struct proc *p;
	time_t syst;

	cnt.v_trap++;
	syst = u.u_vm.vm_stime;
	if (USERMODE(locr0[PS])) {
		type |= USER;
		u.u_ar0 = locr0;
	}
	switch (type) {

	default:
spl7();
		printf("type %d sp x%x code x%x pc x%x psl x%x locr0 x%x\n",
			type, sp, code, pc, psl, locr0);
		panic("trap");

	case PROTFLT + USER:	/* protection fault */
		i = SIGBUS;
		break;

	case PRIVINFLT + USER:	/* privileged instruction fault */
	case RESADFLT + USER:	/* reserved addressing fault */
	case RESOPFLT + USER:	/* resereved operand fault */
		u.u_code = type &~ USER;
		i = SIGILL;
		break;

	case ASTFLT + USER:	/* Allow process switch */
		astoff();
		if ((u.u_procp->p_flag & SOWEUPC) && u.u_prof.pr_scale) {
			addupc(pc, &u.u_prof, 1);
			u.u_procp->p_flag &= ~SOWEUPC;
		}
		goto out;

	case ARITHTRAP + USER:
		u.u_code = code;
		i = SIGFPE;
		break;

	/*
	 * If the user SP is above the stack segment,
	 * grow the stack automatically.
	 */
	case SEGFLT + USER:
		if (grow((unsigned)locr0[SP]) || grow(code))
			goto out;
		i = SIGSEGV;
		break;

	case TABLEFLT:		/* allow page table faults in kernel mode */
	case TABLEFLT + USER:   /* page table fault */
		panic("ptable fault");

	case PAGEFLT:		/* allow page faults in kernel mode */
	case PAGEFLT + USER:	/* page fault */
		i = u.u_error;
		pagein(code, &u);
		u.u_error = i;
		if (type == PAGEFLT)
			return;
		goto out;

	case BPTFLT + USER:	/* bpt instruction fault */
	case TRCTRAP + USER:	/* trace trap */
		locr0[PS] &= ~PSL_T;
		i = SIGTRAP;
		break;

	case XFCFLT + USER:	/* xfc instruction fault */
		i = SIGEMT;
		break;

	case COMPATFLT + USER:	/* compatibility mode fault */
		u.u_code = code;
		i = SIGILL;
		break;
	}
	psignal(u.u_procp, i);
out:
	p = u.u_procp;
	if (p->p_cursig || (p->p_sig && issig()))
		psig();
	p->p_pri = p->p_usrpri;
	if (runrun) {
		/*
		 * Since we are u.u_procp, clock will normally just change
		 * our priority without moving us from one queue to another
		 * (since the running process is not on a queue.)
		 * If that happened after we setrq ourselves but before we
		 * swtch()'ed, we might not be on the queue indicated by
		 * our priority.
		 */
		(void) spl6();
		setrq(p);
		swtch();
	}
	if (u.u_prof.pr_scale && (syst -= u.u_vm.vm_stime))
		addupc(locr0[PC], &u.u_prof, (int)-syst);
	curpri = p->p_pri;
}

/*
 * Called from the trap handler when a system call occurs
 * `loathsome' is pointless, but must stay until offsets in reg.h change
 */

syscall(sp, loathsome, code, pc, psl)
register unsigned code;
{
	register int *locr0 = ((int *)&psl)-PS;
	register caddr_t params;
	register struct sysent *callp;
	register struct proc *p;
	register time_t syst;

	syst = u.u_vm.vm_stime;
	if (!USERMODE(psl))
		panic("syscall");
	cnt.v_syscall++;
	u.u_ar0 = locr0;
	params = (caddr_t)locr0[AP] + sizeof(int);
	if (code == 0) {		/* sys indir */
		code = fuword(params);
		params += sizeof(int);
	}
	if (code >= MAXSYS)
		code = 0;
	syscnt[code]++;
	callp = &sysent[code];
	if (callp->sy_narg)
		if (copyin(params, (caddr_t)u.u_arg, callp->sy_narg*sizeof(int)))
			goto bad;
	u.u_ap = u.u_arg;
	u.u_r.r_val1 = 0;
	u.u_r.r_val2 = locr0[R1];
	u.u_error = 0;
	if (setjmp(u.u_qsav)) {
		if (u.u_error == 0)
			u.u_error = EINTR;
	} else
		(*(callp->sy_call))();
	locr0[PS] &= ~PSL_C;
	if (u.u_error) {
bad:
		locr0[R0] = u.u_error;
		locr0[PS] |= PSL_C;	/* carry bit */
	} else {
		locr0[R0] = u.u_r.r_val1;
		locr0[R1] = u.u_r.r_val2;
	}
	p = u.u_procp;
	if (p->p_cursig || (p->p_sig && issig()))
		psig();
	p->p_pri = p->p_usrpri;
	if (runrun) {
		(void) spl6();
		setrq(p);
		swtch();
	}
	if (u.u_prof.pr_scale && (syst -= u.u_vm.vm_stime))
		addupc(locr0[PC], &u.u_prof, (int)-syst);
	curpri = p->p_pri;
}

/*
 * nonexistent system call-- set fatal error code.
 */
nosys()
{
	u.u_error = 100;
}

/*
 * Ignored system call
 */
nullsys()
{

}
