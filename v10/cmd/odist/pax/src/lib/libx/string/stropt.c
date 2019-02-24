/*
 * Glenn Fowler
 * AT&T Bell Laboratories
 */

#include <namval.h>
#include <ctype.h>

/*
 * parse option expression in s using options in tab
 * options match
 *
 *	[no]name[=['"]value["']] ...
 *
 * f is called for each option
 *
 *	(*f)(void* a, struct namval* p, int n, char* v)
 *
 *	a	from stropt
 *	p	matching tab entry
 *	n	0 if option had ``no'' prefix
 *	v	option value pointer
 *
 * for unmatched options p has the sentinel tab value
 *
 * names in s may be shorter than tab names
 * longer names must have a prefix that matches a tab name
 * the first match is returned
 */

int
stropt(s, tab, f, a)
register char*	s;
struct namval*	tab;
int		(*f)();
char*		a;
{
	register char*	v;
	register char*	t;
	struct namval*	p;
	int		c;
	int		n;
	struct namval	u;

	if (s) for (;;)
	{
		while (isspace(*s)) s++;
		if (*s == 'n' && *(s + 1) == 'o')
		{
			s += 2;
			n = 0;
		}
		else n = 1;
		if (!*s) break;
		for (p = tab; t = p->name; p++)
		{
			for (v = s; *t && *t++ == *v; v++);
			if (!*t || isspace(*v) || *v == '=') break;
		}
		if (!t)
		{
			u.name = v = s;
			u.value = p->value;
			p = &u;
		}
		while (*v && !isspace(*v) && *v != '=') v++;
		if (*v == '=')
		{
			if (t) t = 0;
			else *(t = v) = 0;
			s = ++v;
			if ((c = *s) != '"' && c != '\'') while (*s && !isspace(*s)) s++;
			else
			{
				v++;
				while (*++s && *s != c);
			}
		}
		else
		{
			s = v;
			t = 0;
		}
		c = *s;
		*s = 0;
		n = (*f)(a, p, n, v);
		if (*s = c) s++;
		if (t) *t = '=';
		if (n) return(n);
	}
	return(0);
}
