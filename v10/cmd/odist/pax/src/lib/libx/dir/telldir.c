/*
 * telldir
 *
 * get offset of directory stream pointer
 */

#include "dirlib.h"

extern long	lseek();

long
telldir(dirp)
DIR	*dirp;
{
	return(lseek(dirp->dd_fd, 0L, 1) - (long)dirp->dd_size + (long)dirp->dd_loc);
}
