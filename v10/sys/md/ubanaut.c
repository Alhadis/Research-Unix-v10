/*
 * unibus adapter routines for DWBUA
 */

#include "sys/param.h"
#include "sys/ubaddr.h"
#include "sys/nexus.h"
#include "sys/biaddr.h"
#include "sys/biic.h"
#include "sys/map.h"
#include "sys/uba.h"
#include "sys/pte.h"
#include "sys/vmparam.h"

extern struct uba uba[];
extern struct biaddr ubaaddr[];
extern int ubacnt;

/*
 * 0x2000 bytes of nexus space, followed by the address window
 */

#define	NMAP	496
#define	NBDP	5

struct ubadev {
	struct biic bi;
	long junk0[392];
	long csr;
	long vor;
	long fubar;
	long bifar;
	long junk1[8];
	long dpr[16];		/* 0 == DDP, 5 BDPs, rest unused */
	long junk2[28];
	long mreg[512];
	long junk3[0x400];
	char ubspace[NXWSIZE];
};

/*
 * bits in the data path register
 */

#define	PURGE	0x1		/* purge datapath */

/*
 * bits in map register
 */

#define	BO	0x2000000	/* offset address by one */
#define	MRV	0x80000000	/* map register valid */
#define	PSHIFT	21		/* shift data path number this much */

/*
 * bits in csr
 */

#define	BUAERR	0x80000000	/* BUA error summary */
#define	USSTO	0x8000000	/* VAXBI-to-UNIBUS SSYN timeout */
#define	BUAEIE	0x100000	/* error interrupt enable */
#define	BUAONE	0x8000		/* set when adapter is answering */

/*
 * bits in gpr0
 */

#define	UBPUP	0x1		/* unibus is powered up */

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
	if ((ub->addr = (struct ubadev *)biaddr(&ubaaddr[u])) == 0)
		return (0);
	if (badaddr(&ub->addr->bi.bitype, sizeof(long))) {
		printf("ub%d absent\n");
		return (0);
	}
	rminit(ub->map, UBNMAP, NMAP-1, 1);	/* NMAP-1 because can't alloc 0 */
	ub->path = ((1<<NBDP)-1)<<1;
	if (ubmstart(u) == 0)
		return (0);
	if (ubinit(u) == 0)
		return (0);
	ub->flags |= UBINIT;
	return (1);
}

/*
 * init the unibus adapter hardware
 */

#define	PUSTALL	200000

ubinit(u)
register int u;
{
	register struct uba *ub;
	register struct ubadev *up;
	register int i;
	register int s;
	register long *p;

	ub = &uba[u];
	up = ub->addr;
	s = spl7();
	DELAY(80000);
	up->bi.bicsr |= BINRST|BISTS;
	DELAY(80000);
	for (i = 0; i < PUSTALL; i++)
		if (up->bi.bicsr & BISTS)
			break;
	for (i = 0; i < PUSTALL; i++)
		if (up->csr & BUAONE)
			break;
	for (i = 0; i < PUSTALL; i++)
		if (up->bi.bigpr0 & UBPUP) 
			break;
	splx(s);
	if ((up->bi.bigpr0 & UBPUP) == 0) {
		printf("ub%d not responding; csr x%x\n", u, up->csr);
		return (0);
	}
	biinit(&ubaaddr[u], 1);
	up->vor = ubaaddr[u].ovec;
	up->bi.bieir = ubaaddr[u].vec|EIBR7;
	up->bi.biuir = UIEXVEC;
	up->bi.biber = BIBERCLR;
	up->bi.bicsr |= BIHEIE|BISEIE|BIHIARB;
	up->csr |= BUAEIE;		/* also clears error latches */
	for (i = 0, p = ub->addr->mreg; i < NMAP; i++)
		*p++ = 0;
	ubminit(u);
	return (1);
}

/*
 * determine whether a particular address,
 * which happens to be in UNIBUS space,
 * exists
 */
ubbadaddr(u, a, s)
int u, s;
caddr_t a;
{
	register int p;
	register struct ubadev *up;

	if (u < 0 || u >= ubacnt)
		return (1);
	up = uba[u].addr;
	p = spl7();
	up->csr = up->csr;	/* clear errors */
	s = badaddr(a, s);
	if (up->csr & USSTO)	/* or perhaps any error? */
		s = 1;
	up->csr = up->csr;	/* clear errors again */
	splx(p);
	return (s);
}

/*
 * unibus adapter interrupts
 */

uba0int(dev)
int dev;
{
	register struct ubadev *up;
	register long biber, bicsr, ubcsr;

	if (dev < 0 || dev >= ubacnt) {
		printf("stray intr from ub%d\n", dev);
		return;
	}
	up = uba[dev].addr;
	if (up == 0) {
		printf("ub%d unexpected intr\n", dev);
		return;
	}
	biber = up->bi.biber;
	bicsr = up->bi.bicsr;
	ubcsr = up->csr;
	up->bi.biber = biber;	/* clear latches */
	up->bi.biber &=~ EIFORCE;	/* needed? */
	up->csr |= BUAEIE;
	if (cknofault())
		return;
	if (bicsr & (BIHES|BISES))
		printf("ub%d bicsr x%x biber x%x\n", dev, bicsr, biber);
	if (ubcsr & BUAERR)
		printf("ub%d csr x%x fubar 0%o bifar x%x\n", dev,
			ubcsr, up->fubar<<2, up->bifar);
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

ubmflush(u, path)
int u;
int path;
{

	if (path == 0 || path > NBDP)
		return;
	uba[u].addr->dpr[path] = PURGE;
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
