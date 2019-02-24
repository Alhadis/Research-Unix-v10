#include "decl.h"

checksum::
checksum()
{
	bufend = buf + cksize;
	bufptr = buf;
	char *p = buf;
	while (p < bufend)
		*p++ = '?';
}

void checksum::
combine (char* adr, int len)
{
	register char *p = bufptr, *q = adr;
	register int n = len;

	while (--n >= 0) {
		*p++ ^= *q++;
		if (p >= bufend)
			p = buf;
	}

	bufptr = p;
}

int checksum::
operator== (checksum& c)
{
	register int n = cksize;

	while (--n >= 0) {
		if (buf[n] != c.buf[n])
			return 0;
	}
	return 1;
}
