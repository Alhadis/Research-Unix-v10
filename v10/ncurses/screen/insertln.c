# include	"curses.ext"
/*	@(#) insertln.c: 1.1 10/15/83	(1.4	11/18/82)	*/

/*
 *	This routine performs an insert-line on the _window, leaving
 * (_cury,_curx) unchanged.
 *
 * 4/17/81 (Berkeley) @(#)insertln.c	1.4
 */
winsertln(win)
reg WINDOW	*win; {

	reg chtype	*temp;
	reg int		y;
	reg chtype	*end;

	temp = win->_y[win->_maxy-1];
	win->_firstch[win->_cury] = 0;
	win->_lastch[win->_cury] = win->_maxx - 1;
	for (y = win->_maxy - 1; y > win->_cury; --y) {
		win->_y[y] = win->_y[y-1];
		win->_firstch[y] = 0;
		win->_lastch[y] = win->_maxx - 1;
	}
	for (end = &temp[win->_maxx]; temp < end; )
		*temp++ = ' ';
	win->_y[win->_cury] = temp - win->_maxx;
	if (win->_cury == LINES - 1 && win->_y[LINES-1][COLS-1] != ' ')
		if (win->_scroll && !(win->_flags&_ISPAD)) {
			wrefresh(win);
			scroll(win);
			win->_cury--;
		}
		else
			return ERR;
	return OK;
}
