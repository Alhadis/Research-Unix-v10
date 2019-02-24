/* Copyright Bell Telephone Laboratories Whippany, N.J.

 *	***********************************
 *	***********************************
 *	************** GOTO ***************
 *	*** R. B. Drake WH 8C-005 X4163 ***
 *	**** Fri Aug 24 17:33:53 1979 *****
 *	***********************************
 *	***********************************

*/
/*   "@(#) goto.c:  V 1.1  12/21/80" */

#include "bas.h"
__goto()
{
	int savno;
	extern int start;
	extern char *ptr,*curptr;
	savno = inst.thing.linno;
	start=atoi(expr);
	fetch(start,&ptr);
	if(inst.thing.linno != start)
	{
		error(savno,0);
		inst.thing.linno = savno;
		return(-1);
	}
	ptr = curptr;
	return(0);
}
