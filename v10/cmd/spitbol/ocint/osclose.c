
/*
	osclose( ioptr ) closes the file represented by the ioblk.
	osclose will flush an output file's buffer before closing.
*/

#include "spitblks.h"
#include "spitio.h"

int
osclose( ioptr )

struct	ioblk	*ioptr;

{
	register int	errcnt = 0;

	if ( !(ioptr -> flg & IO_OPN))
		return 0;

	if ( ioptr -> flg & IO_OUP )
		errcnt += flush( ioptr );

	if ( ioptr -> flg & IO_SYS )
		return errcnt;

	if ( close( ioptr -> fdn ) < 0)
		errcnt++;
	ioptr -> flg &= ~IO_OPN;

	if ( ioptr -> flg & IO_PIP ) {
		if ( ioptr -> flg & IO_DED )
			ioptr -> flg &= ~IO_DED;
		else if ( ioptr -> flg & IO_INP ) {
			kill( ioptr -> pid );
			oswait( ioptr -> pid);
		} else oswait( ioptr -> pid );
	}

	return errcnt;
}

