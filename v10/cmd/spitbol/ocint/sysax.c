/*
 *	sysax - after execution cleanup.  Here we just indicate that
 *	further output should go to the diagnostic file.
 */

#include "inter.h"

sysax()
{
	swcoup (outptr);
	return 0;
}
