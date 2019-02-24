/* The Plum Hall Validation Suite for C
 * Unpublished copyright (c) 1986-1991, Chiron Systems Inc and Plum Hall Inc.
 * VERSION: 4
 * DATE: 1993-01-01
 * The "ANSI" mode of this suite corresponds to official ANSI C, X3.159-1989.
 * As per your license agreement, your distribution is not to be moved or copied outside the Designated Site
 * without specific permission from Plum Hall Inc.
 */


#include "flags.h"
#ifndef SKIP46
#define LIB_TEST 1
#include "defs.h"

/*
 * 4.6 - Non-Local jumps
 */

#include <setjmp.h>
void d4_6x();

static jmp_buf buffer;
static int count = 0;
void d4_6()
	{
#if ANSI
	volatile int i;
#else
	int i;
#endif

	Filename = "d46.c";

	/* 4.6.1.1 setjmp
	 */

	/* test in the context of an if */
	if (setjmp(buffer) == 0)	
		{
		/* first time through */
		iequals(__LINE__, count, 0);
		d4_6x();
		}
	else
		iequals(__LINE__, count, 1);
	





	/* now make sure that it works in the context of a switch */
	count = 0;
	switch (setjmp(buffer))
		{
	case 0:	/* initial call, no longjmp */
		iequals(__LINE__, count, 0);
		break;
	case 2:	/* after first longjmp call */
		iequals(__LINE__, count, 1);
#if ANSI
		iequals(__LINE__, i++, 17);
#else
		iequals(-__LINE__, i++, 17);
#endif
		d4_6x();
		break;
	case 1:	/* after second longjmp call */
		/* longjmp with arg of 0 makes setjmp return non-zero */
		iequals(__LINE__, count, 2);
#if ANSI
		iequals(__LINE__, i, 18);
#else
		iequals(-__LINE__, i, 18);
#endif
		return;
		}

	i = 17;
	d4_6x();
	}

void d4_6x()
	{
	/* 4.6.1.2 longjmp
	 */
	if (count++ == 0)
		longjmp(buffer, 2);
	else
		longjmp(buffer, 0);
	
	/* this statement never reached */
	complain(__LINE__);
	}
	

#else /* if SKIP46 */
void d4_6() { pr_skip("d4_6: SKIPPED ENTIRELY\n"); }
#endif
