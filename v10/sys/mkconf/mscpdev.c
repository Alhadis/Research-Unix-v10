/*
 * MSCP device address routines
 */

#include "mkconf.h"

/*
 * addressing stuff
 */

#define	a_adp	addr[0]		/* controller number */
#define	a_unit	addr[1]		/* unit number */
#define	a_type	addr[2]		/* controller type */

mscpsyntax(dp, ap, n)
register Dev *dp;
register char **ap;
register int n;
{
	register int i;
	register Mdev *mp;

	dp->a_type = dp->a_unit = -1;
	while (--n > 0) {
		if (strcmp(*ap, "unit") == 0) {
			if (--n < 0) {
				fprint(STDERR, "%s %d: no unit number\n",
					dp->mdev->name, dp->id);
				errs++;
				return (1);
			}
			dp->a_unit = nconv(*++ap);
			ap++;
			continue;
		}
		if ((mp = mdlook(*ap)) == NULL
		||   mp->adptype != AMSCP) {
			fprint(STDERR, "%s %d: bad word %s\n",
				dp->mdev->name, dp->id, *ap);
			errs++;
			return (1);
		}
		if (--n < 0) {
			fprint(STDERR, "%s %d: no ctl number\n",
				dp->mdev->name, dp->id);
			errs++;
			return (1);
		}
		for (i = 0; i <= nmscp; i++)
			if (mscpsw[i].dev == mp)
				break;
		if (i > nmscp) {
			fprint(STDERR, "%s %d %s: mscp gok\n",
				dp->mdev->name, dp->id, mp->name);
			errs++;
			return (1);
		}
		dp->a_type = i;
		dp->a_adp = nconv(*++ap);
		++ap;
	}
	if (dp->a_type < 0 || dp->a_unit < 0) {
		fprint(STDERR, "%s %d: missing ctl or unit\n",
			dp->mdev->name, dp->id);
		errs++;
		return (1);
	}
	return (0);
}

mscpfixup(dp)
register Dev *dp;
{
	register Dev *ep;

	for (ep = dlist; ep; ep = ep->next)
		if (ep->mdev->adptype == dp->mdev->atype
		&&  ep->id == dp->a_adp)
			break;
	if (ep == NULL) {
		fprint(STDERR, "%s %d: orphan mscp device\n", dp->mdev->name, dp->id);
		errs++;
		return;
	}
	dp->parent = ep;
}

mscpputaddr(fd, dp)
int fd;
register Dev *dp;
{
	register int count;
	register Mdev *mp;
	static int didtab = 0;

	if (didtab == 0) {
		mscptab(fd);
		didtab++;
	}
	mp = dp->mdev;
	count = 0;
	fprint(fd, "struct msaddr %saddr[] = {\n", mp->tag);
	for (mp = dp->mdev; dp && dp->mdev == mp; dp = dp->next) {
		while (count++ < dp->id)
			fprint(fd, "\t{-1},\n");
		fprint(fd, "\t{%ld, %ld, %ld},\n", dp->a_adp, dp->a_type, dp->a_unit);
	}
	count *= mp->rept;
	fprint(fd, "};\nint %scnt = %d;\n", mp->tag, count);
	putstrs(fd, mp, count);
}

/*
 * write the mscp port table
 * do it here so it only exists if there are mscp devices
 *
 * assume <sys/mscp.h> was included in cfdata.h or something
 */

mscptab(fd)
int fd;
{
	register int i;
	register int max;

	max = 0;
	for (i = 0; i <= nmscp; i++)
		if (mscpsw[i].used) {
			fprint(fd, "extern struct msportsw %sport;\n", mscpsw[i].dev->tag);
			max = i;
		}
	fprint(fd, "int nmsport = %d;\nstruct msportsw *msportsw[] = {\n", max+1);
	for (i = 0; i <= max; i++)
		if (mscpsw[i].used)
			fprint(fd, "\t&%sport,\t/* %d */\n",
			  mscpsw[i].dev->tag, i);
		else
			fprint(fd, "\tNULL,\n");
	fprint(fd, "};\n");
}
