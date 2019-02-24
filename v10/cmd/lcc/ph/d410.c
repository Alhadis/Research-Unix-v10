/* The Plum Hall Validation Suite for C
 * Unpublished copyright (c) 1986-1991, Chiron Systems Inc and Plum Hall Inc.
 * VERSION: 4
 * DATE: 1993-01-01
 * The "ANSI" mode of this suite corresponds to official ANSI C, X3.159-1989.
 * As per your license agreement, your distribution is not to be moved or copied outside the Designated Site
 * without specific permission from Plum Hall Inc.
 */


#include "flags.h"
#ifndef SKIP410
#define LIB_TEST 1
#include "defs.h"

/*
 * 4.10 - General Utilities <stdlib.h>
 */

#if ANSI
#include <stdlib.h> 
#if ANSI8703
#include <errno.h>
#endif	/* ANSI8703 */
#else	/* if !ANSI */
#include <stdio.h>	/* for NULL, at least */
extern int errno;
double atof();
int atoi();
long atol();
char *calloc(), *malloc(), *realloc();
void free();
void abort();
void exit();
char *getenv();
int system();
#endif 	/* ANSI */

int strcmp();
size_t strlen();

#include <math.h>
static void d4_10_1();
static void d4_10_2();
static void d4_10_3();
static void d4_10_4();
static void d4_10_4x();
static void d4_10_5();
static void d4_10_6();
static void d4_10_7();
static void d4_10_8();


#ifndef SKIP4_101
static char *string1 = "1.234 2.3e99999999999999999999";
static char *string2 = "         123 10";
static char *string3 = "999999999999999999999999999999999999999999999999999999";
static char *string4 = "-99999999999999999999999999999999999999999999999999999";
static char *string5 = "   XXX";
#endif /* SKIP4_101 */

void d4_10()
	{

	Filename = "d410.c";
#if ANSI
	{
	/* check existence of types */
 	div_t dummy1;
 	ldiv_t dummy2;
#if WIDE_CHARS
	wchar_t wc;
#endif /* WIDE_CHARS */
	size_t si;
	int i;

	/* check definition of required macros */
	#if !defined(EXIT_FAILURE)
		complain(__LINE__);
	#elif !defined(EXIT_SUCCESS)
		complain(__LINE__);
	#elif !defined(RAND_MAX)
		complain(__LINE__);
	#elif !defined(MB_CUR_MAX)
		complain(__LINE__);
	#endif

 	checkthat(__LINE__, sizeof(div_t) >= 2*sizeof(int));
 	checkthat(__LINE__, sizeof(ldiv_t) >= 2*sizeof(long));
#if WIDE_CHARS
	checkthat(__LINE__, MB_CUR_MAX >= 1);
#endif /* WIDE_CHARS */

	/* make sure that these expand to integral expressions (compile
	 * fails if not).
	 */
	i = ivalue(-99); if (i == EXIT_FAILURE) do_nothing(&si);
	i = ivalue(-99); if (i == EXIT_SUCCESS) do_nothing(&si);
	i = ivalue(-99); switch (i) { case RAND_MAX: do_nothing(&si); }
	}
#endif	/* ANSI */
	d4_10_1();
	d4_10_2();
	d4_10_3();
	d4_10_4();
	d4_10_5();
	d4_10_6();
	d4_10_7();
	d4_10_8();
	}

#if ANSI
#include <limits.h>
#include <float.h>
#endif	/* ANSI */
/*
 * 4.10.1 String conversion functions
 */
