/*
/   sysrw - rewind file
*/

#include "osint.h"
#include "spitblks.h"
#include "spitio.h"

sysrw()
{
	register struct fcblk *fcb = WA (struct fcblk *);
	register struct ioblk *iob = fcb->iob;

	/* ensure the file is open */
	if ( osopen( iob ) != 0)
		return EXIT_1;

        /* can't rewind a pipe */
	if ( (iob->flg & IO_PIP)  ||  iob->fdn < 2 )
		return EXIT_2;

	/* seek to the beginning */
	doset( iob, 0, 0 );

	return NORMAL_RETURN;
}

