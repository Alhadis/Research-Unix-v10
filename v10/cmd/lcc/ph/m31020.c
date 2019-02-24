/* The Plum Hall Validation Suite for C
 * Unpublished copyright (c) 1986-1991, Chiron Systems Inc and Plum Hall Inc.
 * VERSION: 4
 * DATE: 1993-01-01
 * The "ANSI" mode of this suite corresponds to official ANSI C, X3.159-1989.
 * As per your license agreement, your distribution is not to be moved or copied outside the Designated Site
 * without specific permission from Plum Hall Inc.
 */

/* 3.1.3.4		ConP1S1	CONSTRAINT-MANDATORY */
/* 	value of octal escape sequence shall fit its storage */
#include <limits.h>
int main() {
	{ int i = sizeof('\777'); }
	#if CHAR_BIT > 8
	FORCE AN ERROR!!! This constraint is totally vacuous, since octal
	sequences are limited to three digits.
	#endif
	return 0; }
