#include "sys/param.h"
#include "sys/map.h"
#include "sys/dmap.h"

/*
 * resource maps, for dishing out pieces of things
 * that can be described by integers
 *
 * to use them:
 * declare struct map xxmap[N]
 * where N is the largest number of pieces free parts of the resource may be
 * fragmented into
 *	rminit to set up the map
 *	rmalloc to allocate pieces
 *	rmfree to give them back
 *
 * the map is an array of (size, address) pairs,
 * ordered by address.
 * allocation is first-fit.
 * if the map becomes very fragmented,
 * and therefore too big to fit,
 * some of the resource may be lost.
 * the first piece of the map is magic, and holds an end-marker;
 * the last piece is kept empty
 */

/*
 * initialize struct map mp[mapsize]
 * to hold size pieces starting at addr
 */
rminit(mp, mapsize, size, addr)
	register struct map *mp;
	int mapsize;
	int size, addr;
{

	if (mapsize < 3)
		panic("rminit");
	mp->m_end = mapsize - 1;	/* leave an end-marker */
	mp++;
	mp->m_size = size;
	mp->m_addr = addr;
	mp++;
	mp->m_size = 0;
	mp->m_addr = 0;
}

rmalloc(mp, size)
	register struct map *mp;
{
	register int addr;
	register struct map *bp;

	if (size <= 0)
		panic("rmalloc");
	for (bp = mp+1; bp->m_size; bp++) {
		if (bp->m_size >= size) {
			addr = bp->m_addr;
			bp->m_addr += size;
			if ((bp->m_size -= size) == 0) {
				do {
					bp++;
					(bp-1)->m_addr = bp->m_addr;
				} while ((bp-1)->m_size = bp->m_size);
			}
			return (addr);
		}
	}
	return (0);
}

/*
 * special version of rmalloc, to handle swapmap silliness
 * historical; get rid of it later
 */
srmalloc(mp, size)
	register struct map *mp;
{
	register int addr;
	register struct map *bp;
	swblk_t first, rest;

	if (size <= 0 || size > dmmax)
		panic("rmalloc");
	for (bp = mp+1; bp->m_size; bp++) {
		if (bp->m_size >= size) {
			/*
			 * If allocating from swapmap,
			 * then have to respect interleaving
			 * boundaries.
			 */
			first = dmmax - bp->m_addr%dmmax;
			if (first<bp->m_size && first<size) {
				if (bp->m_size - first < size)
					continue;
				addr = bp->m_addr + first;
				rest = bp->m_size - first - size;
				bp->m_size = first;
				if (rest)
					rmfree(mp, rest, addr+size);
				return (addr);
			}
			addr = bp->m_addr;
			bp->m_addr += size;
			if ((bp->m_size -= size) == 0) {
				do {
					bp++;
					(bp-1)->m_addr = bp->m_addr;
				} while ((bp-1)->m_size = bp->m_size);
			}
			if (addr % CLSIZE)
				panic("rmalloc swapmap");
			return (addr);
		}
	}
	return (0);
}

/*
 * free a chunk
 * hack: if we need to create a new map entry
 * and there aren't any left,
 * discard the next-to-last,
 * as the last one tends to be larger
 */
rmfree(mp, size, addr)
	register struct map *mp;
	register int size, addr;
{
	register struct map *bp, *xp;

	if (addr <= 0 || size <= 0)
		panic("rmfree");
	for (bp = mp + 1; bp->m_addr <= addr && bp->m_size != 0; bp++)
		continue;
	/*
	 * can merge into previous block, or both sides?
	 */
	xp = bp - 1;
	if (bp > mp + 1 && xp->m_addr + xp->m_size >= addr) {
		if (xp->m_addr + xp->m_size > addr)
			panic("dup rmfree");
		xp->m_size += size;
		if (bp->m_addr && addr+size >= bp->m_addr) {
			if (addr+size > bp->m_addr)
				panic("dup rmfree");
			xp->m_size += bp->m_size;
			while (bp->m_size) {
				bp[0] = bp[1];
				bp++;
			}
		}
		return;
	}
	/*
	 * can merge into next block?
	 */
	if (addr+size >= bp->m_addr && bp->m_size) {
		if (addr+size > bp->m_addr)
			panic("dup rmfree");
		bp->m_addr -= size;
		bp->m_size += size;
		return;
	}
	/*
	 * can't merge; make a new entry before bp
	 */
	xp = bp;
	while (bp->m_size)
		bp++;
	if (bp-mp >= mp->m_end) {	/* overflow? */
		printf("rmfree: map %x overflow, lost %d-%d\n", mp,
			bp[-2].m_addr, bp[-2].m_size+bp[-2].m_addr);
		bp[-2] = bp[-1];
		bp[-1] = bp[0];
		bp--;
	}
	do {
		bp[1] = bp[0];
		bp--;
	} while (bp >= xp);
	xp->m_size = size;
	xp->m_addr = addr;
}
