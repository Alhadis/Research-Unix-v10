/* Copyright Bell Telephone Laboratories Whippany, N.J.

 *	***********************************
 *	***********************************
 *	********** STOP AND END ***********
 *	*** R. B. Drake WH 8C-005 X4163 ***
 *	**** Sun Aug 26 11:37:50 1979 *****
 *	***********************************
 *	***********************************
 * In BITE, 'stop' and 'end' both stop the program execution however 
 * unlike many BASICS, the 'end' statement is optional. Simply hitting the
 * end of the program buffer will stop the program without error.

*/
/*   "@(#) stop.c:  V 1.1  12/21/80" */

#include "bas.h"
extern int stpflg;
stop()
{
	printf("STOP %d\n",inst.thing.linno);
	stpflg = 1;
	return(0);
}
__end()
{
	stpflg = 1;
	return(0);
}
