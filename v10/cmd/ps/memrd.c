/*
 * various memory-reading primitives
 */

#include "ps.h"

/*
 * read the memory page corresponding to a VAX pte
 *
 * trickiness:
 * the call to memrd assumes memory == /dev/mem, not /dev/kmem
 * if mode == 0, this is wrong.
 * but if mode == 0, rdpage is called only when the process is
 * entirely swapped out, and hence its pages are all in the swap area
 * this could use some cleaning up
 */
rdpage(up, pgno, pte, buf)
register struct user *up;
int pgno;
register struct pte *pte;
char *buf;
{

	if (pte->pg_v	/* in memory */
	||  (pte->pg_fod == 0 && pte->pg_pfnum))	/* or unreferenced but in memory */
		return (memrd(memory, buf, NBPG, ctob(pte->pg_pfnum)));
	if (pte->pg_fod) {	/* assume fill-zero */
		memset(buf, 0, NBPG);
		return (NBPG);
	}
	/*
	 * page is in the swap area
	 * assume stack segment
	 */
	return (memrd(drum, buf, NBPG,
		ctob(up->u_smap.dm_map[0] + DMMIN - pgno - 1)));
}

/*
 * read a chunk of something at some offset in some file
 */

memrd(fd, buf, size, off)
int fd;
char *buf;
int size;
long off;
{

	lseek(fd, off, 0);
	return (read(fd, buf, size));
}
