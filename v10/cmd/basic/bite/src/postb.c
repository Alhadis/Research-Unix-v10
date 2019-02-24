/* Copyright Bell Telephone Laboratories Whippany, N.J.

 *	//////////////////////////////////////////////////////////
 *	//////////////////////////////////////////////////////////
 *	/// ATTACH A .b POSTFIX ON STRING IF NOT ALREADY THERE ///
 *	///////////// J. P. Hawkins WH X4610 8C-001 //////////////
 *	//////////////// Tue Sep 11 14:55:49 1979 ////////////////
 *	//////////////////////////////////////////////////////////
 *	//////////////////////////////////////////////////////////

*/
/*   "@(#) postb.c:  V 1.1  12/21/80" */
#include	"bas.h"

postb(name)
char	name[];
{
	register int	i;
	i = 0;

	for(i=0; name[i] != '\0' && name[i] != '\n'; i++);
	name[i] = '\0';		/* insure terminating character is a null */
	if(name[i-2] != '.' || name[i-1] != 'b')
		strcat(name,".b");
	return(0);
}
