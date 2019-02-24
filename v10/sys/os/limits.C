/*
 * limits routines
 */

#include "sys/param.h"
#include "sys/systm.h"
#include "sys/user.h"
#include "sys/file.h"
#include "sys/inode.h"
#include "sys/lnode.h"
#include "sys/proc.h"
#include "sys/share.h"

#ifndef	ETOOMANYU
#define	ETOOMANYU	ENOSPC	/* Must go in errno.h or lnode.h */
#endif

float	NiceDecays[2*NZERO];	/* Table for pre-calculated priority decays */
float	NiceRates[2*NZERO];	/* Table for pre-calculated rate increments */
long	NiceTicks[2*NZERO];	/* Array of costs for a cpu tick biased by p_nice */
KL_p	lastlnode;

/*
 * limits system call
 *
 * Functions
 *
 *	0 - L_MYLIM
 *	9 - L_MYKN
 *	    return own limit struct
 *	    return number of processes attached to node
 *
 *	1 - L_OTHLIM
 *	10- L_OTHKN
 *	    return limit struct of l_uid
 *	    return number of processes attached to node or ESRCH
 *
 *	2 - L_ALLLIM
 *	11- L_ALLKN
 *	    return all active limit structs
 *	    return number of structs returned
 *
 *	3 - L_SETLIM super-user only
 *	    initialize limit struct for l_uid
 *	    ETOOMANYU	- no spare limit structs
 *	    ESRCH	- group not installed
 *
 *	4 - L_DEADLIM
 *	    wait for a dead child, return limit struct and proc entry,
 *	    and if LASTREF, remove dead limit struct from the active list.
 *	    LASTREF set for dead limit struct
 *	    ECHILD	- no children to wait for
 *	    return number of processes attached to node
 *
 *	5 - L_CHNGLIM super-user only
 *	    change limits for active limit struct
 *	    ESRCH	- l_uid not found
 *
 *	6 - L_DEADGROUP
 *	    return limit struct for a dead group and remove group
 *	    ESRCH	- no dead groups
 *
 *	7 - L_GETCOSTS
 *	    return share constants
 *
 *	8 - L_SETCOSTS	super-user only
 *	    set share constants
 */
