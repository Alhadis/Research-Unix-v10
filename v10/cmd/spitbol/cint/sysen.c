/*
/   sysen - endfile
*/

#include "osint.h"
#include "spitblks.h"
#include "spitio.h"

sysen()
{
	register struct fcblk *fcb = WA (struct fcblk *);
	register struct ioblk *iob = fcb->iob;

	/* ensure the file is open */
	if ( osopen( iob ) != 0 )
		return EXIT_1;

	/* copy fcb chain head */
	/* rzfcb = r$fcb; */

	/* now close it */
	osclose( iob );

	return NORMAL_RETURN;
}


