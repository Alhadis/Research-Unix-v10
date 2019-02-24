/* The Plum Hall Validation Suite for C
 * Unpublished copyright (c) 1986-1991, Chiron Systems Inc and Plum Hall Inc.
 * VERSION: 4
 * DATE: 1993-01-01
 * The "ANSI" mode of this suite corresponds to official ANSI C, X3.159-1989.
 * As per your license agreement, your distribution is not to be moved or copied outside the Designated Site
 * without specific permission from Plum Hall Inc.
 */

/* 3.8.1	Syn	SYNTAX-MANDATORY */
/* 	the #if is syntax-checked, and eval'ed, only after all substitutions */
int main() {
	#define A 0
	#define B /* nothing */
	#if A && B
	int i;
	#endif
	return 0; }
