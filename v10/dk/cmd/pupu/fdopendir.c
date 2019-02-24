
static char	SCCSID[] = "@(#)fdopendir.c	2.1 DKHOST 85/01/15";

#include <sys/types.h>
#include <sys/stat.h>
#include "ndir.h"

/*
 * open a directory.
 */
	DIR *
fdopendir(fd)
{
	register DIR *dirp;

	dirp = (DIR *)malloc(sizeof(DIR));
	dirp->dd_fd = fd;

	dirp->dd_loc = 0;
	return dirp;
}
