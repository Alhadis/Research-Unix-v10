/*
 * massbus adapter subroutines
 * this version for comet
 */

#include "sys/param.h"
#include "sys/nxaddr.h"
#include "sys/mbaddr.h"
#include "sys/mba.h"
#include "sys/pte.h"
#include "sys/buf.h"

/*
 * hardware things
 */

#define	NMAP	256

struct mbaregs {
	long conf;	/* configuration register; unused */
	long cr;	/* control register */
	long sr;	/* status register */
	long va;	/* address */
	long bcr;	/* count */
	long _junk0[251];	/* pad up to 0x400 */
	long devreg[MBAUNITS][32];
	long map[NMAP];
};

#define	mbas	devreg[0][4]	/* attn summary reg; same in all devs */

#define	CRINIT	01	/* cr -- adapter init */
#define	CRIE	04	/* cr -- interrupt enable */

#define	MRV	0x80000000	/* map register -- valid bit */
#define	MPAGE	0x001fffff	/* map register -- page number */
		/* really only 15 bits on comet; others seem to read 0 */

/*
 * things from mkconf
 */

extern int mbacnt;
extern struct mba mba[];
extern struct nxaddr mbaaddr[];
extern int (*mbavec[][MBAUNITS])();
extern char mbaid[][MBAUNITS];

#define	BUSY	01	/* this mba already transferring */
#define	INIT	02	/* did init */

#define	NOUNIT	(-1)	/* in mbq */

caddr_t
mbaddr(ap)
register struct mbaddr *ap;
{
	register struct mba *mp;

	if (ap->mbno < 0 || ap->mbno >= mbacnt)
		return (0);
	if (ap->unit < 0 || ap->unit >= MBAUNITS)
		return (0);
	mp = &mba[ap->mbno];
	if ((mp->flags & INIT) == 0
	&&   mbinit(ap->mbno) == 0)
		return (0);
	return ((caddr_t)mp->addr->devreg[ap->unit]);
}

mbinit(dev)
int dev;
{
	register struct mba *mp;

	mp = &mba[dev];
	if ((mp->addr = (struct mbaregs *)nxaddr(&mbaaddr[dev])) == NULL
	||  badaddr(&mp->addr->conf, sizeof(long))) {
		printf("mba%d absent\n");
		return (0);
	}
	mp->addr->cr = CRINIT;
	mp->addr->cr = CRIE;
	mp->first = NOUNIT;
	mp->flags |= INIT;
	return (1);
}

/*
 * enqueue a transfer for this unit;
 * if not busy, start
 * if there's already a transfer for this unit, something is wrong
 */
mbstart(ap, bp, go)
register struct mbaddr *ap;
register struct buf *bp;
int (*go)();
{
	register struct mba *mp;
	register int i;

	mp = &mba[ap->mbno];
	i = ap->unit;
	if (mp->mbuf[i])
		panic("mbstart");
	if (mp->first == NOUNIT)
		mp->first = i;
	else
		mp->mbq[mp->last] = i;
	mp->last = i;
	mp->mbq[i] = NOUNIT;
	mp->mbuf[i] = bp;
	mp->xstart[i] = go;
	if ((mp->flags & BUSY) == 0)
		mbxfer(mp);
}

/*
 * really start a transfer
 * mp->lastoff is saved only for mbadj
 */

mbxfer(mp)
register struct mba *mp;
{
	register struct mbaregs *reg;
	register struct pte *p;
	register long *map;
	register long size;
	register struct buf *bp;

	if (mp->first == NOUNIT)
		return;
	reg = mp->addr;
	bp = mp->mbuf[mp->first];
	mp->lastoff = (long)bp->b_un.b_addr & PGOFSET;
	size = btoc(bp->b_bcount);
	if (mp->lastoff)
		size++;
	if (size >= NMAP)
		size = NMAP-1;	/* tough */
	p = btopte(bp);
	map = reg->map;
	while (--size >= 0)
		*map++ = p++->pg_pfnum | MRV;
	*map = 0;		/* safety */
	reg->bcr = -bp->b_bcount;
	reg->va = mp->lastoff;
	mp->flags |= BUSY;
	(*mp->xstart[mp->first])(bp);
}

