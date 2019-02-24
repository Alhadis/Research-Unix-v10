#include "curses.ext"
/*	@(#) has_ic.c: 1.1 10/15/83	(1.19	3/17/83)	*/

/*
 * Does it have insert/delete char?
 */
has_ic()
{
	return insert_character || enter_insert_mode;
}
