/* The Plum Hall Validation Suite for C
 * Unpublished copyright (c) 1986-1991, Chiron Systems Inc and Plum Hall Inc.
 * VERSION: 4
 * DATE: 1993-01-01
 * The "ANSI" mode of this suite corresponds to official ANSI C, X3.159-1989.
 * As per your license agreement, your distribution is not to be moved or copied outside the Designated Site
 * without specific permission from Plum Hall Inc.
 */

/* 3.3.2.3	ConP1S1	CONSTRAINT-MANDATORY */
/* 	1st opnd of . must have struct-or-union type */
int main() {
	#define a() 1
	#define dot() .
	#define one() 1
	double x = a()dot()one();	/* tokens  a  and  .  and  1  remain distinct - see 2.1.1.2 phase 7 */
	return 0; }
