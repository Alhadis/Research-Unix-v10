/*
 * Q-bus adapter routines for MicroVAX II
 * The names imply `UNIBUS'; most of the system knows no difference
 */

#include "sys/param.h"
#include "sys/ubaddr.h"
#include "sys/map.h"
#include "sys/uba.h"
#include "sys/pte.h"
#include "sys/vmparam.h"
#include "sys/qbio.h"

extern struct uba uba[];
extern struct nxaddr ubaaddr[];
extern int ubacnt;

/*
 * bits in map register
 */

#define	MPAGE	0x3fff		/* physical page number -- small on comet */
#define	MRV	0x80000000	/* map register valid */

#define	QIOBASE	0760000		/* base address in Q-bus io space */

#define	ICR	0777500		/* interprocessor control register */
#define	LMEAE	040		/* local memory external access enable */

/*
 * return the register address for a unibus device
 */

caddr_t
ubaddr(up)
register struct ubaddr *up;
{
	register struct uba *ub;

	if (up->ubno < 0 || up->ubno > ubacnt) {
		printf("bad qbus no %d\n", up->ubno);
		return (0);
	}
	ub = &uba[up->ubno];
	if ((ub->flags & UBINIT) == 0)
		if (ubstart(up->ubno) == 0)
			return (0);
	return ((caddr_t)&ub->addr->ioreg[up->uboff-QIOBASE]);
}

/*
 * init the unibus adapter
 */

ubstart(u)
int u;
{
	register struct uba *ub;
	extern caddr_t qbaaddr();

	ub = &uba[u];
	if ((ub->addr = (struct ubadev *)qbaaddr(u)) == 0)
		return (0);
	rminit(ub->map, UBNMAP, NQMREG-1, 1);	/* NQMREG-1 because can't alloc 0 */
	if (ubmstart(u) == 0)
		return (0);
	ubinit(u);
	ub->flags |= UBINIT|UBQBUS;
	return (1);
}

/*
 * init the unibus adapter hardware
 */

ubinit(u)
int u;
{
	register struct uba *ub;
	register int i;
	register long *p;

	ub = &uba[u];
	*(short *)&ub->addr->ioreg[ICR-QIOBASE] |= LMEAE;
	for (i = 0, p = ub->addr->mreg; i < NQMREG; i++)
		*p++ = 0;
	ubminit(u);
}

/*
 * determine whether a particular address,
 * which happens to be in UNIBUS space,
 * exists
 * -- on MicroVAX, you just get a machine check on error
 */
ubbadaddr(u, a, s)
int u, s;
caddr_t a;
{

	return (badaddr(a, s));
}

/*
 * get/put a single byte to a particular unibus address
 * intended for use by ECC code
 * work it out from first principles,
 * because UNIBUS adapter may loop and hang otherwise
 */

static long
ubphys(ubno, addr)
unsigned int ubno;
uaddr_t addr;
{
	register struct uba *ub;
	register int pg;
	register long m;

	if (ubno >= ubacnt)
		panic("ubphys");
	ub = &uba[ubno];
	if (ub->addr == NULL)
		panic("ubphys");
	pg = addr / NBPG;
	if (pg >= NQMREG)
		return (-1);	/* addr too big */
	m = ub->addr->mreg[pg];
	if ((m & MRV) == 0)
		return (-1);	/* invalid */
	return ((m & MPAGE) * NBPG + addr % NBPG);
}

int
ubgetc(ubno, addr)
unsigned int ubno;
uaddr_t addr;
{
	register long phys;

	phys = ubphys(ubno, addr);
	if (phys < 0)
		return (-1);
	return (phgetc(phys));
}

int
ubputc(ubno, addr, c)
unsigned int ubno;
uaddr_t addr;
char c;
{
	register long phys;

	phys = ubphys(ubno, addr);
	if (phys < 0)
		return (-1);
	return (phputc(phys, c));
}

/*
 * allocate a BDP: MicroVAXes have none
 */

int
ubmapath(u)
int u;
{
	return (0);
}

ubmflush(u, path)
int u;
int path;
{
}

/*
 * fill in a piece of unibus map
 * return the address of the base
 *
 * hardware bug: successive writes to map registers
 * can block Q-bus DMA
 * workaround: CPU write to memory after each map reg write,
 * to slow it down enough
 */

uaddr_t
ubmsetmap(u, p, nreg, um)
int u;
register struct pte *p;
register int nreg;
ubm_t um;
{
	register long *m;
	int junk;

	m = &uba[u].addr->mreg[ubmfirst(um)];
	if (nreg > ubmsize(um)-1)
		panic("ubmsetmap");
	while (--nreg >= 0) {
		*m++ = p++->pg_pfnum | MRV;
		junk = 0;
	}
	return (ctob(ubmfirst(um)));
}
