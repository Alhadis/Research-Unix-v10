#include "curses.ext"
/*	@(#) scrollok.c: 1.1 10/15/83	(1.19	3/17/83)	*/

/*
 * TRUE => OK to scroll screen up when you run off the bottom.
 */
scrollok(win,bf)
WINDOW *win;
int bf;
{
	/* Should consider using scroll/page mode of some terminals. */
	win->_scroll = bf;
}
