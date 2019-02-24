#include "curses.ext"
/*	@(#) idlok.c: 1.1 10/15/83	(1.19	3/17/83)	*/

/*
 * TRUE => OK to use insert/delete line.
 */
idlok(win,bf)
WINDOW *win;
int bf;
{
	win->_use_idl = bf;
}
