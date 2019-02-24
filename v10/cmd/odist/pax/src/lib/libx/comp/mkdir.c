#include "FEATURE/mkdir"

#ifdef _lib_mkdir

int	_lib_dummy;

#else

#include <errno.h>

extern int	errno;

extern int	_exit();
extern int	access();
extern int	chmod();
extern int	execl();
extern int	fork();
extern int	wait();

int
mkdir(path, mode)
char*	path;
int	mode;
{
	register int	n;
	register int	pid;

	n = errno;
	if (!access(path, 0))
	{
		errno = EEXIST;
		return(-1);
	}
	if (errno != ENOENT) return(-1);
	errno = n;
	switch (pid = fork())
	{
	case -1:
		return(-1);
	case 0:
		execl("/bin/mkdir", "mkdir", path, 0);
		execl("/usr/5bin/mkdir", "mkdir", path, 0);
		_exit(1);
	default:
		while ((n = wait((int*)0)) >= 0 && n != pid);
		break;
	}
	return(chmod(path, mode));
}

#endif
