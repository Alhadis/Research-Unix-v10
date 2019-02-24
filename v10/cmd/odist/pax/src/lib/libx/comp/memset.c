#include "FEATURE/memset"

#ifdef _lib_memset

int	_lib_dummy;

#else

char*
memset(sp, c, n)
register char *sp, c;
register int n;
{
	register char *sp0 = sp;

	while (--n >= 0)
		*sp++ = c;
	return (sp0);
}

#endif
