/*
 * stuff for dealing with bi nodes (io nexus) on nautilus
 *
 * iospace is an array of struct nexus (registers + window space),
 * followed by three pages of special registers:
 * NBIA0, NBIA1, and the memory controller
 */

#include "sys/param.h"
#include "sys/nexus.h"
#include "sys/biaddr.h"
#include "sys/biic.h"
#include "sys/pte.h"

static int nexcnt;
char *iospace;			/* array of struct nexus, plus some stuff */
struct nbia {
	long	csr0;
	long	csr1;
};
static struct nbia *nbia[2];
long *mcrcsr;

static int nexreg(), nexwind(), biainit(), bibinit(), nbiavec();

/*
 * how much address space
 * is needed for nexus space?
 * needed when allocating system page table
 * the fudge factor of 3: 2 pages for NBIAs, one for the memory controller
 */

mchiopsize()
{
	register int i;

	for (i = 0; nextab[i].bus >= 0; i++)
		;
	nexcnt = i;
	return (i * sizeof(struct nexus) + 3 * NBPG);
}

/*
 * map appropriate parts of nexus space into kernel space
 * called early on, before memory mapping is set up
 * argument is the first relevant spt entry
 */

mchiopinit(pt)
struct pte *pt;
{
	register long *p;
	register int i, n;
	register int b;

	p = (long *)pt;
	for (i = 0; i < nexcnt; i++) {
		b = nexreg(&nextab[i]);
		for (n = 0; n < btoc(NXSIZE); n++)
			if (b)
				*p++ = PG_V|PG_KW|(b+n);
			else
				*p++ = 0;
		b = nexwind(&nextab[i], 0);
		for (n = 0; n < btoc(NXWSIZE); n++)
			if (b)
				*p++ = PG_V|PG_KW|(b+n);
			else
				*p++ = 0;
	}
	/*
	 * map the remainder to NMI adapters
	 */
	*p++ = PG_KW|PG_V|(0x20080000/NBPG);	/* NBIA 0 */
	*p++ = PG_KW|PG_V|(0x24080000/NBPG);	/* NBIA 1 */
	*p++ = PG_KW|PG_V|(0x3e000000/NBPG);	/* memory */
	mcrcsr = (long *)(iospace + nexcnt * sizeof(struct nexus) + 2 * NBPG);
}

static
nexreg(n)
register struct nextab *n;
{
	if (n->bus >= 4 || n->adp >= 16)
		return(0);
	return (btoc(0x20000000 + (n->bus * 0x2000000) + (n->adp * NXSIZE)));
}

/*
 * `loc' requests local address within that BI
 * the local address is what we put in sadr/eadr
 * the global one is what we put in the page table
 */
static
nexwind(n, loc)
register struct nextab *n;
{
	register long addr;

	addr = 0x20400000 + (n->adp * NXWSIZE);
	if (loc == 0)
		addr += (n->bus * 0x2000000);
	return (btoc(addr));
}

/*
 * generic VAXBI routines
 */

extern int biacnt;
extern char bianode[];
extern struct biaddr biaaddr[];

#define	NNUM	017	/* node to receive interrupts */
#define	NOK	0200	/* this node has been initialized */

#define	DB88	0x106	/* DB88 device type */
#define	NBIE	0x200000	/* NBIA interrupt enable */
#define	NBINIT	0x1	/* NBIA CSR1 init */

#define	NBIASTALL	1000000

/*
 * return the address of a node's registers
 * make sure the adapter has been initialized
 */

caddr_t
biaddr(bi)
register struct biaddr *bi;
{
	register int a;

	if (bi->nexus < 0 || bi->nexus >= nexcnt)
		return (0);
	a = nextab[bi->nexus].bus;
	if (a != bi->adno)
		printf("nx%d adno%d bus%d\n", bi->nexus, bi->adno, a);
	if ((bianode[a] & NOK) == 0 && biainit(a) == 0)
		return (0);
	return ((caddr_t)(iospace + bi->nexus * sizeof(struct nexus)));
}
/*
 * set certain magic numbers in the BIIC:
 * interrupt destination and map
 * call after node reset stuff is all done,
 * lest the numbers disappear
 *
 * can't NRST or look at BROKE here,
 * as conventions vary too much among devices
 */

int
biinit(bi, mapwin)
struct biaddr *bi;
int mapwin;
{
	register struct biic *bp;
	long wind;

	if ((bp = (struct biic *)biaddr(bi)) == NULL)
		panic("biinit");
	bp->biintr = 1<<(bianode[bi->adno] & NNUM);
	if (mapwin == 0) {
		bp->bieadr = 0;
		bp->bisadr = 0;
	} else {
		wind = ctob(nexwind(&nextab[bi->nexus], 1));
		bp->bieadr = wind + NXWSIZE;
		bp->bisadr = wind;
	}
}

