/* The Plum Hall Validation Suite for C
 * Unpublished copyright (c) 1986-1991, Chiron Systems Inc and Plum Hall Inc.
 * VERSION: 4
 * DATE: 1993-01-01
 * The "ANSI" mode of this suite corresponds to official ANSI C, X3.159-1989.
 * As per your license agreement, your distribution is not to be moved or copied outside the Designated Site
 * without specific permission from Plum Hall Inc.
 */

/* 3.6.5	ConP1S1	CONSTRAINT-MANDATORY */
/* 	iteration conditions shall have scalar type */
int main() {
	static struct { int i; } si = {0};
	while (si) ++si.i;
	return 0; }
