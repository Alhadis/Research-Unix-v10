static char ID[] = "@(#) sgetl.c: 1.1 1/8/82";
/*
 * The intent here is to provide a means to make the value of
 * bytes in an io-buffer correspond to the value of a long
 * in the memory while doing the io a `long' at a time.
 * Files written and read in this way are machine-independent.
 *
 */
#include <values.h>

long
sgetl(buffer)
register char *buffer;
{
	register long w = 0;
	register int i = BITSPERBYTE * sizeof(long);

	while ((i -= BITSPERBYTE) >= 0)
		w |= (long) ((unsigned char) *buffer++) << i;
	return (w);
}
