/* Copyright Bell Telephone Labotatories Whippany, N.J.

 *	*******************************************
 *	*******************************************
 *	*** FETCH LINES FROM THE PROGRAM BUFFER ***
 *	******* R. B. Drake WH 8C-005 X4163 *******
 *	******** Fri Aug 24 17:25:20 1979 *********
 *	*******************************************
 *	*******************************************

*/
/*   "@(#) fetch.c:  V 1.1  12/21/80" */

/*
   This routine fetches lines from the program txtbuf as described
   below:
 *
 *
 * calling sequence: fetch([0 -1 n],&ptr)
 * result: if arg1 is 0, fetches the first line of the program
 *         if arg1 is -1, fetches the next sequential line
 *         if arg1 is n, fetches line number n
 *         ptr is the adress of a character pointer to be used by fetch
 *         a zero is returned if the operation was successful
 *         a minus 1 is returned if the operation failed
 *         a minus 2 is returned if a line was found but not line n
 *         data is returned as follows:
 *         inst.thing.linno contains the line number
 *          inst.opcode.lobyte contains the operation code
 *         expr points to the first character of the expression string
 *
*/
#include "bas.h"
char *expr;
extern char *curptr;
fetch(i,ptr)
int i;
char **ptr;
{
	int j;
	/* if line n is requested, search the page index for the
	 * page which contains the requested line. Then search
	 * that page sequentially until the line is found
	*/
	curptr = *ptr;
	if(i>0)
		{
		if(!index[0].maxlin)
		{
			curptr = *ptr = txtbuf;
			return(-1);
		} /*empty txtbuf */
		for(j=0;j<32;j++)
		{
			if((i>index[j].maxlin) &&  (index[j+1].maxlin != 0))
				continue;
			*ptr = index[j].begin;
			inst.thing.linno=0;
			while((i>inst.thing.linno)&&(fetch(-1,ptr) == 0));
			if(i== inst.thing.linno) return(0);
			return(-2);	/* -2 used to indicate that
					 * a line was found but it
					 * is not the one asked for */
		}
		}
	/* if the first line of the program is requested
	 * set the pointer to the beginnig of the txtbuf
	 * retrieve the line found there and leave the
	 * pointer at the beginning of the next line
	*/
	if(i==0)
		curptr= *ptr=txtbuf;
	/* otherwise use ptr to retrieve the next line */
	if(**ptr == '\0' && *(*ptr+1) == '\0' || *ptr >= linptr)
		return(-1);
	for(j=0;j<4;j++)
		inst.byte[j] = *(*ptr)++;
	expr = *ptr;
	while(*(*ptr)++);
	return(0);
}
