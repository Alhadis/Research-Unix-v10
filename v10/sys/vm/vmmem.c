/*	vmmem.c	4.7	81/07/09	*/

#include "sys/param.h"
#include "sys/systm.h"
#include "sys/pte.h"
#include "sys/cmap.h"
#include "sys/user.h"
#include "sys/proc.h"
#include "sys/text.h"
#include "sys/vm.h"
#include "sys/buf.h"
#include "sys/map.h"

/*
 * Shared text pages are not totally abandoned when a process
 * exits, but are remembered while in the free list hashed by <mdev,blkno>
 * off the cmhash structure so that they can be reattached
 * if another instance of the program runs again soon.
 */
#define	CMHSIZ	512		/* SHOULD BE DYNAMIC */
#define	CMHASH(bn)	((bn)&(CMHSIZ-1))
int	cmhash[CMHSIZ];		/* make words big enough for c_hlink */

/*
 * Allocate memory, and always succeed
 * by jolting page-out daemon
 * so as to obtain page frames.
 * To be used in conjunction with vmemfree().
 */
vmemall(pte, size, p, type)
	register struct pte *pte;
	int size;
	struct proc *p;
{
	register int m;

	if (size <= 0 || size > maxmem)
		panic("vmemall size");
	while (size > 0) {
		if (freemem < desfree)
			wakeup((caddr_t)&proc[PAGEPID]);	/* jolt daemon */
		while (freemem == 0)
			sleep((caddr_t)&freemem, PSWP+2);
		m = imin(size, freemem);
		(void) memall(pte, m, p, type);
		size -= m;
		pte += m;
	}
	if (freemem < desfree)
		wakeup((caddr_t)&proc[PAGEPID]);		/* jolt daemon */
	/*
	 * Always succeeds, but return success for
	 * vgetu and vgetpt (e.g.) which call either
	 * memall or vmemall depending on context.
	 */
	return (1);
}

/*
 * Free valid and reclaimable page frames belonging to the
 * count pages starting at pte.  If a page is valid
 * or reclaimable and locked (but not a system page), then
 * we simply mark the page as c_gone and let the pageout
 * daemon free the page when it is through with it.
 * If a page is reclaimable, and already in the free list, then
 * we mark the page as c_gone, and (of course) don't free it.
 *
 * Determines the largest contiguous cluster of
 * valid pages and frees them in one call to memfree.
 */
vmemfree(pte, count)
	register struct pte *pte;
	register int count;
{
	register struct cmap *c;
	register struct pte *spte;
	register int j;
	int size, pcnt;

	if (count % CLSIZE)
		panic("vmemfree");
	for (size = 0, pcnt = 0; count > 0; pte += CLSIZE, count -= CLSIZE) {
		if (pte->pg_fod == 0 && pte->pg_pfnum) {
			c = &cmap[pgtocm(pte->pg_pfnum)];
			pcnt += CLSIZE;
			if (c->c_lock && c->c_type != CSYS) {
				for (j = 0; j < CLSIZE; j++)
					*(int *)(pte+j) &= PG_PROT;
				c->c_gone = 1;
				goto free;
			}
			if (c->c_free) {
				pcnt -= CLSIZE;
				for (j = 0; j < CLSIZE; j++)
					*(int *)(pte+j) &= PG_PROT;
				if (c->c_type == CTEXT)
					distpte(&text[c->c_ndx], (int)c->c_page, pte);
				c->c_gone = 1;
				goto free;
			}
			if (size == 0)
				spte = pte;
			size += CLSIZE;
			continue;
		}
		if (pte->pg_fod) {
			for (j = 0; j < CLSIZE; j++)
				*(int *)(pte+j) &= PG_PROT;
		}
free:
		if (size) {
			memfree(spte, size, 1);
			size = 0;
		}
	}
	if (size)
		memfree(spte, size, 1);
	return (pcnt);
}

/*
 * Unlink a page frame from the free list -
 *
 * Performed if the page being reclaimed
 * is in the free list.
 */
