/* Copyright (c) 1982 Regents of the University of California */


#include <ndir.h>

/*
 * close a directory.
 */
void
closedir(dirp)
	register DIR *dirp;
{
	close(dirp->dd_fd);
	free(dirp);
}
