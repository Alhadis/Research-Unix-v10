#include "curses.ext"
/*	@(#) noecho.c: 1.1 10/15/83	(1.19	3/17/83)	*/

noecho()
{
	SP->fl_echoit = FALSE;
}
