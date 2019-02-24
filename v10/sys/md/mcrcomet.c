/*
 * comet memory controller
 */

#include "sys/param.h"
#include "sys/systm.h"
#include "sys/nxaddr.h"

/*
 * intentional overlays:
 * if no memory controller configured,
 * mcrcnt will be 0
 * but there will be no undefined symbols
 */

int mcrcnt;
struct nxaddr mcraddr[1];	/* one or more */
caddr_t mcrregs[1];		/* one or more */
time_t mcrtime[1];		/* one or more */

int mcrtimeout = 60;		/* seconds between soft error reports */

struct device {
	long csr0;
	long csr1;
	long csr2;
};

/*
 * csr0
 */
#define	HARDERR	0x80000000	/* uncorrectable error */
#define	HARDLST	0x40000000	/* nested hard error */
#define	SOFTERR	0x20000000	/* corrected error */

/*
 * csr1
 */
#define	CRDENB	0x10000000	/* enable reporting soft errors */	

mcrinit()
{
	register int i;
	register struct device *m;

	for (i = 0; i < mcrcnt; i++)
		if ((m = (struct device *)nxaddr(&mcraddr[i])) == NULL
		||  badaddr(&m->csr0, sizeof(long)))
			printf("mcr%d absent\n", i);
		else {
			mcrregs[i] = (caddr_t)m;
			mcrenable((caddr_t)m);
		}
}

mcrenable(mm)
caddr_t mm;
{
	((struct device *)mm)->csr1 = CRDENB;
}

memerr()
{
	register int i;
	register struct device *m;
	long csr[3];

	if (mcrcnt == 0) {
		printf("mem err, no controllers configured\n");
		return;		/* and hope */
	}
	for (i = 0; i < mcrcnt; i++) {
		if ((m = (struct device *)mcrregs[i]) == NULL)
			continue;
		if (m->csr0 & (HARDERR|SOFTERR)) {
			csr[0] = m->csr0;
			csr[1] = m->csr1;
			csr[2] = m->csr2;
			m->csr0 = m->csr0;
			if (mcrtime[i] != time)
				mcrtime[i] = time;
			else {
				m->csr1 = 0;
				timeout(mcrenable, (caddr_t)m, mcrtimeout*HZ);
			}
			printf("mcr%d: mem %s error %x %x %x\n", i,
				csr[0]&HARDERR?"hard":"soft", csr[0], csr[1], csr[2]);
			if (csr[0]&HARDERR)
				panic("memerr");
		}
	}
}
