/*
 * routines for
 * ACNT: print only count, which is in id
 * APARAM: print only count from id, but just as tag, not tagcnt
 */

#include "mkconf.h"

int
cntsyntax(dp, ap, n)
register Dev *dp;
char **ap;
int n;
{
	register Dev *ep, *bp;

	if (n > 0)
		fprint(STDERR, "%s: superfluous junk ignored\n", dp->mdev->name);
	for (ep = dlist, bp = NULL; ep; ) {
		if (ep->mdev != dp->mdev) {
			bp = ep;
			ep = ep->next;
			continue;
		}
		if ((dp->mdev->flags & FDUPOK) == 0) {
			fprint(STDERR, "%s: multiply declared\n", dp->mdev->name);
			errs++;
			return (1);
		}
		/*
		 * dup but ok; delete the old one
		 */
		if (bp == NULL) {
			dlist = ep->next;
			free((char *)ep);
			ep = dlist;
		}
		else {
			bp->next = ep->next;
			free((char *)ep);
			ep = bp->next;
		}
	}
	return (0);
}

int
cntputaddr(fd, dp)
int fd;
register Dev *dp;
{

	fprint(fd, "int %scnt = %d;\n", dp->mdev->tag, dp->id);
	putstrs(fd, dp->mdev, dp->id);
}

int
prmputaddr(fd, dp)
int fd;
register Dev *dp;
{

	fprint(fd, "int %s = %d;\n", dp->mdev->tag, dp->id);
	putstrs(fd, dp->mdev, dp->id);
}
