/* Copyright Bell Telephone Laboratories Whippany, N.J.

 *	/////////////////////////////////////
 *	/////////////////////////////////////
 *	////////////// pause.c //////////////
 *	/// J. P. Hawkins WH X4610 8C-001 ///
 *	///// Sun Aug 26 06:25:33 1979 //////
 *	/////////////////////////////////////
 *	/////////////////////////////////////

 *
 * wait for newline (big deal, right?)
 */
/*   "@(#) pause.c:  V 1.1  12/21/80" */

#include "bas.h"
pause()
{
	char c[10];
	gets(c);
	return(0);
}
