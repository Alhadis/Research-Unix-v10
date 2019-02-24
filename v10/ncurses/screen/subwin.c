# include	"curses.ext"
# include	<signal.h>
/*	@(#) subwin.c: 1.1 10/15/83	(1.17	3/16/83)	*/

char	*calloc();
char	*malloc();
extern	char	*getenv();

extern	WINDOW	*makenew();

WINDOW *
subwin(orig, num_lines, num_cols, begy, begx)
register WINDOW	*orig;
int	num_lines, num_cols, begy, begx;
{

	register int i;
	register WINDOW	*win;
	register int by, bx, nlines, ncols;
	register int j, k;

	by = begy;
	bx = begx;
	nlines = num_lines;
	ncols = num_cols;

	/*
	 * make sure window fits inside the original one
	 */
# ifdef	DEBUG
	if(outf) fprintf(outf, "SUBWIN(%0.2o, %d, %d, %d, %d)\n", orig, nlines, ncols, by, bx);
# endif
	if (by < orig->_begy || bx < orig->_begx
	    || by + nlines > orig->_begy + orig->_maxy
	    || bx + ncols  > orig->_begx + orig->_maxx)
		return NULL;
	if (nlines == 0)
		nlines = orig->_maxy - orig->_begy - by;
	if (ncols == 0)
		ncols = orig->_maxx - orig->_begx - bx;
	if ((win = makenew(nlines, ncols, by, bx)) == NULL)
		return NULL;
	j = by - orig->_begy;
	k = bx - orig->_begx;
	for (i = 0; i < nlines; i++)
		win->_y[i] = &orig->_y[j++][k];
	win->_flags = _SUBWIN;
	return win;
}
