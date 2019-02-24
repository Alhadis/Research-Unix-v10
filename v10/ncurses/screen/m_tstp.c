# include	"curses.ext"
# include	<signal.h>
/*	@(#) m_tstp.c: 1.1 10/15/83	(1.3	12/16/82)	*/

/*
 *	mini.c contains versions of curses routines for minicurses.
 *	They work just like their non-mini counterparts but draw on
 *	std_body rather than stdscr.  This cuts down on overhead but
 *	restricts what you are allowed to do - you can't get stuff back
 *	from the screen and you can't use multiple windows or things
 *	like insert/delete line (the logical ones that affect the screen).
 *	All this but multiple windows could probably be added if somebody
 *	wanted to mess with it.
 *
 * 3/5/81 (Berkeley) @(#)addch.c	1.3
 */

# ifdef SIGTSTP

/*
 * handle stop and start signals
 *
 * 3/5/81 (Berkeley) @(#)_tstp.c	1.1
 */
m_tstp() {

# ifdef DEBUG
	if (outf) fflush(outf);
# endif
	_ll_move(lines-1, 0);
	endwin();
	fflush(stdout);
	kill(0, SIGTSTP);
	signal(SIGTSTP, m_tstp);
	reset_prog_mode();
	SP->doclear = 1;
	_ll_refresh(0);
}
# endif
