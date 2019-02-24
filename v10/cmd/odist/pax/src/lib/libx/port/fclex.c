/*
 * set|clear file descriptor close-on-exec flag
 */

#include <errno.h>
#include <fcntl.h>
#ifndef F_SETFD
#include <sys/filio.h>	/* 9th edition */
#endif

extern int	errno;

int
fclex(fd, flag)
int	fd;
int	flag;
{
	flag &= 01;
#ifdef F_SETFD
	return(fcntl(fd, F_SETFD, flag));
#else
#ifdef FIOCLEX
	return(ioctl(fd, flag ? FIOCLEX : FIONCLEX, 0));
#else
	errno = EINVAL;
	return(-1);
#endif
#endif
}
