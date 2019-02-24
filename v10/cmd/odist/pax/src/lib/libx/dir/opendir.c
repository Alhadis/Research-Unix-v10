/*
 * opendir, closedir
 *
 * open|close directory stream
 *
 * POSIX compatible directory stream access routines:
 *
 *	#include <sys/types.h>
 *	#include <dirent.h>
 *
 * NOTE: readdir() returns a pointer to struct dirent
 */

#include "dirlib.h"

#include <errno.h>

#if !lint
static char	id[] = "\n@(#)directory (AT&T Bell Laboratories) 04/01/90\0\n";
#endif

static DIR*	freedirp;		/* always keep one dirp */

extern int	errno;

extern char*	malloc();
extern char*	strcopy();
extern void	free();
extern int	open();
extern int	close();
extern int	fclex();
extern int	fstat();

DIR*
opendir(path)
register char*	path;
{
	register DIR*	dirp = 0;
	register int	fd;
	dev_t		dev;
	ino_t		ino;
	struct stat	st;

	if ((fd = open(path, 0)) < 0) return(0);
	if (fstat(fd, &st) < 0 ||
	   (st.st_mode & S_IFMT) != S_IFDIR && (errno = ENOTDIR) ||
	   fclex(fd, 1) ||
	   !(dirp = freedirp ? freedirp : (DIR*)malloc(sizeof(DIR) + DIRBLKSIZ)))
	{
		(void)close(fd);
		if (dirp)
		{
			if (!freedirp) freedirp = dirp;
			else free((char*)dirp);
		}
		return(0);
	}
	freedirp = 0;
	dirp->dd_fd = fd;
	dirp->dd_loc = dirp->dd_size = 0;	/* refill needed */
	dirp->dd_buf = (char*)dirp + sizeof(DIR);

	/*
	 * handle 3d and preroot fs
	 */

	dirp->numfd = dirp->nxtfd = 0;
	if (dirp->fs3d = fs3d(2))
	{
		char	buf[PATH_MAX];

		dirp->opaque = 0;
		path = strcopy(buf, path);
		while (dirp->numfd < sizeof(dirp->covered) / sizeof(dirp->covered[0]))
		{
			path = strcopy(path, "/...");
			if ((fd = open(buf, 0)) < 0) break;

			/*
			 * check for non-dirs or dirs covered by virtual dir
			 */

			dev = st.st_dev;
			ino = st.st_ino;
			if (fstat(fd, &st) || ino == st.st_ino && dev == st.st_dev) (void)close(fd);
			else if ((st.st_mode & S_IFMT) != S_IFDIR)
			{
				(void)close(fd);
				break;
			}
			else
			{
				fclex(fd, 1);
				dirp->covered[dirp->numfd++] = fd;
			}
		}
	}
#if defined(FS_PREROOT)
	else if ((fd = realopen(path, 0, 0)) >= 0)
	{
		dev = st.st_dev;
		ino = st.st_ino;
		if (fstat(fd, &st) || ino == st.st_ino && dev == st.st_dev || (st.st_mode & S_IFMT) != S_IFDIR) (void)close(fd);
		else
		{
			fclex(fd, 1);
			dirp->covered[dirp->numfd++] = fd;
		}
	}
#endif
	dirp->overlay = dirp->numfd ? hashalloc((HASHTABLE*)0, HASH_set, HASH_ALLOCATE, 0) : 0;
	return(dirp);
}

void
closedir(dirp)
register DIR*	dirp;
{
	if (dirp)
	{
		(void)close(dirp->dd_fd);
		while (dirp->nxtfd < dirp->numfd) (void)close(dirp->covered[dirp->nxtfd++]);
		if (dirp->overlay) hashfree(dirp->overlay);
		if (!freedirp) freedirp = dirp;
		else free((char*)dirp);
	}
}
