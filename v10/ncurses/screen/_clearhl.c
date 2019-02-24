/*	@(#) _clearhl.c: 1.1 10/15/83	(1.21	2/11/83)	*/

#include "curses.ext"

_clearhl ()
{
#ifdef DEBUG
	if(outf) fprintf(outf, "_clearhl().\n");
#endif
	if (SP->phys_gr) {
		register oldes = SP->virt_gr;
		SP->virt_gr = 0;
		_sethl ();
		SP->virt_gr = oldes;
	}
}
