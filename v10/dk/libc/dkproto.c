/*
 * Make sure that URP protocol is enabled on a datakit file.
 */
#include "sys/ioctl.h"

dkproto(file, linedis)
{
	if (ioctl(file, KIOCISURP, (char *)0) < 0)
		return(ioctl(file, FIOPUSHLD, &linedis));
	ioctl(file, KIOCINIT, (char *)0);
	return(0);
}
