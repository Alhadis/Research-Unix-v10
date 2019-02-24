/*	@(#) wstandend.c: 1.1 10/15/83	(1.2	4/7/82)	*/
/*
 * 1/26/81 (Berkeley) @(#)standout.c	1.1
 */

# include	"curses.ext"

/*
 * exit standout mode
 */
wstandend(win)
register WINDOW	*win;
{
#ifdef DEBUG
	if(outf) fprintf(outf, "WSTANDEND(%x)\n", win);
#endif

	win->_attrs = 0;
	return 1;
}
