#include "inter.h"
#include "spitblks.h"
#include "spitio.h"

sysou()
{
	register struct fcblk *fcb = WA (struct fcblk *);
	register struct scblk *scb = XR (struct scblk *);

	/* ensure iob is open, fail if unsuccessful */
	if (osopen (fcb->iob) != 0)
		return 0;
	
	/* write the data, fail if unsuccessful */
	if (oswrite (fcb->rsz, scb->len, fcb->iob, scb) != 0)
		return 1;

	/* normal return */
	return 2;
}
