/*
 *	sysep - eject standard output
 */

#include "inter.h"

sysep()
{
	static char ff = '\f';
	write (1, &ff, 1);
	return 0;
}
