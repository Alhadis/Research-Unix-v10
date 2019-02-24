/* Copyright (c) 1982 Regents of the University of California */

#include "config.h"
#ifdef LOCALDIR
#include <sys/types.h>
#include "ndir.h"

static char sccsid[] = "@(#)telldir.c 4.1 2/21/82";

/*
 * return a pointer into a directory
 */
long
telldir(dirp)
	DIR *dirp;
{
	return (lseek(dirp->dd_fd, 0L, 1) - dirp->dd_size + dirp->dd_loc);
}
#endif
