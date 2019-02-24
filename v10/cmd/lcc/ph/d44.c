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
#if !ANSI
#define SKIP44 1
#endif


#include "flags.h"
#ifndef SKIP44

/*
 * 4.4 - Localization
 * This file is not relevant to pre ANSI C compilers.
 */
#include <string.h>
void d4_4();
void d4_4_1();
void d4_4_2();
#include <locale.h>

void d4_4()
	{
	char *p;

	/* make sure that the required macros produce constant expressions */
	static int lc_all =	LC_ALL;
	static int lc_collate = LC_COLLATE;
	static int lc_ctype = LC_CTYPE;
	static int lc_numeric = LC_NUMERIC;
	static int lc_time = LC_TIME;
#if ANSI8709
	static int lc_monetary = LC_MONETARY;
#if !defined(LC_MONETARY)
	complain(__LINE__);
#endif	/* LC_MONETARY */
#endif	/* ANSI8709 */

/* make sure that these are macros */
#if !defined(LC_ALL) || !defined(LC_COLLATE) || !defined(LC_CTYPE)
	complain(__LINE__);
#elif !defined(LC_NUMERIC) || !defined(LC_TIME) || !defined(NULL)
	complain(__LINE__);
#endif	/* defined macros */

	Filename = "d44.c";
	d4_4_1();
	d4_4_2();

#if ANSI8712
	/* make sure that NULL is defined in this file */
	p = NULL;
#endif	/* ANSI8712 */
	}

/*
 * 4.4.1 Locale Control
 */
void d4_4_1()
	{
#if ANSI8709
	extern void check_locale();

	/* 4.4.1.1 setlocale
	 * startup condition must be the "C" locale
	 */
	check_locale(__LINE__, LC_COLLATE);
	check_locale(__LINE__, LC_CTYPE);
	check_locale(__LINE__, LC_NUMERIC);
	check_locale(__LINE__, LC_TIME);
	check_locale(__LINE__, LC_MONETARY);
	check_locale(__LINE__, LC_ALL);
#endif	/* ANSI8709 */

	/* Each successful call to setlocale returns the NEW locale string.
	 * Change to the "native" environment
	 * (empty string) and check that non-NULL is returned.
	 */
	checkthat(__LINE__, setlocale(LC_COLLATE, "") != NULL);	
	checkthat(__LINE__, setlocale(LC_CTYPE, "") != NULL);	
	checkthat(__LINE__, setlocale(LC_NUMERIC, "") != NULL);	
	checkthat(__LINE__, setlocale(LC_TIME, "") != NULL);	
#if ANSI8709	
	checkthat(__LINE__, setlocale(LC_MONETARY, "") != NULL);	
#endif	
	checkthat(__LINE__, setlocale(LC_ALL, "") != NULL);	

	/* now reset to the "C" environment for the rest of the tests */
	setlocale(LC_ALL, "C");
	check_locale(__LINE__, LC_COLLATE);
	check_locale(__LINE__, LC_CTYPE);
	check_locale(__LINE__, LC_NUMERIC);
	check_locale(__LINE__, LC_TIME);
	check_locale(__LINE__, LC_MONETARY);
	check_locale(__LINE__, LC_ALL);
	}

#include <limits.h>	/* to get CHAR_MAX for use below */
/*
 * 4.4.2 Locale Control
 */
void d4_4_2()
	{
#if ANSI8709
	struct lconv *plc;
	/* the default locale is "C" ... make sure that the values are correct */
	plc = localeconv();
	stequals(__LINE__, plc->decimal_point, ".");
	stequals(__LINE__, plc->thousands_sep, "");
	stequals(__LINE__, plc->grouping, "");
	stequals(__LINE__, plc->int_curr_symbol, "");
	stequals(__LINE__, plc->currency_symbol, "");
	stequals(__LINE__, plc->mon_decimal_point, "");
	stequals(__LINE__, plc->mon_thousands_sep, "");
	stequals(__LINE__, plc->mon_grouping, "");
	stequals(__LINE__, plc->positive_sign, "");
	stequals(__LINE__, plc->negative_sign, "");
	iequals(__LINE__, plc->int_frac_digits, CHAR_MAX);
	iequals(__LINE__, plc->frac_digits, CHAR_MAX);
	iequals(__LINE__, plc->p_cs_precedes, CHAR_MAX);
	iequals(__LINE__, plc->p_sep_by_space, CHAR_MAX);
	iequals(__LINE__, plc->n_cs_precedes, CHAR_MAX);
	iequals(__LINE__, plc->n_sep_by_space, CHAR_MAX);
	iequals(__LINE__, plc->p_sign_posn, CHAR_MAX);
	iequals(__LINE__, plc->n_sign_posn, CHAR_MAX);
#endif	/* ANSI8709 */
	}

















/*
 * check_locale - make sure that the initial locale
 * is equivalent to the "C" locale.
 */
void check_locale(lineno, localenum)
	int lineno;
	int localenum;
	{
	char *p, buff[512];
	char *nolocale = "Unrecognized Locale String";

	/* get the (non-NULL) inital value of the locale */
	p = setlocale(localenum, NULL);
	if (p == NULL)
		{
		stequals(lineno, "initial locale", "C");
		return;
		}
	strncpy(buff, p, 512);

	/* this should be the same as setting to "C" locale */
	p = setlocale(localenum, "C");
	if (p == NULL)
		{
		stequals(lineno, "C locale", "C");
		return;
		}
	stequals(lineno, p, buff);

	/* now make sure that an unrecognized string returns
	 * NULL and leaves the locale unchanged.
	 */
	p = setlocale(localenum, nolocale);
	if (p != NULL)
		{
		stequals(lineno, "unrecognized locale", "NULL");
		return;
		}

	/* is it unchanged? */
	p = setlocale(localenum, NULL);
	if (p == NULL)
		{
		stequals(lineno, "unchanged locale", "C");
		return;
		}
	stequals(lineno, p, buff);
	}



#else	/* SKIP44 */
void d4_4() { pr_skip("d4_4: SKIPPED ENTIRELY\n"); }
#endif	/* SKIP44 */
