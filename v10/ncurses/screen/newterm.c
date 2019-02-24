# include	"curses.ext"
# include	<signal.h>
/*	@(#) newterm.c: 1.1 10/15/83	(1.17	3/16/83)	*/

char	*calloc();
char	*malloc();
extern	char	*getenv();

extern	WINDOW	*makenew();

struct screen *
newterm(type, outfd, infd)
char *type;
FILE *outfd, *infd;
{
	int		_tstp();
	struct screen *scp;
	struct screen *_new_tty();
	extern int _endwin;

#ifdef DEBUG
	if(outf) fprintf(outf, "NEWTERM() isatty(2) %d, getenv %s\n",
		isatty(2), getenv("TERM"));
# endif
	SP = (struct screen *) calloc(1, sizeof (struct screen));
	SP->term_file = outfd;
	SP->input_file = infd;
	/*
	 * The default is echo, for upward compatibility, but we do
	 * all echoing in curses to avoid problems with the tty driver
	 * echoing things during critical sections.
	 */
	SP->fl_echoit = 1;
	savetty();
	scp = _new_tty(type, outfd);
	if (scp == NULL)
		return NULL;
#ifdef USG
	(cur_term->Nttyb).c_lflag &= ~ECHO;
#else
	(cur_term->Nttyb).sg_flags &= ~ECHO;
#endif
	reset_prog_mode();
# ifdef SIGTSTP
	signal(SIGTSTP, _tstp);
# endif
	if (curscr != NULL) {
# ifdef DEBUG
		if(outf) fprintf(outf, "INITSCR: non null curscr = 0%o\n", curscr);
# endif
	}
# ifdef DEBUG
	if(outf) fprintf(outf, "LINES = %d, COLS = %d\n", LINES, COLS);
# endif
	LINES =	lines;
	COLS =	columns;
	curscr = makenew(LINES, COLS, 0, 0);
	stdscr = newwin(LINES, COLS, 0, 0);
# ifdef DEBUG
	if(outf) fprintf(outf, "SP %x, stdscr %x, curscr %x\n", SP, stdscr, curscr);
# endif
	SP->std_scr = stdscr;
	SP->cur_scr = curscr;
	/* Maybe should use makewin and glue _y's to DesiredScreen. */
	_endwin = FALSE;
	return scp;
}
