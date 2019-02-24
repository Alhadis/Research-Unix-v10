/*
 * read a fine-resolution clock,
 * derived from the VAX interval timer
 */
#include "sys/param.h"
#include "sys/mtpr.h"
#include "sys/clock.h"
#include "sys/systm.h"	/* for time and lbolt */
#include "sys/buf.h"	/* just for B_READ */
#include "sys/conf.h"
#include "sys/user.h"

static int clkread();
struct cdevsw clkcdev = cdinit(nulldev, nulldev, clkread, nodev, nodev);

#define	FINERES	(-CLK60HZ)

static
clkread(dev)
int dev;
{
	register int s;
	long finetime;

	s = spl7();
	finetime = FINERES + mfpr(ICR);	/* ICR is negative remaining count */
	if (mfpr(ICCS) & ICCS_INT)
		finetime += FINERES;	/* one tick already past */
	finetime += (lbolt * FINERES) + (time * FINERES * HZ);
	splx(s);
	iomove((caddr_t)&finetime, MIN(u.u_count, sizeof(finetime)), B_READ);
}
