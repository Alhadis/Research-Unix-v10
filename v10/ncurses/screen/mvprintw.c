# include	"curses.ext"
/*	@(#) mvprintw.c: 1.1 10/15/83	(1.2	4/7/82)	*/

/*
 * implement the mvprintw commands.  Due to the variable number of
 * arguments, they cannot be macros.  Sigh....
 *
 * 1/26/81 (Berkeley) @(#)mvprintw.c	1.1
 */

/* VARARGS */
mvprintw(y, x, fmt, args)
reg int		y, x;
char		*fmt;
int		args; {

	return move(y, x) == OK ? _sprintw(stdscr, fmt, &args) : ERR;
}