munlink(pf)
	unsigned pf;
{
	register int next, prev;

	next = cmap[pgtocm(pf)].c_next;
	prev = cmap[pgtocm(pf)].c_prev;
	cmap[prev].c_next = next;
	cmap[next].c_prev = prev;
	cmap[pgtocm(pf)].c_free = 0;
	if (freemem < minfree)
		wakeup((caddr_t)&proc[PAGEPID]);	/* jolt paging daemon */
	freemem -= CLSIZE;
}

/*
 * Allocate memory -
 *
 * The free list appears as a doubly linked list
 * in the core map with cmap[0] serving as a header.
 */
memall(pte, size, p, type)
	register struct pte *pte;
	int size;
	struct proc *p;
{
	register struct cmap *c;
	register struct pte *rpte;
	register struct proc *rp;
	register int i, j;
	int px;
	unsigned pf;
	struct cmap *c1, *c2;

	if (size % CLSIZE)
		panic("memall");
	if (size > freemem)
		return (0);
	px = p - proc;
	for (i = size; i > 0; i -= CLSIZE) {
		c = &cmap[cmap[CMHEAD].c_next];
		if (c->c_free == 0)
			panic("dup mem alloc");
		if (c->c_gone == 0 && c->c_type != CSYS) {
			if (c->c_type == CTEXT)
				rp = text[c->c_ndx].x_caddr;
			else
				rp = &proc[c->c_ndx];
			switch (c->c_type) {

			case CTEXT:
				rpte = tptopte(rp, c->c_page);
				break;

			case CDATA:
				rpte = dptopte(rp, c->c_page);
				break;

			case CSTACK:
				rpte = sptopte(rp, c->c_page);
				break;
			}
			zapcl(rpte, pg_pfnum) = 0;
			if (c->c_type == CTEXT)
				distpte(&text[c->c_ndx], (int)c->c_page, rpte);
		}
		switch (type) {

		case CSYS:
			c->c_ndx = px;
			break;

		case CTEXT:
			c->c_page = vtotp(p, ptetov(p, pte));
			c->c_ndx = p->p_textp - &text[0];
			break;

		case CDATA:
			c->c_page = vtodp(p, ptetov(p, pte));
			c->c_ndx = px;
			break;

		case CSTACK:
			c->c_page = vtosp(p, ptetov(p, pte));
			c->c_ndx = px;
			break;
		}
		if (c->c_blkno) {
			/*
			 * This is very like munhash(), except
			 * that we really don't want to bother
			 * to calculate a dev to pass to it.
			 */
			j = CMHASH(c->c_blkno);
			c1 = &cmap[cmhash[j]];
			if (c1 == c)
				cmhash[j] = c1->c_hlink;
			else {
				for (;;) {
					if (c1 == ecmap)
						panic("memall ecmap");
					c2 = c1;
					c1 = &cmap[c2->c_hlink];
					if (c1 == c)
						break;
				}
				c2->c_hlink = c1->c_hlink;
			}
			if (mfind(c->c_mdev, (daddr_t)c->c_blkno))
				panic("memall mfind");
			c1->c_mdev = 0;
			c1->c_blkno = 0;
			c1->c_hlink = 0;
		}
		pf = cmtopg(c - cmap);
		for (j = 0; j < CLSIZE; j++)
			*(int *)pte++ = pf++;
		c->c_free = 0;
		c->c_gone = 0;
		if (c->c_intrans || c->c_want)
			panic("memall intrans|want");
		c->c_lock = 1;
		c->c_type = type;
		freemem -= CLSIZE;
		j = c->c_next;
		cmap[CMHEAD].c_next = j;
		cmap[j].c_prev = CMHEAD;
	}
	return (size);
}

/*
 * Free memory -
 *
 * The page frames being returned are inserted
 * to the head/tail of the free list depending
 * on whether there is any possible future use of them.
 *
 * If the freemem count had been zero,
 * the processes sleeping for memory
 * are awakened.
 */
