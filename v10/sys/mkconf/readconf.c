/*
 * read the configuration file
 *	name	id	addr ...
 * addr pieces are just numbers here;
 * it's up to the address drivers to interpret
 */

#include <fio.h>
#include <libc.h>
#include <ctype.h>
#include "mkconf.h"

/*
 * table of address-type drivers
 * entries should match with the numbers above
 */

int nosyntax(), nofixup(), noputaddr(), noputvec();
int sbisyntax(), sbifixup(), sbiputaddr(), nexputvec();
int vbisyntax(), vbifixup(), vbiputaddr();
int cntsyntax(), cntputaddr();
int ubasyntax(), ubafixup(), ubaputaddr(), ubaputvec();
int subsyntax(), subputaddr();
int mbasyntax(), mbafixup(), mbaputaddr();
int prmputaddr();
int mscpsyntax(), mscpfixup(), mscpputaddr();
int nobussyntax(), nobusfixup(), nobusputaddr(), nobusputvec();

struct acode acode[] = {
	nosyntax, nofixup, noputaddr, noputvec,		/* ANONE */
	cntsyntax, nofixup, cntputaddr, noputvec,	/* ACNT */
	sbisyntax, sbifixup, sbiputaddr, nexputvec,	/* ASBI */
	mbasyntax, mbafixup, mbaputaddr, noputvec,	/* AMBA */
	ubasyntax, ubafixup, ubaputaddr, ubaputvec,	/* AUBA */
	subsyntax, nofixup, subputaddr, noputvec,	/* ASUB */
	vbisyntax, vbifixup, vbiputaddr, nexputvec,	/* AVBI */
	cntsyntax, nofixup, prmputaddr, noputvec,	/* APARAM */
	mscpsyntax, mscpfixup, mscpputaddr, noputvec,	/* AMSCP */
	nobussyntax, nobusfixup, nobusputaddr, nobusputvec,	/* ANOBUS */
};

#define	NATYPE	(sizeof(acode)/sizeof(acode[0]))

#define	MAXARGS	(NADDR*2+2)

readconf(f)
char *f;
{
	int fd;
	register char *p;
	char *args[MAXARGS];
	register Dev *dp;
	register int n;
	Mdev *mp;
	Mdev *mdlook();

	if ((fd = open(f, 0)) < 0) {
		perror(f);
		exit(1);
	}
	while ((p = Frdline(fd)) != NULL) {
		while (isspace(*p))
			p++;
		if (*p == '#' || *p == 0)
			continue;
		if ((n = crack(p, args, MAXARGS)) <= 1) {
			fprint(STDERR, "silly conf line: %s\n", p);
			errs++;
			continue;
		}
		if (spconf(args, n))
			continue;
		if ((mp = mdlook(args[0])) == NULL) {
			fprint(STDERR, "unknown device %s\n", p);
			errs++;
			continue;
		}
		if (0 > mp->atype || mp->atype >= NATYPE) {
			fprint(STDERR, "%s gok atype %d\n", mp->name, mp->atype);
			errs++;
			continue;
		}
		if ((dp = (Dev *)ealloc(sizeof(Dev))) == NULL) {
			fprint(STDERR, "out of memory\n");
			exit(1);
		}
		dp->mdev = mp;
		dp->id = nconv(args[1]);
		dp->parent = NULL;
		if ((*acode[mp->atype].syntax)(dp, &args[2], n-2)) {
			free((char *)dp);
			continue;
		}
		dinsert(dp);
	}
	close(fd);
}

/*
 * insert a device into the list of those configured
 * check for duplicates, cluster and order
 * different instances of the same device
 */
dinsert(dp)
register Dev *dp;
{
	register Dev *ep, *bp;

	if (dlist == NULL) {
		dlist = dp;
		dp->next = NULL;
		return;
	}
	bp = NULL;
	for (ep = dlist; ep; bp = ep, ep = ep->next)
		if (dp->mdev == ep->mdev)
			break;
	for (; ep && dp->mdev == ep->mdev; bp = ep, ep = ep->next) {
		if (dp->id == ep->id) {
			fprint(STDERR, "%s %d: dup device\n", dp->mdev->name, dp->id);
			errs++;
			return;
		}
		if (dp->id < ep->id)
			break;
	}
	if (bp == NULL) {
		dp->next = dlist;
		dlist = dp;
	}
	else {
		dp->next = bp->next;
		bp->next = dp;
	}
}

/*
 * things in the conf file
 * that aren't strictly devices
 * the hard-coded names here take precedence over
 * names of devices and drivers
 * keep these as few as possible, please
 *	root	fs	driver	minor
 *	swap	driver	minor	size
 *	dump	drname	minor	low	size
 */

int
spconf(a, n)
register char **a;
int n;
{

	if (strcmp(a[0], "root") == 0) {
		cfroot(a, n);
		return (1);
	}
	if (strcmp(a[0], "swap") == 0) {
		cfswap(a, n);
		return (1);
	}
	if (strcmp(a[0], "dump") == 0) {
		cfdump(a, n);
		return (1);
	}
	return (0);		/* not a special; perhaps a device */
}

Mdev *rootfmp, *rootdmp;
int rootfs;
int rootmaj, rootmin;

cfroot(a, n)
register char **a;
int n;
{
	register Mdev *fmp, *dmp;

	if (n != 4) {
		fprint(STDERR, "bad root spec\n");
		errs++;
		return;
	}
	if ((fmp = mdlook(a[1])) == NULL) {
		fprint(STDERR, "%s: unknown root fs\n", a[1]);
		errs++;
		return;
	}
	if ((dmp = mdlook(a[2])) == NULL) {
		fprint(STDERR, "%s: unknown root device\n", a[2]);
		errs++;
		return;
	}
	rootfmp = fmp;
	rootdmp = dmp;
	rootmin = nconv(a[3]);
}

#define	NSWAP	10

Swapdev swaptab[NSWAP];
int nswapdev;
int swapmaj, swapmin;		/* set elsewhere to `drum' */

cfswap(a, n)
register char **a;
int n;
{
	register Mdev *mp;

	if (n < 4) {
		fprint(STDERR, "bad swap spec\n");
		errs++;
		return;
	}
	if ((mp = mdlook(a[1])) == NULL) {
		fprint(STDERR, "%s: unknown swap device\n");
		errs++;
		return;
	}
	if (nswapdev >= NSWAP) {
		fprint(STDERR, "too many swap devices\n");
		errs++;
		return;
	}
	swaptab[nswapdev].mp = mp;
	swaptab[nswapdev].minor = nconv(a[2]);
	swaptab[nswapdev].size = nconv(a[3]);
	nswapdev++;
}

char *dumprout;
int dumpunit;
long dumplow, dumpsize;

cfdump(a, n)
register char **a;
int n;
{
	if (n != 5) {
		fprint(STDERR, "bad dump spec\n");
		errs++;
		return;
	}
	dumprout = estrdup(a[1]);
	dumpunit = nconv(a[2]);
	dumplow = nconv(a[3]);
	dumpsize = nconv(a[4]);
}
