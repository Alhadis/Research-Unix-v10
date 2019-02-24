/*	@(#) _syncmodes.c: 1.1 10/15/83	(1.21	2/11/83)	*/

#include "curses.ext"

_syncmodes()
{
#ifdef DEBUG
	if(outf) fprintf(outf, "_syncmodes().\n");
#endif
	_sethl();
	_setmode();
	_setwind();
}
