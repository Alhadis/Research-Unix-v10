#include "FEATURE/sgetl"

#ifdef _lib_sgetl

int	_lib_dummy;

#else

/*
 * The intent here is to provide a means to make the value of
 * bytes in an io-buffer correspond to the value of a long
 * in the memory while doing the io a `long' at a time.
 * Files written and read in this way are machine-independent.
 */

#include <limits.h>

long
sgetl(buffer)
register char *buffer;
{
	register long w = 0;
	register int i = CHAR_BIT * sizeof(long);

	while ((i -= CHAR_BIT) >= 0)
		w |= (long) ((unsigned char) *buffer++) << i;
	return (w);
}

#endif
