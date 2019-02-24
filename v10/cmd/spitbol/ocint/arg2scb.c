
/*
	arg2scb( req,argc,argv,scptr ) returns the req-th argument pointed
	to by the argv array in the scblk provided. if req is out of range
	-1 is returned; otherwise the argument's length is returned.
*/

#include "spitblks.h"

int	arg2scb( req,argc,argv,scptr )

int	req;
int	argc;
char	*argv[];
struct	scblk	*scptr;

{
	int	ch, i = 0, maxlen;
	register char	*argcp, *scbcp;

	if ( (req < 0) || (req >= argc) )
		return	-1;

	argcp	= argv[req];
	maxlen	= scptr->len - 1;
	scbcp	= scptr->str;
	while ( i < maxlen ) {
		ch	= *argcp++;
		if (ch == 0)
			break;
		*scbcp++ = ch;
		i++;
	}
	scptr->len = i;
	return	i;
}
