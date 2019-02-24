/*	vmsubr.c	4.6	81/05/28	*/

#include "sys/param.h"
#include "sys/systm.h"
#include "sys/user.h"
#include "sys/vm.h"
#include "sys/proc.h"
#include "sys/mtpr.h"
#include "sys/pte.h"
#include "sys/cmap.h"
#include "sys/inode.h"
#include "sys/buf.h"
#include "sys/text.h"

/*
 * Make uarea of process p addressible at kernel virtual
 * address uarea through sysmap locations starting at map.
 */
uaccess(p, map, uarea)
	register struct proc *p;
	struct pte *map;
	register struct user *uarea;
{
	register int i;
	register struct pte *mp = map;

	for (i = 0; i < UPAGES; i++) {
		*(int *)mp = 0;
		mp->pg_pfnum = p->p_addr[i].pg_pfnum;
		mp++;
	}
	vmaccess(map, (caddr_t)uarea, UPAGES);
}

/*
 * Validate the kernel map for size ptes which
 * start at ppte in the sysmap, and which map
 * kernel virtual addresses starting with vaddr.
 */
vmaccess(ppte, vaddr, size)
	register struct pte *ppte;
	register caddr_t vaddr;
	register int size;
{

	while (size != 0) {
		*(int *)ppte++ |= PG_V|PG_KW;
		mtpr(TBIS, vaddr);
		vaddr += NBPG;
		--size;
	}
}

/* 
 * Convert a pte pointer to
 * a virtual page number.
 */
ptetov(p, pte)
	register struct proc *p;
	register struct pte *pte;
{

	if (isatpte(p, pte))
		return (tptov(p, ptetotp(p, pte)));
	else if (isadpte(p, pte))
		return (dptov(p, ptetodp(p, pte)));
	else
		return (sptov(p, ptetosp(p, pte)));
}

/*
 * Convert a virtual page 
 * number to a pte address.
 */
struct pte *
vtopte(p, v)
	register struct proc *p;
	register unsigned v;
{

	if (isatsv(p, v))
		return (tptopte(p, vtotp(p, v)));
	else if (isadsv(p, v))
		return (dptopte(p, vtodp(p, v)));
	else 
		return (sptopte(p, vtosp(p, v)));
}

/*
 * convert a buffer to a pte address
 * this knows about pageout conventions,
 * swapping conventions, and physio.
 */

struct pte *
btopte(bp)
register struct buf *bp;
{
	struct proc *rp;

	if (bp->b_flags & B_DIRTY)
		rp = &proc[2];		/* ugh */
	else
		rp = bp->b_proc;
	if ((bp->b_flags & B_PHYS) == 0)
		return(&Sysmap[btop(((int)bp->b_un.b_addr)&~KSTART)]);
	else if (bp->b_flags & B_UAREA)
		return(&rp->p_addr[btop(bp->b_un.b_addr)]);
	else if (bp->b_flags & B_PAGET)
		return(&Usrptmap[btokmx((struct pte *)bp->b_un.b_addr)]);
	else
		return(vtopte(rp, btop(bp->b_un.b_addr)));
}

struct	buf *vbmap();
/*
 * Initialize the page tables for paging from an inode,
 * by scouring up the indirect blocks in order.
 * Corresponding area of memory should have been vmemfree()d
 * first or just created.
 */
