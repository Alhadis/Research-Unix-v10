#include "seal.h"

unsigned long
mkcsum (unsigned long csum, char *start, char *limit)
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
