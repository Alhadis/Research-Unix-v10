/*
 *	osopen( ioptr ) ensures that the file represented by the ioblk
 *	is open.
 */

#include "spitblks.h"
#include "spitio.h"

int	osopen( ioptr )

struct	ioblk	*ioptr;

{
	int	fd = -1, len, savchr;
	char	*cp;
	struct	scblk	*scptr;

	if ( ioptr -> flg & IO_OPN )
		return 0;

	scptr	= ioptr -> fnm;
	cp	= scptr -> str;

	len	= lenfnm( scptr );
	if ( cp[0] == '!' ) {
		ioptr -> flg |= IO_PIP;
	} else {
		savchr	= cp[len];
		cp[len]	= 0;
	}

	if ( ioptr -> flg & IO_PIP ) {
		fd	= ospipe( ioptr );
	} else if ( ioptr -> flg & IO_INP ) {
		fd	= open( cp,0 );
	} else {
		if ( ioptr -> flg & IO_APP ) {
			fd	= open( cp,1 );
			if ( fd != 0 )
				lseek( fd,0,2 );
		}
		if ( fd < 0 )
			fd	= creat( cp,0666 );
	}

	if ( !(ioptr -> flg & IO_PIP) )
		cp[len]	= savchr;

	if (fd > 0) {
		ioptr -> fdn = fd;
		ioptr -> flg |= IO_OPN;
		if ( (ioptr -> flg & IO_INP) == 0 && testty( fd ) == 0 )
			ioptr -> flg |= IO_WRC;
		if ( ioptr -> flg & IO_WRC )
			ioptr -> buf = 0;
		return 0;
	} else {
		return -1;
	}

}
