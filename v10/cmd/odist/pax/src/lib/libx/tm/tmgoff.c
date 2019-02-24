/*
 * Glenn Fowler
 * AT&T Bell Laboratories
 *
 * time conversion support
 */

#include <tm.h>
#include <ctype.h>

#if __STDC__ || __cplusplus
#if __cplusplus
extern "C" {
#endif
extern long		strtol(const char*, char**, int);
#if __cplusplus
}
#endif
#else
extern long		strtol();
#endif

/*
 * return minutes offset from [[-+]hh[:mm[:ss]]] expression
 *
 * if e is non-null then it points to the first unrecognized char in s
 * d returned if no offset in s
 */

int
#if __STDC__ || __cplusplus
tmgoff(register const char* s, char** e, int d)
#else
tmgoff(s, e, d)
register char*	s;
char**		e;
int		d;
#endif
{
	register int	n;
	char*		t;

	if (isdigit(n = *s) || n == '-' || n == '+')
	{
		n = strtol(s, &t, 10);
		if (t > s)
		{
			n *= 60;
			if (*t == ':') n += (n < 0 ? -1 : 1) * strtol(t + 1, &t, 10);
			if (*t == ':') (void)strtol(t + 1, &t, 10);
		}
		else n = d;
	}
	else
	{
		n = d;
		t = (char*)s;
	}
	if (e) *e = t;
	return(n);
}
