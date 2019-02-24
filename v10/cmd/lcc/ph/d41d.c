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
#include <stddef.h>

/*
 * 4.1.5 - Common definitions <stddef.h>
 */
void d4_1_5()
	{
	char *p, *q;
	struct s1
		{
		char c;
		int d;
		};
#if WIDE_CHARS
	char c1 = 'a';
	wchar_t w1;
#endif

	Filename = "d41d.c";

	iequals(__LINE__, sizeof(ptrdiff_t), sizeof(p - q)); 
	iequals(__LINE__, offsetof(struct s1, c), 0);
	checkthat(__LINE__, offsetof(struct s1, c) < offsetof(struct s1, d));
#if WIDE_CHARS
	iequals(__LINE__, sizeof(size_t), sizeof(sizeof(int)));
	checkthat(__LINE__, sizeof(wchar_t) >= sizeof(char));

	/* representation as char must == representation as wchar_t */
	w1 = (wchar_t)c1;
	iequals(__LINE__, w1, c1);
	c1 = (char)w1;
	iequals(__LINE__, w1, c1);
	p = NULL;
#endif
	}

#else
int skp41d = 1;	/* must have one def */
#endif	/* SKIP41 */