static void d4_10_1()
	{
#ifndef SKIP4_101
	char *p;
	int i;
	long l;
	double d;
	
	/* atof, atoi, atol do not affect errno */
	errno = 99;

	/* 4.10.1.1 atof
	 * text to float
	 */
	dequals(__LINE__, atof("XXX"), 0.0);
	dequals(__LINE__, atof("\f\t\n\r\v1.234"), 1.234);
	dequals(__LINE__, atof("1.234"), 1.234);
	dequals(__LINE__, atof(" 	 +1.234"), 1.234);
	dequals(__LINE__, atof(" 	 +1.234"), 1.234);
	dequals(__LINE__, atof(" 	 -1.234"), -1.234);
	dequals(__LINE__, atof(" 	 +1.234e1"), 1.234e1);
	dequals(__LINE__, atof(" 	 +1.234E1"), 1.234E1);
	dequals(__LINE__, atof(" 	 +1.234E-1"), 1.234E-1);
	dequals(__LINE__, atof(" 	 +1.234XE-1"), 1.234);

	/* 4.10.1.2 atoi
	 * text to int
	 */
	iequals(__LINE__, atoi("XXX"), 0);
	iequals(__LINE__, atoi("123"), 123);
	iequals(__LINE__, atoi("+123"), 123);
	iequals(__LINE__, atoi("-123"), -123);
	iequals(__LINE__, atoi(" 	+123"), 123);
	iequals(__LINE__, atoi(" 	0123"), 123);
	iequals(__LINE__, atoi(" 	0x123"), 0);
	iequals(__LINE__, atoi(" 	12a3"), 12);

	/* 4.10.1.3 atol
	 * text to long
	 */
	lequals(__LINE__, atol("XXX"), 0L);
	lequals(__LINE__, atol("123456"), 123456);
	lequals(__LINE__, atol("+123456"), 123456);
	lequals(__LINE__, atol("-123456"), -123456);
	lequals(__LINE__, atol(" 	+123456"), 123456);
	lequals(__LINE__, atol(" 	0123456"), 123456);	/* not octal !! */
	lequals(__LINE__, atol(" 	0x123456"), 0L);
	lequals(__LINE__, atol(" 	12a3"), 12L);

	errno = 0;

#if ANSI
	/* 4.10.1.4 strtod
	 * Same as atof, but gets pointer to end character and error checking
	 */
	dequals(__LINE__, strtod("XXX", &p), 0.0);
	dequals(__LINE__, strtod("\f\t\n\r\v1.1", &p), 1.1);
	dequals(__LINE__, strtod("1.234", &p), 1.234);
	dequals(__LINE__, strtod(" 	 +1.234", &p), 1.234);
	dequals(__LINE__, strtod(" 	 +1.234", &p), 1.234);
	dequals(__LINE__, strtod(" 	 -1.234", &p), -1.234);
	dequals(__LINE__, strtod(" 	 +1.234e1", &p), 1.234e1);
	dequals(__LINE__, strtod(" 	 +1.234E1", &p), 1.234E1);
	dequals(__LINE__, strtod(" 	 +1.234E-1", &p), 1.234E-1);
	dequals(__LINE__, strtod(" 	 +1.234XE-1", &p), 1.234);

	/* on no conversion, end pointer doesn't move */
	d = strtod(string5+3, &p);
	aequals(__LINE__, p, string5+3);
	d = strtod(string5, &p);
	aequals(__LINE__, p, string5);

	/* endptr gets pointer to remaining string */
	dequals(__LINE__, strtod(string1, &p), 1.234);
	aequals(__LINE__, p++, string1+5);

	/* overflow returns +/- HUGE_VAL */
	errno = 0;
	dequals(__LINE__, strtod(p, NULL), HUGE_VAL);
	iequals(__LINE__, errno, ERANGE);
	errno = 0;
	dequals(__LINE__, strtod("-1e99999999999999999999", NULL), -HUGE_VAL);
	iequals(__LINE__, errno, ERANGE);

	/* underflow returns 0 */
	errno = 0;
	dequals(__LINE__, strtod("1e-9999999999999999", NULL), 0.0);
	iequals(__LINE__, errno, ERANGE);

	/* 4.10.1.5 strtol
	 * same as atol but with pointer, base, and error checking
	 */
	lequals(__LINE__, strtol("XXX", &p, 0), 0L);
	lequals(__LINE__, strtol(string2, &p, 8), 0123L);
	aequals(__LINE__, p++, string2+12);

	/* check out bases */
	for (i = 2; i <= 36; ++i)
		lequals(__LINE__, strtol("11", NULL, i), (long)(i+1));
	lequals(__LINE__, strtol("zzz", NULL, 36), (((35L*36L)+35L)*36L)+35L);
		
	/* 0 base uses default C parsing rules */
	lequals(__LINE__, strtol("0x12345", NULL, 0), 0x12345L);
	lequals(__LINE__, strtol("0123", NULL, 0), 0123L);
	lequals(__LINE__, strtol("	 	 -123", NULL, 0), -123L);
	lequals(__LINE__, strtol("	 	 +123", NULL, 0), 123L);

	/* otherwise, same as atol */
	lequals(__LINE__, strtol("\f\t\n\r\v7", &p, 10), 7L);
	lequals(__LINE__, strtol("123456", &p, 10), 123456L);
	lequals(__LINE__, strtol("+123456", &p, 10), 123456L);
	lequals(__LINE__, strtol("-123456", &p, 10), -123456L);
	lequals(__LINE__, strtol(" 	+123456", &p, 10), 123456L);
	lequals(__LINE__, strtol(" 	0123456", &p, 10), 123456L);
	lequals(__LINE__, strtol(" 	0x123456", &p, 10), 0L);
	lequals(__LINE__, strtol(" 	0x123456", &p, 16), 0x123456L);
	lequals(__LINE__, strtol(" 	-0X123456", &p, 16), -0x123456L);
	lequals(__LINE__, strtol(" 	12a3", &p, 10), 12L);

	/* on no conversion, end pointer doesn't move */
	d = strtol(string5+3, &p, 10);
	aequals(__LINE__, p, string5+3);
	d = strtol(string5, &p, 10);
	aequals(__LINE__, p, string5);

	/* error checking */
	errno = 0;
	lequals(__LINE__, strtol(string3, NULL, 0), LONG_MAX);
	iequals(__LINE__, errno, ERANGE);
	errno = 0;
	lequals(__LINE__, strtol(string4, NULL, 0), LONG_MIN);
	iequals(__LINE__, errno, ERANGE);



	/* 4.10.1.6 strtoul
	 * same conversions as strtol, but to unsigned long
	 */
	lequals(__LINE__, strtoul("XXX", &p, 0), 0L);
	lequals(__LINE__, strtoul(string2, &p, 8), 0123L);
	aequals(__LINE__, p++, string2+12);

	/* check out bases */
	for (i = 2; i <= 36; ++i)
		lequals(__LINE__, strtoul("11", NULL, i), (long)(i+1));
	lequals(__LINE__, strtoul("zzz", NULL, 36), (((35L*36L)+35L)*36L)+35L);
		
	/* 0 base uses default C parsing rules */
	lequals(__LINE__, strtoul("0x12345", NULL, 0), 0x12345L);
	lequals(__LINE__, strtoul("0123", NULL, 0), 0123L);

	lequals(__LINE__, strtoul("\f\t\n\r\v7", &p, 10), 7L);
	lequals(__LINE__, strtoul("123456", &p, 10), 123456L);
	lequals(__LINE__, strtoul(" 	0123456", &p, 10), 123456L);
	lequals(__LINE__, strtoul(" 	0x123456", &p, 10), 0L);
	lequals(__LINE__, strtoul(" 	0x123456", &p, 16), 0x123456L);
	lequals(__LINE__, strtoul(" 	0X123456", &p, 16), 0x123456L);
	lequals(__LINE__, strtoul(" 	12a3", &p, 10), 12L);
#if ANSI8712
	lequals(__LINE__, strtoul(" 	+12", &p, 10), 12L);
#endif	/* ANSI8712 */

	/* on no conversion, end pointer doesn't move */
	d = strtoul(string5+3, &p, 10);
	aequals(__LINE__, p, string5+3);
	d = strtoul(string5, &p, 10);
	aequals(__LINE__, p, string5);

	/* error checking */
	errno = 0;
	lequals(__LINE__, strtoul(string3, NULL, 0), ULONG_MAX);
	iequals(__LINE__, errno, ERANGE);
#endif	/* ANSI */
#endif /* SKIP4_101 */
	}

