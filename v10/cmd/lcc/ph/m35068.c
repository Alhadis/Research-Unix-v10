/* The Plum Hall Validation Suite for C
 * Unpublished copyright (c) 1986-1991, Chiron Systems Inc and Plum Hall Inc.
 * VERSION: 4
 * DATE: 1993-01-01
 * The "ANSI" mode of this suite corresponds to official ANSI C, X3.159-1989.
 * As per your license agreement, your distribution is not to be moved or copied outside the Designated Site
 * without specific permission from Plum Hall Inc.
 */

/* 3.5.4.3	SemP6S4 (and 3.5 ConP3S1)	CONSTRAINT-MANDATORY */
/* 	... and the type of each proto parm shall be compatible with the widened def parm */
	int f(char); 
	int f(c) char c; { }
