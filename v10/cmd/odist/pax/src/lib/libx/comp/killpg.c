#include "FEATURE/killpg"

#ifdef _lib_killpg

int	_lib_dummy;

#else

extern int	kill();

int
killpg(g, s)
int	g;
int	s;
{
	return(kill(-g, s));
}

#endif
