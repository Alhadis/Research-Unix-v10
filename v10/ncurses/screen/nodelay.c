#include "curses.ext"
/*	@(#) nodelay.c: 1.1 10/15/83	(1.19	3/17/83)	*/

/*
 * TRUE => don't wait for input, but return -1 instead.
 */
nodelay(win,bf)
WINDOW *win; int bf;
{
	_fixdelay(win->_nodelay, bf);
	win->_nodelay = bf;
}