/*
 * 4.10.2 - Random number generation functions.
 */
static void d4_10_2()
	{
#ifndef SKIP4_102
	int i1, i2, i3, j1, j2, j3;

	/* 4.10.2.1 rand
	 * before ansi, all we can count on is that the sequence exists
	 */
	i1 = rand();
	i2 = rand();
	i3 = rand();

	/* 4.10.2.2 srand
	 * that it can be reset to the default
	 */
	srand(1);
	j1 = rand();
	j2 = rand();
	j3 = rand();
	iequals(__LINE__, i1, j1);
	iequals(__LINE__, i2, j2);
	iequals(__LINE__, i3, j3);

	/* and reset arbitrarily */
	srand(11);
	i1 = rand();
	i2 = rand();
	i3 = rand();
	srand(11);
	j1 = rand();
	j2 = rand();
	j3 = rand();
	iequals(__LINE__, i1, j1);
	iequals(__LINE__, i2, j2);
	iequals(__LINE__, i3, j3);

#if ANSI
	/* now the range is specified */
	for (i1 = 0; i1 < 30; ++i1)
		{
		j1 = rand();
		checkthat(__LINE__, 0 <= j1 && j1 <= RAND_MAX);
		}
#endif	/* ANSI */
#endif /* SKIP4_102 */
	}



