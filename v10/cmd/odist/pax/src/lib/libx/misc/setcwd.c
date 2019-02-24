/*
 * K. P. Vo
 * G. S. Fowler
 * AT&T Bell Laboratories
 */

#include <limits.h>
#include <errno.h>

#ifndef PATH_MAX
#define PATH_MAX	1024
#endif

extern int	errno;

#if DEBUG

#undef	PATH_MAX

#define PATH_MAX	16

static int
vchdir(path)
char*	path;
{
	int	n;

	if (strlen(path) >= PATH_MAX)
	{
		errno = ENAMETOOLONG;
		n = -1;
	}
	else n = chdir(path);
	liberror("setcwd", -1, "chdir(%s)=%d", path, n);
	return(n);
}

#define chdir(p)	vchdir(p)

#endif

/*
 * set the current directory to path
 * if path is long and home!=0 then setcwd(home,0)
 * is called on intermediate chdir errors
 */

int
setcwd(path, home)
register char*	path;
char*		home;
{
	register char*	s;
	register int	n;
	int		r;

	r = 0;
	for (;;)
	{
		/*
		 * this should work 99% of the time
		 */

		if (!chdir(path)) return(r);

		/*
		 * chdir failed
		 */

		n = strlen(path);
		if (n < PATH_MAX) return(-1);
#ifdef ENAMETOOLONG
		if (errno != ENAMETOOLONG) return(-1);
#endif

		/*
		 * path is too long
		 */

		for (;;)
		{
			/*
			 * get a short prefix component
			 */

			s = path + PATH_MAX;
			while (--s >= path)
				if (*s == '/') break;
			if (s <= path) break;

			/*
			 * chdir to the prefix
			 */

			*s = '\0';
			if (chdir(path)) break;
			*s++ = '/';

			/*
			 * do the remainder
			 */

			if ((n -= s - path) < PATH_MAX)
			{
				if (chdir(s)) break;
				return(r);
			}
			path = s;
		}

		/*
		 * try to recover back to home
		 */

		if (!(path = home)) return(-1);
		home = 0;
		r = -1;
	}
}
