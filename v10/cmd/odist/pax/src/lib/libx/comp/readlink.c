#include "FEATURE/readlink"

#ifdef _lib_readlink

int	_lib_dummy;

#else

#include <errno.h>

extern int	errno;

/*ARGSUSED*/
int
readlink(path, buf, siz)
char*	path;
char*	buf;
int	siz;
{
	errno = EINVAL;
	return(-1);
}

#endif
