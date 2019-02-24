#include "curses.ext"
/*	@(#) leaveok.c: 1.1 10/15/83	(1.19	3/17/83)	*/

/*
 * TRUE => OK to leave cursor where it happens to fall after refresh.
 */
leaveok(win,bf)
WINDOW *win; int bf;
{
	win->_leave = bf;
}
