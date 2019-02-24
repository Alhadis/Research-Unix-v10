/*
 * put name=value in the environment
 * pointer to name=value returned
 * environ==0 is ok
 *
 *	putenv("N=V")		add N=V
 *	putenv("N")		delete N
 *	putenv(0)		expect more (pre-fork optimization)
 *
 * _ always placed at the top
 */

#define INCREMENT	16		/* environ increment		*/

extern char**	environ;

extern char*	malloc();
extern char*	realloc();
extern char*	strchr();

char*
putenv(key)
char*	key;
{
	static char**	envv;		/* recorded environ		*/
	static char**	next;		/* next free slot		*/
	static char**	last;		/* last free slot (0)		*/

	register char**	v = environ;
	register char**	p = envv;
	register char*	s;
	register char*	t;
	int		n;

	if (p && !v)
	{
		next = p;
		*++next = 0;
	}
	else if (p != v || !v)
	{
		if (v)
		{
			while (*v++);
			n = v - environ + INCREMENT;
			v = environ;
		}
		else n = INCREMENT;
		if (!p || (last - p + 1) < n)
		{
			if (p) free(p);
			else if (fs3d(2))
			{
				(void)close(open(".", 0));
				v = environ;
			}
			if (!(p = (char**)malloc(n * sizeof(char**))))
				return(0);
			last = p + n - 1;
		}
		envv = environ = p;
		if (v && v[0] && v[0][0] == '_' && v[0][1] == '=') *p++ = *v++;
		else *p++ = "_=";
		if (!v) *p = 0;
		else while (*p = *v++)
			if (p[0][0] == '_' && p[0][1] == '=') envv[0] = *p;
			else p++;
		next = p;
		p = envv;
	}
	else if (next == last)
	{
		n = last - v + INCREMENT + 1;
		if (!(p = (char**)realloc(p, n * sizeof(char**))))
			return(0);
		last = p + n - 1;
		next = last - INCREMENT;
		envv = environ = p;
	}
	if (!key) return(*p);
	for (; s = *p; p++)
	{
		t = key;
		do
		{
			if (!*t || *t == '=')
			{
				if (*s == '=')
				{
					if (!*t)
					{
						s = *p;
						v = p++;
						while (*v++ = *p++);
						next--;
						return(s);
					}
					return(*p = key);
				}
				break;
			}
		} while (*t++ == *s++);
	}
	if (!strchr(key, '=')) return(key);
	p = next;
	*++next = 0;
	return(*p = key);
}