/*
 * 4.10.3 - Memory management functions
 */
static void d4_10_3()
	{
#ifndef SKIP4_103
	int i;
	char *pc;
	long *pl;
	double d = 9.87;
	union all
		{
		char c;
		short s;
		int i;
		long l;
		double d;
#if ANSI
		long double ld;
#endif	/* ANSI */
		char *pc;
		long *pl;
		double *pd;
		} *pu;
	int is_all_zero;

	/* 4.10.3.1 calloc
	 * Can't do much more than check for existence and see if
	 * using them doesn't crash the program. Calloc zeros out
	 * the space.
	 */
	pc = calloc(100, sizeof(long));
	is_all_zero = 1;  /* See update item #534, Review Board 8/92 */
	for (i = 0; i < 100*sizeof(long); ++i)
		{
		if (pc[i] != 0)
			is_all_zero = 0;
		}
	iequals(__LINE__, is_all_zero, 1);

	pl = (long *)pc;
	for (i = 0; i < 100; ++i)
		lequals(__LINE__, pl[i], 0L);

	free((char *)pl);

	/* Check alignment properties ... presumably the program
	 * could crash if there is an alignment error. Here is a case
	 * where it is important to try to defeat optimizers, so ivalue, etc.
	 * are used for the assignments..
	 */
	pu = (union all *)calloc(100, sizeof(*pu));
	pu->c = ivalue('a');
	iequals(__LINE__, pu->c, 'a');
	pu->s = ivalue(SHRT_MAX);
	iequals(__LINE__, pu->s, SHRT_MAX);
	pu->i = ivalue(INT_MAX);
	iequals(__LINE__, pu->i, INT_MAX);
	pu->l = lvalue(LONG_MAX);
	lequals(__LINE__, pu->l, LONG_MAX);
	pu->d = dvalue(2.34);
	dequals(__LINE__, pu->d, 2.34);
#if ANSI
	pu->ld = 3.45L;
	ldequals(__LINE__, pu->ld, 3.45L);
#endif	/* ANSI */
	/* pointer to integral types already checked */
	pu->pd = (double *)avalue(&d);
	dequals(__LINE__, *(pu->pd), 9.87);

	/* check for 0 as arguments */
	aequals(- __LINE__, calloc(0, 100), NULL);	/* ANSI8612: behavior is implem-def; see 4.10.3 P1s1 */
	aequals(- __LINE__, calloc(100, 0), NULL);	/* ANSI8612: behavior is implem-def; see 4.10.3 P1s1 */

	/* 4.10.3.2 free used throughout.  ANSI requires free to accept a NULL argument.  */
#if ANSI
	free(NULL);
#endif	/* ANSI */
	
	/* 4.10.3.3 malloc
	 */
	pc = malloc(100 * sizeof(long));
	for (i = 0; i < 100*sizeof(long); ++i)
		pc[i] = i%128;
	for (i = 0; i < 100*sizeof(long); ++i)
		iequals(__LINE__, pc[i], i%128);
	pl = (long *)pc;
	for (i = 0; i < 100; ++i)
		pl[i] = i;
	for (i = 0; i < 100; ++i)
		lequals(__LINE__, pl[i], (long)i);


	/* check for 0 as arguments */
	aequals(- __LINE__, malloc(0), NULL);	/* ANSI8612: behavior is implem-def; see 4.10.3 P1s1 */

	/* 4.10.3.4 realloc
	 * contents should be the same up to the lesser size
	 */
	pc = realloc((char *)pl, 200*sizeof(long));
	pl = (long *)pc;
	for (i = 0; i < 100; ++i)
		lequals(__LINE__, pl[i], (long)i);
	pc = realloc((char *)pl, 50*sizeof(long));
	pl = (long *)pc;
	for (i = 0; i < 50; ++i)
		lequals(__LINE__, pl[i], (long)i);
 	free(pc);
#if ANSI
	/* ANSI requires that NULL and 0 are acceptable as arguments */
 	pc = realloc(NULL, 400);
 	for (i = 0; i < 400; ++i)
 		pc[i] = i%128;
 	for (i = 0; i < 400; ++i)
 		iequals(__LINE__, pc[i], i%128);
 	aequals(- __LINE__, realloc(pc, 0), NULL);	/* ANSI8612: behavior is implem-def; see 4.10.3 P1s1 */
#endif	/* ANSI */
#endif /* SKIP4_103 */
	}

