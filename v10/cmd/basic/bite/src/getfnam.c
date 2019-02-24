/* Copyright Bell Telephone Laboratories Whippany, N.J.

 *	***********************************
 *	***********************************
 *	********* GET A FILE NAME *********
 *	*** R. B. Drake WH 8C-005 X4163 ***
 *	**** Fri Aug 24 17:28:41 1979 *****
 *	***********************************
 *	***********************************
 * When the user types a command which needs a filename such as
 * "old" or "save" or "load", and no default filename currently
 * exists, this routine is called to prompt the user for one

*/
/*   "@(#) getfnam.c:  V 1.1  12/21/80" */

#include "bas.h"
extern char filnam[];
getfnam()
{
	int i;
	while(expr[0] == '\0' && filnam[0] == '\0')
	{
		printf("Filename ?");
		fgets(filnam,40,stdin);
		i = strlen(filnam);
		postb(filnam);
		if(i>=40)
		{
			printf("NAME TOO LONG, TRY AGAIN\n");
			filnam[0]='\0';
			continue;
		}
		break;
	}
	return(0);
}
