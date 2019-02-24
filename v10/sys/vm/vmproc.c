/*	vmproc.c	4.5	81/03/08	*/

#include "sys/param.h"
#include "sys/systm.h"
#include "sys/user.h"
#include "sys/proc.h"
#include "sys/mtpr.h"
#include "sys/pte.h"
#include "sys/map.h"
#include "sys/cmap.h"
#include "sys/text.h"
#include "sys/inode.h"
#include "sys/vm.h"

struct pte *Forkmap;
struct user *forkutl;

struct dmap zdmap;		/* well, it has to be somewhere */

/*
 * Get virtual memory resources for a new process.
 * Called after page tables are allocated, but before they
 * are initialized, we initialize the memory management registers,
 * and then expand the page tables for the data and stack segments
 * creating zero fill pte's there.  Text pte's are set up elsewhere.
 *
 * SHOULD FREE EXTRA PAGE TABLE PAGES HERE OR SOMEWHERE.
 */
vgetvm(ts, ds, ss)
	clicks_t ts, ds, ss;
{

	mtpr(P0LR, ts);
	u.u_pcb.pcb_p0lr = ts | AST_NONE;
	mtpr(P1LR, P1TOP - UPAGES);
	u.u_pcb.pcb_p1lr = P1TOP - UPAGES;
	u.u_procp->p_tsize = ts;
	u.u_tsize = ts;
	expand((int)ss, P1BR);
	expand((int)ds, P0BR);
}

/*
 * Release the virtual memory resources (memory
 * pages, and swap area) associated with the current process.
 * Caller must not be swappable.  Used at exit or execl.
 */
vrelvm()
{
	register struct proc *p = u.u_procp;
	
	/*
	 * Release memory; text first, then data and stack pages.
	 */
	xfree();
	p->p_rssize -= vmemfree(dptopte(p, 0), p->p_dsize);
	p->p_rssize -= vmemfree(sptopte(p, p->p_ssize - 1), p->p_ssize);
	if (p->p_rssize != 0)
		panic("vrelvm rss");
	/*
	 * Wait for all page outs to complete, then
	 * release swap space.
	 */
	p->p_swrss = 0;
	while (p->p_poip)
		sleep((caddr_t)&p->p_poip, PSWP+1);
	(void) vsexpand((clicks_t)0, &u.u_dmap, 1);
	(void) vsexpand((clicks_t)0, &u.u_smap, 1);
	p->p_tsize = 0;
	p->p_dsize = 0;
	p->p_ssize = 0;
	u.u_tsize = 0;
	u.u_dsize = 0;
	u.u_ssize = 0;
}

/*
 * Change the size of the data+stack regions of the process.
 * If the size is shrinking, it's easy-- just release virtual memory.
 * If it's growing, initalize new page table entries as 
 * 'zero fill on demand' pages.
 */
expand(change, region)
{
	register struct proc *p;
	register struct pte *base, *p0, *p1;
	struct pte proto;
	int a0, a1;

	p = u.u_procp;
	if (change == 0)
		return;
	if (change % CLSIZE)
		panic("expand");
#ifdef PGINPROF
	vmsizmon();
#endif
	/*
	 * Update the sizes to reflect the change.  Note that we may
	 * swap as a result of a ptexpand, but this will work, because
	 * the routines which swap out will get the current text and data
	 * sizes from the arguments they are passed, and when the process
	 * resumes the lengths in the proc structure are used to 
	 * build the new page tables.
	 */
	if (region == P0BR) {
		p->p_dsize += change;
		u.u_dsize += change;
	} else {
		p->p_ssize += change;
		u.u_ssize += change;
	}
	/*
	 * Compute the end of the text+data regions and the beginning
	 * of the stack region in the page tables,
	 * and expand the page tables if necessary.
	 */
	p0 = (struct pte *)mfpr(P0BR) + mfpr(P0LR);
	p1 = (struct pte *)mfpr(P1BR) + mfpr(P1LR);
	if (change > p1 - p0)
		ptexpand(clrnd(ctopt(change - (p1 - p0))));
	/* PTEXPAND SHOULD GIVE BACK EXCESS PAGE TABLE PAGES */
	/*
	 * Compute the base of the allocated/freed region.
	 */
	if (region == P0BR) {
		base = (struct pte *)mfpr(P0BR);
		base += (a0 = mfpr(P0LR)) + (change > 0 ? 0 : change);
	} else {
		base = (struct pte *)mfpr(P1BR);
		base += (a1 = mfpr(P1LR)) - (change > 0 ? change : 0);
	}

	/*
	 * If we shrunk, give back the virtual memory.
	 */
	if (change < 0)
		p->p_rssize -= vmemfree(base, -change);

	/*
	 * Update the processor length registers and copies in the pcb.
	 */
	if (region == P0BR)  {
		mtpr(P0LR, a0 + change);
		u.u_pcb.pcb_p0lr = a0 + change | (u.u_pcb.pcb_p0lr & AST_CLR);
	} else {
		mtpr(P1LR, a1 - change);
		u.u_pcb.pcb_p1lr = a1 - change;
	}
	*(int *)&proto = PG_UW;
	if (change < 0)
		change = -change;
	else {
		proto.pg_fod = 1;
		((struct fpte *)&proto)->pg_source = PG_FZERO;
		cnt.v_nzfod += change;
	}
	while (--change >= 0)
		*base++ = proto;
	mtpr(TBIA,0);
}

