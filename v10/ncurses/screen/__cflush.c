/*
 * This routine is one of the main things
 * in this level of curses that depends on the outside
 * environment.
 */
#include "curses.ext"
/*	@(#) __cflush.c: 1.1 10/15/83	(1.7	10/27/82)	*/

/*
 * Flush stdout.
 */
__cflush()
{
	fflush(SP->term_file);
}
