/*
 *	sysep - eject standard output
 */

#include "osint.h"

sysep()
{
        static char ff = '\f';
	write( 1, &ff, 1 );
	return NORMAL_RETURN;
}

