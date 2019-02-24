/*
 * Handy functions to put out a string with padding.
 * These make two assumptions:
 *	(1) Output is via stdio to stdout through putchar.
 *	(2) There is no count of affected lines.  Thus, this
 *	    routine is only valid for certain capabilities,
 *	    i.e. those that don't have *'s in the documentation.
 */
#include <stdio.h>
/*	@(#) _outchar.c: 1.1 10/15/83	(1.7	9/28/82)	*/

/*
 * Routine to act like putchar for passing to tputs.
 */
_outchar(ch)
char ch;
{
	putchar(ch);
}
