#include "curses.ext"
/*	@(#) has_il.c: 1.1 10/15/83	(1.19	3/17/83)	*/

/*
 * Queries: does the terminal have insert/delete line?
 */
has_il()
{
	return insert_line && delete_line || change_scroll_region;
}
