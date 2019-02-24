#include "FEATURE/strdup"

#ifdef _lib_strdup

int	_lib_dummy;

#else

/*
 * return a copy of s using malloc
 */

extern char*	malloc();
extern char*	memcpy();

extern int	strlen();

char*
strdup(s)
register char	*s;
{
	register char*	t;
	register int	n;

	return((t = malloc(n = strlen(s) + 1)) ? memcpy(t, s, n) : 0);
}

#endif
