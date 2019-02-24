# include	"curses.ext"
/*	@(#) mvscanw.c: 1.1 10/15/83	(1.3	6/25/82)	*/

/*
 * implement the mvscanw commands.  Due to the variable number of
 * arguments, they cannot be macros.  Another sigh....
 *
 * 1/26/81 (Berkeley) @(#)mvscanw.c	1.1
 */

/* VARARGS */
mvscanw(y, x, fmt, args)
reg int		y, x;
char		*fmt;
int		args; {

	return move(y, x) == OK ? _sscans(stdscr, fmt, &args) : ERR;
}
