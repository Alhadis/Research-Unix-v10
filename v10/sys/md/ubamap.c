/*
 * general UNIBUS map setup routines
 */

#include "sys/param.h"
#include "sys/ubaddr.h"
#include "sys/map.h"
#include "sys/uba.h"
#include "sys/buf.h"
#include "sys/stream.h"
#include "sys/vmmac.h"
#include "sys/pte.h"

extern struct uba uba[];
extern struct block cblock[];
extern struct buf *cblkbuf[];
extern int blkcnt, blkbcnt;

extern uaddr_t ubmsetmap();

/*
 * init data structures:
 * grab a place to store map for the stream buffers
 */

ubmstart(u)
int u;
{
	register struct uba *ub;
	register struct buf *bp;

	ub = &uba[u];
	if ((bp = geteblk()) == NULL)
		return (0);
	if ((blkbcnt*sizeof(ubm_t) + blkbcnt*sizeof(uaddr_t)) > bp->b_bcount) {
		printf("ub%d: too many stream bufs\n", u);
		brelse(bp);
		return (0);
	}
	clrbuf(bp);
	ub->sbmap = (ubm_t *)bp->b_un.b_addr;
	ub->sbaddr = (uaddr_t *)(ub->sbmap + blkbcnt);
	ub->shmap = 0;
	return (1);
}

/*
 * after hardware reset:
 * clear out junk for stream data
 */
ubminit(u)
int u;
{
	register struct uba *ub;
	register int i;

	ub = &uba[u];
	for (i = 0; i < blkbcnt; i++)
		if (ub->sbmap[i]) {
			ubmfree(u, ub->sbmap[i]);
			ub->sbmap[i] = 0;
		}
	if (ub->shmap) {
		ubmfree(u, ub->shmap);
		ub->shmap = 0;
	}
}

/*
 * general map-allocation routine
 */

ubm_t
ubmalloc(u, size, flag)
int u;
int size;
register int flag;
{
	register struct uba *ub;
	int base, nreg, path;
	int s;

	ub = &uba[u];
	nreg = btoc(size);
	if ((flag & UPAG) == 0)
		nreg++;		/* allow for alignment */
	nreg++;			/* allow for endmarker for DW780 */
	s = spl6();
	while ((base = rmalloc(ub->map, nreg)) == 0) {
		if ((flag & USLP) == 0)
			return (0);
		ub->flags |= UBMWANT;
		sleep((caddr_t)ub->map, PZERO);
	}
	splx(s);
	if (flag & UBDP)
		path = ubmapath(u);
	else
		path = 0;
	return ((base<<UMFIRST) | ((nreg-UMSZERO)<<UMSIZE) | (path<<UMDP));
}
	
ubmfree(u, um)
ubm_t um;
{
	register struct uba *ub;
	register int path;
	register int s;

	if (um == NOMAP)
		return;
	ub = &uba[u];
	s = spl6();
	if ((path = ubmpath(um)) != 0) {
		ubmflush(u, path);
		ub->path |= 1<<path;
	}
	rmfree(ub->map, ubmsize(um), ubmfirst(um));
	if (ub->flags & UBMWANT) {
		wakeup((caddr_t)ub->map);
		ub->flags &=~ UBMWANT;
	}
	splx(s);
}

/*
 * general-purpose address mapper
 * for kernel address space
 * don't use this if there's a better choice below
 */

uaddr_t
ubmaddr(u, cp, size, um)
int u;
caddr_t cp;
ubm_t um;
int size;
{
	register int off;

	off = (int)cp & PGOFSET;
	return (ubmsetmap(u, &Sysmap[btop((int)cp & ~KSTART)],
		btoc(size + off), um) + off);
}

/*
 * allocate/set maps for various kinds of object
 * ubmxxx returns a map sufficient for the object;
 * ubadxxx sets the map, and returns a unibus-space address
 * sometimes ubmxxx will return NOMAP,
 * which means the space was pre-mapped (and any data path will be ignored)
 */


/*
 * io buffers
 */

ubm_t
ubmbuf(u, bp, flags)
int u;
register struct buf *bp;
int flags;
{

	if ((bp->b_flags & B_PHYS) == 0)
		flags |= UPAG;
	return (ubmalloc(u, bp->b_bcount, flags));
}

uaddr_t
ubadbuf(u, bp, um)
int u;
register struct buf *bp;
ubm_t um;
{
	register int off;

	off = (int)bp->b_un.b_addr & PGOFSET;
	return (ubmsetmap(u, btopte(bp), btoc(bp->b_bcount + off), um) + off);
}

/*
 * stream blocks
 * headers (and the tiny bit of data in them) pre-mapped
 * the rest of the data mapped a buf at a time, when first touched
 */

ubm_t
ubmblk(u, bp, flags)
int u;
register struct block *bp;
int flags;
{
	register int i;
	register struct uba *ub;
	register struct buf *bf;

	ub = &uba[u];
	flags &=~ UBDP;
	if ((i = bp->bufix) == NOBUFIX) {	/* data all in the header */
		if (ub->shmap)
			return (NOMAP);
		if ((ub->shmap = ubmalloc(u, blkcnt*sizeof(struct block), flags)) == 0)
			return (0);
		ub->shaddr = ubmaddr(u, (char *)cblock, blkcnt*sizeof(struct block), ub->shmap);
		return (NOMAP);
	}
	if (ub->sbmap[i])			/* this buf already mapped */
		return (NOMAP);
	bf = cblkbuf[i];
	if ((ub->sbmap[i] = ubmbuf(u, bf, flags)) == 0)
		return (0);
	ub->sbaddr[i] = ubmsetmap(u, btopte(bf), btoc(bf->b_bcount), ub->sbmap[i]);
	return (NOMAP);
}

/*
 * ubadwptr returns the unibus address of the write pointer
 * ubadrptr for the read pointer
 */

uaddr_t
ubadwptr(u, bp, um)
int u;
register struct block *bp;
ubm_t um;
{
	register struct uba *ub;
	register int i;

	ub = &uba[u];
	if ((i = bp->bufix) == NOBUFIX)
		return ((uaddr_t)(bp->wptr-(u_char *)cblock + (int)ub->shaddr));
	if (ub->sbmap[i] == 0)
		panic("ubadwptr");
	return (ub->sbaddr[i]+bp->wptr-(u_char *)cblkbuf[i]->b_un.b_addr);
}

uaddr_t
ubadrptr(u, bp, um)
int u;
register struct block *bp;
ubm_t um;
{
	register struct uba *ub;
	register int i;

	ub = &uba[u];
	if ((i = bp->bufix) == NOBUFIX)
		return ((uaddr_t)(bp->rptr-(u_char *)cblock + (int)ub->shaddr));
	if (ub->sbmap[i] == 0)
		panic("ubadrptr");
	return (ub->sbaddr[i]+bp->rptr-(u_char *)cblkbuf[i]->b_un.b_addr);
}
