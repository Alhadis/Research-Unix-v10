/*
 * read directories, research-style
 * uses dirread system call, which does just what we want
 */

#include <rf.h>
#include "zarf.h"

long lseek();

dodirread(f, buf, size, off, offp)
register Rfile *f;
char *buf;
int size;
long off;
long *offp;
{
	extern int errno;
	int n;

	lseek(fsp(f)->fd, off, 0);
	if ((n = dirread(fsp(f)->fd, buf, size)) < 0)
		fserrno = errno;
	else
		*offp = lseek(fsp(f)->fd, 0L, 1);
	return (n);
}

dodircleanup(f)
Rfile *f;
{
	/* nothing to do */
}
