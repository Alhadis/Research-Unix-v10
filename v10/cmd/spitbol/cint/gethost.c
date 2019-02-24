

/*
	gethost( scptr ) reads the host file and returns its first
	line in the scblk.
*/

#include "spitblks.h"

gethost( scptr,maxlen )

struct	scblk	*scptr;
int	maxlen;

{
	int	cnt = 0, fd, j;

	if ( (fd = open( "/usr/lib/spithost",0 )) >= 0 ) {
		cnt	= read( fd,scptr -> str,maxlen );
		close( fd );
	}

        if (( cnt > 0 ) && ( scptr -> str[cnt - 1] == '\n' ))
		--cnt;
	scptr -> len = cnt;

}

