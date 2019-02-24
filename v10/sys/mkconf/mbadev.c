/*
 * MASSBUS device address routines
 *
 * interrupt vectors are all in the nexus code;
 * they come for the adapter, not the device
 */

#include "mkconf.h"

/*
 * addressing stuff
 */

static char *mbafields[] = {
	"mb", "drive", NULL
};

#define	a_adp	addr[0]		/* MBA number */
#define	a_drive	addr[1]		/* drive within MBA */

#define	NMBD	8		/* max drives per massbus */
#define	NMBA	10		/* enough */

static Dev *mbdev[NMBD][NMBA];
static int mbamax;

static newmbdev(), mbaputtab();

mbasyntax(dp, ap, n)
register Dev *dp;
char **ap;
int n;
{
	char *p;

	if ((p = keyread(dp->addr, ap, n, mbafields)) != NULL) {
		fprint(STDERR, "%s %d: bad field %s\n", dp->mdev->name, dp->id, p);
		errs++;
		return (1);
	}
	if (dp->a_drive < 0 || dp->a_drive >= NMBD) {
		fprint(STDERR, "%s %d: only %d drives per massbus\n", dp->mdev->name, dp->id, NMBD);
		errs++;
		return (1);
	}
	return (0);
}

mbafixup(dp)
register Dev *dp;
{
	register Dev *ep;

	for (ep = dlist; ep; ep = ep->next)
		if (ep->mdev->adptype == dp->mdev->atype
		&&  ep->id == dp->a_adp)
			break;
	if (ep == NULL) {
		fprint(2, "%s %d: orphan massbus device\n", dp->mdev->name, dp->id);
		errs++;
		return;
	}
	dp->parent = ep;
	newmbdev(dp);
}

mbaputaddr(fd, dp)
int fd;
register Dev *dp;
{
	register int count;
	register Mdev *mp;

	mbaputtab(fd);
	mp = dp->mdev;
	count = 0;
	fprint(fd, "struct mbaddr %saddr[] = {\n", mp->tag);
	for (mp = dp->mdev; dp && dp->mdev == mp; dp = dp->next) {
		while (count++ < dp->id)
			fprint(fd, "\t{-1, -1},\n");
		fprint(fd, "\t{%ld, %ld},\n", dp->a_drive, dp->a_adp);
	}
	count *= mp->rept;
	fprint(fd, "};\nint %scnt = %d;\n", mp->tag, count);
	putstrs(fd, mp, count);

}

static
newmbdev(dp)
register Dev *dp;
{
	register Dev *ep;

	ep = dp->parent;
	if (ep->id > mbamax)
		mbamax = ep->id;
	if (ep->id >= NMBA) {
		fprint(STDERR, "%s %d: too many mbas, max %d\n", ep->mdev->tag, ep->id, NMBA);
		errs++;
		return;
	}
	mbdev[dp->a_drive][ep->id] = dp;
}

static
mbaputtab(fd)
int fd;
{
	static int done;
	register int i, j;
	register Dev *dp;

	if (done++)
		return;
	for (i = 0; i <= mbamax; i++)
		for (j = 0; j < NMBD; j++)
			if ((dp = mbdev[j][i]) != NULL)
				fprint(fd, "extern %s0int();\n", dp->mdev->tag);
	fprint(fd, "char mbaid[][%d] = {\n", NMBD);
	for (i = 0; i <= mbamax; i++) {
		for (j = 0; j < NMBD; j++) {
			if ((dp = mbdev[j][i]) == NULL)
				fprint(fd, " 0%o,", (i<<3)|j);	/* stray track */
			else
				fprint(fd, " %d,", dp->id);
		}
		fprint(fd, "\n");
	}
	fprint(fd, "};\nint mbastray();\nint (*mbavec[][%d])() = {\n", NMBD);
	for (i = 0; i <= mbamax; i++) {
		for (j = 0; j < NMBD; j++) {
			if ((dp = mbdev[j][i]) == NULL)
				fprint(fd, " mbastray,");
			else
				fprint(fd, " %s0int,", dp->mdev->tag);
		}
		fprint(fd, "\n");
	}
	fprint(fd, "};\n");
}
