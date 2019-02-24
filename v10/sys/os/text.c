#include "sys/param.h"
#include "sys/systm.h"
#include "sys/user.h"
#include "sys/proc.h"
#include "sys/text.h"
#include "sys/inode.h"
#include "sys/buf.h"
#include "sys/pte.h"
#include "sys/vm.h"
#include "sys/cmap.h"

/*
 * relinquish use of the shared text segment
 * of a process.
 */
xfree()
{
	register struct text *xp, *xp1;
	register struct inode *ip;

	if((xp=u.u_procp->p_textp) == NULL)
		return;
	xlock(xp);
	ip = xp->x_iptr;
	if(--xp->x_count==0) {
		xunlink(u.u_procp);
		xp->x_rssize -= vmemfree(tptopte(u.u_procp, 0), u.u_tsize);
		if (xp->x_rssize != 0)
			panic("xfree rssize");
		for (xp1 = text; xp1 < textNTEXT; xp1++)
			if (xp1->x_iptr == ip && xp1 != xp)
				break;
		if (xp1 >= textNTEXT)
			ip->i_flag &= ~ITEXT;
		if (ip->i_flag&ILOCK)
			ip->i_count--;
		else
			iput(ip);
		while (xp->x_poip)
			sleep((caddr_t)&xp->x_poip, PSWP+1);
		vsxfree(xp, xp->x_size);
		xp->x_flag &= ~XLOCK;
		xp->x_iptr = NULL;
	} else {
		xp->x_flag &= ~XLOCK;
		xccdec(xp, u.u_procp);
	}
	u.u_procp->p_textp = NULL;
}

/*
 * Attach to a shared text segment.
 * If there is no shared text, just return.
 * If there is, hook up to it:
 * if it is not currently being used, it has to be read
 * in from the inode (ip); the written bit is set to force it
 * to be written out as appropriate.
 * If it is being used, but is not currently in core,
 * a swap has to be done to get it back.
 */
xalloc(ip, pagi)
register struct inode *ip;
{
	register struct text *xp;
	register clicks_t ts;
	register struct text *xp1;
	register struct proc *p = u.u_procp;

	if(u.u_exdata.ux_tsize == 0)
		return;
again:
	xp1 = NULL;
	for (xp = text; xp < textNTEXT; xp++) {
		if(xp->x_iptr == NULL) {
			if (xp1 == NULL) {
				xp1 = xp;
				if (PTRACED(p))
					break;
			}
			continue;
		}
		/* don't attach if proc is traced */
		if (PTRACED(p))
			continue;
		if (xp->x_iptr==ip && (xp->x_flag&XTRC)==0 && xp->x_count!=0) {
			if (xp->x_flag&XLOCK) {
				xwait(xp);
				goto again;
			}
			xlock(xp);
			xp->x_count++;
			p->p_textp = xp;
			xlink(p);
			xunlock(xp);
			return;
		}
	}
	if((xp=xp1) == NULL) {
		tablefull("text");
		psignal(p, SIGKILL);
		return;
	}
	xp->x_flag = XLOAD|XLOCK;
	if (PTRACED(p))
		xp->x_flag |= XTRC;	/* if proc is traced, so is text */
	if (pagi)
		xp->x_flag |= XPAGI;
	ts = clrnd(btoc(u.u_exdata.ux_tsize));
	xp->x_size = ts;
	if (vsxalloc(xp) == NULL) {
		swkill(p, "xalloc");
		return;
	}
	xp->x_count = 1;
	xp->x_ccount = 0;
	xp->x_rssize = 0;
	xp->x_iptr = ip;
	ip->i_flag |= ITEXT;
	ip->i_count++;
	p->p_textp = xp;
	xlink(p);
	if (pagi == 0) {
		u.u_count = u.u_exdata.ux_tsize;
		if(u.u_exdata.ux_mag == 0413)	/* 0413 on 4k file sys */
			u.u_offset = ltoL(BSIZE(0));
		else
			u.u_offset = ltoL(sizeof(u.u_exdata));
		u.u_base = 0;
		u.u_segflg = SEGUINST;
		settprot(1);
		p->p_flag |= SKEEP;
		readi(ip);
		p->p_flag &= ~SKEEP;
	}
	settprot(0);
	u.u_segflg = SEGUDATA;
	xp->x_flag |= XWRIT;
	xp->x_flag &= ~XLOAD;
	xunlock(xp);
}

