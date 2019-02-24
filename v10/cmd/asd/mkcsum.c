#include "seal.h"

unsigned long
mkcsum (csum, start, limit)
	register unsigned long csum;
	char *start, *limit;
{
	register int c;

	while (start < limit) {
		c = (unsigned char) *start++;
		if (csum & 1)
			csum = (csum >> 1) | CSHIBIT;
		else

			csum >>= 1;
		csum += c;
		csum &= CSMASK;
	}
	return csum;
}
