# include	"curses.ext"
/*	@(#) scroll.c: 1.1 10/15/83	(1.3	7/17/82)	*/

/*
 *	This routine scrolls the window up a line.
 *
 * 7/8/81 (Berkeley) @(#)scroll.c	1.2
 */
scroll(win)
WINDOW *win;
{
	_tscroll(win, 1);
}
