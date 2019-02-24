/*
 *	sysax - after execution cleanup.  Here we just indicate that
 *	further output should go to the diagnostic file.
 */

#include "osint.h"

extern char	*outptr;	/*  compiler output filename		*/

sysax()
{
	swcoup( outptr );
	return NORMAL_RETURN;
}

