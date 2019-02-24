/* Copyright Bell Telephone Laboratories Whippany, N.J.

 *	***********************************
 *	***********************************
 *	***** COVER UP DELETED LINES ******
 *	*** R. B. Drake WH 8C-005 X4163 ***
 *	**** Fri Aug 24 17:11:58 1979 *****
 *	***********************************
 *	***********************************
 * This routine is
 * called by "bed" and used to slide the program txtbuf down when
 *   necessary to reclaim space created by the shortening of a line
 *   or by a delete 

*/
/*   "@(#) cover.c:  V 1.1  12/21/80" */
#include "bas.h"
cover(f,t)
char *f,*t;

{
register char *from,*to;
		from=f;to=t;
		curptr = to;
		clear();
		while(from != linptr)
			*to++ = *from++;
		linptr -= from - to;
		*linptr = '\0';
		*(linptr+1) = '\0';
		*(linptr+2) = '\0'; /* two zero bytes for eof */
		update();
		return(0);
}
