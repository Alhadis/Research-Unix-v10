

/*
	ospipe( ioptr ) builds a pipe for the input association.
	the value returned is the file descriptor.
*/

#include "spitblks.h"
#include "spitio.h"

ospipe( ioptr )

struct	ioblk	*ioptr;

{
	int	childfd, childpid, fd[2], len, parentfd, status;
	char	*cp;
	struct	scblk	*scptr;

	if ( (status = pipe( fd )) < 0 )
		return -1;

	if ( ioptr -> flg & IO_INP ) {
		parentfd = fd[0];
		childfd  = fd[1];
	} else {
		parentfd = fd[1];
		childfd  = fd[0];
	}

	switch ( childpid = fork() ) {

		case -1:
			close( parentfd );
			close( childfd );
			parentfd = -1;
			break;

		case 0:  
			if ( ioptr -> flg & IO_INP ) 
				close( 1 );
			else
				close( 0 );
			dup( childfd );
			for ( childfd = 3; childfd <= 32; close( childfd++ ));
			scptr = ioptr -> fnm;
			cp    = scptr -> str;
			len   = lenfnm( scptr );
			cp[len] = '\0';
			if ( cp[len - 1] == cp[1] )
				cp[len - 1] = ' ';
			cp[1] = ' ';
			cp[0] = ' ';
			execl( "/bin/sh","sh","-c",cp,0 );
			break;

		default:
			ioptr -> pid = childpid;
			close( childfd );
			break;
	}

	return parentfd;

}	

