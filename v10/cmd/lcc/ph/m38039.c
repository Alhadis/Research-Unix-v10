/* The Plum Hall Validation Suite for C
 * Unpublished copyright (c) 1986-1991, Chiron Systems Inc and Plum Hall Inc.
 * VERSION: 4
 * DATE: 1993-01-01
 * The "ANSI" mode of this suite corresponds to official ANSI C, X3.159-1989.
 * As per your license agreement, your distribution is not to be moved or copied outside the Designated Site
 * without specific permission from Plum Hall Inc.
 */

/* 3.8.5	SemP1	MANDATORY-DIAGNOSTIC */
/* 	#error pp-tokens  produces a diagnostic containing the pp-tokens */
int main() {
	#error This message must appear in diagnostic.
	return 0; }
