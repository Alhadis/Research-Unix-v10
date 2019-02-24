/* The Plum Hall Validation Suite for C
 * Unpublished copyright (c) 1986-1991, Chiron Systems Inc and Plum Hall Inc.
 * VERSION: 4
 * DATE: 1993-01-01
 * The "ANSI" mode of this suite corresponds to official ANSI C, X3.159-1989.
 * As per your license agreement, your distribution is not to be moved or copied outside the Designated Site
 * without specific permission from Plum Hall Inc.
 */


#include "flags.h"
#ifndef SKIP42
#define LIB_TEST 1
#include "defs.h"
#if ANSI
	/* this definition should make the assertion disappear */
#define NDEBUG
#include <assert.h>
void d4_2_1b()
	{
	int i = ivalue(1);
	assert(i != 1);
	}
#endif

#endif /* SKIP42 */

