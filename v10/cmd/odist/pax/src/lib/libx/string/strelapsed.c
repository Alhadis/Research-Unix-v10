/*
 * Glenn Fowler
 * AT&T Bell Laboratories
 *
 * parse elapsed time in 1/n secs from s
 * compatible with fmtelapsed()
 * if e!=0 then it is set to first unrecognized char
 */

#include <ctype.h>

unsigned long
strelapsed(s, e, n)
register char*	s;
char**		e;
int		n;
{
	register int		c;
	register unsigned long	v;
	unsigned long		t = 0;
	int			f = 0;
	int			m;
	char*			last;

	while (isspace(*s)) s++;
	while (*(last = s))
	{
		v = 0;
		while ((c = *s++) >= '0' && c <= '9')
			v = v * 10 + c - '0';
		v *= n;
		if (c == '.')
			for (m = n; (c = *s++) >= '0' && c <= '9';)
				f += (m /= 10) * (c - '0');
		switch (c)
		{
		case 'S':
			v *= 20;
		case 'Y':
			v *= 12;
		case 'M':
			v *= 4;
		case 'w':
			v *= 7;
		case 'd':
			v *= 24;
		case 'h':
			v *= 60;
		case 'm':
			v *= 60;
			break;
		case '0':
			s--;
		case 's':
			v += f;
			f = 0;
			break;
		default:
			goto done;
		}
		t += v;
	}
 done:
	if (e) *e = last;
	return(t);
}
