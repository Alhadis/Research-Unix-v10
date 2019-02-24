#include "FEATURE/memdup"

#ifdef _lib_memdup

int	_lib_dummy;

#else

/*
 * return a copy of s of n chars using malloc
 */

extern char*	malloc();
extern char*	memcpy();

char*
memdup(s, n)
register char*	s;
register int	n;
{
	register char*	t;

	return((t = malloc(n)) ? memcpy(t, s, n) : 0);
}

#endif
