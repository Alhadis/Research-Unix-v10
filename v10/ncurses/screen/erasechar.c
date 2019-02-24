#include "curses.ext"
/*	@(#) erasechar.c: 1.1 10/15/83	(1.19	3/17/83)	*/

char
erasechar()
{
#ifdef USG
	return cur_term->Ottyb.c_cc[VERASE];
#else
	return cur_term->Ottyb.sg_erase;
#endif
}
