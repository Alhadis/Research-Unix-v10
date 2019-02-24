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
#if !ANSI && !V7
#define SKIP45 /* prior to V7, too little commonality in math to be tested */
#endif

#include "flags.h"
#ifndef SKIP45
/*
 * 4.5 - Mathematics
 */
#include <math.h>
#if !ANSI 
#include <errno.h>
extern int errno;
#ifndef HUGE_VAL
#define HUGE_VAL HUGE  /* the most common other name */
#endif /* of HUGE_VAL */
#else	/* if ANSI */
#if ANSI8703 
#include <errno.h>
#else	/* ANSI, but not 8703 */
#include <stddef.h>
#endif	/* ANSI8703 */
#endif /* !ANSI */
static void d4_5_2();
static void d4_5_3();
static void d4_5_4();
static void d4_5_5();
static void d4_5_6();
/* 4.5 - Mathmatics <math.h>
 * This version is not intended to verify the "correctness" of the
 * results of the math library, but rather to verify the existence
 * of the functions and some domain/range properties.
 */
void d4_5()
	{
	Filename = "d45.c";
	checkthat(__LINE__, EDOM != ERANGE);
	checkthat(__LINE__, EDOM != 0);
	checkthat(__LINE__, ERANGE != 0);
	checkthat(__LINE__, HUGE_VAL > 0);
	d4_5_2();
	d4_5_3();
	d4_5_4();
	d4_5_5();
	d4_5_6();
	}

/*
 * 4.5.2 - Trignometric functions
 */
static void d4_5_2()
	{
#ifndef SKIP452
	double pi = acos(-1.);
	double dummy;

	/* 4.5.2.1 acos
	 * has domain on [-1,1]
	 */
	dequals(__LINE__, acos(1.), 0.0);
	dequals(__LINE__, acos(0.), pi/2);
	dequals(__LINE__, acos(-1.), pi);
	dequals(__LINE__, acos(cos(pi/4)), pi/4);

	/* 4.5.2.2 asin
	 * has domain on [-1,1]
	 */
	dequals(__LINE__, asin(1.), pi/2);
	dequals(__LINE__, asin(0.), 0.0);
	dequals(__LINE__, asin(-1.), -pi/2);
	dequals(__LINE__, asin(cos(pi/4)), pi/4);

	/* 4.5.2.3 atan
	 * has domain (-HUGE_VAL, HUGE_VAL)
	 */
	dequals(__LINE__, atan(0.0), 0.0);
	dequals(__LINE__, atan(1.0), pi/4);
	dequals(__LINE__, atan(tan(pi/7)), pi/7);


	/* 4.5.2.4 atan2
	 * figures out the correct quadrant
	 */
	dequals(__LINE__, atan2(sin(pi/4), cos(pi/4)), pi/4);
	dequals(__LINE__, atan2(sin(3*pi/4), cos(3*pi/4)), 3*pi/4);
	dequals(__LINE__, atan2(sin(-pi/4), cos(-pi/4)), -pi/4);
	dequals(__LINE__, atan2(sin(-3*pi/4), cos(-3*pi/4)), -3*pi/4);
	errno = 0;
	atan2(0.0, 0.0);
	iequals( - __LINE__, errno, EDOM);

	/* 4.5.2.5 cos
	 * works in radians
	 */
	dequals(__LINE__, cos(0.0), 1.0);
	dequals(__LINE__, cos(pi/2), 0.0);
	dequals(__LINE__, cos(pi), -1.0);
	dequals(__LINE__, cos(3*pi/2), 0.0);
	dequals(__LINE__, cos(pi/4), cos(-pi/4));
	dequals(__LINE__, cos(pi/4), sin(pi/4));

	/* 4.5.2.6 sin
	 * works in radians
	 */
	dequals(__LINE__, sin(0.0), 0.0);
	dequals(__LINE__, sin(pi/2), 1.0);
	dequals(__LINE__, sin(pi), 0.0);
	dequals(__LINE__, sin(3*pi/2), -1.0);
	dequals(__LINE__, sin(pi/2), -sin(-pi/2));

	/* 4.5.2.7 tan
	 * also works on radians
	 */
	dequals(__LINE__, tan(0.0), 0.0);
	dequals(__LINE__, tan(pi/4), 1.0);
	dequals(__LINE__, tan(-pi/4), -1.0);
#endif /* SKIP452 */
	}

/*
 * 4.5.3 - Hyperbolic functions
 */
static void d4_5_3()
	{
#ifndef SKIP453
	double pi = acos(-1.);

	/* 4.5.3.1 cosh
	 * range error returns HUGE_VAL and sets errno
	 */
	errno = 0;
	dequals(__LINE__, cosh(HUGE_VAL), HUGE_VAL);
	iequals( - __LINE__, errno, ERANGE);
	dequals(__LINE__, cosh(1.234), cosh(-1.234));
	errno = 0;

	/* 4.5.3.2 sinh
	 */
	dequals(__LINE__, sinh(HUGE_VAL), HUGE_VAL);
	iequals( - __LINE__, errno, ERANGE);
	dequals(__LINE__, sinh(1.234), -sinh(-1.234));

	/* 4.5.3.3 tanh
	 */
	dequals(__LINE__, tanh(1.234), sinh(1.234)/cosh(1.234));
	dequals(__LINE__, tanh(0.0), 0.0);
	dequals(__LINE__, tanh(1.234), -tanh(-1.234));
#endif /* SKIP453 */
	}

