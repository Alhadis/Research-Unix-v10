/*
 * G. S. Fowler
 * AT&T Bell Laboratories
 *
 * hash table library
 */

#include "hashlib.h"

/*
 * return a running 32 bit checksum of buffer b of length n
 *
 * c is the return value from a previous
 * memsum() or strsum() call, 0 on the first call
 *
 * the result is the same on all implementations
 */

#if __STDC__ || __cplusplus
unsigned long
memsum(const char* b, int n, register unsigned long c)
#else
unsigned long
memsum(b, n, c)
char*			b;
int			n;
register unsigned long	c;
#endif
{
	register unsigned char*	p;
	register unsigned char*	e;

	p = (unsigned char*)b;
	e = p + n;
	while (p < e) HASHPART(c, *p++);
#if LONG_MAX > 2147483647
	return(c & 0xffffffff);
#else
	return(c);
#endif
}
