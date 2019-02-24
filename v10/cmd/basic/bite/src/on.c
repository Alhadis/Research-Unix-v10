/* Copyright Bell Telephone Laboratories Whippany, N.J.

 *	***********************************
 *	***********************************
 *	********** ON GOTO/GOSUB **********
 *	*** R. B. Drake WH 8C-005 X4163 ***
 *	**** Sun Aug 26 11:02:16 1979 *****
 *	***********************************
 *	***********************************

*/
/*   "@(#) on.c:  V 1.1  12/21/80" */

/* routine to handle on goto */
#include "bas.h"
extern int stpflg;
double evalx();
char *prncpy();
extern char *expr,*eoexpr;
on()
{
	int i,j;
	char txtbuf[80];
	char c;
	if((j = evalx(expr)) <= 0)
	{
		error(inst.thing.linno,38);
		return(-1);
	}
	if(stpflg == 1)
		return(-1);
	if((c= *eoexpr++) > '\003' && c != '\016')
	{
		error(inst.thing.linno,8); /* expression syntax */
		return(-1);
	}
	for(i=0;i<j;i++)
	{
		eoexpr = prncpy(txtbuf,eoexpr);
		if(*eoexpr++ == '\0' && i != (j-1))
		{
			error(inst.thing.linno,38); /* not enough references*/
			return(-1);
		}
	}
	expr = txtbuf;
	if(c != '\016')
	{
	if(__goto() < 0)
		return(-1);
	return(0);
	}
	if(gosub() < 0)
		return(-1);
	return(0);
}
