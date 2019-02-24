#include "curses.ext"
/*	@(#) set_term.c: 1.1 10/15/83	(1.13	8/20/82)	*/

struct screen *
set_term(new)
struct screen *new;
{
	register struct screen *rv = SP;

#ifdef DEBUG
	if(outf) fprintf(outf, "setterm: old %x, new %x\n", rv, new);
#endif

#ifndef		NONSTANDARD
	SP = new;
#endif		NONSTANDARD

	cur_term = SP->tcap;
	LINES = lines;
	COLS = columns;
	stdscr = SP->std_scr;
	curscr = SP->cur_scr;
	return rv;
}
