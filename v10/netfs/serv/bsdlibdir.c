/*
 * read a directory, with the directory library routines
 * bsd version: readdir header and structure have different names
 *
 * fill the buffer with directory entries of the form
 *	nnnn tab filename NUL
 * nnnn is the i-number, in decimal ascii
 *
 * return the number of bytes used in the buffer, or 0 (EOF) or -1 (error)
 * leave *offp set to something we'll want to use next time.
 */

#include <rf.h>
#include "zarf.h"
#include <sys/types.h>	/* for pointless u_long and friends */
#include <sys/dir.h>
#define	dirent	direct

/*
 * our private context
 */
#define	MAXDIRS 128
typedef struct {
	Rfile *f;
	DIR *dfile;
} Dirfile;
static Dirfile dirfiles[MAXDIRS];
static Dirfile *ftod();

dodirread(f, buf, size, off, offp)
Rfile *f;
char *buf;
register int size;
long off;
long *offp;
{
	extern int errno;
	static char tbuf[MAXNAMLEN+1+20];	/* room for NUL and some digits */
	register struct dirent *dp;
	register Dirfile *dirf;
	register int n;
	register char *p;

	dirf = ftod(f);
	if (dirf == NULL) {
		fserrno = RFEINVAL;
		return (-1);
	}
	if (dirf->f == NULL) {
		if ((dirf->dfile = opendir(fsp(f)->name)) == NULL) {
			fserrno = errno;	/* hope it's correct */
			return (-1);
		}
		dirf->f = f;
	}
	seekdir(dirf->dfile, off);
	p = buf;
	for (;;) {
		if ((dp = readdir(dirf->dfile)) == NULL)
			break;
		sprintf(tbuf, "%d\t%s", dp->d_ino, dp->d_name);
		n = strlen(tbuf) + 1;
		if (size < n)
			break;
		strcpy(p, tbuf);
		p += n;
		size -= n;
		off = telldir(dirf->dfile);
	}
	if (dp != NULL && p == buf) {	/* no room for anything */
		fserrno = RFENOSPC;
		return (-1);
	}
	*offp = off;
	return (p - buf);
}

static Dirfile *
ftod(f)
register Rfile *f;
{
	register Dirfile *dirf;
	register Dirfile *empty;

	empty = NULL;
	for (dirf = dirfiles; dirf < &dirfiles[MAXDIRS]; dirf++) {
		if (dirf->f == f)
			return (dirf);
		if (empty == NULL && dirf->f == NULL)
			empty = dirf;
	}
	return (empty);
}

dodircleanup(f)
Rfile *f;
{
	register Dirfile *dirf;

	if ((dirf = ftod(f)) == NULL || dirf->f == NULL)
		return;
	closedir(dirf->dfile);
	dirf->f = NULL;
}
