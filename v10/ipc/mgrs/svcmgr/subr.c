/*
 * miscellany
 */

#include "mgr.h"

/*
 * read input up to a null
 */
char *
rdline(f)
int f;
{
	static char buf[2*ARB];
	register char *p;
	register int n;

	for (p = buf; p-buf<ARB-1; p++) {
		n = read(f, p, 1);
		if (n <= 0)
			break;
		if (*p == '\0')
			return (buf);
	}
	return NULL;
}
