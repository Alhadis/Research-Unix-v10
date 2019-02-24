/* The Plum Hall Validation Suite for C
 * Unpublished copyright (c) 1986-1991, Chiron Systems Inc and Plum Hall Inc.
 * VERSION: 4
 * DATE: 1993-01-01
 * The "ANSI" mode of this suite corresponds to official ANSI C, X3.159-1989.
 * As per your license agreement, your distribution is not to be moved or copied outside the Designated Site
 * without specific permission from Plum Hall Inc.
 */


#include "flags.h"
#ifndef SKIP43
#define LIB_TEST 1
#include "defs.h"

void d4_3_1();
void d4_3_real();

/*
 * 4.3 - Character handling.
 * Note that these tests assume the "C" locale.
 */
void d4_3()
	{
	d4_3_1();
	d4_3_real();
	}

#if (!ANSI && !ALL_CTYPE_FNS)
#define isgraph(c) (isprint(c) && (c)!=' ')
#define toupper(c) (islower(c) ? (c)+('A'-'a') : (c))
#define tolower(c) (isupper(c) ? (c)+('a'-'A') : (c))
#endif

#include <stdio.h>
#include <ctype.h>
size_t strlen();
int in_set();
/*
 * 4.3.1 Character testing functions
 */
void d4_3_1()
	{
#include "d43h.h"
	}







/*
 * IN_SET - check that the character is in the set. We
 * won't try to use the string library functions.
 */
int in_set(i, string)
	int i;
	char *string;
	{
	while (*string)
		{
		if (*string == i)
			return(TRUE);
		++string;
		}
	return(FALSE);
	}

/* 
 * D4_3_REAL: undefine all of the macros and do the
 * same set of tests for the underlying functions.
 */
#undef isalpha
#undef isupper
#undef islower
#undef isdigit
#undef isxdigit
#undef isspace
#undef ispunct
#undef isalnum
#undef isprint
#undef isgraph
#undef iscntrl
#undef toupper
#undef tolower
void d4_3_real()
	{
#if ANSI
#include "d43h.h"
#endif
	}

#else /* if SKIP43 */
void d4_3() { pr_skip("d4_3: SKIPPED ENTIRELY\n"); }
#endif
