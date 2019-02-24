/* The Plum Hall Validation Suite for C
 * Unpublished copyright (c) 1986-1991, Chiron Systems Inc and Plum Hall Inc.
 * VERSION: 4
 * DATE: 1993-01-01
 * The "ANSI" mode of this suite corresponds to official ANSI C, X3.159-1989.
 * As per your license agreement, your distribution is not to be moved or copied outside the Designated Site
 * without specific permission from Plum Hall Inc.
 */

/* 3.3.15	ConP2	CONSTRAINT-MANDATORY */
/* 	?: opnds 2 & 3 arith, compatible s/u/ptr type, ptrs-to-compatible-types, void exprs, obj-ptr:generic, or ptr:NULL */
int main() {
	int i = 3;
	static union ui {int a;} ui ;
	static const struct structi {int a;} cs = {0};
	void *genptr = &i;
	genptr = (void *)(i ? &ui : &cs);
	return 0; }