vinifod(pte, ip, bstart, count)
	register struct fpte *pte;
	register struct inode *ip;
	daddr_t bstart;
	clicks_t count;
{
	register int i, j;
	struct buf *bp;
	int indx;
	register daddr_t *pp;

	if(ip->i_fstyp)
		panic("vinifod, fstyp");
	while (count > 0) {
		if (bstart < NADDR - 3) {
			pte->pg_fod = 1;
			pte->pg_source = PG_FTEXT;
			pte->pg_blkno = ip ? ip->i_un.i_addr[bstart] : 0;
			if (pte->pg_blkno == 0) {
				pte->pg_source = PG_FZERO;
				pte->pg_blkno = 0;
				cnt.v_nzfod += CLSIZE;
			}
				cnt.v_nexfod += CLSIZE;
			for (j = 1; j < CLSIZE; j++)
				pte[j] = pte[0];
			pte += CLSIZE;
			bstart++;
			count -= CLSIZE;
		} else {
			mtpr(TBIA, 0);		/* conservative */
			bp = vbmap(ip, bstart);
			indx = (bstart - (NADDR - 3)) % NINDIR(ip->i_dev);
			i = imin((NINDIR(ip->i_dev) - indx) * CLSIZE, count);
			bstart += i / CLSIZE;
			count -= i;
			if (bp) {
				pp = &bp->b_un.b_daddr[indx];
				do {
					pte->pg_fod = 1;
					pte->pg_blkno = *pp++;
					if (pte->pg_blkno) {
						pte->pg_source = PG_FTEXT;
						cnt.v_nexfod += CLSIZE;
					} else {
						pte->pg_source = PG_FZERO;
						pte->pg_blkno = 0;
						cnt.v_nzfod += CLSIZE;
					}
					for (j = 1; j < CLSIZE; j++)
						pte[j] = pte[0];
					pte += CLSIZE;
				} while ((i -= CLSIZE) > 0);
				brelse(bp);
			} else {
				cnt.v_nzfod += i;
				do {
					pte->pg_fod = 1;
					pte->pg_source = PG_FZERO;
					distcl(pte);
					pte += CLSIZE;
				} while ((i -= CLSIZE) > 0);
			}
		}
	}
	mtpr(TBIA, 0);		/* necessary! */
}

/*
 * Vbmap returns a block full of indirect pointers for a given block offset
 * in a file.  It returns 0 if a missing address block was encountered,
 * in which case the pages can be normal zfod pages.
 */
struct buf *
vbmap(ip, bn)
register struct inode *ip;
daddr_t bn;
{
	register i;
	struct buf *bp;
	int j, sh;
	daddr_t nb;
	dev_t dev = ip->i_dev;

	if (bn < NADDR-3)
		panic("vbmap");
	if (ip == 0)
		return (0);

	/*
	 * addresses NADDR-3, NADDR-2, and NADDR-1
	 * have single, double, triple indirect blocks.
	 * the first step is to determine
	 * how many levels of indirection.
	 */
	sh = 0;
	nb = 1;
	bn -= NADDR-3;
	for (j = 3; j > 0; j--) {
		sh += NSHIFT(dev);
		nb <<= NSHIFT(dev);
		if(bn < nb)
			break;
		bn -= nb;
	}
	if (j == 0)
		goto noblk;

	/*
	 * fetch the address from the inode
	 */
	nb = ip->i_un.i_addr[NADDR-j];

	/*
	 * fetch through the indirect blocks
	 */
	for (;;) {
		if (nb == 0)
			return (0);
		bp = bread(dev, nb);
		if (bp->b_flags & B_ERROR) {
			brelse(bp);
			goto noblk;
		}
		if (j == 3)
			break;
		j++;
		sh -= NSHIFT(dev);
		i = (bn>>sh) & NMASK(dev);
		nb = bp->b_un.b_daddr[i];
		brelse(bp);
		if (nb == 0)
			goto noblk;
	}
	return (bp);

noblk:
	return ((struct buf *)0);
}

/*
 * Set a red zone in the kernel stack after the u. area.
 */
setredzone(pte, vaddr)
	register struct pte *pte;
	caddr_t vaddr;
{

	pte += (sizeof (struct user) + NBPG - 1) / NBPG;
	*(int *)pte &= ~PG_PROT;
	*(int *)pte |= PG_URKR;
	if (vaddr)
		mtpr(TBIS, vaddr + sizeof (struct user));
}
