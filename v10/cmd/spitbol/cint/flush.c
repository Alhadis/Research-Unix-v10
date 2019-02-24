
/*
 *	flush( ioptr ) writes the characters in the buffer implied by ioptr.
 */

#include "spitblks.h"
#include "spitio.h"

int
flush( ioptr )

struct	ioblk	*ioptr;

{
	register struct bfblk	*bfptr;
	register int	n, ioerrcnt = 0;

	bfptr	= ioptr -> buf;
	if ( bfptr ) {
		n = bfptr -> off;
		if ( n > 0 ) {
			if ( write( ioptr -> fdn, bfptr -> buf, n ) != n )
				ioerrcnt++;
		}
		bfptr -> off = 0;
		bfptr -> rem = bfptr -> siz;
	}
	return ioerrcnt;
}