/*
 * Lock and unlock a text segment from swapping
 */
xlock(xp)
register struct text *xp;
{

	while(xp->x_flag&XLOCK) {
		xp->x_flag |= XWANT;
		sleep((caddr_t)xp, PSWP);
	}
	xp->x_flag |= XLOCK;
}

/*
 * Wait for xp to be unlocked if it is currently locked.
 */
xwait(xp)
register struct text *xp;
{

	xlock(xp);
	xunlock(xp);
}

xunlock(xp)
register struct text *xp;
{

	if (xp->x_flag&XWANT)
		wakeup((caddr_t)xp);
	xp->x_flag &= ~(XLOCK|XWANT);
}

/*
 * Decrement the in-core usage count of a shared text segment.
 * When it drops to zero, free the core space.
 */
xccdec(xp, p)
register struct text *xp;
register struct proc *p;
{

	if (xp==NULL || xp->x_ccount==0)
		return;
	xlock(xp);
	if (--xp->x_ccount == 0) {
		if (xp->x_flag & XWRIT) {
			vsswap(p, tptopte(p, 0), CTEXT, 0, xp->x_size, (struct dmap *)0);
			if (xp->x_flag & XPAGI)
				swap(p, xp->x_ptdaddr, (caddr_t)tptopte(p, 0),
				    xp->x_size * sizeof (struct pte),
				    B_WRITE, B_PAGET, swapdev, 0);
			xp->x_flag &= ~XWRIT;
		} else
			xp->x_rssize -= vmemfree(tptopte(p, 0), xp->x_size);
		if (xp->x_rssize != 0)
			panic("text rssize");
	}
	xunlink(p);
	xunlock(xp);
}

/*
 * free the swap image of all unused saved-text text segments
 * which are in the same filesystem as ip
 * called when unmounting
 */
xumount(ip)
register struct inode *ip;
{
	register struct text *xp;

	for (xp = text; xp < textNTEXT; xp++) 
		if (xp->x_iptr!=NULL && ip->i_mpoint==xp->x_iptr->i_mpoint)
			xuntext(xp);
}

/*
 * remove a shared text segment from the text table, if possible.
 */
xrele(ip)
register struct inode *ip;
{
	register struct text *xp;

	if ((ip->i_flag&ITEXT)==0)
		return;
	for (xp = text; xp < textNTEXT; xp++)
		if (ip==xp->x_iptr)
			xuntext(xp);
}

/*
 * remove text image from the text table.
 * the use count must be zero.
 */
xuntext(xp)
register struct text *xp;
{
	register struct inode *ip;

	xlock(xp);
	if (xp->x_count) {
		xunlock(xp);
		return;
	}
	ip = xp->x_iptr;
	xp->x_flag &= ~XLOCK;
	xp->x_iptr = NULL;
	vsxfree(xp, xp->x_size);
	ip->i_flag &= ~ITEXT;
	if (ip->i_flag&ILOCK)
		ip->i_count--;
	else
		iput(ip);
}

/*
 * Add a process to those sharing a text segment by
 * getting the page tables and then linking to x_caddr.
 */
xlink(p)
	register struct proc *p;
{
	register struct text *xp = p->p_textp;

	if (xp == 0)
		return;
	vinitpt(p);
	p->p_xlink = xp->x_caddr;
	xp->x_caddr = p;
	xp->x_ccount++;
}

xunlink(p)
	register struct proc *p;
{
	register struct text *xp = p->p_textp;
	register struct proc *q;

	if (xp == 0)
		return;
	if (xp->x_caddr == p) {
		xp->x_caddr = p->p_xlink;
		p->p_xlink = 0;
		return;
	}
	for (q = xp->x_caddr; q->p_xlink; q = q->p_xlink)
		if (q->p_xlink == p) {
			q->p_xlink = p->p_xlink;
			p->p_xlink = 0;
			return;
		}
	panic("lost text");
}
