#define _POSIX_SOURCE
/*
 *	ftwlk - file tree walk
 *
 *	int ftwlk (path, fn, depth)  char *path; int (*fn)(); int depth;
 *
 *	Given a path name, ftwlk starts from the file given by that path
 *	name and visits each file and directory in the tree beneath
 *	that file.  If a single file has multiple links within the
 *	structure, it will be visited once for each such link.
 *	For each object visited, fn is called with four arguments.
 *	The fourth can often be ignored; it is a pointer, say S,
 *	declared "struct FTWLK *S", discussed in more detail below.
 *	The first contains the path name of the object, the second
 *	contains a pointer to a stat buffer which will usually hold
 *	appropriate information for the object and the third contains
 *	an integer value giving additional information about the
 *	object, as follows:
 *
 *	FTWLK_F		The object is a file for which stat was
 *			successful.  It does not guarantee that the
 *			file can actually be read.
 *
 *	FTWLK_D		The object is a directory for which stat and
 *			open for read were both successful.  This is
 *			a preorder visit -- objects in the directory
 *			are yet to be visited.
 *
 *	FTWLK_DNR	The object is a directory for which stat
 *			succeeded, but which cannot be read.  Because
 *			the directory cannot be read, fn will not be
 *			called for any descendants of this directory.
 *
 *	FTWLK_DP	The object is a directory for which stat and
 *			open for read were both successful.  This is
 *			a postorder visit -- everything in the directory
 *			has already been visited.
 *
 *	FTWLK_NS	Lstat failed on the object.  If errno is EACCES,
 *			then the failure stems from lack of
 *			appropriate permission.  This indication will
 *			be given, for example, for each file in a directory
 *			with read but no execute permission.  Whenever
 *			stat fails, it is not possible to determine
 *			whether this object is a file or a directory.
 *			The stat buffer passed to fn will contain garbage.
 *
 *	FTWLK_SL	The object is a symbolic link.  Set S->quit
 *			(a component of the structure pointed to by
 *			the fourth parameter to fn) to FTWLK_FOLLOW to
 *			have the link followed and the object to which
 *			it points visited.
 *
 *	FTWLK_NSL	Lstat succeeded, but stat failed on the object.
 *			This is only possible when following a symbolic
 *			link.
 *
 *	Among the components of the structure to which the fourth
 *	parameter, S, to fn points is S->quit.  If the caller sets
 *	S->quit to FTWLK_SKR, then no more files in the current directory
 *	will be visited.  (The current directory is the one containing
 *	the object being visited.)  If the third parameter to fn is
 *	FTWLK_D and the caller sets S->quit to FTWLK_SKD, then this directory
 *	(the one named in the first parameter to fn) will be skipped.
 *
 *	Other components pointed to by the fourth parameter S are
 *	the current recursion level S->level (top level = 0) and
 *	the offset S->base in the pathname of the current object
 *	(the first parameter to fn) of the object's base name.
 *	By expanding the definition of struct FTWLK given below and
 *	including the files included below, one can arrange for
 *	S to point to a larger structure, components of which can
 *	be initialized (for example) on calls to fn with third
 *	parameter FTWLK_D.
 *
 *	If fn returns nonzero, ftwlk stops and returns the same value
 *	to its caller.  Ftw only initiates a nonzero return if malloc
 *	fails; in this case ftwlk sets errno to ENOMEM and returns -1.
 *
 *	The third argument to ftwlk does not limit the depth to which
 *	ftwlk will go.  Rather, it limits the depth to which ftwlk will
 *	go before it starts recycling file descriptors.  In general,
 *	it is necessary to use a file descriptor for each level of the
 *	tree, but they can be recycled for deep trees by saving the position,
 *	closing, re-opening, and seeking.  It is possible to start
 *	recycling file descriptors by sensing when we have run out, but
 *	in general this will not be terribly useful if fn expects to be
 *	able to open files.  We could also figure out how many file descriptors
 *	are available and guarantee a certain number to fn, but we would not
 *	know how many to guarantee, and we do not want to impose the extra
 *	overhead on a caller who knows how many are available without
 *	having to figure it out.
 *
 *	It is possible for ftwlk to die with a memory fault in the event
 *	of a file system so deeply nested that the stack overflows.
 */

