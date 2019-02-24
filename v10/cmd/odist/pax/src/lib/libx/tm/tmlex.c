/*
 * Glenn Fowler
 * AT&T Bell Laboratories
 *
 * time conversion support
 */

#include <tm.h>
#include <ctype.h>

/*
 * return the tab table index that matches s ignoring case and .'s
 *
 * ntab and nsuf are the number of elements in tab and suf,
 * -1 for 0 sentinel
 *
 * all isalpha() chars in str must match
 * suf is a table of nsuf valid str suffixes 
 * if e is non-null then it will point to first unmatched char in str
 * which will always be non-isalpha()
 */

int
#if __STDC__ || __cplusplus
tmlex(register const char* s, char** e, char** tab, register int ntab, char** suf, int nsuf)
#else
tmlex(s, e, tab, ntab, suf, nsuf)
register char*	s;
char**		e;
char**		tab;
register int	ntab;
char**		suf;
int		nsuf;
#endif
{
	register char**	p;

	for (p = tab; ntab-- && *p; p++)
		if (tmword(s, e, *p, suf, nsuf))
			return(p - tab);
	return(-1);
}
