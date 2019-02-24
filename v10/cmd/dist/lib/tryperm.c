#define _POSIX_SOURCE
#include <ctype.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include "lib.h"

/*
 * determine if we can recursively create the given pathname.  not a
 * generally useful function since it destroys the pathname.
 */
static int
cancreate(char *path, int recurse)
{
	char *slash;

	for (;;) {
		slash = strrchr(path, '/');
		if (! slash)
			return 0;	/* absolute pathnames only */
		*slash = '\0';
		if (!*path)
			break;
		if (access(path, W_OK) == 0)
			return 1;
		if (!recurse)
			return 0;
		if (access(path, F_OK) == 0)
			return 0;
	}
	return access("/", W_OK) == 0;
}
		
/*
 * try the instructions of the package on the given fd against the
 * permissions currently in the file system.  return 1 if we think we
 * ought to go ahead and try inspkg.
 * the right place to put this guy would be in the tarf command;
 * then eventually we could remove all tarfile support from dist
 * proper when (if?) most systems have adequate tar commands.
 */
int
tryperm(int fd)
{
	return 1;
}
