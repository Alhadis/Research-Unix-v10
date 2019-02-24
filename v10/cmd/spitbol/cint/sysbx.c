/*
 *	sysbx - before execution.  Here we just indicate that program output
 *	should go to standard output file.
 */

#include "osint.h"

extern char	*outptr;	/*  compiler output filename		*/

sysbx()
{
	swcoup( outptr );
	return NORMAL_RETURN;
}

