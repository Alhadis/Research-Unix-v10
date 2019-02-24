/*
 * master devices file:
 *	name	tag	[stuff]
 */

#include <fio.h>
#include <libc.h>
#include <ctype.h>
#include "mkconf.h"

#define	BIGARGS	30

readdevs(f)
char *f;
{
	int fd;
	register char *p;
	char *args[BIGARGS];
	register int n;
	register Mdev *mp;
	register int i;

	if ((fd = open(f, 0)) < 0) {
		perror(f);
		exit(1);
	}
	while ((p = Frdline(fd)) != NULL) {
		while (isspace(*p))
			p++;
		if (*p == '#' || *p == 0)
			continue;
		if ((n = crack(p, args, BIGARGS)) < 2) {
			fprint(STDERR, "%s: bad line ignored: %s\n", f, p);
			errs++;
			continue;
		}
		if ((mp = (Mdev *)ealloc(sizeof(Mdev))) == NULL) {
			fprint(STDERR, "out of memory\n");
			exit(1);
		}
		mp->name = estrdup(args[0]);
		mp->tag = estrdup(args[1]);
		mp->atype = ANONE;
		mp->adptype = ANONE;
		mp->nvec = 1;
		mp->rept = 1;
		mp->flags = 0;
		for (i = 0; i < NSTR; i++) {
			mp->strs[i] = 0;
			mp->fstrs[i] = 0;
		}
		mdflags(mp, &args[2], n - 2);
		if (badmdev(mp)) {
			free((char *)mp);
			errs++;
			continue;
		}
		mp->next = mlist;
		mlist = mp;
	}
	close(fd);
}

badmdev(mp)
register Mdev *mp;
{
	/* various consistency checks here */
	return (0);
}

mdflags(mp, a, n)
register Mdev *mp;
register char **a;
register int n;
{
	register int i;

	for (; --n >= 0; a++) {
		/*
		 * address types: mine or the bus i control
		 */
		if (strcmp(*a, "sbi") == 0)
			mp->atype = ASBI;
		else if (strcmp(*a, "sbia") == 0)
			mp->adptype = ASBI;
		else if (strcmp(*a, "vaxbi") == 0)
			mp->atype = AVBI;
		else if (strcmp(*a, "vaxbia") == 0)
			mp->adptype = AVBI;
		else if (strcmp(*a, "mb") == 0)
			mp->atype = AMBA;
		else if (strcmp(*a, "mba") == 0)
			mp->adptype = AMBA;
		else if (strcmp(*a, "ub") == 0)
			mp->atype = AUBA;
		else if (strcmp(*a, "uba") == 0)
			mp->adptype = AUBA;
		else if (strcmp(*a, "sub") == 0)
			mp->atype = ASUB;
		else if (strcmp(*a, "count") == 0)
			mp->atype = ACNT;
		else if (strcmp(*a, "param") == 0)
			mp->atype = APARAM;
		else if (strcmp(*a, "mscp") == 0)
			mp->atype = AMSCP;
		else if (strcmp(*a, "mscpa") == 0)
			mp->adptype = AMSCP;
		else if (strcmp(*a, "nobus") == 0)
			mp->atype = ANOBUS;
		/*
		 * hacks
		 */
		else if (strcmp(*a, "vec") == 0) {
			if (--n < 0) {
				fprint(STDERR, "%s: vec what?\n", mp->name);
				errs++;
				continue;
			}
			mp->nvec = atoi(*++a);
		} else if (strcmp(*a, "rep") == 0) {
			if (--n < 0) {
				fprint(STDERR, "%s: rep what?\n", mp->name);
				errs++;
				continue;
			}
			mp->rept = atoi(*++a);
		} else if (strcmp(*a, "dupok") == 0)
			mp->flags |= FDUPOK;
		else if (strcmp(*a, "rawvec") == 0)
			mp->flags |= FRAWVEC;
		/*
		 * structures, includes
		 */
		else if (strcmp(*a, "data") == 0) {
			i = mddata(mp, a, n);
			a += i;
			n -= i;
		} else if (strcmp(*a, "inc") == 0) {
			i = mdincl(mp, a, n);
			a += i;
			n -= i;
		} else {
			fprint(STDERR, "%s: unknown arg %s\n", mp->name, *a);
			errs++;
		}
	}
}

int
mddata(mp, a, n)
register Mdev *mp;
register char **a;
int n;
{
	register int i;
	char buf[80];
	register char *p, *q;
	register int xn;
	int c;

	i = 0;
	xn = n - 1;
	++a;		/* skip "data" */
	while (xn >= 0) {
		q = buf;
		do {
			p = *a++;
			while ((*q++ = *p++) != 0)
				;
			q[-1] = ' ';
		} while (--xn >= 0 && q[-2] != ';' && q[-2] != ',');
		*--q = 0;	/* smash last blank */
		c = q[-1];
		if (c == ';' || c == ',')
			*--q = 0;
		if (i < NSTR-1)
			mp->strs[i++] = estrdup(buf);
		else {
			fprint(STDERR, "%s: too much data\n", mp->name);
			errs++;
		}
		if (c == ';')
			break;
	}
	return (n - xn - 1);
}

Include *inclist;

int
mdincl(mp, a, n)
Mdev *mp;
register char **a;
int n;
{
	register int xn;
	register Include *ip, *jp;
	register char *p;
	Include *bjp;
	char *s;
	int last;

	xn = n;
	a++;		/* skip "inc" */
	last = 0;
	while (last == 0 && --xn >= 0) {
		s = *a++;
		for (p = s; *p; p++)
			;
		if (*--p == ';') {
			*p = 0;
			last = 1;
		}
		bjp = NULL;
		for (jp = inclist; jp; bjp = jp, jp = jp->next)
			if (strcmp(jp->incl, s) == 0)
				break;
		ip = (Include *)ealloc(sizeof(Include));
		ip->inuse = 0;
		ip->dev = mp;
		if (jp)
			ip->incl = jp->incl;
		else
			ip->incl = estrdup(s);
		if (bjp) {
			ip->next = bjp->next;
			bjp->next = ip;
		}
		else {
			ip->next = inclist;
			inclist = ip;
		}
	}
	return (n - xn);
}

incfix(dp)
Dev *dp;
{
	register Mdev *mp;
	register Include *ip;

	mp = dp->mdev;
	for (ip = inclist; ip; ip = ip->next)
		if (ip->dev == mp)
			ip->inuse = 1;
}
