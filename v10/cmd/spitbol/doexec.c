/*
	doexec does an "execle" function call to invoke the shell
	on the command string in the passed scblk.
*/

#include "spitblks.h"

doexec( scbptr )

struct	scblk	*scbptr;

{
	int	l,savchr;
	char	*cp;
	extern	char	**environ;

	l	= scbptr -> len;
 	cp	= scbptr->str;

	savchr	= cp[l];
	cp[l]	= 0;
	execle( "/bin/sh","sh","-c",cp,0,environ );
	cp[l]	= savchr;
}
