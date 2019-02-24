/* Copyright (c) 1982 Regents of the University of California */
/* and modified by pjw in 1986 */
#include <sys/types.h>
#include "ndir.h"

/*
 * read any style directory entry and present it as a bsd one
 */
/* classical unix */
#define	ODIRSIZ	14
struct	olddirect {
	ino_t	d_ino;
	char	d_name[ODIRSIZ];
};
/* current cray */
#define CDIRSIZ 24
struct craydirect {
	long x;
	long d_ino;
	char d_name[CDIRSIZ];
};

/*
 * get next entry in a directory.
 */
struct direct *
readdir(dirp)
	register DIR *dirp;
{
	register struct olddirect *dp;
	register struct craydirect *cp;
	register struct direct *np;
	static struct direct dir;

loop:
	if (dirp->dd_loc == 0) {
		dirp->dd_size = read(dirp->dd_fd, dirp->dd_buf, 
		    DIRBLKSIZ);
		if (dirp->dd_size <= 0)
			return NULL;
	}
	if (dirp->dd_loc >= dirp->dd_size) {
		dirp->dd_loc = 0;
		goto loop;
	}
	switch(dirp->dd_type) {
	case TCRAY:
		cp = (struct craydirect *) (dirp->dd_buf + dirp->dd_loc);
		dirp->dd_loc += sizeof(struct craydirect);
		if(cp->d_ino == 0)
			goto loop;
		strncpy(dir.d_name, cp->d_name, CDIRSIZ);
		dir.d_name[CDIRSIZ] = 0;
		dir.d_ino = (ino_t) dir_rev4(cp->d_ino);
		break;
	default:
	case TUNK:
		return(0);
	case TOLD:
		dp = (struct olddirect *)(dirp->dd_buf + dirp->dd_loc);
		dirp->dd_loc += sizeof(struct olddirect);
		if (dp->d_ino == 0)
			goto loop;
		dir.d_ino = (ino_t) dp->d_ino;
		strncpy(dir.d_name, dp->d_name, ODIRSIZ);
		dir.d_name[ODIRSIZ] = '\0'; /* insure null termination */
		break;
	case TSUN:
		np = (struct direct *)(dirp->dd_buf + dirp->dd_loc);
		dirp->dd_loc += dir_rev2(np->d_reclen);
		if(np->d_ino == 0)
			goto loop;
		dir.d_ino = (ino_t) dir_rev4(np->d_ino);
		strcpy(dir.d_name, np->d_name);
		break;
	case TBSD:
		np = (struct direct *)(dirp->dd_buf + dirp->dd_loc);
		dirp->dd_loc += np->d_reclen;
		if(np->d_ino == 0)
			goto loop;
		dir.d_ino = (ino_t) np->d_ino;
		strcpy(dir.d_name, np->d_name);
		break;
	}
	dir.d_namlen = strlen(dir.d_name);
	dir.d_reclen = DIRSIZ(&dir);
	return (&dir);
}

dir_rev2(n)
{	union {
		short x;
		unsigned char b[2];
	} u;
	u.b[1] = n & 0xff;
	u.b[0] = (n >> 8) & 0xff;
	return(u.x);
}

dir_rev4(n)
{	union {
		long x;
		unsigned char b[4];
	} u;
	u.b[3] = n & 0xff;
	u.b[2] = (n >> 8) & 0xff;
	u.b[1] = (n >> 16) & 0xff;
	u.b[0] = (n >> 24) & 0xff;
	return(u.x);
}
