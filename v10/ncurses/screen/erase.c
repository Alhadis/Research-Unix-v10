# include	"curses.ext"
/*	@(#) erase.c: 1.1 10/15/83	(1.2	4/7/82)	*/

/*
 *	This routine erases everything on the _window.
 *
 * 1/27/81 (Berkeley) @(#)erase.c	1.2
 */
werase(win)
reg WINDOW	*win; {

	reg int		y;
	reg chtype	*sp, *end, *start, *maxx;
	reg int		minx;

# ifdef DEBUG
	if(outf) fprintf(outf, "WERASE(%0.2o), _maxx %d\n", win, win->_maxx);
# endif
	for (y = 0; y < win->_maxy; y++) {
		minx = _NOCHANGE;
		maxx = NULL;
		start = win->_y[y];
		end = &start[win->_maxx];
		for (sp = start; sp < end; sp++) {
#ifdef DEBUG
			if (y == 23) if(outf) fprintf(outf,
				"sp %x, *sp %c %o\n", sp, *sp, *sp);
#endif
			if (*sp != ' ') {
				maxx = sp;
				if (minx == _NOCHANGE)
					minx = sp - start;
				*sp = ' ';
			}
		}
		if (minx != _NOCHANGE) {
			if (win->_firstch[y] > minx
			     || win->_firstch[y] == _NOCHANGE)
				win->_firstch[y] = minx;
			if (win->_lastch[y] < maxx - win->_y[y])
				win->_lastch[y] = maxx - win->_y[y];
		}
# ifdef DEBUG
	if(outf) fprintf(outf, "WERASE: minx %d maxx %d _firstch[%d] %d, start %x, end %x\n",
		minx, maxx ? maxx-start : NULL, y, win->_firstch[y], start, end);
# endif
	}
	win->_curx = win->_cury = 0;
}
