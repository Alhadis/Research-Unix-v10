

/*
	doset( ioptr,offset,whence ) does an lssek on the file described
	by ioptr with offset and whence.  for output files, the buffer
	must be flushed before doing the lseek.  for input files, any
	"unread" characters in the buffer must be lseeked back.
*/

#include "spitblks.h"
#include "spitio.h"

doset( ioptr,offset,whence )

struct	ioblk	*ioptr;
int	offset;
int	whence;

{
	struct	bfblk	*bfptr;

	if ( ioptr -> flg & IO_OUP ) {
		flush( ioptr );
	} else {
		ioptr -> flg &= ~IO_EOF;
		bfptr	= ioptr -> buf;
		if ( (bfptr -> rem != 0) && (whence == 1) )
			lseek( ioptr -> fdn,-(bfptr -> rem),1 );
		bfptr -> rem = 0;
		bfptr -> off = 0;
	}
	return lseek( ioptr -> fdn,offset,whence );
}


