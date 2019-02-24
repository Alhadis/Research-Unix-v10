# include	"curses.ext"
/*	@(#) mvwprintw.c: 1.1 10/15/83	(1.2	4/7/82)	*/

/* VARARGS */
mvwprintw(win, y, x, fmt, args)
reg WINDOW	*win;
reg int		y, x;
char		*fmt;
int		args;
{

	return wmove(win, y, x) == OK ? _sprintw(win, fmt, &args) : ERR;
}
