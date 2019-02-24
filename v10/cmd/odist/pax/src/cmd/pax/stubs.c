/*
 * G. S. Fowler
 * AT&T Bell Laboratories
 *
 * pax libdelta stubs for external distribution
 */

#include "pax.h"

/*ARGSUSED*/
int
delta(old, oldsize, new, newsize, fd)
char*	old;
long	oldsize;
char*	new;
long	newsize;
int	fd;
{
	return(-1);
}

/*ARGSUSED*/
int
update(oldfd, oldoffset, dfd, wfd, rfd, newoffset)
int	oldfd;
long	oldoffset;
int	dfd;
int	wfd;
int	rfd;
long	newoffset;
{
	return(-1);
}
