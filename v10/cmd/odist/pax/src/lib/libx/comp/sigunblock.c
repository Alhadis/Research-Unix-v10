#include "FEATURE/sigunblock"

#ifdef _lib_sigunblock

int	_lib_dummy;

#else

#include <sig.h>

int
sigunblock(s)
int	s;
{
	return(sigsetmask(sigsetmask(0) & ~sigmask(s)));
}

#endif
