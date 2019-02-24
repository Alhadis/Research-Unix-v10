#include "FEATURE/vfork"

#ifdef _lib_vfork

int	_lib_dummy;

#else

extern int	fork();

int
vfork()
{
	return(fork());
}

#endif
