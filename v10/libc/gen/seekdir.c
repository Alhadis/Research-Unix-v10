/* Copyright (c) 1982 Regents of the University of California
 * and modified by pjw in 1987 */

#include "ndir.h"

/*
 * seek to an entry in a directory.
 *
 * offset `told' is interpreted in a non-obvious way:
 * telldir computes it as  dd_offset*DIRBLKSIZ + dd_loc
 * dd_loc is the byte offset into the current dd_buf contents;
 * dd_offset is the number of times dirread(fd, buf, DIRBLKSIZ)
 * must be called to reproduce the same buffer.
 *
 * Why not just use lseek?  Because told isn't a seek offset,
 * and cannot conveniently be turned into one: the byte count
 * in dd_loc is unrelated to the seek offset required to reach
 * lseek(fd, 0L, 1)+dd_loc, because the seek offset refers to the raw
 * directory, but dirread has reformatted the data.
 * One might let dd_offset be the seek offset to the the beginning
 * of dd_buf, compute told the same way, and let seekdir become
 *	lseek(fd, told/DIRBLKSIZ, 0); dirp->dd_loc = told%DIRBLKSIZ;
 * but a somewhat strident prior implementor worries that this unfairly
 * constrains the size of directories.  This is probably a silly worry
 * (will any real system have directories a megabyte long?) but it
 * doesn't matter much except in programs that use seekdir a lot.
 */
void
seekdir(dirp, told)
register DIR *dirp;
long told;
{	long offset;

	offset = told / DIRBLKSIZ;
	dirp->dd_loc = told % DIRBLKSIZ;
	if (offset < dirp->dd_offset)
		lseek(dirp->dd_fd, dirp->dd_offset = 0L, 0);
	if(offset > dirp->dd_offset) {
		do {
			dirp->dd_size = dirread(dirp->dd_fd, dirp->dd_buf,
						DIRBLKSIZ);
			if(dirp->dd_size < 0)
				dirp->dd_size = pdirread(dirp->dd_fd,
					dirp->dd_buf, DIRBLKSIZ);
			}
			while(++dirp->dd_offset < offset);
		}
}
