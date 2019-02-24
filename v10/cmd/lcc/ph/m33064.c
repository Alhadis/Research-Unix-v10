/* The Plum Hall Validation Suite for C
 * Unpublished copyright (c) 1986-1991, Chiron Systems Inc and Plum Hall Inc.
 * VERSION: 4
 * DATE: 1993-01-01
 * The "ANSI" mode of this suite corresponds to official ANSI C, X3.159-1989.
 * As per your license agreement, your distribution is not to be moved or copied outside the Designated Site
 * without specific permission from Plum Hall Inc.
 */

/* 3.3.16	ConP1 (and 3.3.15, 3.1.2.6, 3.5.3)	CONSTRAINT-MANDATORY */
/* 	result type of conditional "unions" the qualifiers, and const char isn't modifiable */
int main() {
	char c;
	char *p = &c;
	* (1 ? (char *)p : (const char *)p) = 0;
	return 0; }
