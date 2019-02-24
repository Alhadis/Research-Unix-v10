/* The Plum Hall Validation Suite for C
 * Unpublished copyright (c) 1986-1991, Chiron Systems Inc and Plum Hall Inc.
 * VERSION: 4
 * DATE: 1993-01-01
 * The "ANSI" mode of this suite corresponds to official ANSI C, X3.159-1989.
 * As per your license agreement, your distribution is not to be moved or copied outside the Designated Site
 * without specific permission from Plum Hall Inc.
 */

/* 3.6.4.2	ConP1S4	CONSTRAINT-MANDATORY */
/* 	at most one "default" label */
int main() {
	int i = 0;
	switch (i)
		{
		case 1:
			i = 0;
		default:
		default:
			i = 1;
		}
	return 0; }
