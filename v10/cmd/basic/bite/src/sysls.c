/* Copyright Bell Telephone Laboratories Whippany, N.J.

 *	/////////////////////////////////////
 *	/////////////////////////////////////
 *	////////////// sysls.c //////////////
 *	/// J. P. Hawkins WH X4610 8C-001 ///
 *	///// Wed May  6 17:25:53 1981 //////
 *	/////////////////////////////////////
 *	/////////////////////////////////////
 *	@(#) sysls.c: V1.1 6/17/81
 * ls command for restricted use

*/

#include	"bas.h"

sysls()
{
	char	cmdlin[80];

	cmdlin[0] = '\0';
	strcat(cmdlin,"ls ");
	strcat(cmdlin,expr);
	if(chksec(cmdlin) < 0)	/* check for security violating chars */
		return(-1);
	callsys(cmdlin);
	return(0);
}
