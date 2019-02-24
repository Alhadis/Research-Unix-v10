/* Copyright Bell Telephone Laboratories Whippany, N.J.

 *	***********************************
 *	***********************************
 *	****** UPDATE THE PAGE INDEX ******
 *	*** R. B. Drake WH 8C-005 X4163 ***
 *	**** Sun Aug 26 12:02:08 1979 *****
 *	***********************************
 *	***********************************

*/
/*   "@(#) update.c:  V 1.1  12/21/80" */

/* after a shift of the lines in the program txtbuf, that portion of
 * the page index from the current page upward must be rebuilt or
 * "updated" */
#include "bas.h"

update()
{
char *uptr;
	if(index[pg].maxlin == 0)
		uptr = txtbuf;
	else
	uptr = index[pg].begin;
	while(1)
	{
		pg = (uptr - txtbuf) /PGSIZ;
		if(fetch(-1,&uptr) != 0) return(0);
		if(index[pg].maxlin == 0)
		{
			index[pg].maxlin = inst.thing.linno;
			index[pg].begin = curptr;
		}
		if(inst.thing.linno > index[pg].maxlin)
			index[pg].maxlin = inst.thing.linno;
	}
}
