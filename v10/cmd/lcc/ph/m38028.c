/* The Plum Hall Validation Suite for C
 * Unpublished copyright (c) 1986-1991, Chiron Systems Inc and Plum Hall Inc.
 * VERSION: 4
 * DATE: 1993-01-01
 * The "ANSI" mode of this suite corresponds to official ANSI C, X3.159-1989.
 * As per your license agreement, your distribution is not to be moved or copied outside the Designated Site
 * without specific permission from Plum Hall Inc.
 */

/* 3.8.3	ConP3S1	CONSTRAINT-MANDATORY */
/* 	re-#define'ing function-like macro fails if number of tokens differs */
int main() {
	#define A(a) 1
	#define A(a) (1)
	return 0; }
