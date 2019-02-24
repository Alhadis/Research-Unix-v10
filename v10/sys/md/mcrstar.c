/*
 * VAX-11/780 memory controller
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
	long csra;
	long csrb;
	long csrc;
};

/*
 * csrc
 */
#define	ICRD	0x40000000	/* inhibit soft errors */
#define	HIERR	0x20000000	/* error lost */
#define	ERLOG	0x10000000	/* error log request */

#define	SBIER	52
#define	RDSIE	0x8000		/* memory intr enable */
#define	ERCRD	0x4000		/* soft error */
#define	ERRDS	0x2000		/* hard error */

mcrinit()
{
	register int i;
	register struct device *m;

	for (i = 0; i < mcrcnt; i++)
		if ((m = (struct device *)nxaddr(&mcraddr[i])) == NULL
		||  badaddr(&m->csra, sizeof(long)))
			printf("mcr%d absent\n", i);
		else {
			mcrregs[i] = (caddr_t)m;
			mcrenable((caddr_t)m);
		}
}

/*
 * enable interrupts, clear stale error complaints
 */
mcrenable(mm)
caddr_t mm;
{
	((struct device *)mm)->csrc = HIERR|ERLOG;
	mtpr(SBIER, RDSIE|ERCRD|ERRDS);
}

/*
 * here on:
 * SBI alert
 * corrected read data
 * read data substitute (machine check)
 */
memerr()
{
	register int i;
	register struct device *m;
	long csr[3];
	register long er;

	er = mfpr(SBIER);
	mtpr(SBIER, er);	/* clear latches */
	printf("mem error: sbier %x\n", er);
	if (mcrcnt == 0) {
		printf("no controllers configured\n");
		return;		/* and hope */
	}
	for (i = 0; i < mcrcnt; i++) {
		if ((m = (struct device *)mcrregs[i]) == NULL)
			continue;
		if (m->csrc & ERLOG) {
			csr[0] = m->csra;
			csr[1] = m->csrb;
			csr[2] = m->csrc;
			m->csrc = m->csrc;	/* clear latches */
			if (mcrtime[i] != time)
				mcrtime[i] = time;
			else {
				m->csrc = ICRD;
				mtpr(SBIER, 0);	/* clear RDS enable */
				timeout(mcrenable, (caddr_t)m, mcrtimeout*HZ);
			}
			printf("mcr%d: %x %x %x\n", i, csr[0], csr[1], csr[2]);
		}
	}
}
