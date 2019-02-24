/*	@(#) beep.c: 1.1 10/15/83	(1.21	2/11/83)	*/

#include "curses.ext"

extern	int	_outch();

beep()
{
#ifdef DEBUG
	if(outf) fprintf(outf, "beep().\n");
#endif
    if (bell)
	tputs (bell, 0, _outch);
    else
	tputs (flash_screen, 0, _outch);
    __cflush();
}
