/*	@(#) wrefresh.c: 1.1 10/15/83	(1.14	3/6/83)	*/
/*
 * make the current screen look like "win" over the area covered by
 * win.
 *
 * 7/9/81 (Berkeley) @(#)refresh.c	1.6
 */

# include	"curses.ext"

/* Put out window and update screen */
wrefresh(win)
WINDOW	*win;
{
	wnoutrefresh(win);
	return doupdate();
}
