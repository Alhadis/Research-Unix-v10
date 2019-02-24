#include "curses.ext"
/*	@(#) keypad.c: 1.1 10/15/83	(1.19	3/17/83)	*/

/*
 * TRUE => special keys should be passed as a single character by getch.
 */
keypad(win,bf)
WINDOW *win; int bf;
{
	win->_use_keypad = bf;
}