#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include "ftwlk.h"
/*
 * Struct FTWLK (whose definition starts at the end of ftwlk.h) must
 * must include at least the integers quit, base, and level.
 */

#define FTWLK_PATHLEN0 1000
#define FTWLK_PATHINC 1000

#ifndef S_ISDIR
#define S_ISDIR(M) (((M) & S_IFMT) == S_IFDIR)
#ifdef S_IFLNK
#define S_ISLNK(M) (((M) & S_IFMT) == S_IFLNK)
#endif
#endif

#ifndef S_ISLNK
#define lstat stat
#endif

#ifndef ENOMEM
#include <errno.h>
#endif

	extern int errno;

/*
 *  Each generation of ftwlk1 (the real ftwlk) allocates one copy, R, of the
 *  following structure; it passes a pointer to this structure when it
 *  recursively invokes itself.  These structures are chained together,
 *  so that if it becomes necessary to recycle file descriptors, then
 *  the oldest descriptor (the one at the shallowest depth still open)
 *  can be recycled.
 */

	struct FTWLK_rec {
		struct FTWLK_rec *prev;
		long here;	/* seek to here when reopening at this level */
		DIR *fd;	/* file descriptor at this level */
		};

/*
 *  One instance, T, of the following structure is allocated by ftwlk; a
 *  pointer to it is passed to all generations of ftwlk1 (the real ftwlk).
 *  T could often be a global variable, but this way the parameter fn
 *  can invoke ftwlk for an independent tree walk.
 *  Component T->path points to storage for the object path-names;
 *  this storage may be relocated by realloc if T->path needs to be
 *  more than T->pathlast characters long.
 *  T->path[T->pathnext] is the next free character in the pathnames.
 *  T->depth = parameter depth to ftwlk.  T->lastout is the deepest level at
 *  which a file descriptor has been recycled.
 */

	struct FTWLK_top {
		int (*fn)();
		char *path;
		unsigned pathlast, pathnext;
		int lastout;
		int depth;
		};

static ftwlk_1_();

int
ftwlk (path, fn, depth)
	char *path;
	int (*fn)();
	int depth;
{
	struct FTWLK_top T;
	struct FTWLK_rec R;
	struct FTWLK S;
	int rc;
	char *malloc(), *strcpy();

	T.depth = depth;
	T.lastout = -1;
	T.fn = fn;
	S.quit = 0;
	S.level = -1;

	/* initialize S.base, T.pathnext... */
		{
		register char c, *p, *q;
		for (p = q = path; c = *p; p++) if (c == '/') q = p + 1;
		S.base = q - path;
		T.pathnext = p - path;
		}

	T.pathlast = T.pathnext + FTWLK_PATHLEN0;
	T.path = malloc(T.pathlast);
	if (!T.path) { errno = ENOMEM; return -1; }
	strcpy(T.path, path);
	rc = ftwlk_1_(&R, &T, 0, &S);
	free(T.path);
	return rc;
}

int
static
ftwlk_1_ (R, T, level, S1)
	register struct FTWLK_rec *R;
	register struct FTWLK_top *T;
	int level;
	struct FTWLK *S1;
{
	int rc, n;
	DIR *fd;
	struct dirent *dirp;
	char *component, *path;
	struct stat sb;
	struct FTWLK_rec mr;
	unsigned nextsave;
	struct FTWLK S;
	char *realloc();
	long lseek();

	mr.prev = R;
	path = T->path;
	S.level = level;
	S.quit = 0;
	S.base = S1->base;

	/* Try to get file status.  If unsuccessful, errno will say why. */
	if (lstat(path, &sb) < 0) {
		rc = (*T->fn) (path, &sb, FTWLK_NS, &S);
		S1->quit = S.quit;
		return rc;
		};

