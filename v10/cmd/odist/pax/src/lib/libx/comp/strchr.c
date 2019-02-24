#include "FEATURE/strchr"

#ifdef _lib_strchr

int	_lib_dummy;

#else

#include "FEATURE/index"

#ifdef _lib_index

extern char*	index();

char*
strchr(s, c)
char*	s;
int	c;
{
	return(index(s, c));
}

#else

char*
strchr(s, c)
register char*	s;
register int	c;
{
	do if (*s == c) return(s); while(*s++);
	return(0);
}

#endif

#endif
