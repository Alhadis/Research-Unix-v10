/*	@(#) def_prog.c: 1.1 10/15/83	(1.36	3/18/83)	*/

#include	"curses.h"
#include	"term.h"

extern	struct term *cur_term;

def_prog_mode()
{
#ifdef USG
	ioctl(cur_term -> Filedes, TCGETA, &(cur_term->Nttyb));
#else
	ioctl(cur_term -> Filedes, TIOCGETP, &(cur_term->Nttyb));
#endif
}
