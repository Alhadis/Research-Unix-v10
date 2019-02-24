# include	"curses.ext"
/*	@(#) mvwin.c: 1.1 10/15/83	(1.2	4/7/82)	*/

/*
 * relocate the starting position of a _window
 *
 * 1/26/81 (Berkeley) @(#)mvwin.c	1.1
 */

mvwin(win, by, bx)
reg WINDOW	*win;
reg int		by, bx; {

	if (by + win->_maxy > LINES || bx + win->_maxx > COLS)
		return ERR;
	win->_begy = by;
	win->_begx = bx;
	touchwin(win);
	return OK;
}
