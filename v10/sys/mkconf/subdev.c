/*
 * routines for subdevices
 */

#include "mkconf.h"

static char *subfields[] = {
	"ctl", "unit", NULL
};

#define	a_ctl	addr[0]	/* controller number; arbitrary */
#define	a_unit	addr[1]	/* unit number; also arbitrary */

int
subsyntax(dp, ap, n)
register Dev *dp;
char **ap;
int n;
{
	char *p;

	if ((p = keyread(dp->addr, ap, n, subfields)) != 0) {
		fprint(STDERR, "%s %d: bad field %s\n", dp->mdev->name, dp->id, p);
		errs++;
		return (1);
	}
	return (0);
}

int
subputaddr(fd, dp)
int fd;
register Dev *dp;
{
	register int count;
	register Mdev *mp;

	mp = dp->mdev;
	count = 0;
	fprint(fd, "struct subaddr %saddr[] = {\n", mp->tag);
	for (mp = dp->mdev; dp && dp->mdev == mp; dp = dp->next) {
		while (count++ < dp->id)
			fprint(fd, "\t{-1, -1},\n");
		fprint(fd, "\t{%ld, %ld},\n", dp->a_ctl, dp->a_unit);
	}
	fprint(fd, "};\nint %scnt = %d;\n", mp->tag, count);
	putstrs(fd, mp, count);
}
