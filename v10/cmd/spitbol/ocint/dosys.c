/*
	dosys does a "system" function call with the string in
	in the passed scblk.
*/

#include "spitblks.h"

dosys( scbptr )

struct	scblk	*scbptr;

{
	int	l,savchr;
	char	*cp;

	l	= scbptr -> len;
 	cp	= scbptr->str;

	savchr	= cp[l];
	cp[l]	= 0;
	system( cp );
	cp[l]	= savchr;
}
