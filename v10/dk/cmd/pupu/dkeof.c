
	static char	SCCSID[] = "@(#)dkeof.c	2.1 DKHOST 85/01/15";

#include	<dk.h>


dkeof(fd)
{
	char	nothing[1];


	write(fd, nothing, 0);
}
