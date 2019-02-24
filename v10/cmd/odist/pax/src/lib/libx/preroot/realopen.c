/*
 * AT&T Bell Laboratories
 * disable preroot and open path relative to the real root
 */

#include <preroot.h>

#if FS_PREROOT

#include <limits.h>

#ifndef PATH_MAX
#define PATH_MAX	1024
#endif

int
realopen(path, mode, perm)
char*	path;
int	mode;
int	perm;
{
	char		buf[PATH_MAX + 8];

	extern int	open();
	extern char*	strcopy();

	if (*path != '/' || !ispreroot((char*)0)) return(-1);
	(void)strcopy(strcopy(buf, PR_REAL), path);
	return(open(buf, mode, perm));
}

#else

int	_lib_dummy;

#endif
