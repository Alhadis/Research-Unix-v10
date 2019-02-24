#include "sys/param.h"
#include "sys/user.h"
#include "sys/proc.h"
#include "sys/vm.h"
#include "sys/pte.h"
#include "sys/cmap.h"
#include "sys/mtpr.h"
#include "sys/vlimit.h"
#include "sys/vadvise.h"

int	both;

vadvise()
{
	register struct a {
		int	anom;
	} *uap;
	register struct proc *rp = u.u_procp;
	int oanom = rp->p_flag & SUANOM;
	register struct pte *pte;
	register struct cmap *c;
	register int i;

	uap = (struct a *)u.u_ap;
	rp->p_flag &= ~(SSEQL|SUANOM);
	switch (uap->anom) {

	case VA_ANOM:
		rp->p_flag |= SUANOM;
		break;

	case VA_SEQL:
		rp->p_flag |= SSEQL;
		break;
	}
	if (both || (oanom && (rp->p_flag & SUANOM) == 0)) {
		for (i = 0; i < rp->p_dsize; i += CLSIZE) {
			pte = dptopte(rp, i);
			if (pte->pg_v) {
				c = &cmap[pgtocm(pte->pg_pfnum)];
				if (c->c_lock)
					continue;
				pte->pg_v = 0;
				if (anycl(pte, pg_m))
					pte->pg_m = 1;
				distcl(pte);
			}
		}
		mtpr(TBIA, 0);
	}
}

vtimes()
{
	register struct a {
		struct	vtimes *par_vm;
		struct	vtimes *ch_vm;
	} *uap = (struct a *)u.u_ap;
	if (uap->par_vm) {
		if (copyout((caddr_t)&u.u_vm, (caddr_t)uap->par_vm,
		    sizeof(struct vtimes)) < 0)
			u.u_error = EFAULT;
	}
	if (uap->ch_vm == 0)
		return;
	if (copyout((caddr_t)&u.u_cvm, (caddr_t)uap->ch_vm,
	    sizeof(struct vtimes)) < 0)
		u.u_error = EFAULT;
}

vmsadd(vp, wp)
	register struct vtimes *vp, *wp;
{

	vp->vm_utime += wp->vm_utime;
	vp->vm_stime += wp->vm_stime;
	vp->vm_nswap += wp->vm_nswap;
	vp->vm_idsrss += wp->vm_idsrss;
	vp->vm_ixrss += wp->vm_ixrss;
	if (vp->vm_maxrss < wp->vm_maxrss)
		vp->vm_maxrss = wp->vm_maxrss;
	vp->vm_majflt += wp->vm_majflt;
	vp->vm_minflt += wp->vm_minflt;
	vp->vm_inblk += wp->vm_inblk;
	vp->vm_oublk += wp->vm_oublk;
}

/*
 * Affect per-process limits.
 * To just return old limit, specify negative new limit.
 */
vlimit()
{
	register struct a {
		unsigned which;
		int	limit;
	} *uap;

	uap = (struct a *)u.u_ap;
	if (uap->which > NLIMITS) {
		u.u_error = EINVAL;
		return;
	}
	u.u_r.r_val1 = u.u_limit[uap->which];
	if (uap->limit < 0)
		return;
	switch (uap->which) {

	case LIM_DATA:
		if (uap->limit > ctob(maxdsize))
			uap->limit = ctob(maxdsize);
		break;

	case LIM_TEXT:
		if (uap->limit > ctob(maxtsize))
			uap->limit = ctob(maxtsize);
		break;

	case LIM_STACK:
		if (uap->limit > ctob(maxssize))
			uap->limit = ctob(maxssize);
		break;
	}
	if (u.u_limit[LIM_NORAISE] && uap->limit > u.u_limit[uap->which] &&
	    !suser()) {
		u.u_error = EACCES;
		return;
	}
	u.u_limit[uap->which] = uap->limit;
	if (uap->which == LIM_MAXRSS)
		u.u_procp->p_maxrss = uap->limit/NBPG;
}
