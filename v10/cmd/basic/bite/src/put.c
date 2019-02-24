/* Copyright Bell Telephone Laboratories Whippany, N.J.

 *	*********************************************
 *	*********************************************
 *	*** INSERT A LINE INTO THE PROGRAM BUFFER ***
 *	******** R. B. Drake WH 8C-005 X4163 ********
 *	********* Sun Aug 26 11:16:53 1979 **********
 *	*********************************************
 *	*********************************************

*/
/*   "@(#) put.c:  V 1.1  12/21/80" */

/* Called by "bed" */
/* in the case of line insertion or replacement, the task of writting
 * the new line into the program txtbuf is the same and can be handled
 * by a common routine "put" */
#include "bas.h"
put()
{
int i;
		for(i=0;i<4;i++)
			*curptr++=local.byte[i];
		expr = curptr;
		for(i=0;linbuf[i] !=0;i++)
			*curptr++=linbuf[i];
		*curptr++='\0';
}
