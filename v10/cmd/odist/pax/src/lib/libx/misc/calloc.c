/*
 * since libx provides malloc(), these must be included
 * to avoid multiple link definitions for some shared
 * library implementations
 */

#include "FEATURE/bzero"
#ifdef	_lib_bzero
#define memzero(p,n)		bzero(p,n)
#else
#define memzero(p,n)		memset(p,0,n)
#endif

extern char*	malloc();

char*
calloc(count, size)
register unsigned int	count;
register unsigned int	size;
{
	register char*	ptr;

	if (ptr = malloc(size *= count))
		memzero(ptr, size);
	return(ptr);
}

void
cfree(old)
char*	old;
{
	free(old);
}