/*
 * cheats needed for ECC correction
 * must be called from interrupt code,
 * before we've started another transfer
 */

long
mbcuraddr(ap)
struct mbaddr *ap;
{
	return (mba[ap->mbno].addr->va);
}

/*
 * adjust registers to redo current transfer
 * somewhere in the middle, but not where we were
 * new buffer address is original+aoff;
 * new count is as given
 */

mbadj(ap, aoff, count)
struct mbaddr *ap;
int aoff, count;
{
	register struct mba *mp;
	register struct mbaregs *reg;

	mp = &mba[ap->mbno];
	reg = mp->addr;
	reg->bcr = -count;
	reg->va = aoff + mp->lastoff;
}

/*
 * xfer was restarted
 * just mark adapter busy again, and delicately re-insert us in the queue
 * -- we know we were at the head before
 */

mbcontin(ap)
register struct mbaddr *ap;
{
	register struct mba *mp;

	mp = &mba[ap->mbno];
	mp->flags |= BUSY;
	mp->mbq[ap->unit] = mp->first;
	mp->first = ap->unit;
}

/*
 * get, put a byte in MASSBUS space
 */

static long
mbphys(ap, addr)
struct mbaddr *ap;
long addr;
{
	register long m;
	register int pg;
	register struct mba *mp;

	mp = &mba[ap->mbno];
	if (mp->addr == 0)
		panic("mbphys");
	pg = addr / NBPG;
	if (pg >= NMAP)
		return (-1);
	m = mp->addr->map[pg];
	if ((m & MRV) == 0)
		return (-1);
	return ((m & MPAGE) * NBPG + addr % NBPG);
}

mbgetc(ap, addr)
struct mbaddr *ap;
long addr;
{
	register long phys;

	phys = mbphys(ap, addr);
	if (phys < 0)
		return (-1);
	return (phgetc(phys));
}

int
mbputc(ap, addr, c)
struct mbaddr *ap;
long addr;
char c;
{
	register long phys;

	phys = mbphys(ap, addr);
	if (phys < 0)
		return (-1);
	return (phputc(phys, c));
}

/*
 * massbus interrupt
 * if a transfer in progress, call the driver
 * if attn, call drivers for that too
 * it is up to the driver to clear attn
 * (but for convenience, we promise to tell them what bit to clear)
 */

mba0int(dev)
register int dev;
{
	register struct mba *mp;
	register struct mbaregs *reg;
	register long sr;
	register int as, i;

	if (dev < 0 || dev > mbacnt
	||  (mp = &mba[dev], (mp->flags & INIT)) == 0) {
		mbshutup(dev);
		return;
	}
	reg = mp->addr;
	sr = reg->sr;
	reg->sr = sr;		/* clear attn and error latches */
	as = reg->mbas&0377;
	if (mp->flags & BUSY) {
		i = mp->first;
		mp->mbuf[i] = NULL;
		mp->first = mp->mbq[i];
		mp->flags &=~ BUSY;
		(*mbavec[dev][i])(mbaid[dev][i], sr, (-reg->bcr)&0xffff, as & (1<<i));
		as &=~ (1<<i);
	}
	for (i = 0; as; i++, as >>= 1)
		if (as & 01)
			(*mbavec[dev][i])(mbaid[dev][i], 0, 0, 1<<i);
	if ((mp->flags & BUSY) == 0)
		mbxfer(mp);
}

/*
 * mbavec for non-existent drives points here
 */

mbastray(dev)
int dev;
{
	printf("mba%d drive %d stray attn\n", dev>>3, dev&07);
}

/*
 * here if didn't expect mba interrupt
 * might just be a leftover ATTN from a previous boot;
 * not necessarily cleared
 */

mbshutup(dev)
int dev;
{
	register struct mbaregs *reg;

	if (dev < 0 || dev >= mbacnt
	||  (reg = (struct mbaregs *)nxaddr(&mbaaddr[dev])) == NULL
	||  badaddr(&reg->conf, sizeof(long))) {
		printf("mb%d: unacceptable interrupt\n", dev);
		return;	/* and probably get stuck forever */
	}
	reg->cr = CRINIT;	/* quiet, please */
}
