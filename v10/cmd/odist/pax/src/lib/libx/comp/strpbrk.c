#include "FEATURE/strpbrk"

#ifdef _lib_strpbrk

int	_lib_dummy;

#else

/*
 * Return ptr to first occurance of any character from `brkset'
 * in the character string `string'; 0 if none exists.
 */

char*
strpbrk(string, brkset)
register char*	string;
register char*	brkset;
{
	register char*	p;

	do
	{
		for (p = brkset; *p && *p != *string; p++);
		if (*p) return(string);
	} while (*string++);
	return(0);
}

#endif
