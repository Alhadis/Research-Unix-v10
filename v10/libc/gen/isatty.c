/*
 * Returns 1 iff file is a tty
 */

#include <sys/ttyio.h>

isatty(f)
{
	extern errno;
	struct sgttyb ttyb;

	if (ioctl(f, TIOCGETP, &ttyb) < 0) {
		errno = 0;
		return(0);
	}
	return(1);
}
