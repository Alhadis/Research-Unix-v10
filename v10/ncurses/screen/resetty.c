#include "curses.ext"
/*	@(#) resetty.c: 1.1 10/15/83	(1.19	3/17/83)	*/

resetty()
{
#ifdef USG
	if (SP == NULL || SP->save_tty_buf.c_cflag&CBAUD == 0)
		return;	/* Never called savetty */
#else
	if (SP == NULL || SP->save_tty_buf.sg_ospeed == 0)
		return;	/* Never called savetty */
#endif
	cur_term->Nttyb = SP->save_tty_buf;
#ifdef DEBUG
# ifdef USG
	if(outf) fprintf(outf, "savetty(), file %x, SP %x, flags %x,%x,%x,%x\n", SP->term_file, SP, cur_term->Nttyb.c_iflag, cur_term->Nttyb.c_oflag, cur_term->Nttyb.c_cflag, cur_term->Nttyb.c_lflag);
# else
	if(outf) fprintf(outf, "resetty(), file %x, SP %x, flags %x\n", SP->term_file, SP, cur_term->Nttyb.sg_flags);
# endif
#endif
	reset_prog_mode();
}
