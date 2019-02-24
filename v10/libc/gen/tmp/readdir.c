/* Copyright (c) 1982 Regents of the University of California */
/* and modified by pjw in 1986 and 1987, dmg in 1989 */

#include <sys/types.h>	/* only for ino_t */
#include "ndir.h"
#include <ctype.h>

/*
 * get next entry in a directory.
 *
 * subtlety:
 * ino_t is still a 16-bit type,
 * but i-numbers returned by dirread or pdirread may be larger.
 * stat returns an ino_t in st_ino.
 * d_ino is a long.
 * if d_ino has 32 significant bits, programs like pwd may fail.
 * if it is truncated to 16, information is lost.
 * it would be helpful to fix stat, but it wouldn't be a magic cure;
 * the i-number from dirread may have more than 32 bits!
 * truncate for now, via dir.d_ino = (ino_t) ...
 *
 * most of the tedious code below is error checking
 */
struct direct *
readdir(dirp)
register DIR *dirp;
{
	static struct direct dir;
	register char *p;
	register int i;

loop:
	if (dirp->dd_size <= 0 || dirp->dd_loc >= dirp->dd_size) {
		dirp->dd_offset++;	/* see seekdir.c */
		dirp->dd_size = dirread(dirp->dd_fd, dirp->dd_buf, DIRBLKSIZ-1);
		if(dirp->dd_size < 0)
			dirp->dd_size = pdirread(dirp->dd_fd, dirp->dd_buf,
				DIRBLKSIZ-1);
		if(dirp->dd_size <= 0)
			return(0);
		dirp->dd_loc = 0;
		dirp->dd_buf[dirp->dd_size] = 0;	/* sentinel */
	}
	p = dirp->dd_buf + dirp->dd_loc;
	dir.d_ino = 0;
	while (isdigit(*p))
		dir.d_ino = dir.d_ino*10 + *p++ - '0';
	dir.d_ino = (ino_t)dir.d_ino;
	while (*p != '\t')	/* in case we add fields someday */
		if (*p++ == 0) {	/* badly formed */
			dirp->dd_loc = p - dirp->dd_buf;
			goto loop;
		}
	p++;
	for (i = 0; i < MAXNAMLEN; i++)
		if ((dir.d_name[i] = *p++) == 0)
			break;
	if (i >= MAXNAMLEN && *p++ != 0) {	/* name too long */
		while (*p++)
			;
		dirp->dd_loc = p - dirp->dd_buf;
		goto loop;
	}
	dir.d_name[i] = 0;
	dir.d_namlen = i;
	dir.d_reclen = sizeof(dir);	/* bogus, but who cares? */
	dirp->dd_loc = p - dirp->dd_buf;
	if (dirp->dd_loc > dirp->dd_size)	/* hit the sentinel */
		goto loop;
	return (&dir);
}