/*
 * 4.10.4 - Communication with the environment.
 */
typedef int (*PIF)();
#ifndef SKIP4_104
static void d4_10_4y();
static void d4_10_4z();
#endif	/* SKIP4_104 */
static void d4_10_4()
	{
#ifndef SKIP4_104
	PIF p;
	int i;
	void d4_10_4x();

	/* Existence of these functions has been checked in 4.1.
	 * Most of these we can't call without terminating the program,
	 * in which case no real testing is done.
	 */

#if ANSI
	/* 4.10.4.1 abort
	 * terminates the program, so don't call it.
	 */
	p = (PIF)abort;

	/* 4.10.4.2 atexit
	 * register functions to be called at exit. At least
	 * 32 must be allowed. As of ANSI8712 it is required that
	 * they be called in reverse order of registry.
	 */
#if ANSI8712
	iequals(__LINE__, atexit(d4_10_4x), 0);
	for (i = 0; i < 30; ++i)
		iequals(__LINE__, atexit(d4_10_4y), 0);
	iequals(__LINE__, atexit(d4_10_4z), 0);
#else
	for (i = 0; i < 32; ++i)
		iequals(__LINE__, atexit(d4_10_4x), 0);
#endif	/* ANSI8712 */
#endif	/* ANSI */

	/* 4.10.4.3 exit
	 * will be tested at the end of main
	 */
#if ANSI8612
	checkthat(__LINE__, EXIT_SUCCESS != EXIT_FAILURE);
#endif	/* ANSI8612 */
	p = (PIF)exit;

	/* 4.10.4.4 getenv
	 * returns an implementation dependant pointer
	 */
	aequals(- __LINE__, getenv("this is probably a NULL pointer"), NULL);

	/* 4.10.4.4 system
	 * is very implementation dependent. Does a command processor 
	 * exist? Return of 0 says not.
	 */
	checkthat(- __LINE__, system(NULL) != 0);
#endif	/* SKIP4_104 */
	}

static int i = 0;
#ifndef SKIP4_104
static void d4_10_4z()
	{
	Filename = "d410.c";
	/* this function was registered last, so it should be called first */
	if (++i != 1)
		complain(__LINE__);
	}
static void d4_10_4y()
	{
	Filename = "d410.c";
	/* there should be exactly 30 invocations of this function */
	if (i < 1 || i > 30) 
		complain(__LINE__);
	++i;
	}
#if ANSI8712
static void d4_10_4x()
	{
	Filename = "d410.c";
	/* this function was registered first, so it should be called last */
	if (++i != 32)
		complain(__LINE__);
	}
#else	/* if !ANSI8712 */
static void d4_10_4x()
	{
	if (++i == 32)
		printf("ATEXIT successfully registered 32 functions\n");
	Filename = "d410.c";
	checkthat(__LINE__, i <= 32);
	}
#endif	/* ANSI8712 */
#endif /* SKIP4_104 */

/*
 * 4.10.5 - Searching and sorting.
 */

#if !ANSI
static void d4_10_5() { }
#else	/* if ANSI */
int mycmpfn PARMS((const void *, const void *));
int mycmpfn(s1, s2)
	const void *s1, *s2;
	{
	return (strcmp((const char *)s1, (const char *)s2));
	}

