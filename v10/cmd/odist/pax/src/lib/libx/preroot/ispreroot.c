/*
 * AT&T Bell Laboratories
 * return 1 if dir [any dir] is the preroot
 */

#include <preroot.h>

#if FS_PREROOT

#include <sys/types.h>
#include <sys/stat.h>

static int	same();

int
ispreroot(dir)
char*	dir;
{
	static int	prerooted = -1;

	if (dir) return(same("/", dir));
	if (prerooted < 0) prerooted = !same("/", PR_REAL);
	return(prerooted);
}

/*
 * return 1 if files a and b are the same under preroot
 *
 * NOTE: the kernel disables preroot for set-uid processes
 */

static int
same(a, b)
char*	a;
char*	b;
{
	int		i;
	int		euid;
	int		ruid;

	struct stat	ast;
	struct stat	bst;

	extern int	geteuid();
	extern int	getuid();
	extern int	setuid();
	extern int	stat();

	if ((ruid = getuid()) != (euid = geteuid())) (void)setuid(ruid);
	i = !stat(a, &ast) && !stat(b, &bst) && ast.st_dev == bst.st_dev && ast.st_ino == bst.st_ino;
	if (ruid != euid) (void)setuid(euid);
	return(i);
}

#else

int	_lib_dummy;

#endif
