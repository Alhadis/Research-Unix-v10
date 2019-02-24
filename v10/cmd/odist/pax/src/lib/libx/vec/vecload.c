/*
 * Glenn Fowler
 * AT&T Bell Laboratories
 *
 * string vector load support
 */

extern char**	malloc();

/*
 * load a string vector from lines in buf
 * buf may be modified on return
 *
 * each line in buf is treated as a new vector element
 * lines with # as first char are comments
 * \ as the last char joins consecutive lines
 *
 * the vector ends with a 0 sentinel
 *
 * the string array pointer is returned
 */

char**
vecload(buf)
char*	buf;
{
	register char*	s;
	register int	n;
	register char**	p;
	char**		vec;

	vec = 0;
	n = (*buf == '#') ? -1 : 0;
	for (s = buf;; s++)
	{
		if (*s == '\n')
		{
			if (s > buf && *(s - 1) == '\\') *(s - 1) = *s = ' ';
			else
			{
				*s = 0;
				if (*(s + 1) != '#')
				{
					n++;
					if (!*(s + 1)) break;
				}
			}
		}
		else if (!*s)
		{
			n++;
			break;
		}
	}
	if (n < 0) n = 0;
	if (p = (char**)malloc((n + 3) * sizeof(char*)))
	{
		*p++ = s = buf;
		vec = ++p;
		if (n > 0) for (;;)
		{
			if (*s != '#')
			{
				*p++ = s;
				if (--n <= 0) break;
			}
			while (*s) s++;
			s++;
		}
		*p = 0;
		*(vec - 1) = (char*)p;
	}
	return(vec);
}
