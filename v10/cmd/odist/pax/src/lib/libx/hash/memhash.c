/*
 * G. S. Fowler
 * AT&T Bell Laboratories
 *
 * hash table library
 */

#include "hashlib.h"

/*
 * return the hash of buffer s of length n
 */

#if __STDC__ || __cplusplus
unsigned int
memhash(register const char* s, int n)
#else
#ifndef const
#define const
#endif
unsigned int
memhash(s, n)
register char*	s;
int		n;
#endif
{
	register const char*	e;
	register unsigned int	c;

	e = s + n;
	c = 0;
	while (s < e) HASHPART(c, *s++);
	return(c);
}
