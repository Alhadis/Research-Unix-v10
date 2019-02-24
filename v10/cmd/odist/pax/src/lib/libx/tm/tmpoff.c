/*
 * Glenn Fowler
 * AT&T Bell Laboratories
 *
 * time conversion support
 */

#include <tm.h>
#include <stdio.h>

/*
 * append p and hh:mm part of n to s
 *
 * n ignored if n==d
 * end of s is returned
 */

char*
#if __STDC__ || __cplusplus
tmpoff(register char* s, register const char* p, register int n, int d)
#else
tmpoff(s, p, n, d)
register char*	s;
register char*	p;
register int	n;
int		d;
#endif
{
	while (*s = *p++) s++;
	if (n != d)
	{
		if (n < 0)
		{
			n = -n;
			*s++ = '-';
		}
		else *s++ = '+';
		(void)sprintf(s, "%d", n / 60);
		while (*++s);
		if (n %= 60)
		{
			(void)sprintf(s, ":%02d", n);
			while (*++s);
		}
	}
	return(s);
}