/*
 * Create a duplicate copy of the current process
 * in process slot p, which has been partially initialized
 * by newproc().
 *
 * Could deadlock here if two large proc's get page tables
 * and then each gets part of his UPAGES if they then have
 * consumed all the available memory.  This can only happen when
 *	USRPTSIZE + UPAGES * NPROC > maxmem
 * which happens only when large procs fork on machines with tiny real memories
 */
procdup(p)
	register struct proc *p;
{
	extern struct map kernelmap[];

	/*
	 * allocate page tables and a user block;
	 * copy parent's user to child
	 */
	while (vgetpt(p, vmemall) == 0) {
		kmapwnt++;
		sleep((caddr_t)kernelmap, PSWP+4);
	}
	resume(pcbb(u.u_procp));
	(void) vgetu(p, vmemall, Forkmap, forkutl, &u);
	/*
 	 * child: return here when scheduled and returns 1
	 */
	forkutl->u_pcb.pcb_sswap = u.u_ssav;
	if (savectx(forkutl->u_ssav))
		return (1);
	/*
	 * parent:
	 * finish setting up child:
	 * clear vm counters, attach text, copy data and stack
	 * return 0
	 */
	forkutl->u_vm = zvms;
	forkutl->u_cvm = zvms;
	forkutl->u_dmap = u.u_cdmap;
	forkutl->u_smap = u.u_csmap;
	forkutl->u_outime = 0;
	if (p->p_textp && ((p->p_textp->x_flag&XTRC) == 0 || vmxdup(p))) {
		p->p_textp->x_count++;
		xlink(p);
	}
	vmdup(p, dptopte(p, 0), dptov(p, 0), p->p_dsize, CDATA);
	vmdup(p, sptopte(p, p->p_ssize - 1), sptov(p, p->p_ssize - 1), p->p_ssize, CSTACK);
	return (0);
}

/*
 * Duplicate text segment (cf. xalloc).
 */
vmxdup(p)
register struct proc *p;
{
	register struct text *xp, *uxp;
	register clicks_t ts;

	if ((uxp = u.u_procp->p_textp) == 0)
		return 0;

	for (xp = text; xp < textNTEXT && xp->x_iptr; xp++)
		/* void */ ;
	if (xp >= textNTEXT)
		return 1;

	xp->x_flag = XLOCK|XTRC|XLOAD;
	xp->x_size = ts = uxp->x_size;
	if (vsxalloc(xp) == NULL)
		return 1;

	xp->x_count = 1;
	xp->x_ccount = 0;
	xp->x_rssize = 0;
	(xp->x_iptr = uxp->x_iptr)->i_count++;
	p->p_textp = xp;
	xlink(p);
	vmdup(p, tptopte(p, 0), tptov(p, 0), ts, CTEXT);
	xp->x_flag |= XWRIT;
	xp->x_flag &= ~XLOAD;
	xunlock(xp);

	return 0;
}

vmdup(p, pte, v, count, type)
	struct proc *p;
	register struct pte *pte;
	register unsigned v;
	register clicks_t count;
	int type;
{
	register struct pte *opte = vtopte(u.u_procp, v);
	register int i;

	while (count != 0) {
		count -= CLSIZE;
		if (opte->pg_fod && type != CTEXT) {
			v += CLSIZE;
			for (i = 0; i < CLSIZE; i++)
				*(int *)pte++ = *(int *)opte++;
			continue;
		}
		opte += CLSIZE;
		(void) vmemall(pte, CLSIZE, p, type);
		if (type == CTEXT)
			p->p_textp->x_rssize += CLSIZE;
		else
			p->p_rssize += CLSIZE;
		for (i = 0; i < CLSIZE; i++) {
			copyseg((caddr_t)ctob(v+i), (pte+i)->pg_pfnum);
			*(int *)(pte+i) |= (type == CTEXT) ?
			    ((PG_V|PG_M) + PG_URKR) : ((PG_V|PG_M) + PG_UW);
		}
		v += CLSIZE;
		munlock(pte->pg_pfnum);
		pte += CLSIZE;
	}
}

/*
 * Check that a process will not be too large.
 */
chksize(ts, ds, ss)
	clicks_t ts, ds, ss;
{

	if (ts>maxtsize || ds>maxdsize || ss>maxssize) {
		u.u_error = ENOMEM;
		return(1);
	}
	return (0);
}
