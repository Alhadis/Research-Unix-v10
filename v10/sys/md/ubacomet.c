/*
 * unibus adapter routines for VAX-11/750
 */

#include "sys/param.h"
#include "sys/ubaddr.h"
#include "sys/nexus.h"
#include "sys/nxaddr.h"
#include "sys/map.h"
#include "sys/uba.h"
#include "sys/pte.h"
#include "sys/vmparam.h"

extern struct uba uba[];
extern struct nxaddr ubaaddr[];
extern int ubacnt;

/*
 * `registers', such as they are,
 * for the comet unibus
 * 0x2000 bytes of nexus space, followed by the address window
 */

#define	NMAP	496
#define	NBDP	3

struct ubadev {
	long junk0[0x10];
	long dpr[16];		/* data path registers */
	long junk1[0x1e0];
	long mreg[512];		/* unibus map registers; last 16 unused */
	long junk2[0x400];
	char ubspace[NXWSIZE];
};

/*
 * bits in the data path register
 */

#define	PURGE	0x1		/* purge datapath */
#define	DPERR	0x80000000	/* any datapath error */

/*
 * bits in map register
 */

#define	MPAGE	0x3fff		/* physical page number -- small on comet */
#define	BO	0x2000000	/* offset address by one */
#define	MRV	0x80000000	/* map register valid */
#define	PSHIFT	21		/* shift data path number this much */

/*
 * return the register address for a unibus device
 */

caddr_t
ubaddr(up)
register struct ubaddr *up;
{
	register struct uba *ub;

	if (up->ubno < 0 || up->ubno > ubacnt) {
		printf("bad ubano %d\n", up->ubno);
		return (0);
	}
	ub = &uba[up->ubno];
	if ((ub->flags & UBINIT) == 0)
		if (ubstart(up->ubno) == 0)
			return (0);
	return ((caddr_t)&ub->addr->ubspace[up->uboff]);
}

/*
 * init the unibus adapter
 */

ubstart(u)
int u;
{
	register struct uba *ub;

	ub = &uba[u];
	if ((ub->addr = (struct ubadev *)nxaddr(&ubaaddr[u])) == 0)
		return (0);
	if (badaddr(&ub->addr->dpr[1], sizeof(long))) {
		printf("ub%d not responding\n", u);
		return (0);
	}
	rminit(ub->map, UBNMAP, NMAP-1, 1);	/* NMAP-1 because can't alloc 0 */
	ub->path = ((1<<NBDP)-1)<<1;
	if (ubmstart(u) == 0)
		return (0);
	ubinit(u);
	ub->flags |= UBINIT;
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
	for (i = 0, p = ub->addr->mreg; i < NMAP; i++)
		*p++ = 0;
	ubminit(u);
}

/*
 * determine whether a particular address,
 * which happens to be in UNIBUS space,
 * exists
 * -- on comet, you just get a machine check on error
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
	if (pg >= NMAP)
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
 * unibus adapter interrupts;
 * on the comet, these are always wrong
 */

uba0int(dev)
int dev;
{
	printf("uba%d stray intr\n", dev);
}

/*
 * allocate a buffered data path
 * return the ddp if none available
 */
int ubnopath;

int
ubmapath(u)
int u;
{
	register struct uba *ub;
	register int path;
	register int s;

	ub = &uba[u];
	s = spl6();
	for (path = NBDP; path > 0; path--)
		if (ub->path & (1<<path)) {
			ub->path &=~ (1<<path);
			break;
		}
	splx(s);
	if (path == 0)
		ubnopath++;
	return (path);
}

/*
 * flush (in hardware) a bdp
 */
#define	PRGTIM	10

ubmflush(u, path)
int u;
int path;
{
	register long *reg;
	register int i;

	if (path == 0)
		return;
	reg = &uba[u].addr->dpr[path];
	*reg = PURGE;
	for (i = PRGTIM; i > 0; --i)
		if ((*reg & PURGE) == 0)
			break;
	if (i <= 0)
		printf("uba%d bdp%d stuck %x\n", u, path, *reg);
	if (*reg & DPERR) {
		printf("uba%d bdp%d err %x\n", u, path, *reg);
		*reg = *reg;
	}
}

/*
 * fill in a piece of unibus map
 * return the address of the base
 */

uaddr_t
ubmsetmap(u, p, nreg, um)
int u;
register struct pte *p;
register int nreg;
ubm_t um;
{
	register long *m;
	register long x;

	m = &uba[u].addr->mreg[ubmfirst(um)];
	x = (ubmpath(um)<<PSHIFT)|MRV;
	if (nreg > ubmsize(um)-1)
		panic("ubmsetmap");
	while (--nreg >= 0)
		*m++ = p++->pg_pfnum | x;
	return (ctob(ubmfirst(um)));
}
