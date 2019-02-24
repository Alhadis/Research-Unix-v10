/*	@(#) _setmode.c: 1.1 10/15/83	(1.21	2/11/83)	*/

#include "curses.ext"

extern	int	_outch();

_setmode ()
{
#ifdef DEBUG
	if(outf) fprintf(outf, "_setmode().\n");
#endif
	if (SP->virt_irm == SP->phys_irm)
		return;
	tputs(SP->virt_irm==1 ? enter_insert_mode : exit_insert_mode, 0, _outch);
	SP->phys_irm = SP->virt_irm;
}
