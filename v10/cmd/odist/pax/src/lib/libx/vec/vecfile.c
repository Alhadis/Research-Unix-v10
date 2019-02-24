/*
 * Glenn Fowler
 * AT&T Bell Laboratories
 *
 * string vector load support
 */

#include <sys/types.h>
#include <sys/stat.h>

extern char**	vecload();
extern int	close();
extern int	fstat();
extern char*	malloc();
extern int	open();

/*
 * load a string vector from lines in file
 */

char**
vecfile(file)
char*	file;
{
	register int	n;
	register char*	buf;
	register char**	vec;
	int		fd;
	struct stat	st;

	vec = 0;
	if ((fd = open(file, 0)) >= 0)
	{
		if (!fstat(fd, &st) && (n = st.st_size) > 0 && (buf = malloc(n + 1)))
		{
			if (read(fd, buf, n) == n)
			{
				buf[n] = 0;
				vec = vecload(buf);
			}
			if (!vec) free(buf);
		}
		(void)close(fd);
	}
	return(vec);
}
