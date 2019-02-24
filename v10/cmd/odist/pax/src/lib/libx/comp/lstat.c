#include "FEATURE/lstat"

#ifdef _lib_lstat

int	_lib_dummy;

#else

#include <sys/types.h>
#include <sys/stat.h>

extern int	stat();

int
lstat(path, st)
char*		path;
struct stat*	st;
{
	return(stat(path, st));
}

#endif
