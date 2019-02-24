# include	"curses.ext"
/*	@(#) touchwin.c: 1.1 10/15/83	(1.3	8/18/82)	*/

/*
 * make it look like the whole window has been changed.
 *
 * 1/26/81 (Berkeley) @(#)touchwin.c	1.1
 */
touchwin(win)
reg WINDOW	*win;
{
	reg int		y, maxy, maxx;

#ifdef DEBUG
	if (outf) fprintf(outf, "touchwin(%x)\n", win);
#endif
	maxy = win->_maxy;
	maxx = win->_maxx - 1;
	for (y = 0; y < maxy; y++) {
		win->_firstch[y] = 0;
		win->_lastch[y] = maxx;
	}
}
