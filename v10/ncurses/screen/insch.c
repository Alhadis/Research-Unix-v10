# include	"curses.ext"
/*	@(#) insch.c: 1.1 10/15/83	(1.4	11/18/82)	*/

/*
 *	This routine performs an insert-char on the line, leaving
 * (_cury,_curx) unchanged.
 *
 * @(#)insch.c	1.2 (Berkeley) 4/17/81
 */
winsch(win, c)
reg WINDOW	*win;
chtype		c; {

	register chtype	*temp1, *temp2;
	register chtype	*end;

	end = &win->_y[win->_cury][win->_curx];
	temp1 = &win->_y[win->_cury][win->_maxx - 1];
	temp2 = temp1 - 1;
	while (temp1 > end)
		*temp1-- = *temp2--;
	*temp1 = c;
	win->_lastch[win->_cury] = win->_maxx - 1;
	if (win->_firstch[win->_cury] == _NOCHANGE ||
	    win->_firstch[win->_cury] > win->_curx)
		win->_firstch[win->_cury] = win->_curx;
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
