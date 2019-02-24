# include	"curses.ext"
/*	@(#) box.c: 1.1 10/15/83	(1.5	3/6/83)	*/

/*
 *	This routine draws a box around the given window with "vert"
 * as the vertical delimiting char, and "hor", as the horizontal one.
 *
 * 1/26/81 (Berkeley) @(#)box.c	1.1
 */

/* Defaults - might someday be terminal dependent using graphics chars */
#define DEFVERT '|'
#define DEFHOR  '-'

box(win, vert, hor)
register WINDOW	*win;
chtype	vert, hor;
{
	register int	i;
	register int	endy, endx;
	register chtype	*fp, *lp;

	if (vert == 0)
		vert = DEFVERT;
	if (hor == 0)
		hor = DEFHOR;
	endx = win->_maxx;
	endy = win->_maxy -  1;
	fp = win->_y[0];
	lp = win->_y[endy];
	for (i = 0; i < endx; i++)
		fp[i] = lp[i] = hor;
	endx--;
	for (i = 0; i <= endy; i++)
		win->_y[i][0] = (win->_y[i][endx] = vert);
	touchwin(win);
}
