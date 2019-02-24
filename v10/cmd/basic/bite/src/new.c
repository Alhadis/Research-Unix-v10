/* Copyright Bell Telephone Laboratories Whippany, N.J.

 *	/////////////////////////////////////
 *	/////////////////////////////////////
 *	/////////////// new.c ///////////////
 *	/// J. P. Hawkins WH X4610 8C-001 ///
 *	///// Sun Aug 26 06:20:23 1979 //////
 *	/////////////////////////////////////
 *	/////////////////////////////////////

 *
 *
 * routine to clear the contents of the working storage
 */
/*   "@(#) new.c:  V 1.1  12/21/80" */

#include "bas.h"
extern char filnam[];
extern char txtbuf[];
new()
{
	filnam[0] = '\0';
	onew();
}
onew()
{
	int i;
	lbdptr=curptr=linptr=txtbuf;
	inst.thing.linno = 0;
	clear();
	expunge();	/* clear variable stack and variable symbol tables */
	for(i=0;i<4;i++)
		txtbuf[i]= '\0';
	return(0);
}
