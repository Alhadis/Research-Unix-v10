/*
/   sysef - eject file
*/

#include "osint.h"
#include "spitblks.h"
#include "spitio.h"

struct scblk	ffscblk =
{
    0,		/*  type word - ignore		*/
    1,		/*  string length		*/
    '\f'	/*  string is a form-feed	*/
};

sysef()
{
	register struct fcblk *fcb = WA (struct fcblk *);
	register struct ioblk *iob = fcb->iob;

	/* ensure the file is open */
	if ( osopen( iob ) != 0 )
		return EXIT_1;

	/* write the data, fail if unsuccessful */
	if ( oswrite( fcb->rsz, ffscblk.len, iob, &ffscblk) != 0 )
		return EXIT_2;

	return NORMAL_RETURN;
}
