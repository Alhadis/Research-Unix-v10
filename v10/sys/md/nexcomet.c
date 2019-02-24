/*
 * stuff for dealing with io nexus on comets
 */

#include "sys/param.h"
#include "sys/nexus.h"
#include "sys/nxaddr.h"
#include "sys/pte.h"

static int nexcnt;
char *iospace;		/* an array of struct nexus */

static int nexwind(), nexreg();

/*
 * how much address space
 * is needed for nexus space?
 * needed when allocating system page table
 */

mchiopsize()
{
	register int i;

	for (i = 0; nextab[i].bus >= 0; i++)
		;
	nexcnt = i;
	return (i * sizeof(struct nexus));
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
		b = nexwind(&nextab[i]);
		for (n = 0; n < btoc(NXWSIZE); n++)
			if (b)
				*p++ = PG_V|PG_KW|(b+n);
			else
				*p++ = 0;
	}
}

static
nexreg(n)
register struct nextab *n;
{
	if (n->bus)
		return (0);	/* only one set of registers in comet */
	if (n->adp >= 16)
		return (0);
	return (btoc(n->adp * NXSIZE + 0xf20000));
}

static
nexwind(n)
register struct nextab *n;
{
	if (n->bus)
		return (0);	/* only one set of registers in comet */
	switch (n->adp) {
	case 8:		/* first DW750 */
		return (btoc(0xfc0000));

	case 9:		/* second DW750 */
		return (btoc(0xf80000));

	default:
		return (0);
	}
}

caddr_t
nxaddr(np)
register struct nxaddr *np;
{
	if (np->adno != 0)
		return (0);
	if (np->nexus < 0 || np->nexus >= nexcnt)
		return (0);
	return((caddr_t)(iospace + np->nexus*sizeof(struct nexus)));
}
