#include "asd.h"

extern char *mktemp(), *strcpy(), *strcat();
static char str[TMPNAML], seed[] = { 'a', 'a', 'a', '\0' };

char *
tmpname(s)
char	*s;
{
	register char *p, *q;

	p = (s == NULL)? str: s;
	(void) strcpy(p, TMPDIR);
	(void) strcat(p, seed);
	(void) strcat(p, "XXXXXX");

	q = seed;
	while(*q == 'z')
		*q++ = 'a';
	++*q;

	(void) mktemp(p);
	return(p);
}
