/* Copyright Bell Telephone Laboratories Whippany, N.J.

 *	***************************************************
 *	***************************************************
 *	*** EQUIVALENT TO THE UNIX EDITOR'S '-' COMMAND ***
 *	*********** R. B. Drake WH 8C-005 X4163 ***********
 *	************ Sun Aug 26 10:54:49 1979 *************
 *	***************************************************
 *	***************************************************
 * Find the beginning of the previous line and print it

*/
/*   "@(#) minus.c:  V 1.1  12/21/80" */

#include "bas.h"
minus()
{
	int n,c;
	if(expr[0] != '\0')
		n=atoi(expr);
	else
		n=1;
	while(curptr != txtbuf && n--)
	{
		--curptr;
		while((c=((*--curptr)&0377)) != 0 && c < 0200) ;
		curptr -= 2; /* decriment to beginning of line */
	}
	lbdptr=curptr;
	if(fetch(-1,&lbdptr) < 0)
	{
		error(inst.thing.linno,0);
		return(-1);
	}
	prin();
	return(0);
}
