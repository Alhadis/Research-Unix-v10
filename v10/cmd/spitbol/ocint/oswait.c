/*
	oswait( pid ) waits for the termination of the process with
	id pid.
*/

#include "spitblks.h"
#include "spitio.h"
#include <signal.h>

oswait( pid )

int	pid;

{
	int	deadpid, status;
	register (*hstat)(), (*istat)(), (*qstat)();
	extern	struct	chfcb	*rzfcb;
	struct	chfcb	*chptr;

	istat	= signal( SIGINT,SIG_IGN );
	qstat	= signal( SIGQUIT,SIG_IGN );
	hstat	= signal( SIGHUP,SIG_IGN );
	
	while ( (deadpid = wait( &status )) != pid  && 
		(deadpid != -1) ) {
		for ( chptr = rzfcb; chptr != 0; chptr = chptr -> nxt ) {
			if ( deadpid == chptr -> fcb -> iob -> pid ) {
				chptr -> fcb -> iob -> flg |= IO_DED;
				break;
			}
		}
	}

	signal( SIGINT,istat );
	signal( SIGQUIT,qstat );
	signal( SIGHUP,hstat );

	return status;
}
