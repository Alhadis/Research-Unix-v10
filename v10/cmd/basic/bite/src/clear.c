/* Copyright Bell Telephone Laboratories Whippany, N.J.

 *	***********************************
 *	***********************************
 *	****** CLEAR THE PAGE INDEX *******
 *	*** R. B. Drake WH 8C-005 X4163 ***
 *	**** Fri Aug 24 17:07:40 1979 *****
 *	***********************************
 *	***********************************
 * This routine is used
 * as part of the houskeeping required to keep the page index
 * updated, the page index must sometimes be cleared from the
 * current page up 

*/
/*   "@(#) clear.c:  V 1.1  12/21/80" */
#include "bas.h"
extern int pg;
clear()
{
int i;
	pg = (curptr - txtbuf ) /PGSIZ;
	for(i=pg;i<32;i++)
		index[i].maxlin = 0;
}
