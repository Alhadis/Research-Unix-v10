
/*
	testty is a c function that determines whether or not
	a file descriptor represents a tty device.  testty
	returns 0 on success, -1 on failure.
*/
#include <sgtty.h>

int testty( fd )
int	fd;

{
	struct	sgttyb	buffer;

	return	ioctl( fd,TIOCGETP,&buffer );
}

