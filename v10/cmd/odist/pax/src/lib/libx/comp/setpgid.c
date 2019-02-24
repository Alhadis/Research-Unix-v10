#include "FEATURE/setpgid"

#ifdef _lib_setpgid

int	_lib_dummy;

#else

extern int	setpgrp();

int
setpgid(pid, pgid)
int	pid;
int	pgid;
{
	return(setpgrp(pid, pgid));
}

#endif
