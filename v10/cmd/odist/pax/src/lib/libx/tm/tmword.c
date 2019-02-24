/*
 * Glenn Fowler
 * AT&T Bell Laboratories
 *
 * time conversion support
 */

#include <tm.h>
#include <ctype.h>

/*
 * match s against t ignoring case and .'s
 *
 * suf is an n element table of suffixes that may trail s
 * if all isalpha() chars in s match then 1 is returned
 * and if e is non-null it will point to the first unmatched
 * char in s, otherwise 0 is returned
 */

int
#if __STDC__ || __cplusplus
tmword(register const char* s, char** e, register const char* t, char** suf, int n)
#else
tmword(s, e, t, suf, n)
register char*	s;
char**		e;
register char*	t;
char**		suf;
int		n;
#endif
{
	register int	c;
#if __STDC__ || __cplusplus
	const char*	b;
#else
	char*		b;
#endif

	if (*s && *t)
	{
		b = s;
		while (c = *s++)
		{
			if (c != '.')
			{
				if (!isalpha(c) || c != *t && (islower(c) ? toupper(c) : tolower(c)) != *t) break;
				t++;
			}
		}
		s--;
		if (!isalpha(c))
		{
			if (e) *e = (char*)s;
			return(1);
		}
		if (!*t && s > (b + 1))
		{
			b = s;
			while (n-- && (t = *suf++))
			{
				s = b;
				while (isalpha(c = *s++) && (c == *t || (islower(c) ? toupper(c) : tolower(c)) == *t)) t++;
				if (!*t && !isalpha(c))
				{
					if (e) *e = (char*)s - 1;
					return(1);
				}
			}
		}
	}
	return(0);
}
