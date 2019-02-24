#include "curses.ext"
/*	@(#) nocbreak.c: 1.1 10/15/83	(1.19	3/17/83)	*/

nocbreak()
{
#ifdef USG
	(cur_term->Nttyb).c_lflag |= ICANON;
	(cur_term->Nttyb).c_cc[VEOF] = (cur_term->Ottyb).c_cc[VEOF];
	(cur_term->Nttyb).c_cc[VEOL] = (cur_term->Ottyb).c_cc[VEOL];
# ifdef DEBUG
	if(outf) fprintf(outf, "nocrmode(), file %x, SP %x, flags %x\n", SP->term_file, SP, cur_term->Nttyb.c_lflag);
# endif
#else
	(cur_term->Nttyb).sg_flags &= ~CBREAK;
# ifdef DEBUG
	if(outf) fprintf(outf, "nocrmode(), file %x, SP %x, flags %x\n", SP->term_file, SP, cur_term->Nttyb.sg_flags);
# endif
#endif
	SP->fl_rawmode=FALSE;
	reset_prog_mode();
}
