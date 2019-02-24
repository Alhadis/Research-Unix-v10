/* Copyright Bell Telephone Laboratories Whippany, N.J.

 *	***********************************
 *	***********************************
 *	************** LOAD ***************
 *	*** R. B. Drake WH 8C-005 X4163 ***
 *	**** Sun Aug 26 10:48:06 1979 *****
 *	***********************************
 *	***********************************
 * This routine is used to load "BITE" programs form a UNIX file
 * the filenames must be suffixed ".b" which is appended automatically
 * by the "save" routine if the user leaves it off

*/
/*   "@(#) load.c:  V 1.1  12/21/80" */

#include "bas.h"


char filnam[40];
load()
{
FILE *fp,*pathopen();
	if(expr[0] == '\0' && filnam[0] == '\0')
		getfnam();
	if(expr[0] != 0)
	{
		if(strlen(expr) > 40)
		{
			error(inst.thing.linno,4);
			return(-1);
		}
		strcpy(filnam,expr);

	}
	postb(filnam);	/* attach .b postfix if not there */
	if((fp = pathopen(filnam)) == 0)
		return(-1);

	while(bed(fp) == 0);
	fclose(fp);
	fetch(0,&lbdptr); /* set txtbuf to first line */
	return(0);
}
