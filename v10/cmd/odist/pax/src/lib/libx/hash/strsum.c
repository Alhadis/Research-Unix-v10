/*
 * G. S. Fowler
 * AT&T Bell Laboratories
 *
 * hash table library
 */

#include "hashlib.h"

/*
 * return a running 32 bit checksum of string s
 *
 * c is the return value from a previous
 * memsum() or strsum() call, 0 on the first call
 *
 * the result is the same on all implementations
 */

#if __STDC__ || __cplusplus
unsigned long
strsum(const char* s, register unsigned long c)
#else
unsigned long
strsum(s, c)
char*			s;
register unsigned long	c;
#endif
{
	register unsigned char*	p;
	register int		n;

	p = (unsigned char*)s;
	while (n = *p++) HASHPART(c, n);
#if LONG_MAX > 2147483647
	return(c & 0xffffffff);
#else
	return(c);
#endif
}
