# include	"curses.ext"
/*	@(#) getstr.c: 1.1 10/15/83	(1.10	11/18/82)	*/

/*
 *	This routine gets a string starting at (_cury,_curx)
 *
 * 7/2/81 (Berkeley) @(#)getstr.c	1.4
 */
wgetstr(win,str)
WINDOW	*win; 
char	*str;
{
	char myerase, mykill;
	char rownum[256], colnum[256];
	int doecho = SP->fl_echoit;
	int savecb = SP->fl_rawmode;
	register int cpos = 0;
	register int ch;
	register char *cp = str;

#ifdef DEBUG
	if (outf) fprintf(outf, "doecho %d, savecb %d\n", doecho, savecb);
#endif

	myerase = erasechar();
	mykill = killchar();
	noecho(); crmode();

	for (;;) {
		rownum[cpos] = win->_cury;
		colnum[cpos] = win->_curx;
		if (! (win->_flags&_ISPAD))
			wrefresh(win);
		ch = wgetch(win);
		if (ch <= 0 ||ch == ERR || ch == '\n' || ch == '\r')
			break;
		if (ch == myerase || ch == KEY_LEFT || ch == KEY_BACKSPACE) {
			if (cpos > 0) {
				cp--; cpos--;
				if (doecho) {
					wmove(win, rownum[cpos], colnum[cpos]);
					wclrtoeol(win);
				}
			}
		} else if (ch == mykill) {
			cp = str;
			cpos = 0;
			if (doecho) {
				wmove(win, rownum[cpos], colnum[cpos]);
				wclrtoeol(win);
			}
		} else {
			*cp++ = ch;
			cpos++;
			if (doecho) {
				waddch(win, ch);
			}
		}
	}

	*cp = '\0';

	if (doecho)
		echo();
	if (!savecb)
		nocrmode();
	waddch(win, '\n');
	if (win->_flags & _ISPAD);
		wrefresh(win);
	if (ch == ERR)
		return ERR;
	return OK;
}
