# include	"curses.ext"
/*	@(#) mvwscanw.c: 1.1 10/15/83	(1.3	6/25/82)	*/

/* VARARGS */
mvwscanw(win, y, x, fmt, args)
reg WINDOW	*win;
reg int		y, x;
char		*fmt;
int		args; {

	return wmove(win, y, x) == OK ? _sscans(win, fmt, &args) : ERR;
}
