/*
 *	sysbx - before execution.  Here we just indicate that
 *	further output should go to the diagnostic file.
 */

#include "inter.h"

sysbx()
{
	swcoup (outptr);
	return 0;
}
