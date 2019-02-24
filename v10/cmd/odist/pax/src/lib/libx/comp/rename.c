#include "FEATURE/rename"

#ifdef _lib_rename

int	_lib_dummy;

#else

extern int	link();
extern int	unlink();

int
rename(from, to)
char*	from;
char*	to;
{
	int	n;

	if (!(n = link(from, to))) (void)unlink(from);
	return(n);
}

#endif
