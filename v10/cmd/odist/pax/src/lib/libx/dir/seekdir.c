/*
 * seekdir
 *
 * seek on directory stream
 */

#include "dirlib.h"

extern long	lseek();

void
seekdir(dirp, loc)
register DIR	*dirp;
long		loc;
{
	long	base;		/* file location of block */
	long	offset; 	/* offset within block */

	if (telldir(dirp) != loc)
	{
		offset = loc % DIRBLKSIZ;
		base = loc - offset;
		(void)lseek(dirp->dd_fd, base, 0);
		dirp->dd_loc = dirp->dd_size = 0;

		/*
		 * skip some entries
		 */

		while (dirp->dd_loc < offset)
			if (!readdir(dirp))
				break; 	/* "can't happen" */
	}
}