memfree(pte, size, detach)
	register struct pte *pte;
	register int size;
{
	register int i, j, prev, next;
	register struct cmap *c;
	
	if (size % CLSIZE)
		panic("memfree");
	if (freemem < CLSIZE * KLMAX)
		wakeup((caddr_t)&freemem);
	while (size > 0) {
		size -= CLSIZE;
		i = pte->pg_pfnum;
		if (i < firstfree || i > maxfree)
			panic("bad mem free");
		i = pgtocm(i);
		c = &cmap[i];
		if (c->c_free)
			panic("dup mem free");
		if (detach && c->c_type != CSYS) {
			for (j = 0; j < CLSIZE; j++)
				*(int *)(pte+j) &= PG_PROT;
			c->c_gone = 1;
		}
		if (detach && c->c_blkno == 0) {
			next = cmap[CMHEAD].c_next;
			cmap[next].c_prev = i;
			c->c_prev = CMHEAD;
			c->c_next = next;
			cmap[CMHEAD].c_next = i;
		} else {
			prev = cmap[CMHEAD].c_prev;
			cmap[prev].c_next = i;
			c->c_next = CMHEAD;
			c->c_prev = prev;
			cmap[CMHEAD].c_prev = i;
		}
		c->c_free = 1;
		freemem += CLSIZE;
		pte += CLSIZE;
	}
}

/*
 * Enter cmap block c on the hash chains.
 * It contains file system block bn from device dev.
 * Dev must either be a mounted file system or the swap device
 * so we panic if getfsx() cannot find it.
 */
mhash(c, mdev, bn)
	register struct cmap *c;
	int mdev;
	daddr_t bn;
{
	register int i = CMHASH(bn);

	if (mdev == -1)
		panic("mhash");
	c->c_hlink = cmhash[i];
	cmhash[i] = c - cmap;
	c->c_blkno = bn;
	c->c_mdev = mdev;
}

/*
 * Pull the clist entry of <dev,bn> off the hash chains.
 * We have checked before calling (using mfind) that the
 * entry really needs to be unhashed, so panic if we can't
 * find it (can't happen).
 */
munhash(mdev, bn)
	register int mdev;
	daddr_t bn;
{
	register int i = CMHASH(bn);
	register struct cmap *c1, *c2;

	c1 = &cmap[cmhash[i]];
	if (c1 == ecmap)
		panic("munhash");
	if (c1->c_blkno == bn && mdev == c1->c_mdev)
		cmhash[i] = c1->c_hlink;
	else {
		for (;;) {
			c2 = c1;
			c1 = &cmap[c2->c_hlink];
			if (c1 == ecmap)
				panic("munhash");
			if (c1->c_blkno == bn && mdev == c1->c_mdev)
				break;
		}
		c2->c_hlink = c1->c_hlink;
	}
	if (mfind(mdev, bn))
		panic("munhash mfind");
	c1->c_mdev = 0;
	c1->c_blkno = 0;
	c1->c_hlink = 0;
}

/*
 * Look for block bn of device dev in the free pool.
 * Currently it should not be possible to find it unless it is
 * c_free and c_gone, although this may later not be true.
 * (This is because active texts are locked against file system
 * writes by the system.)
 */
struct cmap *
mfind(mdev, bn)
	register int mdev;
	daddr_t bn;
{
	register struct cmap *c1 = &cmap[cmhash[CMHASH(bn)]];

	while (c1 != ecmap) {
		if (c1->c_blkno == bn && c1->c_mdev == mdev)
			return (c1);
		c1 = &cmap[c1->c_hlink];
	}
	return ((struct cmap *)0);
}

/*
 * Purge blocks from device dev from incore cache
 * before umount().
 */
mpurge(mdev)
	int mdev;
{
	register struct cmap *c1, *c2;
	register int i;

	for (i = 0; i < CMHSIZ; i++) {
more:
		c1 = &cmap[cmhash[i]];
		if (c1 == ecmap)
			continue;
		if (c1->c_mdev == mdev)
			cmhash[i] = c1->c_hlink;
		else {
			for (;;) {
				c2 = c1;
				c1 = &cmap[c1->c_hlink];
				if (c1 == ecmap)
					goto cont;
				if (c1->c_mdev == mdev)
					break;
			}
			c2->c_hlink = c1->c_hlink;
		}
		c1->c_mdev = 0;
		c1->c_blkno = 0;
		c1->c_hlink = 0;
		goto more;
cont:
		;
	}
}

