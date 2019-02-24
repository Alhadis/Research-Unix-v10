
	static char	SCCSID[] = "@(#)dkeof.c	2.1 DKHOST 85/01/15";

#include	<dk.h>

	static short	endoffile[3] = {
		106, 0, 0
	};	/* End of File Level-D code */

dkeof(fd)
{
	char	nothing[1];

	ioctl(fd, DIOCXCTL, endoffile);

	write(fd, nothing, 0);
}
