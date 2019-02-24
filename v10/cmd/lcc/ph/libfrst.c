/* The Plum Hall Validation Suite for C
 * Unpublished copyright (c) 1986-1991, Chiron Systems Inc and Plum Hall Inc.
 * VERSION: 4
 * DATE: 1993-01-01
 * The "ANSI" mode of this suite corresponds to official ANSI C, X3.159-1989.
 * As per your license agreement, your distribution is not to be moved or copied outside the Designated Site
 * without specific permission from Plum Hall Inc.
 */

/*
 * LIB - standard library tests
 * FREESTANDING version 
 * only <float.h>, <limits.h>, <stdarg.h>, and <stddef.h>
 */
#define LIB_TEST 1		/* prevents inclusion of limits.h and float.h */
#include "defs.h"
void d4_1();
void d4_8();
int main(argc, argv)
	int argc;
	char **argv;
	{
	extern int Debug;

	if (argc > 1)
		Debug = TRUE;
#include "license.h"
	Filename = "libfrst.c";
	d4_1();
	d4_8();
	return report("LIBFRST");
	}
