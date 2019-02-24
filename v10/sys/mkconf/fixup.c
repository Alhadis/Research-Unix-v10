/*
 * intermediate table-building and consistency checks
 */

#include "mkconf.h"

fixup()
{
	register Dev *dp;
	register int i;
	register Mdev *mp;

	for (dp = dlist; dp; dp = dp->next) {
		devtfix(dp);
		incfix(dp);
		(*acode[dp->mdev->atype].fixup)(dp);
	}
	if (rootfmp == NULL) {
		fprint(STDERR, "no root device\n");
		errs++;
	} else {
		if ((rootfs = tablook(rootfmp, fstypsw, nfs)) < 0) {
			fprint(STDERR, "%s: root fs not a filesystem\n", rootfmp->name);
			errs++;
		}
		if ((rootmaj = tablook(rootdmp, bdevsw, nblk)) < 0) {
			fprint(STDERR, "%s: root dev not a block device\n", rootdmp->name);
			errs++;
		}
	}
	if (nswapdev == 0) {
		fprint(STDERR, "no swap device\n");
		errs++;
	} else if ((mp = mdlook("drum")) == NULL) {
		fprint(STDERR, "no drum device\n");
		errs++;
	} else {
		if ((swapmaj = tablook(mp, bdevsw, nblk)) < 0) {
			fprint(STDERR, "drum not configured\n");
			errs++;
		}
		swapmin = 0;
		for (i = 0; i < nswapdev; i++)
			if ((swaptab[i].major = tablook(swaptab[i].mp, bdevsw, nblk)) < 0) {
				fprint(STDERR, "%s: swap dev not a block device\n",
					swaptab[i].mp->name);
				errs++;
			}
	}
}