static void d4_10_5()
	{
#ifndef SKIP4_105
/* 	generic_ptr bsearch(); depend upon declaration in <stdlib.h> */
	char *p;
	static char names[][4] = {"abc", "jkl", "yzz", "def", "stu", "mno", "vwx", "pqr", "ghi"};
							/* Since we are in a "C" locale, these strings can be expected */
							/* to collate as abc, def, ghi, jkl, mno, pqr, stu, vwx, yzz. */
	/* 4.10.5.1 qsort
	 * sort using user comparison routine.
	 */
	qsort(names, 9, 4, mycmpfn);
	stequals(__LINE__, names[0], "abc");
	stequals(__LINE__, names[1], "def");
	stequals(__LINE__, names[2], "ghi");
	stequals(__LINE__, names[3], "jkl");
	stequals(__LINE__, names[4], "mno");
	stequals(__LINE__, names[5], "pqr");
	stequals(__LINE__, names[6], "stu");
	stequals(__LINE__, names[7], "vwx");
	stequals(__LINE__, names[8], "yzz");
	/* 4.10.5.2 bsearch
	 * find the entries in the array
	 */
	checkthat(__LINE__, bsearch("abc", names, 9, 4, mycmpfn) == names[0]);	
	aequals(__LINE__, bsearch("def", names, 9, 4, mycmpfn), names[1]);	
	aequals(__LINE__, bsearch("ghi", names, 9, 4, mycmpfn), names[2]);	
	aequals(__LINE__, bsearch("jkl", names, 9, 4, mycmpfn), names[3]);	
	aequals(__LINE__, bsearch("mno", names, 9, 4, mycmpfn), names[4]);	
	aequals(__LINE__, bsearch("pqr", names, 9, 4, mycmpfn), names[5]);	
	aequals(__LINE__, bsearch("stu", names, 9, 4, mycmpfn), names[6]);	
	aequals(__LINE__, bsearch("vwx", names, 9, 4, mycmpfn), names[7]);	
	/* no match returns NULL */
	aequals(__LINE__, bsearch("123", (char*)names, 9, 4, mycmpfn), NULL);	
#endif /* SKIP4_105 */
	}
#endif	/* !ANSI */

/*
 * 4.10.6 - Integer arithmetic functions.
 */
static void d4_10_6()
	{
#ifndef SKIP4_106
	/* 4.10.6.1 abs
	 * absolute value
	 */
	iequals(__LINE__, abs(10), 10);
	iequals(__LINE__, abs(-10), 10);

#if ANSI
	{
	div_t d;
	ldiv_t ld;

	/* 4.10.6.2 div
	 * get dividend and remainder.
	 */
	d = div(-5, 2);
	iequals(__LINE__, d.quot, -2);
	iequals(__LINE__, d.rem, -1);
	d = div(5, 2);
	iequals(__LINE__, d.quot, 2);
	iequals(__LINE__, d.rem, 1);

	/* 4.10.6.3 labs
	 * same as abs, but for longs
	 */
	lequals(__LINE__, labs(2147483647), 2147483647);
	lequals(__LINE__, labs(-2147483647), 2147483647);

	/* 4.10.6.4 ldiv
	 * get long dividend and remainder.
	 */
	ld = ldiv(-5L, 2L);
	lequals(__LINE__, ld.quot, -2L);
	lequals(__LINE__, ld.rem, -1L);
	ld = ldiv(5L, 2L);
	lequals(__LINE__, ld.quot, 2L);
	lequals(__LINE__, ld.rem, 1L);
	}
#endif	/* ANSI */
#endif /* SKIP4_106 */
	}

/*
 * 4.10.7 - Multibyte character functions
 */
