# include	"curses.ext"
/*	@(#) addstr.c: 1.1 10/15/83	(1.3	7/17/82)	*/

/*
 *	This routine adds a string starting at (_cury,_curx)
 *
 * 1/26/81 (Berkeley) @(#)addstr.c	1.1
 */
waddstr(win,str)
register WINDOW	*win; 
register char	*str;
{
# ifdef DEBUG
	if(outf)
	{
		if( win == stdscr )
		{
			fprintf(outf, "WADDSTR(stdscr, ");
		}
		else
		{
			fprintf(outf, "WADDSTR(%o, ", win);
		}
		fprintf(outf, "\"%s\")\n", str);
	}
# endif	DEBUG
	while( *str )
	{
		if( waddch( win, ( chtype ) *str++ ) == ERR )
		{
			return ERR;
		}
	}
	return OK;
}
