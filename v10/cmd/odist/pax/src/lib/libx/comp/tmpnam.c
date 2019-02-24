#include "FEATURE/tmpnam"

#ifdef _lib_tmpnam

int	_lib_dummy;

#else

#include <stdio.h>

#ifndef P_tmpdir
#define P_tmpdir	"/usr/tmp/"
#endif

#ifndef L_tmpnam
#define L_tmpnam	(sizeof(P_tmpdir)+10)
#endif

static char	buf[L_tmpnam];
static char	seed[] = { 'a', 'a', 'a', 0 };

extern char*	mktemp();
extern char*	strcopy();


char*
tmpnam(p)
register char*	p;
{
	register char*	q;

	if (!p) p = buf;
	(void)strcopy(strcopy(strcopy(p, P_tmpdir), seed), "XXXXXX");
	q = seed;
	while (*q == 'z') *q++ = 'a';
	if (*q) ++*q;
	return(mktemp(p));
}

#endif