	/*
	 *	The stat succeeded, so we know the object exists.
	 *	If not a directory, call the user function and return.
	 */
#ifdef S_ISLNK
	if (S_ISLNK(sb.st_mode )) {
		rc = (*T->fn) (path, &sb, FTWLK_SL, &S);
		S1->quit = S.quit;
		if (rc || S.quit == FTWLK_SKR) return rc;
		if (S.quit != FTWLK_FOLLOW) return 0;
		S1->quit = S.quit = 0;
		if (stat(path, &sb) < 0) {
			rc = (*T->fn) (path, &sb, FTWLK_NSL, &S);
			S1->quit = S.quit;
			return rc;
			};
		}
#endif
		
	if (!S_ISDIR(sb.st_mode)) {
		rc = (*T->fn) (path, &sb, FTWLK_F, &S);
		S1->quit = S.quit;
		return rc;
		}

	/*
	 *	The object was a directory.
	 *
	 *	Open a file to read the directory
	 */
	mr.fd = fd = opendir(path);

	/*
	 *	Call the user function, telling it whether
	 *	the directory can be read.  If it can't be read
	 *	call the user function or indicate an error,
	 *	depending on the reason it couldn't be read.
	 */
	if (!fd) {
		rc = (*T->fn) (path, &sb, FTWLK_DNR, &S);
		S1->quit = S.quit;
		return rc;
		}

	/* We could read the directory.  Call user function. */
	rc = (*T->fn) (path, &sb, FTWLK_D, &S);
	if (rc != 0)
		goto rtrn;
	if (S.quit == FTWLK_SKD) goto rtrn;
	if (S.quit == FTWLK_SKR) {S1->quit = FTWLK_SKR; goto rtrn;}

	/* Make sure path is big enough to hold generated pathnames. */

	n = nextsave = T->pathnext;
	if (n + MAXNAMLEN + 1 >= T->pathlast) {
		T->pathlast += FTWLK_PATHINC;
		path = T->path = realloc(T->path, T->pathlast);
		if (!path) {
			errno = ENOMEM;
			rc = -1;
			goto rtrn;
			}
		}
	
	/* Create a prefix to which we will append component names */

	if (n > 0 && path[n-1] != '/') path[n++] = '/';
	component = path + n;

	/*
	 *	Read the directory one component at a time.
	 *	We must ignore "." and "..", but other than that,
	 *	just create a path name and call self to check it out.
	 */
	while (dirp = readdir(fd)) {
		if (dirp->d_ino != 0
		    && strcmp (dirp->d_name, ".") != 0
		    && strcmp (dirp->d_name, "..") != 0) {
			int i;
			struct FTWLK_rec *pr;

			/* Append the component name to the working path */
			strcpy(component, dirp->d_name);
			T->pathnext = n + strlen(dirp->d_name);

			/*
			 *	If we are about to exceed our depth,
			 *	remember where we are and close the file.
			 */
			if (level - T->lastout >= T->depth) {
				pr = &mr;
				i = T->lastout++;
				while (++i < level) pr = pr->prev;
				pr->here = telldir(pr->fd);
				closedir(pr->fd);
			}

			/*
			 *	Do a recursive call to process the file.
			 */
			S.quit = 0;
			S.base = n;
			rc = ftwlk_1_(&mr, T, level+1, &S);
			if (rc != 0 || S.quit == FTWLK_SKR) {
				if (level > T->lastout) closedir(fd);
				T->pathnext = nextsave;
				return rc;
			}

			/*
			 *	If we closed the file, try to reopen it.
			 */
			if (level <= T->lastout) {
				char c = path[nextsave];
				path[nextsave] = 0;
				T->lastout = level - 1;
				mr.fd = fd = opendir(path);
				if (!fd) {
					rc = (*T->fn) (path, &sb, FTWLK_DNR, &S);
					S1->quit = S.quit;
					T->pathnext = nextsave;
					return rc;
					}
				path[nextsave] = c;
				seekdir(fd, mr.here);
				}
			}
		}
	T->pathnext = nextsave;
	path[nextsave] = 0;

	/*
	 *	We got out of the subdirectory loop.  Call the user
	 *	function again at the end and clean up.
	 */

	rc = (*T->fn) (path, &sb, FTWLK_DP, &S);
	S1->quit = S.quit;
rtrn:
	closedir(fd);
	return rc;
}
