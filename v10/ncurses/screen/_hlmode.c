/*	@(#) _hlmode.c: 1.1 10/15/83	(1.21	2/11/83)	*/

#include "curses.ext"

_hlmode (on)
{
#ifdef DEBUG
	if(outf) fprintf(outf, "_hlmode(%o).\n", on);
#endif
	SP->virt_gr = on;
}
