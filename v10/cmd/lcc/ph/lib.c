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
 */
#define LIB_TEST 1		/* prevents inclusion of limits.h and float.h */
#include "defs.h"
void d4_1();
void d4_2();
void d4_3();
void d4_4();
void d4_5();
void d4_6();
void d4_7();
void d4_8();
void d4_9();
void d4_10();
void d4_11();
void d4_12();
static void save_errno();
int main(argc, argv)
	int argc;
	char **argv;
	{
	extern int Debug;

	if (argc > 1)
		Debug = TRUE;
	save_errno();
#include "license.h"
	Filename = "lib.c";
	d4_1();
	d4_2();
	d4_3();
	d4_4();
	d4_5();
	d4_6();
	d4_7();
	d4_8();
	d4_9();
	d4_10();
	d4_11();
	d4_12();
	return report("LIB");
	}
/* save_errno - pick up initial value of errno before any possible alteration
 */
#if ANSI && HAS_PROTOTYPES && !SKIP41
#include <errno.h>
int errno_init = -1;
static void save_errno()
	{
	errno_init = errno;
	}
#else
static void save_errno() { }
#endif /* ANSI etc */
/*
 * For "almost-ANSI" compilers that are missing parts of the latest ANSI library,
 * include stubs for these functions here.
 */
#if ANSI && !LIB_NOV_86
#include "stubs.c"
#endif
