#include "FEATURE/mount"

#ifdef _lib_mount

int	_lib_dummy;

#else

#include <errno.h>

extern int	errno;

/*ARGSUSED*/
int
mount(a, b, c)
char*	a;
char*	b;
int	c;
{
	errno = EINVAL;
	return(-1);
}

#endif
