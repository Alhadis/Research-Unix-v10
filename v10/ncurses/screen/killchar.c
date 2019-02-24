#include "curses.ext"
/*	@(#) killchar.c: 1.1 10/15/83	(1.19	3/17/83)	*/

char
killchar()
{
#ifdef USG
	return cur_term->Ottyb.c_cc[VKILL];
#else
	return cur_term->Ottyb.sg_kill;
#endif
}
