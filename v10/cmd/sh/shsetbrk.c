/*	@(#)setbrk.c	1.4	*/
/*
 *	UNIX shell
 *
 *	Bell Telephone Laboratories
 *
 */

#include	"defs.h"

char 	*sbrk();

char*
setbrk(incr)
{

	register char *a = sbrk(incr);

	brkend = a + incr;
#ifdef	CRAY
	brkend = endcp;
#endif
	return(a);
}
