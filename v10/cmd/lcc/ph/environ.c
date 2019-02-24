/* The Plum Hall Validation Suite for C
 * Unpublished copyright (c) 1986-1991, Chiron Systems Inc and Plum Hall Inc.
 * VERSION: 4
 * DATE: 1993-01-01
 * The "ANSI" mode of this suite corresponds to official ANSI C, X3.159-1989.
 * As per your license agreement, your distribution is not to be moved or copied outside the Designated Site
 * without specific permission from Plum Hall Inc.
 */

#include "flags.h"
#include "defs.h"
#if !ANSI || !TRIGRAPHS
#define SKIP2 1
#endif
#ifndef SKIP2
#include "b2.h"
#else
int main() { pr_skip("environ(b2): SKIPPED ENTIRELY\n"); return report("ENVIRON"); }
#endif /* SKIP2 */
