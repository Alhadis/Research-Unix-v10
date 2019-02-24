/* Copyright Bell Telephone Laboratories Whippany, N.J.

 *	***********************************
 *	***********************************
 *	******* LINE NUMBER PARSER ********
 *	*** R. B. Drake WH 8C-005 X4163 ***
 *	**** Sun Aug 26 10:46:06 1979 *****
 *	***********************************
 *	***********************************

*/
/*   "@(#) lnpars.c:  V 1.1  12/21/80" */

/*mini parser to handle expressions of the form
 * "line number" - "line number"
*/
#include "bas.h"
extern int start,finish;
lnpars()
{
	char as[2][10];
	int i,j,k;
	char c;
	/* zero the ascii txtbuf */
	for(j=0;j<2;j++)
		for(k=0;k<10;k++)
			as[j][k] = '\0';
	/* separate the line into two parts */

	for(i=0,j=0,k=0;expr[i] != 0; i++,k++)
	{
		if((c = expr[i])== '-' || c==',' || c== ' ')
		{
			j++;
			k= -1;
			continue;
		}
		if(j >1 || k > 9)
		{
			error(inst.thing.linno,0);
			return(-1);
		}
		as[j][k]=expr[i];
	}
	/* convert ascii strings to int */
	start=atoi(&as[0][0]);
	finish=atoi(&as[1][0]);
	if(j !=0 && finish == 0)
		finish=32767;
	return 0;
}


