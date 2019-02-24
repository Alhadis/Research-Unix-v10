# include	"curses.ext"
# include	<signal.h>
/*	@(#) newpad.c: 1.1 10/15/83	(1.17	3/16/83)	*/

char	*calloc();
char	*malloc();
extern	char	*getenv();

extern	WINDOW	*makenew();

/*
 * Like newwin, but makes a pad instead of a window.  A pad is not
 * associated with part of the screen, so it can be bigger.
 */
WINDOW *
newpad(nlines, ncols)
register int	nlines;
{
	register WINDOW	*win;
	register chtype	*sp;
	register int i;
	char *calloc();

	if ((win = makenew(nlines, ncols, 0, 0)) == NULL)
		return NULL;
	win->_flags |= _ISPAD;
	for (i = 0; i < nlines; i++)
		if ((win->_y[i] = (chtype *) calloc(ncols, sizeof (chtype))) == NULL) {
			register int j;

			for (j = 0; j < i; j++)
				cfree((char *)win->_y[j]);
			cfree((char *)win->_firstch);
			cfree((char *)win->_lastch);
			cfree((char *)win->_y);
			cfree((char *)win);
			return NULL;
		}
		else
			for (sp = win->_y[i]; sp < win->_y[i] + ncols; )
				*sp++ = ' ';
	return win;
}
