/*
 * G. S. Fowler
 * AT&T Bell Laboratories
 *
 * return path to file a/b with access mode using : separated dirs
 * both a and b may be 0
 * if (mode&PATH_REG) then path must not be a directory
 * if (mode&PATH_ABS) then path must be rooted
 * path returned in path buffer
 */

#include <limits.h>
#include <sys/types.h>
#include <sys/stat.h>

#define PATH_RWX	007
#define	PATH_REG	010
#define PATH_ABS	020

#ifndef PATH_MAX
#define PATH_MAX	1024
#endif

static char*	pathcat();

extern int	access();
extern char*	getcwd();
extern char*	pathcanon();

char*
pathaccess(path, dirs, a, b, mode)
register char*	path;
register char*	dirs;
char*		a;
char*		b;
register int	mode;
{
	int		sep = ':';
	char		cwd[PATH_MAX];
	struct stat	st;

	do
	{
		dirs = pathcat(path, dirs, sep, a, b);
		(void)pathcanon(path);
		if (!access(path, mode & PATH_RWX))
		{
			if ((mode & PATH_REG) && (stat(path, &st) || (st.st_mode & S_IFMT) == S_IFDIR)) continue;
			if (*path == '/' || !(mode & PATH_ABS)) return(path);
			dirs = getcwd(cwd, sizeof(cwd));
			sep = 0;
		}
	} while (dirs);
	return(0);
}

/*
 * single dir support for pathaccess()
 */

static char*
pathcat(path, dirs, sep, a, b)
char*		path;
register char*	dirs;
int		sep;
char*		a;
register char*	b;
{
	register char*	s;

	s = path;
	while (*dirs && *dirs != sep) *s++ = *dirs++;
	if (s != path) *s++ = '/';
	if (a)
	{
		while (*s = *a++) s++;
		if (b) *s++ = '/';
	}
	else if (!b) b = ".";
	if (b) while (*s++ = *b++);
	return(*dirs ? ++dirs : 0);
}
