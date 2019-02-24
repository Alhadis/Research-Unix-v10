/*
 * restart/boot the machine on big VAXes
 * (not MicroVAX)
 */

#include "sys/cons.h"
#include "sys/mtpr.h"

static tocons();

/*
 * arrange for restart on next halt
 */

setrestart()
{
	tocons(TXDB_CWSI);	/* clear warm start inhibit */
	tocons(TXDB_CCSI);	/* clear cold start inhibit too */
}

/*
 * arrange for a boot
 */

setboot()
{
	tocons(TXDB_BOOT);	/* boot; might not happen until halt */
}

static
tocons(c)
{

	while ((mfpr(TXCS)&TXCS_RDY) == 0)
		continue;
	mtpr(TXDB, c);
}