#if !ANSI
static void d4_10_7(){}
#else	/* if ANSI */
static void d4_10_7()
	{
#ifndef SKIP4_107
	const char *s = "Test string";
	int len = strlen(s);
	const char *ps = s + len;					/* points to 0 */
#if WIDE_CHARS
	wchar_t wc;
#endif	/* WIDE_CHARS */
	char buff[10];

	/* Only the "C" locale can be tested here, which has no
	 * specific requirements.  Vendor specific tests can be added to
	 * test actual multibyte encodings.
	 */
#if WIDE_CHARS
	/* 4.10.7.1 The mblen function */
	iequals( - __LINE__, mblen(NULL, 1000), 0);		/* no multibytes */
	iequals( - __LINE__, mblen(NULL, 0), 0);			/* no multibytes */
	iequals(__LINE__, mblen(ps, 1), 0);				/* pointer to 0 */
	iequals(__LINE__, mblen(s, len), 1);			/* 1 byte per char */

	/* 4.10.7.2 The mbtowc function */
	iequals(__LINE__, mbtowc(&wc, NULL, 0), 0);		/* no multibytes */
	iequals(__LINE__, mbtowc(&wc, ps, 1), 0);		/* pointer to 0 */
	lequals(__LINE__, (long)wc, 0L);				/* big enough for wchar_t */
	iequals(__LINE__, mbtowc(&wc, s, len), 1);		/* pointer to 0 */
	lequals(__LINE__, (long)wc, (long)'T');			/* big enough for wchar_t */
	checkthat(__LINE__, MB_CUR_MAX >= 1);

	/* 4.10.7.3 The wctomb function */
	iequals(__LINE__, wctomb(NULL, wc), 0);			/* no multibytes */
	iequals(__LINE__, wctomb(NULL, 0), 0);			/* no multibytes */
	iequals(__LINE__, wctomb(buff, wc), 1);			/* 1 byte per char */
	iequals(__LINE__, buff[0], 'T');				
	wc = 0;
	iequals(__LINE__, wctomb(buff, wc), 1);	
	iequals(__LINE__, buff[0], 0);				
#endif	/* WIDE_CHARS */
#endif /* SKIP4_107 */
	}
#endif	/* ANSI */

/*
 * 4.10.8 - Multibyte string functions
 * The size_t checks are done as long because sizeof(long) >= sizeof(size_t).
 * Also, it is assumed that sizeof(long) >= sizeof(wchar_t);
 */
#if !ANSI || !WIDE_CHARS
static void d4_10_8()
		{}
#else	/* if ANSI && WIDE_CHARS */
static void d4_10_8()
	{
#ifndef SKIP4_108
	int i;
	const char *s = "A test string";
	long len = (long)strlen(s);
	#define WLEN 20
	wchar_t warray[WLEN];
	char buf[WLEN];

	/* if wchar_t is bigger than a long, these tests will not work */
	if (sizeof(long) < sizeof(wchar_t))
		{
		complain(- __LINE__);
		return;
		}
	
	/* initialize warray and buf to all 1 */
	for (i = 0; i < WLEN; ++i)
		{
		warray[i] = 1;
		buf[i] = 1;
		}

	/* in the "C" locale it is assumed that the MB representation
	 * is the same as the wchar_t representation for printable
	 * characters.
	 */

	/* 4.10.8.1 the mbstowcs function */
	lequals(__LINE__, (long) mbstowcs(warray, s, 1), 1L);
	lequals(__LINE__, (long) warray[0], (long)'A');
	lequals(__LINE__, (long) warray[1], 1L);			/* no terminating 0 */
	lequals(__LINE__, (long) mbstowcs(warray, s, WLEN), len);
	lequals(__LINE__, (long) warray[len-1], (long)'g');
	lequals(__LINE__, (long) warray[len], 0L);			/* terminating 0 */

	/* 4.10.8.2 the wcstombs function */
	lequals(__LINE__, (long) wcstombs(buf, warray, 1), 1L);
	lequals(__LINE__, (long) buf[0], (long)'A');
	lequals(__LINE__, (long) buf[1], 1L);				/* no terminating 0 */
	lequals(__LINE__, (long) wcstombs(buf, warray, WLEN), len);
	lequals(__LINE__, (long) buf[len-1], (long)'g');
	lequals(__LINE__, (long) buf[len], 0L);				/* terminating 0 */
#endif /* SKIP4_108 */
	}
#endif	/* ANSI  && WIDE_CHARS */

#else /* if SKIP410 */
void d4_10() { pr_skip("d4_10: SKIPPED ENTIRELY\n"); }
#endif	/* SKIP410 */

