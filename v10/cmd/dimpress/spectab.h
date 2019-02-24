/*	@(#)spectab.h	1.1	*/

/*
 *
 * Control strings used by the post-processor when it is building up
 * characters. The format of the strings is, an optional motion followed
 * by a character that we want to print. The ordered pair (dx,dy) defines
 * a motion. dx and dy specify relative horizontal and vertical movements
 * in terms of a percentage of the width of the character we are trying to
 * build up. Both dx and dy must be integers. If you decide to change this
 * syntax, routine dostring() will have to be rewritten.
 *
 */

char *spectab[] = {

"fi", "f(55,0)i",
"fl", "f(60,0)l",
"ff", "f(50,0)f",
"Fi", "f(33,0)f(45,0)i",
"Fl", "f(33,0)f(45,0)l",
0, 0,

};