limits()
{
	register struct a {
		struct lnode *lp;
		int select;
	} *uap;
	register KL_p ol;
	register KL_p nl;
	register KL_p gl;
	register int i;
	register struct proc *up;
	register int size;
	static struct kern_lnode kl;

	up = u.u_procp;
	size = sizeof(struct kern_lnode);

	uap = (struct a *)u.u_ap;
	switch(uap->select) {
	case L_MYLIM:
		if(copyout((caddr_t)&up->p_lnode->kl, (caddr_t)uap->lp, sizeof(struct lnode)))
			u.u_error = EFAULT;
		u.u_r.r_val1 = up->p_lnode->kl_refcount;
		return;

	case L_MYKN:
		if(copyout((caddr_t)up->p_lnode, (caddr_t)uap->lp, size))
			u.u_error = EFAULT;
		u.u_r.r_val1 = up->p_lnode->kl_refcount;
		return;

	case L_OTHLIM:
		size = sizeof(struct lnode);
	case L_OTHKN:
		if(copyin((caddr_t)uap->lp, (uap->select == L_OTHLIM)?(caddr_t)&kl.kl:(caddr_t)&kl, size)) {
			u.u_error = EFAULT;
			return;
		}
		for(ol = lnodes; ol != (KL_p)0; ol = ol->kl_next) {
			if(ol->kl.l_uid == kl.kl.l_uid) {
				if(copyout((uap->select == L_OTHLIM)?(caddr_t)&ol->kl:(caddr_t)ol, (caddr_t)uap->lp, size))
					u.u_error = EFAULT;
				u.u_r.r_val1 = ol->kl_refcount;
				return;
			}
		}
		u.u_error = ESRCH;
		return;

	case L_ALLLIM:
		size = sizeof(struct lnode);
	case L_ALLKN:
		i = 0;
		for(ol = lnodes; ol != (KL_p)0; ol = ol->kl_next) {
			if(copyout((uap->select == L_ALLLIM)?(caddr_t)&ol->kl:(caddr_t)ol, (caddr_t)uap->lp, size)) {
				u.u_error = EFAULT;
				return;
			}
			*(caddr_t *)&uap->lp += size;
			i++;
		}
		u.u_r.r_val1 = i;
		return;

	case L_SETLIM:
		if(!suser())
			return;
		if(copyin((caddr_t)uap->lp, (caddr_t)&kl.kl, sizeof(struct lnode))) {
			u.u_error = EFAULT;
			return;
		}
		if(kl.kl.l_uid == up->p_lnode->kl.l_uid)
			return;
		nl = gl = (KL_p)0;
		for(ol = lnodes, i = 0; ol < lnodesMAXUSERS; ol++) {
			if(!(ol->kl.l_flags & ACTIVELNODE)) {
				if(nl == NULL)
					nl = ol;
				continue;
			}
			if(ol->kl.l_uid == kl.kl.l_uid) {
				nl = ol;
				goto found;
			}
			if(!(ol->kl.l_flags & NOTSHARED) && ++i >= MAXUSERS) {
				u.u_error = ETOOMANYU;
				return;
			}
			if(ol->kl.l_uid == kl.kl.l_group)
				gl = ol;	/* my group */
		}
		if(nl == NULL) {
			u.u_error = ETOOMANYU;
			return;
		}
		if(gl == (KL_p)0) {
			u.u_error = ESRCH;	/* No group installed yet */
			return;
		}
		if(grouplevel(gl)>=MAXGROUPS) {
			u.u_error = ESRCH;	/* ETOODEEPFORME? */
			return;
		}
		nl->kl = kl.kl;
		nl->kl.l_flags &= ~CHNGDLIMITS;
		nl->kl_ghead = (KL_p)0;
		if(nl->kl.l_flags & NOTSHARED)
			nl->kl_gshares = zerof;
		else
			nl->kl_gshares = nl->kl.l_shares;
		nl->kl_temp = zerof;
		nl->kl_children = 0;
		nl->kl_cost = 0;
		nl->kl_refcount = 0;
		nl->kl_muse = 0;
		nl->kl_rate = onef;
		splshsched();	/* Hopefully, this locks out the clock scan too */
		addgroup(gl, nl);
		normshares(gl, 0);
		fixusage(nl);
		nl->kl_prev = gl;	/* insert into active list */
		if((nl->kl_next = gl->kl_next) == (KL_p)0)
			lastlnode = nl;
		else
			gl->kl_next->kl_prev = nl;
		gl->kl_next = nl;
		spl0();
	found:
		nl->kl.l_flags &= ~(LASTREF|DEADGROUP);
		nl->kl.l_flags |= ACTIVELNODE;
		ol = up->p_lnode;
		if(--ol->kl_refcount == 0) {
			if(ol->kl_children == 0)
				ol->kl.l_flags |= DEADGROUP;
		}
		nl->kl_refcount++;
		i = up->p_dsize + up->p_ssize;
		ol->kl_muse -= i;
		nl->kl_muse += i;
		up->p_lnode = nl;
		return;

#if NOTDEF		/* unused and unpleasant */
	case L_DEADLIM:
		wait1(WRETLIM, NULL);	/* will call retlimits() */
		return;
#endif
	case L_DEADGROUP:
		if(!suser())
			return;
		for(ol = lnodes[0].kl_next; ol != (KL_p)0; ol = ol->kl_next)
			if(ol->kl_refcount == 0 && ol->kl_children == 0) {
				remlnode(ol);
				if(copyout((caddr_t)&ol->kl, (caddr_t)uap->lp, sizeof(struct lnode)))
					u.u_error = EFAULT;
				return;
			}
		u.u_error = ESRCH;
		return;

	case L_CHNGLIM:
		if(!suser())
			return;
		if(copyin((caddr_t)uap->lp, (caddr_t)&kl.kl, sizeof(struct lnode))) {
			u.u_error = EFAULT;
			return;
		}
		if(kl.kl.l_uid == 0) {
			u.u_error = EINVAL;
			return;
		}
		for(ol = lnodes, gl = (KL_p)0; ol != (KL_p)0; ol = ol->kl_next) {
			if(ol->kl.l_uid == kl.kl.l_uid) {
				if(ol->kl.l_group != kl.kl.l_group) {
					if(gl==(KL_p)0)
						break;	/* ESRCH */
					else {
						splshsched();
						remgroup(ol);
						ol->kl.l_group = kl.kl.l_group;
						addgroup(gl, ol);
					}
				} else if(gl==(KL_p)0)
					panic("lost group");

				if(ol->kl_norms && ol->kl_usage < MAXUSAGE)
					TotUsage -= onef / ol->kl_usage;
				ol->kl.l_usage = kl.kl.l_usage;

				if(ol->kl.l_shares != kl.kl.l_shares
				|| ((ol->kl.l_flags^kl.kl.l_flags) & NOTSHARED)) {
					gl->kl_gshares -= ol->kl.l_shares;
					if(!(ol->kl.l_flags & NOTSHARED))
						ol->kl_gshares -= ol->kl.l_shares;
					ol->kl.l_shares = kl.kl.l_shares;
					gl->kl_gshares += ol->kl.l_shares;
					if(!(kl.kl.l_flags & NOTSHARED))
						ol->kl_gshares += ol->kl.l_shares;
				}
				ol->kl.l_flags &= (ACTIVELNODE|LASTREF|DEADGROUP);
				ol->kl.l_flags |= (kl.kl.l_flags & ~(ACTIVELNODE|LASTREF|DEADGROUP)) | CHNGDLIMITS;
				normshares(gl, 0);
				fixusage(ol);
				spl0();
				return;
			} else if(ol->kl.l_uid == kl.kl.l_group)
				gl = ol;	/* Remember [new] group */
		}
		u.u_error = ESRCH;
		return;

	case L_GETCOSTS:
		if(copyout((caddr_t)&shconsts, (caddr_t)uap->lp, sizeof shconsts))
			u.u_error = EFAULT;
		return;

	case L_SETCOSTS:
		if(!suser())
			return;
		splshsched();
		if(copyin((caddr_t)uap->lp, (caddr_t)&shconsts, (caddr_t)&LASTPARAM - (caddr_t)&shconsts))
			u.u_error = EFAULT;
		setcosts();
		spl0();
		return;
			
	default:
		u.u_error = EINVAL;
		return;
	}
}

