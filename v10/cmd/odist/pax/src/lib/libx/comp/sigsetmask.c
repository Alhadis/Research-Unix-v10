#include "FEATURE/sigsetmask"

#ifdef _lib_sigsetmask

int	_lib_dummy;

#else

int
sigsetmask(mask)
int	mask;
{
	return(0);
}

#endif
