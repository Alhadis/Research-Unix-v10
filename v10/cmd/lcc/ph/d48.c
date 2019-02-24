/* The Plum Hall Validation Suite for C
 * Unpublished copyright (c) 1986-1991, Chiron Systems Inc and Plum Hall Inc.
 * VERSION: 4
 * DATE: 1993-01-01
 * The "ANSI" mode of this suite corresponds to official ANSI C, X3.159-1989.
 * As per your license agreement, your distribution is not to be moved or copied outside the Designated Site
 * without specific permission from Plum Hall Inc.
 */


#include "flags.h"
#ifndef SKIP48
#define LIB_TEST 1
#include "defs.h"

/*
 * 4.8 - Variable arguments <stdarg.h>
 * This file is not relevant for pre ANSI C compilers.
 */

#if !ANSI
void d4_8()
	{
	}
#else

#include <stdarg.h>
int d4_8_1(int, ...);

void d4_8()
	{
	Filename = "d48.c";

	/* make sure that va_start and va_arg are macros */
	#if !defined(va_start) || !defined(va_end)
		complain(__LINE__);
	#endif
	iequals(__LINE__, 1, d4_8_1(1, 1, 2, 3));
	iequals(__LINE__, 2, d4_8_1(2, 1., 2., 3.));
	}











#if NEW_STYLE_FN_DEF
int d4_8_1(int i, ...)
#else
int d4_8_1(i)
	int i;
#endif
	{
	va_list p;
	int j;

	va_start(p, i);
	for (j = 1; j <= 3; ++j)
		{
		if (i == 1)
			iequals(__LINE__, j, va_arg(p, int));
		else
			dequals(__LINE__, (double)j, va_arg(p, double));
		}
	va_end(p);
	return (i);
	}
#endif

#else /* if SKIP48 */
void d4_8() { pr_skip("d4_8: SKIPPED ENTIRELY\n"); }
#endif