/*
 * Add to group list
 */

addgroup(gl, kl)
register KL_p gl;
register KL_p kl;
{
	kl->kl_parent = gl;
	kl->kl_gnext = gl->kl_ghead;
	gl->kl_ghead = kl;
	gl->kl_children++;
	gl->kl_gshares += kl->kl.l_shares;
	gl->kl.l_flags &= ~DEADGROUP;
}

#if NOTDEF

/*
 * Return dead proc and limits to init for accounting.
 */

retlimits(p)
register struct proc *p;
{
	register KL_p lp;
	register struct a {
		struct retlim *rp;
	} *uap;

	lp = p->p_lnode;
	if(lp->kl_refcount == 0) {
		lp->kl.l_flags |= LASTREF;
		if(lp->kl_children == 0)
			remlnode(lp);
	}

	uap = (struct a *)u.u_ap;
	if(copyout((caddr_t)p, (caddr_t)&uap->rp->r_proc, sizeof(struct xproc))
	|| copyout((caddr_t)&lp->kl, (caddr_t)&uap->rp->r_lnode, sizeof(struct lnode)))
		u.u_error = EFAULT;

	u.u_r.r_val1 = lp->kl_refcount;
}

#endif

/*
 * Remove lnode from active and group lists.
 */

remlnode(lp)
register KL_p lp;
{
	splshsched();

	if((lp->kl_prev->kl_next = lp->kl_next) == (KL_p)0)
		lastlnode = lp->kl_prev;
	else
		lp->kl_next->kl_prev = lp->kl_prev;

	remgroup(lp);

	spl0();

	if(lp->kl_temp += lp->kl_cost) {
		lp->kl.l_charge += lp->kl_temp;
		lp->kl_parent->kl_temp += lp->kl_temp;
		lp->kl.l_usage += lp->kl_temp;
	}

	lp->kl.l_flags &= ~ACTIVELNODE;

	if(lp->kl_parent->kl.l_flags & DEADGROUP)
		lp->kl.l_flags |= DEADGROUP;	/* init should pick up group */
}

/*
 * Remove lnode from group list.
 */

