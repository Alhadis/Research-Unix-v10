/*
 * routines for unibus devices
 */

#include "mkconf.h"

/*
 * address parts
 */

static char *ubafields[] = {
	"ub", "reg", "vec", NULL
};

#define	a_adp	addr[0]		/* adapter number */
#define	a_addr	addr[1]		/* UNIBUS address */
#define	a_vec	addr[2]		/* UNIBUS vector */

#define	VFMT	"X%s%d%d"

/*
 * syntax checks
 */

int
ubasyntax(dp, ap, n)
register Dev *dp;
char **ap;
int n;
{
	char *p;

	dp->a_vec = 0;
	if ((p = keyread(dp->addr, ap, n, ubafields)) != NULL) {
		fprint(STDERR, "%s %d: bad field %s\n", dp->mdev->name, dp->id, p);
		errs++;
		return (1);
	}
	if (dp->a_addr == 0) {
		fprint(STDERR, "%s %d: no address\n", dp->mdev->name, dp->id);
		return (1);
	}
	dp->a_addr |= 0760000;	/* 16->18 bit language */
	return (0);
}

/*
 * intermediate fixups:
 *	find the parent
 *	store the vector
 */

int
ubafixup(dp)
register Dev *dp;
{
	register int vec;
	register int i;
	char vname[30];

	ubaparent(dp);
	if (dp->parent == NULL) {
		fprint(STDERR, "%s %d: no unibus adapter %ld\n",
			dp->mdev->name, dp->id, dp->a_adp);
		errs++;
		return;
	}
	if (dp->a_vec) {
		vec = dp->parent->vecoff + dp->a_vec;
		for (i = 0; i < dp->mdev->nvec; i++, vec += 4) {
			sprint(vname, VFMT, dp->mdev->tag, dp->id, i);
			newvec(vec, SINTR, vname);
		}
	}
}

ubaparent(dp)
register Dev *dp;
{
	register Dev *ep;

	for (ep = dlist; ep; ep = ep->next)
		if (ep->mdev->adptype == dp->mdev->atype
		&&  ep->id == dp->a_adp)
			break;
	dp->parent = ep;
}

ubaputaddr(fd, dp)
register Dev *dp;
{
	register int count;
	register Mdev *mp;

	mp = dp->mdev;
	count = 0;
	fprint(fd, "struct ubaddr %saddr[] = {\n", mp->tag);
	for (mp = dp->mdev; dp && dp->mdev == mp; dp = dp->next) {
		while (count++ < dp->id)
			fprint(fd, "\t{-1, -1, -1},\n");
		fprint(fd, "\t{0%lo, 0%lo, %ld},\n",
			dp->a_addr, dp->a_vec, dp->a_adp);
	}
	count *= mp->rept;
	fprint(fd, "};\nint %scnt = %d;\n", mp->tag, count);
	putstrs(fd, mp, count);

}

ubaputvec(fd, dp)
register Dev *dp;
{
	register int i;
	char vname[30];

	if (dp->a_vec == 0)
		return;
	for (i = 0; i < dp->mdev->nvec; i++) {
		sprint(vname, VFMT, dp->mdev->tag, dp->id, i);
		stdputvec(fd, dp, vname, i);
	}
}
