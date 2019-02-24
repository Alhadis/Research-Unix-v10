h50675
s 00004/00001/00020
d D 1.2 87/10/22 09:22:25 bin 2 1
c Bug fix to free windows in curses.
e
s 00021/00000/00000
d D 1.1 87/10/22 09:17:54 bin 1 0
c date and time created 87/10/22 09:17:54 by bin
e
u
U
t
T
I 1
/*      %M%     %I%     %G%             */
D 2

E 2
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
I 2
/* FIX START */
	cfree((char *)win->_firstch);
	cfree((char *)win->_lastch);
/* FIX END */
E 2
	cfree((char *) win->_y);
	cfree((char *) win);
}
E 1
