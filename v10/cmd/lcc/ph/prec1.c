/* The Plum Hall Validation Suite for C
 * Unpublished copyright (c) 1986-1991, Chiron Systems Inc and Plum Hall Inc.
 * VERSION: 4
 * DATE: 1993-01-01
 * The "ANSI" mode of this suite corresponds to official ANSI C, X3.159-1989.
 * As per your license agreement, your distribution is not to be moved or copied outside the Designated Site
 * without specific permission from Plum Hall Inc.
 */

/*
 *	PREC1 - this module checks precedence by combining all
 *	pairs of operators. Note that PREC2 does the same thing
 *  in the other order.
 *			
 */
#include "defs.h"
void prec1a();
void prec1b();
void prec1c();
int main(argc, argv)
	int argc;
	char **argv;
	{
	extern int Debug;
	if (argc > 1)
		Debug = TRUE;

	Filename = "prec1.c";
	prec1a();
	prec1b();
	prec1c();
	return report("PREC1");
	}
