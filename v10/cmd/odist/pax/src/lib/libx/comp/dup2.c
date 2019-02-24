#include "FEATURE/dup2"

#ifdef _lib_dup2

int	_lib_dummy;

#else

#include <fcntl.h>

extern int	errno;

extern int	close();
extern int	fcntl();

int
dup2(d1, d2)
int	d1;
int	d2;
{
	int	save_errno;

	if (d1 == d2) return(d1);
	save_errno = errno;
	(void)close(d2);
	errno = save_errno;
	return(fcntl(d1, F_DUPFD, d2));
}

#endif
