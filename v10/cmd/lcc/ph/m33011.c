/* The Plum Hall Validation Suite for C
 * Unpublished copyright (c) 1986-1991, Chiron Systems Inc and Plum Hall Inc.
 * VERSION: 4
 * DATE: 1993-01-01
 * The "ANSI" mode of this suite corresponds to official ANSI C, X3.159-1989.
 * As per your license agreement, your distribution is not to be moved or copied outside the Designated Site
 * without specific permission from Plum Hall Inc.
 */

/* 3.3.2.3	ConP1S1	CONSTRAINT-MANDATORY */
/* 	2nd opnd of . must be a member of 1st opnd */
int main() {
	static const struct structi {int a;} cs = {0};
	int i = 3;
	i = cs.i;
	return 0; }
