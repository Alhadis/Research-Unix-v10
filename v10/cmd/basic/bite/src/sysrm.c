
/* Copyright Bell Telephone Laboratories Whippany, N.J.

 *	/////////////////////////////////////
 *	/////////////////////////////////////
 *	////////////// sysrm.c //////////////
 *	/// J. P. Hawkins WH X4610 8C-001 ///
 *	///// Wed May  6 17:25:53 1981 //////
 *	/////////////////////////////////////
 *	/////////////////////////////////////
 *	@(#) sysrm.c: V1.1 6/17/81
 * rm command for restricted use

*/

#include	"bas.h"

sysrm()
{
	char	cmdlin[80];

	cmdlin[0] = '\0';
	strcat(cmdlin,"rm ");
	strcat(cmdlin,expr);
	if(chksec(cmdlin) < 0)	/* check for security violating chars */
		return(-1);
	callsys(cmdlin);
	return(0);
}
