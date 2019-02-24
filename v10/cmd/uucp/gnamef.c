/*	/sccs/src/cmd/uucp/s.gnamef.c
	gnamef.c	1.1	8/30/84 17:37:25
*/
#include "uucp.h"
#ifndef READDIR
#include <sys/dir.h>
#endif
VERSION(@(#)gnamef.c	1.1);

/*
 * get next file name from directory
 *	p	 -> file description of directory file to read
 *	filename -> address of buffer to return filename in
 *		    must be of size DIRSIZ+1
 * returns:
 *	FALSE	-> end of directory read
 *	TRUE	-> returned name
 */
gnamef(p, filename)
register char *filename;
DIR *p;
{
	struct direct dentry;
	register struct direct *dp = &dentry;

	while (1) {
#ifdef	READDIR
		if ((dp = readdir(p)) == NULL)
#else
		if (fread((char *)dp,  sizeof(dentry), 1, p) != 1)
#endif
			return(FALSE);
		if (dp->d_ino != 0 && dp->d_name[0] != '.')
			break;
	}

	(void) strncpy(filename, dp->d_name, MAXBASENAME);
	filename[MAXBASENAME] = '\0';
	return(TRUE);
}

/*
 * get next directory name from directory
 *	p	 -> file description of directory file to read
 *	filename -> address of buffer to return filename in
 *		    must be of size DIRSIZ+1
 * returns:
 *	FALSE	-> end of directory read
 *	TRUE	-> returned dir
 */
gdirf(p, filename, dir)
register char *filename;
DIR *p;
char *dir;
{
	char statname[MAXNAMESIZE];

	while (1) {
		if(gnamef(p, filename) == FALSE)
			return(FALSE);
		(void) sprintf(statname, "%s/%s", dir, filename);
		DEBUG(4, "stat %s\n", statname);
		if (DIRECTORY(statname))
		    break;
	}

	return(TRUE);
}
