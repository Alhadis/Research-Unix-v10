/*
 * system-mode mapping
 * 32032/sequent version
 */

#include "defs.h"
#include "machine.h"
#include "space.h"
#include <machine/pte.h>
#include <machine/param.h>

static long sysptb, usrptb;
static int kmode;

#define	KOFF	0
#define	KSYS	1
#define	KUSR	2

printkm()
{

	printf("%R$k\n%R$u\n", sysptb, usrptb);
	if (kmode == KSYS)
		printf("sys\n");
	else if (kmode == KUSR)
		printf("user\n");
	else
		printf("off\n");
}

kmsys()
{

	if (adrflg) {
		sysptb = dot;
		kmode = KSYS;
		return;
	}
	if (expr(0)) {
		if (expv)
			kmode = KSYS;
		else
			kmode = KUSR;
		return;
	}
	printkm();
}

kmproc()
{

	if (adrflg) {
		sysptb = dot;
		kmode = KUSR;
		return;
	}
	if (expr(0)) {
		if (expv)
			kmode = KUSR;
		else
			kmode = KSYS;
		return;
	}
	printkm();
}

/*
 * map a kernel address to a physical address
 * arg is a pointer to be filled in
 * returns nonzero if address is valid
 */

int
kmap(paddr, sp)
ADDR *paddr;
{
	switch (kmode) {
	case KUSR:
		return (transl(paddr, sp, usrptb));

	case KSYS:
		return (transl(paddr, sp, sysptb));

	default:
		return (1);
	}
}

/*
 * virtual -> physical addr, given the right pt base
 */
static long
transl(ap, sp, ptb)
register long *ap;
int sp;
long ptb;
{
	long pt;

	pt = ltow(lget((ADDR)(ptb + L1IDX(*ap) * sizeof(struct pte)), sp|RAWADDR));
	if (errflg)
		return (0);
	if ((pt & PG_V) == 0)
		return (0);
	pt = PTETOPHYS(pt) + L2IDX(*ap) * sizeof(struct pte);
	pt = ltow(lget((ADDR)pt, sp|RAWADDR));
	if (errflg)
		return (0);
	*ap = (*ap & PGOFSET) + PTETOPHYS(pt);
	return (1);
}
