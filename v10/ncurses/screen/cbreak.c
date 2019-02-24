#include "curses.ext"
/*	@(#) cbreak.c: 1.1 10/15/83	(1.19	3/17/83)	*/

cbreak()
{
#ifdef USG
	(cur_term->Nttyb).c_lflag &= ~ICANON;
	(cur_term->Nttyb).c_cc[VMIN] = 1;
	(cur_term->Nttyb).c_cc[VTIME] = 1;
# ifdef DEBUG
	if(outf) fprintf(outf, "crmode(), file %x, SP %x, flags %x\n", SP->term_file, SP, cur_term->Nttyb.c_lflag);
# endif
#else
	(cur_term->Nttyb).sg_flags |= CBREAK;
# ifdef DEBUG
	if(outf) fprintf(outf, "crmode(), file %x, SP %x, flags %x\n", SP->term_file, SP, cur_term->Nttyb.sg_flags);
# endif
#endif
	SP->fl_rawmode = TRUE;
	reset_prog_mode();
}
