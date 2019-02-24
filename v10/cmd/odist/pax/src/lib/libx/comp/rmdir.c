#include "FEATURE/rmdir"

#ifdef _lib_rmdir

int	_lib_dummy;

#else

#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>

extern int	errno;

extern int	_exit();
extern int	access();
extern int	chmod();
extern int	execl();
extern int	fork();
extern int	stat();
extern int	wait();

int
rmdir(path)
char*	path;
{
	register int	n;
	register int	pid;
	struct stat	st;

	if (stat(path, &st) < 0) return(-1);
	if ((st.st_mode & S_IFMT) != S_IFDIR)
	{
		errno = ENOTDIR;
		return(-1);
	}
	switch (pid = fork())
	{
	case -1:
		return(-1);
	case 0:
		execl("/bin/rmdir", "rmdir", path, 0);
		execl("/usr/5bin/rmdir", "rmdir", path, 0);
		_exit(1);
	default:
		while ((n = wait((int*)0)) >= 0 && n != pid);
		break;
	}
	n = errno;
	if (access(path, 0) < 0)
	{
		errno = n;
		return(0);
	}
	errno = EPERM;
	return(-1);
}

#endif
