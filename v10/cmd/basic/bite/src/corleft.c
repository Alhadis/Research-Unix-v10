/* Copyright Bell Telephone Laboratories Whippany, N.J.

 *	/////////////////////////////////////
 *	/////////////////////////////////////
 *	///////////// corleft.c /////////////
 *	/// J. P. Hawkins WH X4610 8C-001 ///
 *	///// Sun Aug 26 06:45:07 1979 //////
 *	/////////////////////////////////////
 *	/////////////////////////////////////

 *
 * SIZE command
 * DISPLAY REMAINING CORE SPACE
 */
/*   "@(#) corleft.c:  V 1.1  12/21/80" */
#include	"bas.h"
extern	char *hicore;
corleft()
{
	char	*hi,*low;	/* for getting unsigned values */

	hi = (char *)hicore;
	low = linptr;
printf("Core used: %d bites.\tCore remaining: %d bites.\n",PGSIZ*NMPAG-(hi-low),hi-low);
}
