#include "FEATURE/strspn"

#ifdef _lib_strspn

int	_lib_dummy;

#else

/*
 * Return the number of characters in the maximum leading segment
 * of string which consists solely of characters from charset.
 */

int
strspn(string, charset)
char*		string;
register char*	charset;
{
	register char*	p;
	register char*	q;

	for (q = string; *q; q++)
	{
		for (p = charset; *p && *p != *q; p++);
		if (!*p) break;
	}
	return(q - string);
}

#endif
