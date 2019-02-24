/* Copyright (c) 1982 Regents of the University of California
 * and modified by pjw in 1987 */

#include "ndir.h"

/*
 * return a pointer into a directory
 * see seekdir.c for an explanation of the return value
 */
long
telldir(dirp)
	DIR *dirp;
{
	return(DIRBLKSIZ * dirp->dd_offset  +  dirp->dd_loc);
}
