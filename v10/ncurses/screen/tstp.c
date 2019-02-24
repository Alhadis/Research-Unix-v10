# include	<signal.h>
/*	@(#) tstp.c: 1.1 10/15/83	(1.2	4/7/82)	*/

# ifdef SIGTSTP

# include	"curses.ext"

/*
 * handle stop and start signals
 *
 * 3/5/81 (Berkeley) @(#)_tstp.c	1.1
 */
_tstp() {

# ifdef DEBUG
	if (outf) fflush(outf);
# endif
	_ll_move(lines-1, 0);
	endwin();
	fflush(stdout);
	kill(0, SIGTSTP);
	signal(SIGTSTP, _tstp);
	fixterm();
	SP->doclear = 1;
	wrefresh(curscr);
}
# endif
