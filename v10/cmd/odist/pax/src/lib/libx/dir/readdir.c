/*
 * readdir
 *
 * read from directory stream
 *
 * NOTE: directory entries must fit within DIRBLKSIZ boundaries
 */

#include "dirlib.h"

struct dirent*
readdir(dirp)
register DIR*	dirp;
{
	register struct dirent*	dp;

	for (;;)
	{
		if (dirp->dd_loc >= dirp->dd_size)
		{
			if (dirp->dd_size < 0) return(0);
			dirp->dd_loc = 0;
			if ((dirp->dd_size = getdents(dirp->dd_fd, dirp->dd_buf, DIRBLKSIZ)) <= 0)
			{
			uncover:
				if (dirp->nxtfd < dirp->numfd)
				{
					(void)close(dirp->dd_fd);
					dirp->dd_fd = dirp->covered[dirp->nxtfd++];
					dirp->dd_loc = 0;
					dirp->opaque = 0;
					continue;
				}
				return(0);
			}
		}
		dp = (struct dirent*)&dirp->dd_buf[dirp->dd_loc];
		if (dp->d_reclen <= 0) goto uncover;
		dirp->dd_loc += dp->d_reclen;
		if (dp->d_ino)
		{
			if (dirp->fs3d)
			{
				register char*	s = dp->d_name;

				/*
				 * skip opaque and hidden entries
				 */

				if (*s++ == '.' && *s++ == '.' && *s++ == '.')
				{
					if (!*s) dirp->opaque = dp->d_ino;
					continue;
				}
			}
			if (dirp->overlay)
			{
				if (hashget(dirp->overlay, dp->d_name)) continue;
				if (dirp->nxtfd < dirp->numfd) hashput(dirp->overlay, 0, (char*)1);
			}
			if (dp->d_ino == dirp->opaque) continue;
			return(dp);
		}
	}
	/*NOTREACHED*/
}
