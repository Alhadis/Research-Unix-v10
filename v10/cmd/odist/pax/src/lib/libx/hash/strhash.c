/*
 * G. S. Fowler
 * AT&T Bell Laboratories
 *
 * hash table library
 */

#include "hashlib.h"

/*
 * return the hash of the null terminated string s
 */

#if __STDC__ || __cplusplus
unsigned int
strhash(const char* s)
#else
unsigned int
strhash(s)
char*	s;
#endif
{
	register unsigned char*	p;
	register unsigned int	i;
	register unsigned int	c;

	i = 0;
	p = (unsigned char*)s;
	while (c = *p++) HASHPART(i, c);
	return(i);
}
