#include "FEATURE/symlink"

#ifdef _lib_symlink

int	_lib_dummy;

#else

#include <errno.h>

extern int	errno;

/*ARGSUSED*/
int
symlink(a, b)
char*	a;
char*	b;
{
	errno = EINVAL;
	return(-1);
}

#endif
