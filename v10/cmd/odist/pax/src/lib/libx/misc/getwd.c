/*
 * getwd() using getcwd()
 *
 * some getwd()'s are incredible
 */

#include <limits.h>

#ifndef PATH_MAX
#define PATH_MAX	1024
#endif

char*
getwd(path)
char*	path;
{
	if (getcwd(path, PATH_MAX)) return(path);
	(void)strcpy(path, "getwd: error in . or ..");
	return(0);
}
