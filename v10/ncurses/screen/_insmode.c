/*	@(#) _insmode.c: 1.1 10/15/83	(1.21	2/11/83)	*/

#include "curses.ext"

/*
 * Set the virtual insert/replacement mode to new.
 */
_insmode (new)
int new;
{
#ifdef DEBUG
	if(outf) fprintf(outf, "_insmode(%d).\n", new);
#endif
	SP->virt_irm = new;
}
