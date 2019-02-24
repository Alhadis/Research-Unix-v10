#include "FEATURE/strrchr"

#ifdef _lib_strrchr

int	_lib_dummy;

#else

#include "FEATURE/rindex"

#ifdef _lib_rindex

extern char*	rindex();

char*
strrchr(s, c)
char*	s;
int	c;
{
	return(rindex(s, c));
}

#else

char*
strrchr(s, c)
register char*	s;
register int	c;
{
	register char*	r;

	r = 0;
	do if (*s == c) r = s; while(*s++);
	return(r);
}

#endif

#endif
