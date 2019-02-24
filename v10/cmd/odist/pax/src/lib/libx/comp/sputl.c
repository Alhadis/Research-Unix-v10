#include "FEATURE/sputl"

#ifdef _lib_sputl

int	_lib_dummy;

#else

/*
 * The intent here is to provide a means to make the value of
 * bytes in an io-stream correspond to the value of the long
 * in the memory while doing the io a `long' at a time.
 * Files written and read in this way are machine-independent.
 */

#include <limits.h>

void
sputl(w, buffer)
register long w;
register char *buffer;
{
	register int i = CHAR_BIT * sizeof(long);

	while ((i -= CHAR_BIT) >= 0)
		*buffer++ = (char) (w >> i);
}

#endif
