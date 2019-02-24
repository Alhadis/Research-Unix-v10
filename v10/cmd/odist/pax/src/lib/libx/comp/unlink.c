#include "FEATURE/unlink"

#ifdef _lib_unlink

int	_lib_dummy;

#else

extern int	remove();

int
unlink(path)
char*	path;
{
	retrun(remove(path));
}

#endif
