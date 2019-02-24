/*
 * null routines; used by ANONE and others
 */

#include "mkconf.h"

/*
 * check syntax just after reading conf
 * return 0 if good, 1 if bad
 */
int
nosyntax(dp)
Dev *dp;
{
	return (0);
}

/*
 * do any fixups
 * everything in dp is set, even parent
 */

int
nofixup(dp)
Dev *dp;
{
}

/*
 * print the address data, count, data structures
 * called once per Mdev; expected to loop along dp->next
 * until a different type of dev
 */

int
noputaddr(fd, dp)
int fd;
register Dev *dp;
{
	register int count;
	register Mdev *mp;

	count = 0;
	for (mp = dp->mdev; dp && dp->mdev == mp; dp = dp->next)
		count = dp->id;
	count++;
	putstrs(fd, mp, count*mp->rept);
}


/*
 * print vector extras, e.g. interface routine
 * called once per dp
 */

int
noputvec(fd, dp)
int fd;
Dev *dp;
{
}

/*
 * common code for driver routines
 */

putstrs(fd, mp, count)
int fd;
register Mdev *mp;
int count;
{
	register int i;

	for (i = 0; i < NSTR && mp->strs[i]; i++)
		fprint(fd, "%s[%d];\n", mp->strs[i], count);
}
