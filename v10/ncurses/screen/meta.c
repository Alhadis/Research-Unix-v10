#include "curses.ext"
/*	@(#) meta.c: 1.1 10/15/83	(1.19	3/17/83)	*/

/*
 * TRUE => all 8 bits of input character should be passed through.
 */
meta(win,bf)
WINDOW *win; int bf;
{
	int _outch();

	if (!has_meta_key)
		return ERR;
	/*
	 * Do the appropriate fiddling with the tty driver to make it send
	 * all 8 bits through.  On USG this means clearing ISTRIP, on
	 * V7 you have to resort to RAW mode.
	 */
#ifdef USG
	if (bf) {
		(cur_term->Nttyb).c_iflag &= ~ISTRIP;
		(cur_term->Nttyb).c_cflag &= ~CSIZE;
		(cur_term->Nttyb).c_cflag |= CS8;
		(cur_term->Nttyb).c_cflag &= ~PARENB;
	} else {
		(cur_term->Nttyb).c_iflag |= ISTRIP;
		(cur_term->Nttyb).c_cflag &= ~CSIZE;
		(cur_term->Nttyb).c_cflag |= CS7;
		(cur_term->Nttyb).c_cflag |= PARENB;
	}
#else
	if (bf)
		raw();
	else
		noraw();
#endif
	reset_prog_mode();

	/*
	 * Do whatever is needed to put the terminal into meta-mode.
	 */
	if (bf)
		tputs(meta_on, 1, _outch);
	else
		tputs(meta_off, 1, _outch);

	/* Keep track internally. */
	win->_use_meta = bf;

	return OK;
}
