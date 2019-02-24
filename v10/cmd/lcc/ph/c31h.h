/* The Plum Hall Validation Suite for C
 * Unpublished copyright (c) 1986-1991, Chiron Systems Inc and Plum Hall Inc.
 * VERSION: 4
 * DATE: 1993-01-01
 * The "ANSI" mode of this suite corresponds to official ANSI C, X3.159-1989.
 * As per your license agreement, your distribution is not to be moved or copied outside the Designated Site
 * without specific permission from Plum Hall Inc.
 */

/* c31h.h - This header hides some new ANSI lexical stuff from older conformance levels.
 */

	Filename = __FILE__;
	/* ANSI allows backslash in C text. Earlier standards did not. */
	iequals(__LINE__, (a+b-c),  (a +	b  \
							/* */-
							c));
	/* This file ends with vertical tab, formfeed, newline. */

