#include "FEATURE/memcpy"

#ifdef _lib_memcpy

int	_lib_dummy;

#else

#include "FEATURE/bcopy"

#ifdef _lib_bcopy

extern void	bcopy();

char*
memcpy(s1, s2, n)
char*	s1;
char*	s2;
int	n;
{
	bcopy(s2, s1, n);
	return(s1);
}

#else

char*
memcpy(s1, s2, n)
register char*	s1;
register char*	s2;
register int	n;
{
	register char*	os1 = s1;

	while (--n >= 0)
		*s1++ = *s2++;
	return(os1);
}

#endif

#endif
