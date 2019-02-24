/*
 * routines for ANOBUS: adapter with no address info
 */

#include "mkconf.h"

static char *nobusfields[] = {
	"voff", "vec", 0
};

#define	a_voff	addr[0]
#define	a_vec	addr[1]

#define	VFMT	"X%s%d%d"	/* name of vector interface routine */

int
nobussyntax(dp, ap, n)
register Dev *dp;
char **ap;
int n;
{
	char *p;

	dp->a_voff = 0;
	if ((p = keyread(dp->addr, ap, n, nobusfields)) != NULL) {
		fprint(STDERR, "%s %d: bad field %s\n", dp->mdev->name, dp->id, p);
		errs++;
		return (1);
	}
	dp->vecoff = dp->a_voff;
	if (dp->mdev->nvec && dp->a_vec == 0) {
		fprint(STDERR, "%s %d: needs a vector\n", dp->mdev->name, dp->id);
		errs++;
		return (1);
	}
	return (0);
}

/*
 * fixup routine:
 * just insert our vectors, if any
 */
int
nobusfixup(dp)
register Dev *dp;
{
	register int vec;
	register int i;
	register Mdev *mp;
	char vname[20];

	mp = dp->mdev;
	vec = dp->a_vec;
	if (dp->parent)
		vec += dp->parent->vecoff;
	for (i = 0; i < mp->nvec; i++, vec += 4) {
		sprint(vname, VFMT, mp->tag, dp->id, i);
		newvec(vec, SINTR, vname);
	}
}

int
nobusputaddr(fd, dp)
int fd;
register Dev *dp;
{
	register int count;
	register Mdev *mp;

	count = 0;
	for (mp = dp->mdev; dp && dp->mdev == mp; dp = dp->next)
		count++;
	count *= mp->rept;
	fprint(fd, "int %scnt = %d;\n", mp->tag, count);
	putstrs(fd, mp, count);
}

/*
 * put out vectors
 */

nobusputvec(fd, dp)
int fd;
register Dev *dp;
{
	register int i;
	char vname[30];

	for (i = 0; i < dp->mdev->nvec; i++) {
		sprint(vname, VFMT, dp->mdev->tag, dp->id, i);
		stdputvec(fd, dp, vname, i);
	}
}
