/* The Plum Hall Validation Suite for C
 * Unpublished copyright (c) 1986-1991, Chiron Systems Inc and Plum Hall Inc.
 * VERSION: 4
 * DATE: 1993-01-01
 * The "ANSI" mode of this suite corresponds to official ANSI C, X3.159-1989.
 * As per your license agreement, your distribution is not to be moved or copied outside the Designated Site
 * without specific permission from Plum Hall Inc.
 */

#include "flags.h"
#ifndef SKIP38
/*
 * 3.8 - Preprocessor directives
 */
#include "defs.h"

static char save_date[20], save_time[20];
static char tmp_str[20];
static int f(n) int n; { return (n); }

void c3_8_1();
void c3_8_2();
void c3_8_3();
void c3_8_4();
void c3_8_5();
void c3_8_6();
void c3_8_7();
void c3_8_8();

void c3_8()
	{
	Filename = "c38.c";

#if ANSI8709
	str_cpye(save_date, __DATE__);	/* to be tested in c3_8_8 */
	str_cpye(save_time, __TIME__);
#endif
	c3_8_1();
	c3_8_2();
	c3_8_3();
	c3_8_4();
	c3_8_5();
	c3_8_6();
	c3_8_7();
	c3_8_8();
	}



/*
 * 3.8.1 - conditional inclusion
 */
void c3_8_1()
	{
#define X 0
#define Y 1
	int i, j, k;

	i = j = k = 0;

#if 0
	complain(__LINE__);
#endif

#if ANSI
	#include "c381.h"
	/* contents of included file: 
	 *	Filename = "c381.h";
	 *	#if !defined X
	 *		syntax error !!!
	 *	#elif !defined (X)
	 *		syntax error !!!
	 *	#endif
	 *	#define c3835 1
	 */
	Filename = "c38.c";
	#if c3835 != 1
		complain(__LINE__);
	#endif
#endif
#if !~~0 == (1*2/3%4+6-5^7&8|9) || 10 > 11 && 12 >= 13
	complain(__LINE__);
#endif
#if UNDEFINED_NAME
	complain(__LINE__);
#endif
#if X
	complain(__LINE__);
#endif
#if ANSI
	#if ~0 > 2147483647
		complain(__LINE__);
	#endif

	#if '\33' != 033			/* interpretation of escapes is implem-defined */
		complain(- __LINE__);
	#endif

	#if Y
		i = 3;
	#elif Y
		i = 4;
	#endif
	iequals(__LINE__, i, 3);
#endif

/* test nesting and else clauses */
#ifdef /* comments can appear anywhere */X
#ifndef UNDEFINED_NAME	/* comments can appear anywhere */
	i = 7;
#else
	complain(__LINE__);
#endif
#else
	complain(__LINE__);
#endif	/* comments can appear anywhere */
	iequals(__LINE__, i, 7);
	}

/*
 * 3.8.2  - Source file inclusion
 */
#if ANSI
#define NXSTR(a) #a			/* See 3.8.3.2 */
#include NXSTR(stdarg.h)	/* produces "stdarg.h", then looks for <stdarg.h> */
#endif /* ANSI */

void c3_8_2()
	{
	char *p = NULL;
#if ANSI
	#  define INCLUDE "c382a.h"
	#	include INCLUDE
#else
#include "c382a.h"
#endif /* ANSI */
	/* contents of "c382a.h":  	#include "c382b.h"
	 * contents of "c382b.h":	int i;
	 */
	i = 1;	/* syntax error if the include is wrong */
#if ANSI && !defined(va_start)	/* to prove inclusion of <stdarg.h> */
	complain(__LINE__);	
#endif
	}


/*
 * 3.8.3 - Macro replacement
 */
