# include	"curses.ext"
/*	@(#) clear.c: 1.1 10/15/83	(1.2	4/7/82)	*/

/*
 *	This routine clears the _window.
 *
 * 1/26/81 (Berkeley) @(#)clear.c	1.1
 */
wclear(win)
reg WINDOW	*win; {

	if (win == curscr)
		win = stdscr;
	werase(win);
	win->_clear = TRUE;
	return OK;
}
