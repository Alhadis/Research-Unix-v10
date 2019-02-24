#include "FEATURE/memcmp"

#ifdef _lib_memcmp

int	_lib_dummy;

#else

int
memcmp(b1, b2, n)
register unsigned char*	b1;
register unsigned char*	b2;
unsigned int		n;
{
	register unsigned char*	e;

	e = b1 + n;
	while (b1 < e)
		if (*b1++ != *b2++)
			return(*--b1 - *--b2);
	return(0);
}

#endif
