#include "inter.h"
#include "spitblks.h"
#include "spitio.h"

sysrw()
{
	register struct fcblk *fcb = WA (struct fcblk *);
	register struct ioblk *iob = fcb->iob;

	/* ensure the file is open */
	if (osopen (iob) != 0)
		return 0;
	
	/* can't rewind a pipe */
	if ((iob->flg & IO_PIP) || iob->fdn < 2)
		return 1;
	
	/* seek to the beginning */
	doset (iob, 0, 0);

	return 3;
}
