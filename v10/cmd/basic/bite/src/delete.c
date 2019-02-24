/* Copyright Bell Telephone Laboratories Whippany, N.J.

 *	***********************************
 *	***********************************
 *	********** DELETE LINES ***********
 *	*** R. B. Drake WH 8C-005 X4163 ***
 *	**** Fri Aug 24 17:14:56 1979 *****
 *	***********************************
 *	***********************************
 * This routine is
 * called by "bed" to delete lines either singly or in large batches
 *   on a single line delete, a copy of the line is saved in the array
 *   "temp" for possible restoration by the "undo" routine.

*/
/*   "@(#) delete.c:  V 1.1  12/21/80" */
#include "bas.h"
extern char temp[];
extern int start,finish;
extern int line;
delete()
{
	char *save;
	if(expr[0] == '\0')
	{
		fetch(local.thing.linno,&lbdptr);
		if(local.thing.linno != inst.thing.linno)
		{
			error(local.thing.linno,0);
			return(-1);
		}
	/* save line for possible undo */
	expand();
	sprintf(temp,"%d %s %s\n\0",inst.thing.linno,decode(inst.thing.opcode.lobyte),linbuf);
	line = inst.thing.linno;
	save=curptr;
	cover(lbdptr,curptr);
	lbdptr=curptr=save;
	return(0);
	}
	lnpars(); 
	if(finish == 0)
	{
		local.thing.linno = start;
		expr[0] = '\0';
		delete();
		return(0);
	}
	if(fetch(start,&lbdptr) == -1) return(-1);
	save = curptr;
	fetch(finish,&lbdptr);
	cover(lbdptr,save);
	return(0);
}
