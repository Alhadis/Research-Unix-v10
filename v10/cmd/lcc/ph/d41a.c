/* The Plum Hall Validation Suite for C
 * Unpublished copyright (c) 1986-1991, Chiron Systems Inc and Plum Hall Inc.
 * VERSION: 4
 * DATE: 1993-01-01
 * The "ANSI" mode of this suite corresponds to official ANSI C, X3.159-1989.
 * As per your license agreement, your distribution is not to be moved or copied outside the Designated Site
 * without specific permission from Plum Hall Inc.
 */

#define LIB_TEST 1
#include "defs.h"
#if !ANSI || !HAS_PROTOTYPES
#define SKIP41 1
#endif

#include "flags.h"
#ifndef SKIP41
#include <errno.h>

/*
 * 4.1.3 - Errors <errno.h>
 */
void d4_1_3()
	{
	extern int errno_init;	/* set in lib.c */
	Filename = "d41a.c";
	/* make sure that EDOM and ERANGE are macros */
	#if !defined(EDOM) || !defined(ERANGE)
		complain(__LINE__);
	#endif

	/* errno is a modifiable lvalue initialized to 0 */
	iequals(__LINE__, errno_init, 0);
	errno = 1;
	iequals(__LINE__, errno, 1);
	errno = 0;
	iequals(__LINE__, errno, 0);
	inotequals(__LINE__, EDOM, 0);
	inotequals(__LINE__, ERANGE, 0);
	}
#else
int skp41a = 1;	/* must have one def */
#endif /* SKIP41 */
