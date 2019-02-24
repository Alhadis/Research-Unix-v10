/*
/   sysou - outpue a record
*/

#include "osint.h"
#include "spitblks.h"
#include "spitio.h"

sysou()

{
	register struct fcblk *fcb = WA (struct fcblk *);
	register struct scblk *scb = XR (struct scblk *);

	/* ensure iob is open, fail if unsuccessful */
	if ( osopen( fcb->iob ) != 0 )
		return EXIT_1;

	/* write the data, fail if unsuccessful */
	if ( oswrite( fcb->rsz, scb->len, fcb->iob, scb) != 0 )
		return EXIT_2;

	/* normal return */
	return NORMAL_RETURN;
}

