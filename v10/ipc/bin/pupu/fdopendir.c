
static char	SCCSID[] = "@(#)fdopendir.c	2.1 DKHOST 85/01/15";

#include <sys/types.h>
#include <sys/stat.h>
#include <ndir.h>

/*
 * open a directory.
 */
	DIR *
fdopendir(name)
	char *name;
{
	register DIR *dirp;

	dirp = opendir(name);
	if(dirp!=NULL)
		openfiles[dirp->dd_fd] = 1;
	return dirp;
}
