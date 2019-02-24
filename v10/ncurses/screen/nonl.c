#include "curses.ext"
/*	@(#) nonl.c: 1.1 10/15/83	(1.19	3/17/83)	*/

nonl()	
{
#ifdef USG
	(cur_term->Nttyb).c_iflag &= ~ICRNL;
	(cur_term->Nttyb).c_oflag &= ~ONLCR;
# ifdef DEBUG
	if(outf) fprintf(outf, "nonl(), file %x, SP %x, flags %x,%x\n", SP->term_file, SP, cur_term->Nttyb.c_iflag, cur_term->Nttyb.c_oflag);
# endif
#else
	(cur_term->Nttyb).sg_flags &= ~CRMOD;
# ifdef DEBUG
	if(outf) fprintf(outf, "nonl(), file %x, SP %x, flags %x\n", SP->term_file, SP, cur_term->Nttyb.sg_flags);
# endif
#endif
	reset_prog_mode();
}
