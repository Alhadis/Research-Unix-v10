/*
 * miscellaneous common routines for config
 */

#include <libc.h>
#include <ctype.h>
#include "mkconf.h"

/*
 * little routines for input
 */

int
crack(p, a, n)
char *p;
char **a;
int n;
{
	register char *rp;

	if ((rp = strchr(p, '#')) != NULL)
		*rp = 0;
	return (getmfields(p, a, n));
}

/*
 * common parsing hack for device addresses
 */

char *
keyread(lp, ap, n, kp)
register long *lp;
register char **ap;
int n;
register char **kp;
{
	register int i;

	for (; n > 1; n-=2, ap+=2) {
		for (i = 0; kp[i]; i++)
			if (strcmp(ap[0], kp[i]) == 0) {
				lp[i] = nconv(ap[1]);
				break;
			}
		if (kp[i] == 0)
			return (*ap);	/* unknown keyword */
	}
	return (NULL);
}

/*
 * 0nnn -> octal
 * 0xnnn -> hex
 * nnn -> decimal
 */
long
nconv(a)
register char *a;
{
	register long n;
	register int base, d;
	register int sign;

	base = 10;
	sign = 1;
	n = 0;
	if (*a == '-') {
		sign = -1;
		a++;
	}
	if (*a == '0') {
		base = 8;
		a++;
		if (*a == 'x' || *a == 'X') {
			base = 16;
			a++;
		}
	}
	for (; isxdigit(*a); a++) {
		d = *a - '0';
		if (*a >= 'a')
			d -= 'a' - '0' - 10;
		else if (*a >= 'A')
			d -= 'A' - '0' - 10;
		if (d >= base)
			break;
		n = n * base + d;
	}
	return (n * sign);
}

Mdev *
mdlook(name)
char *name;
{
	register Mdev *mp;

	for (mp = mlist; mp; mp = mp->next)
		if (strcmp(mp->name, name) == 0)
			return (mp);
	return (NULL);
}

/*
 * skip to next type of device in dlist
 */

Dev *
advtype(dp)
register Dev *dp;
{
	register Dev *ep;

	for (ep = dp; ep && ep->mdev == dp->mdev; ep = ep->next)
		;
	return (ep);
}

/*
 * look up a driver in a table
 */

int
tablook(mp, tab, lim)
register Mdev *mp;
register Devtab *tab;
register int lim;
{
	register int i;

	for (i = 0; i <= lim; i++, tab++)
		if (tab->dev == mp)
			return (i);
	return (-1);
}

/*
 * miscellany
 */

char *
ealloc(size)
int size;
{
	char *p;

	if ((p = malloc(size)) != NULL)
		return (p);
	fprint(STDERR, "out of memory\n");
	exit(1);
}

char *
estrdup(s)
char *s;
{
	char *t;

	t = ealloc(strlen(s)+1);
	strcpy(t, s);
	return (t);
}
