/* The Plum Hall Validation Suite for C
 * Unpublished copyright (c) 1986-1991, Chiron Systems Inc and Plum Hall Inc.
 * VERSION: 4
 * DATE: 1993-01-01
 * The "ANSI" mode of this suite corresponds to official ANSI C, X3.159-1989.
 * As per your license agreement, your distribution is not to be moved or copied outside the Designated Site
 * without specific permission from Plum Hall Inc.
 */

/* 3.3.3.2	ConP1 (and 3.3.2.3 SemP1S2)	CONSTRAINT-MANDATORY */
/* 	fn-ret-struct().member is not an lvalue, and & needs lvalue */
int main() {
	struct structi { int a; };
	struct structi fstruct();
	int i = 3;
	int *pi = &i;
	pi = (int *)&fstruct().a;
	return 0; }
