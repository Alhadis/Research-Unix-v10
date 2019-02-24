/*
 * read device tables
 */

#include <libc.h>
#include <ctype.h>
#include <fio.h>
#include "mkconf.h"

#define	MAXDEV	100	/* highest allowed major dev number */

Devtab bdevsw[MAXDEV];
int nblk;
Devtab cdevsw[MAXDEV];
int nchr;
Devtab fstypsw[MAXDEV];
int nfs;
Devtab ldispsw[MAXDEV];
int nld;
Devtab mscpsw[MAXDEV];
int nmscp;

#define	BIGARGS	5

readtab(f)
char *f;
{
	int fd;
	register char *p;
	char *args[BIGARGS];

	if ((fd = open(f, 0)) < 0) {
		perror(f);
		exit(1);
	}
	while ((p = Frdline(fd)) != NULL) {
		while (isspace(*p))
			p++;
		if (*p == '#' || *p == 0)
			continue;
		if (crack(p, args, BIGARGS) != 3) {
			fprint(STDERR, "%s: bad line: %s\n", f, p);
			errs++;
			continue;
		}
		if (strcmp(args[0], "bdev") == 0)
			newent(bdevsw, &nblk, "bdev", args[1], args[2]);
		else if (strcmp(args[0], "cdev") == 0)
			newent(cdevsw, &nchr, "cdev", args[1], args[2]);
		else if (strcmp(args[0], "fs") == 0)
			newent(fstypsw, &nfs, "fs", args[1], args[2]);
		else if (strcmp(args[0], "ld") == 0)
			newent(ldispsw, &nld, "ld", args[1], args[2]);
		else if (strcmp(args[0], "mscp") == 0)
			newent(mscpsw, &nmscp, "mscp", args[1], args[2]);
		else {
			fprint(STDERR, "%s: unknown table %s\n", f, args[0]);
			errs++;
		}
	}
	close(fd);
}

newent(tab, max, tnam, num, name)
Devtab *tab;
int *max;
char *tnam;
char *num, *name;
{
	register Mdev *mp;
	register int i;

	i = atoi(num);
	if (i < 0 || i >= MAXDEV) {
		fprint(STDERR, "%s %d %s: devno out of range\n", tnam, i, name);
		errs++;
		return;
	}
	if ((mp = mdlook(name)) == NULL) {
		fprint(STDERR, "%s: no such %s\n", name, tnam);
		errs++;
		return;
	}
	if (tab[i].dev) {
		fprint(STDERR, "%s %d %s: dup %s\n", tnam, i, name, tnam);
		errs++;
		return;
	}
	tab[i].dev = mp;
	if (*max < i)
		*max = i;
}

devtfix(dp)
register Dev *dp;
{
	register int i;
	register Mdev *mp;

	mp = dp->mdev;
	if (mp->flags & FTBDONE)
		return; 
	for (i = 0; i <= nblk; i++)
		if (bdevsw[i].dev == mp)
			bdevsw[i].used = 1;
	for (i = 0; i <= nchr; i++)
		if (cdevsw[i].dev == mp)
			cdevsw[i].used = 1;
	for (i = 0; i <= nfs; i++)
		if (fstypsw[i].dev == mp)
			fstypsw[i].used = 1;
	for (i = 0; i <= nld; i++)
		if (ldispsw[i].dev == mp)
			ldispsw[i].used = 1;
	for (i = 0; i <= nmscp; i++)
		if (mscpsw[i].dev == mp)
			mscpsw[i].used = 1;
	mp->flags |= FTBDONE;
}