/*
 * Initialize core map
 */
meminit(first, last)
	int first, last;
{
	register int i;
	register struct cmap *c;

	firstfree = clrnd(first);
	maxfree = clrnd(last - (CLSIZE - 1));
	freemem = maxfree - firstfree;
	ecmx = ecmap - cmap;
	if (ecmx < freemem / CLSIZE)
		freemem = ecmx * CLSIZE;
	for (i = 1; i <= freemem / CLSIZE; i++) {
		cmap[i-1].c_next = i;
		c = &cmap[i];
		c->c_prev = i-1;
		c->c_free = 1;
		c->c_gone = 1;
		c->c_type = CSYS;
		c->c_mdev = 0;
		c->c_blkno = 0;
	}
	cmap[freemem / CLSIZE].c_next = CMHEAD;
	for (i = 0; i < CMHSIZ; i++)
		cmhash[i] = ecmx;
	cmap[CMHEAD].c_prev = freemem / CLSIZE;
	cmap[CMHEAD].c_type = CSYS;
	avefree = freemem;
}

/*
 * Wait for frame pf to become unlocked
 * if it is currently locked.
 *
 * THIS ROUTINE SHOULD TAKE A CMAP STRUCTURE AS ARGUMENT.
 */
mwait(pf)
	unsigned pf;
{

	mlock(pf);
	munlock(pf);
}

/*
 * Lock a page frame.
 *
 * THIS ROUTINE SHOULD TAKE A CMAP STRUCTURE AS ARGUMENT.
 */
mlock(pf)
	unsigned pf;
{
	register struct cmap *c = &cmap[pgtocm(pf)];

	while (c->c_lock) {
		c->c_want = 1;
		sleep((caddr_t)c, PSWP+1);
	}
	c->c_lock = 1;
}

/*
 * Unlock a page frame.
 *
 * THIS ROUTINE SHOULD TAKE A CMAP STRUCTURE AS ARGUMENT.
 */
munlock(pf)
	unsigned pf;
{
	register struct cmap *c = &cmap[pgtocm(pf)];

	if (c->c_lock == 0)
		panic("dup page unlock");
	if (c->c_want)
		wakeup((caddr_t)c);
	c->c_lock = 0;
	c->c_want = 0;
}

/* 
 * Lock a virtual segment.
 *
 * For each cluster of pages, if the cluster is not valid,
 * touch it to fault it in, otherwise just lock page frame.
 * Called from physio to ensure that the pages 
 * participating in raw i/o are valid and locked.
 * We use SDLYU to keep pagein from unlocking pages,
 * so they make it safely back here locked.
 */
vslock(base, count)
	caddr_t base;
{
	register unsigned v;
	register int npf;
	register struct pte *pte;

	u.u_procp->p_flag |= SDLYU;
	v = btop(base);
	pte = vtopte(u.u_procp, v);
	npf = btoc(count + ((int)base & CLOFSET));
	while (npf > 0) {
		if (pte->pg_v) 
			mlock(pte->pg_pfnum);
		else
			if (fubyte((caddr_t)ctob(v)) < 0)
				panic("vslock");
		pte += CLSIZE;
		v += CLSIZE;
		npf -= CLSIZE;
	}
	u.u_procp->p_flag &= ~SDLYU;
}

/* 
 * Unlock a virtual segment.
 */
vsunlock(base, count, rw)
	caddr_t base;
{
	register struct pte *pte;
	register int npf;

	pte = vtopte(u.u_procp, btop(base));
	npf = btoc(count + ((int)base & CLOFSET));
	while (npf > 0) {
		munlock(pte->pg_pfnum);
		if (rw == B_READ)	/* Reading from device writes memory */
			pte->pg_m = 1;
		pte += CLSIZE;
		npf -= CLSIZE;
	}
}