/*
 * init an NBIA
 * and both the DB88s on this NBIA, if need be
 */

static int
biainit(a)
register int a;
{
	register caddr_t cp;
	register int v;
	register struct nbia *np;

	if (a >= biacnt)
		return (0);
	if (nbia[0] == 0) {
		cp = iospace + (nexcnt * sizeof(struct nexus));
		nbia[0] = (struct nbia *)cp;
		nbia[1] = (struct nbia *)(cp + NBPG);
	}
	np = ((a & 02) == 0) ? nbia[0] : nbia[1];
	if (badaddr(&np->csr0, sizeof(long))) {
		printf("nbia for bi%d not responding\n", a);
		return (0);
	}
	np->csr1 |= NBINIT;
	for (v = 0; v < NBIASTALL; v++)
		;
	if ((v = nbiavec(a & ~1)) == 0)
		return (0);
	bibinit(a & ~1);	/* the even one */
	bibinit(a | 1);		/* the odd one */
	v &= 0x7c00;	/* just the useful bits of vector */
	np->csr0 = np->csr0;	/* clear error complaints */
	np->csr1 = np->csr1;
	np->csr0 = v|NBIE;
	setnmi((a & 02) ? 1 : 0);
	return ((bianode[a] & NOK) != 0);
}

/*
 * init one DB88
 */

static
bibinit(a)
register int a;
{
	register struct biic *bp;
	register struct biaddr *bi;

	if (a >= biacnt || biaaddr[a].nexus < 0)
		return;
	bi = &biaaddr[a];
	if (bi->nexus < 0 || bi->nexus >= nexcnt)
		return;
	bp = (struct biic *)(iospace + bi->nexus * sizeof(struct nexus));
	bp->bitype = (bp->bitype & 0xffff0000)|DB88;
	bp->bisadr = 0;
	bp->bieadr = 0x20000000;	/* all non-IO space */
	bp->bieir = EIBR7|bi->vec;
	bp->bicsr |= BIBROKE|BIHEIE|BISEIE;	/* write BROKE to clear it */
	bp->biber = bp->biber;
	bp->bibci |= BCINTREN|BCRTOEN;
	bp->biintr = 1<<(bp->bicsr & BINODEID);
	if ((bp->bitype & 0xffff) != DB88)
		return;	/* oops */
	bianode[a] = NOK|(bp->bicsr & BINODEID);
}

/*
 * figure out the vector offset for the nbia
 * annoying sanity checks on the pair of DB88s
 */

static int
nbiavec(a)
register int a;
{
	register struct biaddr *o, *e;
	register int err;

	e = &biaaddr[a];
	o = &biaaddr[a+1];
	err = 0;
	if (a+1 < biacnt && e->nexus >= 0 && o->nexus >= 0)
		if (o->ovec != e->ovec+0x200)
			err++;
	if (e->nexus >= 0 && (e->ovec & 0x200) != 0)
		err++;
	if (a+1 < biacnt && o->nexus >= 0 && (o->ovec & 0x200) == 0)
		err++;
	if (err)
		printf("bi%d bi%d: fix the damned vectors\n", a, a+1);
	/*
	 * now for the vector
	 */
	if (e->nexus >= 0)
		return (e->ovec);
	if (a+1 < biacnt && o->nexus >= 0)
		return (o->ovec - 0x200);
	return (0);
}

/*
 * here on nbia interrupt
 * -- bi or nbi parity error, bi powerup
 * figure out how to ignore the latter
 */

nbiaintr(dev)
int dev;
{
	register struct nbia *np;
	register long csr0, csr1;

	np = nbia[dev];
	csr0 = np->csr0;
	csr1 = np->csr1;
	np->csr0 = csr0;	/* clear interrupt conditions */
	np->csr1 = csr1;
	printf("nbi%d intr csr0 %x csr1 %x\n", dev, csr0, csr1);
}

/*
 * here on nbib error interrupt
 */

bia0int(dev)
register int dev;
{
	register struct biic *bp;
	register long biber, bicsr;

	if (dev >= biacnt || biaaddr[dev].nexus < 0 || biaaddr[dev].nexus >= nexcnt)
		return;
	bp = (struct biic *)(iospace + biaaddr[dev].nexus * sizeof(struct nexus));
	bicsr = bp->bicsr;
	biber = bp->biber;
	bp->biber = biber;	/* clear latches */
	if ((biber & (BIHES|BISES)) == 0)
		return;
	if (cknofault())
		return;
	printf("bi%d: csr %x ber %x\n", dev, bicsr, biber);
}
