#include "FEATURE/strtod"

#ifdef _lib_strtod

int	_lib_dummy;

#else

#include <ctype.h>

extern double	atof();

double
strtod(s, e)
register char*	s;
char**		e;
{
	double	n;

	n = atof(s);
	if (e)
	{
		while (isspace(*s)) s++;
		if (*s == '-' || *s == '+') s++;
		while (isdigit(*s)) s++;
		if (*s == '.') while (isdigit(*++s));
		if (*s == 'e' || *s == 'E')
		{
			if (*++s == '-' || *s == '+') s++;
			while (isdigit(*s)) s++;
		}
		*e = s;
	}
	return(n);
}

#endif
