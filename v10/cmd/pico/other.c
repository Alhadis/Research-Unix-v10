#include <stdio.h>
#include <signal.h>
#include "pico.h"

extern short	CUROLD, CURSCRATCH;
extern struct	SRC src[MANY];
extern char	linenumber;
extern FILE	*INPUT;
extern int	nsrc;
extern int	DEF_LL, DEF_NL;

Usage(s)
	char *s;
{
	if (strlen(s) > 0)
	fprintf(stderr, "pico: %s\n", s);
	fprintf(stderr, "usage: pico -fto -m[0-9] -wN -hN [filenames]\n");
	fprintf(stderr, "\t-f	open framebuffer display (kwee)\n");
	fprintf(stderr, "\t-m[012..]	open metheus display /dev/om[012..]\n");
	fprintf(stderr, "\t-t	show parse trees\n");
	fprintf(stderr, "\t-o	turn off optimizer\n");
	fprintf(stderr, "\t-wN	set width work buffer to N pel\n");
	fprintf(stderr, "\t-hN	set depth work buffer to N pel\n");
	exit(1);
}

whatarray(inname)
	char *inname;
{
	int i, j, k;

	j = findbase(inname);
	for (i = 2; i < nsrc; i++)
	{	k = findbase(src[i].fname);
		if (strcmp(&inname[j], &src[i].fname[k]) == 0)
			return i;
	}
	return -1;
}

findbase(inname)
	char *inname;
{
	int i = strlen(inname)+1;

	while (--i > 0)
		if (inname[i] == '/')
			return i+1;
	return 0;
}

wsize(a,b,c,d)	{ fprintf(stderr, "(%d,%d,%d,%d)\n", a,b,c,d); }

files()
{	int i;

	fprintf(stderr, "$0:\told\t");
	fprintf(stderr, "%s\t\t", (src[CUROLD].nchan >= 3)?"color" : "b&w  ");
	fprintf(stderr, "%s\t",  (src[CUROLD].incore==2)?"resident" : "absent  ");
	wsize(src[CUROLD].sx, src[CUROLD].sy, src[CUROLD].ex, src[CUROLD].ey);
	if (nsrc==2) fprintf(stderr, "no files open\n");
	for (i = 2; i < nsrc; i++)
	{	fprintf(stderr, "$%d:\t%s\t", i-1, src[i].fname);
		if (strcmp(src[i].fname, " empty") == 0)
		{	fprintf(stderr, "\n");
			continue;
		}
		fprintf(stderr, "%s  ",  (src[i].nchan >= 3)?"color":"b&w  ");
		fprintf(stderr, "%s ",  (src[i].incore==2)? "resident":"absent  ");
		fprintf(stderr, "xo %d yo %d ", src[i].ox, src[i].oy);
		fprintf(stderr, "ll %d nl %d ", src[i].ll, src[i].nl);
		wsize(src[i].sx, src[i].sy, src[i].ex, src[i].ey);
	}
}

char *
Emalloc(N)
	unsigned N;
{
	char *try, *malloc();
	if ((try = malloc(N)) == NULL)
		whoops("out of memory");
	return try;
}

whoops(s)
	char *s;
{
	fprintf(stderr, "pico: fatal error `%s'\n", s);
	exit(1);
}

append(n, m, k, l, s)
	char *s;
{	int i, reopened=0;

	if ((i = whatarray(s)) >= 2 && strcmp(s, src[i].fname) == 0)
	{	lseek(src[i].fd, src[i].sop, 0);
		reopened = 1;
	} else
		i = prepare(s);

	if (i < 2) return 0;

	src[i].ox = src[i].oy = 0;
	if (n > 0)
	{	src[i].sx = src[i].sx - n;
		if (src[i].sx < 0)
		{	src[i].ox = min(src[i].ll, -src[i].sx);
			src[i].sx = 0;
		}
	} else	src[i].sx = min(DEF_LL, src[i].sx - n);

	if (m > 0)
	{	src[i].sy = src[i].sy - m;
		if (src[i].sy < 0)
		{	src[i].oy = min(src[i].nl, -src[i].sy);
			src[i].sy = 0;
		}
	} else	src[i].sy = min(DEF_LL, src[i].sy - m);

	if (k > 0) src[i].ll = k;
	if (l > 0) src[i].nl = l;

	src[i].ex = min(DEF_LL, src[i].sx + src[i].ll - src[i].ox);
	src[i].ey = min(DEF_NL, src[i].sy + src[i].nl - src[i].oy);

	if (reopened && src[i].incore==2)
		src[i].incore = 1;

	return i;
}

setwindow(a, b, c, d)
{
	int e = min(DEF_LL, a+c);
	int f = min(DEF_NL, b+d);

	(Old)->sx = (Scratch)->sx = a;
	(Old)->sy = (Scratch)->sy = b;
	(Old)->ex = (Scratch)->ex = e;
	(Old)->ey = (Scratch)->ey = f;
	setmid();
}