void c3_8_3()
	{
#define INT_I_J_K int i, j, k;	/* object-like macro */
#define NOTHING
#define INIT() = 1;				/* fn-like macro, no args */

	INT_I_J_K		/* semi-colon is part of macro */

	NOTHING			/* syntax error if anything remains */
	i INIT()		/* semi-colon is part of def -- i = 1; */
	j INIT()
	k INIT()

#if ANSI
	#define OBJ 1 + 1
	#define OBJ 1  +/* comment is whitespace */1
	#define OBJ    1   +   1
	# define/* */OBJ 1 + 1
	iequals(__LINE__, OBJ, 2);

	#define FN(a) 1 + 1
	#define FN(a) 1  +  /* comment is whitespace */ 1
	#define FN(a)    1   +   1
	# define  /* */ FN(a) 1 + 1
	iequals(__LINE__, FN(0), 2);

	#define define 1
	# define A 2
	iequals(__LINE__, A, 2);
	#undef define
	#undef A
#endif
/* comments can be embedded in other preprocessor statements */
#define /* */ SUM(c, /* */b, a) (a+b+/* */c /* */)

	/* grouping parenthesis and commas inside don't affect expansion 
	 * Also, comments can now be ANYWHERE in the text. */
#if ANSI
	iequals(__LINE__, SUM(/* */(7,/* */i),
		(((3,j))),k), 3);
#endif
	/* make sure that macro expansion is literal, e.g., does no C grouping */
#define ADD(a,b) a+b
	iequals(__LINE__, ADD(2,3)*4, 14);


																					/* 3.8.3 (cont.) */
	/* character constants and string literals are not scanned */
	inotequals(__LINE__, 'X', '0');
	inotequals(__LINE__, "X"[0], '0');

	/* nested definitions -- also, commas in expansion don't affect the expansion */
#define A B
#define B (j=1,C)
#define C X
	iequals(__LINE__, A, 0);

#if ANSI

	/* 3.8.3.1 - Argument substitution */
	#define f(a) (2 * f(a) + 1)
	iequals(__LINE__, f(f(1)), 7);
	#undef f

	/* 3.8.3.2 - The # operator */
	#define STR(a) NXSTR(a)
	#define NXSTR(a) #a
	#define NAME  /* whitespace */ abc

	stequals(__LINE__, STR("\a"), "\"\\a\"");
	stequals(__LINE__, STR(\7), "\7");
	stequals(__LINE__, STR(NAME), "abc");
	stequals(__LINE__, NXSTR(NAME), "NAME");
	stequals(__LINE__, NXSTR(a b), "a b");
	stequals(__LINE__, NXSTR(  a  ), "a");
	stequals(__LINE__, STR(Y.1), "1.1");
	#define f h
	#define h(a) a+f
	str_cpye(tmp_str, STR( f(1)(2) ));
	if (str_cmp(tmp_str, "1+f(2)") != 0 && str_cmp(tmp_str, "1+h(2)") != 0)
		stequals(__LINE__, tmp_str, "NEITHER '1+f(2)' NOR '1+h(2)'");

	#undef f
	#undef h












	/* 3.8.3.3 - The ## operator */
	#if MACRO_NOV_86
	#define CAT(a, b) NXCAT(a, b)
	#define NXCAT(a, b) a ## b
	#define Xx  x
		{
		int xy = 1;

		iequals(__LINE__, NXCAT(x, y), 1);
		iequals(__LINE__, CAT(Xx, y), 1);
		}

	#define pp1 x ## y
	#define pp3(a) x ## a
	#define xy b
	#define cat3() x ## x ## y
	#define op1(a) a
	#define op(n, a) op ## n (a)
		{
		int xxy = 1;
		int b = 2;
		int x = 3;

		iequals(__LINE__, pp3(xy), 1);		/* -> xxy */
		iequals(__LINE__, pp1, 2);			/* -> xy -> b */
		iequals(__LINE__, pp3(y+2), 4);		/* -> xy + 2 -> b + 2 */
		iequals(__LINE__, pp3(xy + xy), 3);	/* -> xxy + xy -> xxy + b */
		iequals(__LINE__, cat3(), 1);		/* -> xxy */
		iequals(__LINE__, op(1, xxy), 1);	/* -> op1(xxy) -> xxy */
		}
	#endif /* MACRO_NOV_86 */

	/* 3.8.3.4 - Re-scanning and further replacement */
	#define f(a) f(1 + a
	#define apply(f, n) f(n)
		{
		#if MACRO_NOV_86
		iequals(__LINE__, f(1) ), 2);		/* -> f(1 + 1 ) */
		iequals(__LINE__, f(f(2)) )), 4);	/* -> f(1 + f(1 + 2 )) */
		iequals(__LINE__, f(1) + f(1) )), 4); /* -> f(1 + 1 + f(1 + 1 )) */
		iequals(__LINE__, apply(f, 2) ), 3); /* -> f(2) ) -> f(1 + 2 ) */
		#endif
		#undef f
		#define f g
		#define g f
		iequals(__LINE__, f(1), 1);		/* -> g(1) -> f(1) */
		#define checkthat  checkthat ( __LINE__, 0 == 0
		checkthat );
		#undef checkthat
		}
	/* 3.8.3.5 - Scope of macro definitions */
	#if !defined c3835
		complain(__LINE__);
	#endif
	#undef c3835
	#if defined c3835
		complain(__LINE__);
	#endif
	#undef c3835

