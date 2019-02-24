#include "curses.ext"
/*	@(#) typeahead.c: 1.1 10/15/83	(1.19	3/17/83)	*/

/*
 * Set the file descriptor for typeahead checks to fd.  fd can be -1
 * to disable the checking.
 */
typeahead(fd)
int fd;
{
	SP->check_fd = fd;
}
