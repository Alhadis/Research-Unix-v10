#include <sys/types.h>
#include <sys/stat.h>

extern int	errno;

/*
 * physical stat if logical fails
 */

int
lpstat(path, st)
char*		path;
struct stat*	st;
{
#ifdef S_IFLNK
	int	oerrno;

	oerrno = errno;
	if (!stat(path, st)) return(0);
	errno = oerrno;
	return(lstat(path, st));
#else
	return(stat(path, st));
#endif
}