#endif

	/* undef */
#undef INIT
#define INIT = 2;
	i INIT
	j INIT
	k INIT
	iequals(__LINE__, SUM(i,j,k), 6);

	/* __LINE__ and __FILE__ have been checked throughout */

	} /* end c3_8_3 */

/*
 * 3.8.4 - line control
 */
void c3_8_4()
	{
	int i;

#if (V7 || ANSI)
#line 32767
	i = __LINE__;
#line 336 "newfile"
	iequals(__LINE__, i, 32767);
	checkthat( - __LINE__, str_cmp(__FILE__, "newfile") == 0);
#line 339 "c38.c"
	iequals(__LINE__, __LINE__, 339);
	checkthat( - __LINE__, str_cmp(__FILE__, "c38.c") == 0);
#if ANSI
	#define ALINE 10306 "X38.c"

	#line ALINE
	i = __LINE__;
	#line 347
	iequals(__LINE__, i, 10306);
	checkthat( - __LINE__, str_cmp(__FILE__, "X38.c") == 0);
	#line 350 "c38.c"
#endif /* ANSI */
#endif /* V7 || ANSI */
	} /* end c3_8_4 */

/*
 * 3.8.5  - #error directive
 */
void c3_8_5()
	{
	/* #error is tested in diagnostic-tests */
	}

/*
 * 3.8.6  - #pragma directive
 */
void c3_8_6()
	{
#if ANSI
	#include "c386.h"
	/* contents of included file: 
	 *	#pragma ignore_this_unimplemented_pragma
	 */
#endif
	}

/*
 * 3.8.7  - Null directive
 */
void c3_8_7()
	{
#if ANSI
	#	/* comments */		/* and other whitespace */  /* allowed */
	#
	#
#endif
	}
















/*
 * 3.8.8  - Predefined macro names
 */
void c3_8_8()
	{
#if ANSI
	/* __LINE__ and __FILE__ already tested */
#if ANSI8709
	stequals(__LINE__, save_date, __DATE__);
	stequals(__LINE__, save_time, __TIME__);
#endif /* ANSI8709 */
	iequals(__LINE__, str_len(__DATE__), 11);
	iequals(__LINE__, __DATE__[3], ' ');
	iequals(__LINE__, __DATE__[6], ' ');
	iequals(__LINE__, __DATE__[7], '1');
	iequals(__LINE__, __DATE__[8], '9');
	iequals(__LINE__, str_len(__TIME__), 8);
	iequals(__LINE__, __TIME__[2], ':');
	iequals(__LINE__, __TIME__[5], ':');
	iequals(__LINE__, __STDC__, 1);
#else
#ifdef __STDC__
	warning -- __STDC__ should only be pre-defined in conforming environment
#endif /* __STDC__ */
#endif /* ANSI */
	} /* end c3_8 */

#else /* if SKIP38 */

void c3_8() { pr_skip("c3_8: SKIPPED ENTIRELY\n"); }
#endif /* SKIP38 */

