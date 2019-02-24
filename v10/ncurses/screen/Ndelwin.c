# include	"curses.ext"
/*	@(#) delwin.c: 1.1 10/15/83	(1.2	4/7/82)	*/

/*
 *	This routine deletes a _window and releases it back to the system.
 *
 * 1/26/81 (Berkeley) @(#)delwin.c	1.1
 */
delwin(win)
reg WINDOW	*win; {

	reg int	i;

	if (!(win->_flags & _SUBWIN))
		for (i = 0; i < win->_maxy && win->_y[i]; i++)
			cfree((char *) win->_y[i]);
/* FIX START */
	cfree((char *)win->_firstch);
	cfree((char *)win->_lastch);
/* FIX END */
	cfree((char *) win->_y);
	cfree((char *) win);
}
