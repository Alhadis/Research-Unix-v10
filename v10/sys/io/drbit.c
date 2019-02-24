/*
 *  DR-11C interface
 *  The routines in this driver are not called through the normal
 *  device interface.  Instead, they are available for other device
 *  drivers to use to send arbitrary information out on a DR-11C.
 */

#include "sys/param.h"
#include "sys/ubaddr.h"

/* number of output bits available in a DR -- must be a power of 2 */
#define DRWIDTH 16

/* DRWIDTH is 2**DRSHIFT */
#define DRSHIFT 4

extern int drcnt;
extern struct ubaddr draddr[];
extern caddr_t drreg[];

struct drreg {
	u_short drcsr, drout, drin;
};

/* set bit n of the dr-11 complex to v */
drsetbit (n, v)
	register int n;
	register int v;
{
	register struct drreg *drptr;
	register int i;

	i = n>>DRSHIFT;
	if (i < 0 || i >= drcnt)
		return;
	if ((drptr = (struct drreg *)drreg[i]) == NULL) {
		if ((drptr = (struct drreg *)ubaddr(&draddr[i])) == NULL
		||   ubbadaddr(draddr[i].ubno, &drptr->drout, sizeof(short))) {
			printf("drbit dr11 %d absent\n", i);
			return;
		}
		drreg[i] = (caddr_t)drptr;
	}
	i = 1 << (n & (DRWIDTH - 1));
	if (v)
		drptr->drout |= i;
	else
		drptr->drout &= ~i;
}