remgroup(lp)
register KL_p lp;
{
	register KL_p gl, ol;

	for(ol = lp->kl_parent, gl = ol->kl_ghead; gl != (KL_p)0; ol = gl, gl = gl->kl_gnext)
		if(gl == lp) {
			/* remove from group list */
			if(ol == lp->kl_parent)
				ol->kl_ghead = lp->kl_gnext;
			else
				ol->kl_gnext = lp->kl_gnext;
			break;
		}

	if(gl==(KL_p)0) panic("remgroup");

	gl = lp->kl_parent;

	if(--gl->kl_children == 0 && gl->kl_refcount == 0)
		gl->kl.l_flags |= DEADGROUP;

	gl->kl_gshares -= lp->kl.l_shares;
	normshares(gl, 0);
}

/*
 * Adjust effective shares for group.
 */

normshares(gl, d)
register KL_p gl;
{
	register KL_p lp;
	register float es;
	register float gs;

	if((gs = gl->kl_gshares) && !(gl->kl.l_flags & NOTSHARED)) {
		es = gl->kl_eshare * gl->kl.l_shares;
		es /= gs;
		gl->kl_norms = es * es;
	} else if(gl->kl.l_uid == 0)
		gl->kl_norms = onef;
	else
		gl->kl_norms = zerof;

	for(lp = gl->kl_ghead; lp != (KL_p)0; lp = lp->kl_gnext) {
		if(gs) {
			es = gl->kl_eshare * lp->kl.l_shares;
			es /= gs;
		} else
			es = zerof;

		lp->kl_eshare = es;

		if(lp->kl_ghead) {
			if(d >= MAXGROUPS)
				printf("MAXGROUPS(%d) exceeded for group %d\n", MAXGROUPS, lp->kl.l_uid);
			else
				normshares(lp, d+1);
		} else
			lp->kl_norms = es * es;
	}
}

/*
 * Count group nesting
 */

grouplevel(gl)
register KL_p gl;
{
	register int i;

	for(i = 0; (gl = gl->kl_parent) != (KL_p)0; i++);
	return i;
}

/*
 * Check and set cost dependent variables
 */

setcosts()
{
	register int i;
	register float f = onef;
	register float g;

	if(MAXUSERS > lnodecnt)
		MAXUSERS = lnodecnt;
	if(MAXGROUPS > (lnodecnt-3))
		MAXGROUPS = lnodecnt-3;
	if(MAXUSHARE < f)
		MAXUSHARE = f;
	if(MINGSHARE > f)
		MINGSHARE = f;

	f -= DecayRate;
	g = PriDecay / (PriDecayBase + (2*NZERO) - 1);
	for(i = 0; i < (2*NZERO); i++) {
		NiceDecays[i] = g * (PriDecayBase + i);
		NiceRates[i] = i <= NZERO ? f : (f * NZERO) / i;
		NiceTicks[i] = (shconsts.sc_tick * ((3*NZERO) - i)) / (2*NZERO);
	}

	if(NiceTicks[2*NZERO-1] > 0)
		NiceTicks[2*NZERO-1] = 1;	/* nice -19 is almost free */
	NiceRates[2*NZERO-1] = zerof;
}

/*
 *	Initialise lnodes
 */

initlnodes(root)
register KL_p root;
{
	lastlnode = root;
	MAXUSERS = lnodecnt;
	MaxSharePri = onef;

	setcosts();

	root->kl.l_shares = 1;
	root->kl.l_flags = ACTIVELNODE|NOTSHARED;
	root->kl_norms = onef;
	root->kl_eshare = onef;
	root->kl_gshares = zerof;
	root->kl_temp = zerof;
	root->kl_usage = onef;
	root->kl_rate = onef;
}

/*
 *	Adjust usage variables for new value.
 */

fixusage(lp)
	register KL_p	lp;
{
	register float	f;

	if(f = lp->kl_norms) {
		if((f = lp->kl.l_usage / f) < twof || (Shareflags & NOSHARE))
			f = twof;
		if(f > MaxUsage && f < MAXUSAGE)
			MaxUsage = f;
		TotUsage += onef / f;
		lp->kl_usage = f;
	} else
		lp->kl_usage = MAXUSAGE;
}
