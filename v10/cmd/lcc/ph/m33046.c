/* The Plum Hall Validation Suite for C
 * Unpublished copyright (c) 1986-1991, Chiron Systems Inc and Plum Hall Inc.
 * VERSION: 4
 * DATE: 1993-01-01
 * The "ANSI" mode of this suite corresponds to official ANSI C, X3.159-1989.
 * As per your license agreement, your distribution is not to be moved or copied outside the Designated Site
 * without specific permission from Plum Hall Inc.
 */

/* 3.3.9	ConP1S1	CONSTRAINT-MANDATORY */
/* 	equality opnds both arith, compatible ptr type, obj-or-incomplete ptr vs generic, or ptr vs NULL */
int main() {
	char a2c[2];
	long int li = 4;
	long *pli = &li;
	ff(a2c == pli);
	return 0; }
