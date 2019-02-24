#include "FEATURE/remove"

#ifdef _lib_remove

int	_lib_dummy;

#else

extern int	unlink();

int
remove(path)
char*	path;
{
	return(unlink(path));
}

#endif
