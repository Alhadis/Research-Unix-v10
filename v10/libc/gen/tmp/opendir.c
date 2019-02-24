/* Copyright (c) 1982 Regents of the University of California and pjw*/


#include "errno.h"
#include "ndir.h"

/*
 * open a directory.
 */
DIR *
opendir(name)
	char *name;
{
	register DIR *dirp;

	dirp = (DIR *)malloc(sizeof(DIR));
	if(!dirp) {
		errno = ENOSPC;
		return(0);
	}
	dirp->dd_fd = open(name, 0);
	if (dirp->dd_fd == -1) {
		free(dirp);
		return NULL;
	}
	dirp->dd_loc = 0;
	dirp->dd_offset = 0;
	dirp->dd_size = 0;
	return dirp;
}