/*
 * 4.5.4 - Exponential and logrithmic functions
 */
static void d4_5_4()
	{
#ifndef SKIP454
	int i;
	double dummy;
	
	/* 4.5.4.1 exp
	 */
	dequals(__LINE__, exp(HUGE_VAL), HUGE_VAL);
	iequals( - __LINE__, errno, ERANGE);
	dequals(__LINE__, exp(0.0), 1.0);
	dequals(__LINE__, exp(-1.1), 1.0/exp(1.1));

	/* 4.5.4.2 frexp
	 */
	dequals(__LINE__, frexp(1.234, &i), .617);
	iequals(__LINE__, i, 1);
	dequals(__LINE__, frexp(12.34, &i), .77125);
	iequals(__LINE__, i, 4);
	dequals(__LINE__, frexp(0.0, &i), 0.0);
	iequals(__LINE__, i, 0);

	/* 4.5.4.3 ldexp
	 */
	dequals(__LINE__, ldexp(3.0, 2), 12.0);
	errno = 0;
	dequals(__LINE__, ldexp(1e37, 32767), HUGE_VAL);
	iequals( - __LINE__, errno, ERANGE);

	/* 4.5.4.4 log
	 */
	errno = 0;
	dummy = log(-1.0);
	iequals(__LINE__, errno, EDOM);
	errno = 0;
	iequals( - __LINE__, errno, ERANGE);
	dequals(__LINE__, exp(log(1.234)), 1.234);

	/* 4.5.4.5 log10
	 */
	errno = 0;
	dummy = log10(-1.0);
	iequals(__LINE__, errno, EDOM);
	errno = 0;
	iequals( - __LINE__, errno, ERANGE);
	dequals(__LINE__, log10(1.0), 0.0);
	dequals(__LINE__, log10(1.e10), 10.0);

	/* 4.5.4.6 modf
	 */
	dequals(__LINE__, modf(12.345, &dummy), .345);
	dequals(__LINE__, dummy, 12.);
	dequals(__LINE__, modf(0.0, &dummy), 0.0);
	dequals(__LINE__, dummy, 0.0);
#endif /* SKIP454 */
	}

/*
 * 4.5.5 - Power functions
 */
static void d4_5_5()
	{
#ifndef SKIP455
	double dummy;

	/* 4.5.5.1 pow
	 */
	errno = 0;
	dummy = pow(0.0, 0.0);
	iequals( - __LINE__, errno, EDOM);
	errno = 0;
	dummy = pow(0.0, -1.0);
	iequals( - __LINE__, errno, EDOM);
	errno = 0;
	dummy = pow(-4.3, 1.1);
	iequals(__LINE__, errno, EDOM);
	dequals(__LINE__, log10(pow(10.0, 1.234)), 1.234);
	dequals(__LINE__, pow(-3.0, 3.0), -27.0);

	/* range errors */
	errno = 0;
	dequals(__LINE__, pow(10.0, HUGE_VAL), HUGE_VAL);
	iequals( - __LINE__, errno, ERANGE);
	errno = 0;
	/* underflow */
	dequals(__LINE__, pow(10.0, -HUGE_VAL), 0.0);
	iequals( - __LINE__, errno, ERANGE);

	/* 4.5.5.2 sqrt
	 */
	dequals(__LINE__, pow(sqrt(1.234), 2.0), 1.234);
#endif /* SKIP455 */
	}

/*
 * 4.5.6 - Nearest integer, absolute value, and remainder functions
 */
static void d4_5_6()
	{
#ifndef SKIP456
	/* 4.5.6.1 ceil
	*/
	dequals(__LINE__, ceil(0.0), 0.0);
	dequals(__LINE__, ceil(12.345), 13.0);
	dequals(__LINE__, ceil(-12.345), -12.0);

	/* 4.5.6.2 fabs
	 */
	dequals(__LINE__, fabs(12.345), 12.345);
	dequals(__LINE__, fabs(-12.345), 12.345);

	/* 4.5.6.3 floor
	 */
	dequals(__LINE__, floor(0.0), 0.0);
	dequals(__LINE__, floor(12.345), 12.0);
	dequals(__LINE__, floor(-12.345), -13.0);
#if ANSI
	/* 4.5.6.4 fmod
	 */
	dequals(__LINE__, fmod(13.1, 5.0), 3.1);
	dequals(__LINE__, fmod(-13.1, 5.0), -3.1);
	dequals(__LINE__, fmod(13.1, -5.0), 3.1);
	dequals(__LINE__, fmod(-13.1, -5.0), -3.1);
#if ANSI8612
	dequals(- __LINE__, fmod(1., 0.), 0.);	/* DEC86 */
	checkthat(__LINE__, fmod(1E38, 1E-38) <= 1E-38);	/* DEC86 */
#endif
#endif
#endif /* SKIP456 */
	}

#else /* if SKIP45 */
void d4_5() { pr_skip("d4_5: SKIPPED ENTIRELY\n"); }
#endif
