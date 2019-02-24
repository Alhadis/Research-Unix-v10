/*	@(#) deleteln.c: 1.1 10/15/83	(1.2	4/7/82)	*/

#include	"curses.h"

/*
 *	This routine deletes a line from the screen.  It leaves
 * (_cury,_curx) unchanged.
 *
 * 4/16/81 (Berkeley) @(#)deleteln.c	1.3
 */
wdeleteln(win)
reg WINDOW	*win; {

	reg chtype	*temp;
	reg int		y;
	reg chtype	*end;

	temp = win->_y[win->_cury];
	for (y = win->_cury; y < win->_maxy; y++) {
		win->_y[y] = win->_y[y+1];
		win->_firstch[y] = 0;
		win->_lastch[y] = win->_maxx - 1;
	}
	for (end = &temp[win->_maxx]; temp < end; )
		*temp++ = ' ';
	win->_y[win->_maxy-1] = temp - win->_maxx;
}
