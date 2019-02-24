#include "FEATURE/link"

#ifdef _lib_link

int	_lib_dummy;

#else

#include <errno.h>

extern int	errno;

int
link(from, to)
char*	from;
char*	to;
{
	errno = EINVAL;
	return(-1);
}

#endif
