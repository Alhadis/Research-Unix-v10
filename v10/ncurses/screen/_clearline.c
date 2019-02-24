/*	@(#) _clearline.c: 1.1 10/15/83	(1.9	10/27/82)	*/

#include "curses.ext"

/*
 * '_clearline' positions the cursor at the beginning of the
 * indicated line and clears the line (in the image)
 */
_clearline (row)
{
	_ll_move (row, 0);
	SP->std_body[row+1] -> length = 0;
}
