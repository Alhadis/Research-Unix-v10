#include "curses.ext"
/*	@(#) raw.c: 1.1 10/15/83	(1.19	3/17/83)	*/

raw()
{
#ifdef USG
	/* Disable interrupt characters */
	(cur_term->Nttyb).c_cc[VINTR] = 0377;
	(cur_term->Nttyb).c_cc[VQUIT] = 0377;
	/* Allow 8 bit input/output */
	(cur_term->Nttyb).c_iflag &= ~ISTRIP;
	(cur_term->Nttyb).c_cflag &= ~CSIZE;
	(cur_term->Nttyb).c_cflag |= CS8;
	(cur_term->Nttyb).c_cflag &= ~PARENB;
	crmode();
#else
	(cur_term->Nttyb).sg_flags|=RAW;
#ifdef DEBUG
	if(outf) fprintf(outf, "raw(), file %x, SP %x, flags %x\n", SP->term_file, SP, cur_term->Nttyb.sg_flags);
#endif
	SP->fl_rawmode=TRUE;
#endif
	reset_prog_mode();
}
