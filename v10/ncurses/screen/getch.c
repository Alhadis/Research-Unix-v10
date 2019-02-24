# include	"curses.ext"
# include	<signal.h>
# include	<errno.h>

/*	@(#) getch.c: 1.1 10/15/83	(1.16	3/16/83)	*/

static int sig_caught;

/*
 *	This routine reads in a character from the window.
 *
 *	wgetch MUST return an int, not a char, because it can return
 *	things like ERR, meta characters, and function keys > 256.
 */
int
wgetch(win)
register WINDOW	*win;
{

	register int inp;
	register int i, j;
	char c;
	int arg;
	bool	weset = FALSE;
	FILE *inf;

	if (SP->fl_echoit && !win->_scroll && (win->_flags&_FULLWIN)
	    && win->_curx == win->_maxx && win->_cury == win->_maxy)
		return ERR;
# ifdef DEBUG
	if(outf) fprintf(outf, "WGETCH: SP->fl_echoit = %c, SP->fl_rawmode = %c\n", SP->fl_echoit ? 'T' : 'F', SP->fl_rawmode ? 'T' : 'F');
	if (outf) fprintf(outf, "_use_keypad %d, kp_state %d\n", win->_use_keypad, SP->kp_state);
# endif
	if (SP->fl_echoit && !SP->fl_rawmode) {
		cbreak();
		weset++;
	}

#ifdef KEYPAD
	/* Make sure keypad on is in proper state */
	if (win->_use_keypad != SP->kp_state) {
		_kpmode(win->_use_keypad);
		fflush(stdout);
	}
#endif

	/* Make sure we are in proper nodelay state */
	if (win->_nodelay != SP->fl_nodelay)
		_fixdelay(SP->fl_nodelay, win->_nodelay);

	/* Check for pushed typeahead.  We make the assumption that
	 * if a function key is being typed, there is no pushed
	 * typeahead from the previous key waiting.
	 */
	if (SP->input_queue[0] >= 0) {
		inp = SP->input_queue[0];
		for (i=0; i<16; i++) {
			SP->input_queue[i] = SP->input_queue[i+1];
			if (SP->input_queue[i] < 0)
				break;
		}
		goto gotit;
	}

	inf = SP->input_file;
	if (inf == stdout)	/* so output can be teed somewhere */
		inf = stdin;
#ifdef FIONREAD
	if (win->_nodelay) {
		ioctl(fileno(inf), FIONREAD, &arg);
#ifdef DEBUG
		if (outf) fprintf(outf, "FIONREAD returns %d\n", arg);
#endif
		if (arg < 1)
			return -1;
	}
#endif
	for (i = -1; i<0; ) {
		extern int errno;
		sig_caught = 0;
		i = read(fileno(inf), &c, 1);
		/*
		 * I hope the system won't retern infinite EINTRS - maybe
		 * there should be a hop count here.
		 */
		if (i < 0 && errno != EINTR && !sig_caught) {
			inp = ERR;
			goto gotit;
		}
	}
	if (i > 0) {
		inp = c;
		if (!win->_use_meta)
			inp &= 0177;
		else
			inp &= 0377;
	} else {
		inp = ERR;
		goto gotit;
	}
# ifdef DEBUG
	if(outf) fprintf(outf,"WGETCH got '%s'\n",unctrl(inp));
# endif

#ifdef KEYPAD
	/* Check for arrow and function keys */
	if (win->_use_keypad) {
		SP->input_queue[0] = inp;
		SP->input_queue[1] = -1;
		for (i=0; SP->kp[i].keynum >= 0; i++) {
			if (SP->kp[i].sends[0] == inp) {
				for (j=0; ; j++) {
					if (SP->kp[i].sends[j] <= 0)
						break;	/* found */
					if (SP->input_queue[j] == -1) {
						SP->input_queue[j] = _fpk(inf);
						SP->input_queue[j+1] = -1;
					}
					if (SP->kp[i].sends[j] != SP->input_queue[j])
						goto contouter; /* not this one */
				}
				/* It matched the function key. */
				inp = SP->kp[i].keynum;
				SP->input_queue[0] = -1;
				goto gotit;
			}
		contouter:;
		}
		/* Didn't match any function keys. */
		inp = SP->input_queue[0];
		for (i=0; i<16; i++) {
			SP->input_queue[i] = SP->input_queue[i+1];
			if (SP->input_queue[i] < 0)
				break;
		}
		goto gotit;
	}
#endif

	if (SP->fl_echoit) {
		waddch(win, (chtype) inp);
		wrefresh(win);
	}
gotit:
	if (weset)
		nocbreak();
#ifdef DEBUG
	if(outf) fprintf(outf, "getch returns %o, pushed %o %o %o\n",
		inp, SP->input_queue[0], SP->input_queue[1], SP->input_queue[2]);
#endif
	return inp;
}
_catch_alarm()
{
	sig_caught = 1;
}

/*
 * Fast peek key.  Like getchar but if the right flags are set, times out
 * quickly if there is nothing waiting, returning -1.
 * f is an output stdio descriptor, we read from the fileno.  win is the
 * window this is supposed to have something to do with.
 */
#ifndef FIONREAD
/*
 * Traditional implementation.  The best resolution we have is 1 second,
 * so we set a 1 second alarm and try to read.  If we fail for 1 second,
 * we assume there is no key waiting.  Problem here is that 1 second is
 * too long, people can type faster than this.
 */
static
_fpk(f)
FILE *f;
{
	char c;
	int rc;
	int (*oldsig)();
	int oldalarm;

	oldsig = signal(SIGALRM, _catch_alarm);
	oldalarm = alarm(1);
	sig_caught = 0;
	rc = read(fileno(f), &c, 1);
	if (sig_caught) {
		sig_caught = 0;
		alarm(oldalarm);
		return -2;
	}
	signal(SIGALRM, oldsig);
	alarm(oldalarm);
	return rc == 1 ? c : -1;
}
#else FIONREAD
/*
 * If we have the select system call, we can do much better.
 * We wait for long enough for a terminal to send another character
 * (at 15cps repeat rate, this is 67 ms, I'm using 100ms to allow
 * a bit of a fudge factor) and time out more quickly.  Even if we
 * don't have the real 4.2BSD select, we can emulate it with napms
 * and FIONREAD.  napms might be done with only 1 second resolution,
 * but this is no worse than what we have above.
 */
static
_fpk(f)
FILE *f;
{
	int infd, rc;
	int *outfd, *exfd;
	char c;

	infd = 1 << fileno(f);
	outfd = exfd = (int *) NULL;
	rc = select(20, &infd, outfd, exfd, 100);
	if (rc < 0)
		return -2;
	rc = read(fileno(f), &c, 1);
	return rc == 1 ? c : -1;
}
#endif FIONREAD
