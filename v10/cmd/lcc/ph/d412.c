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
#define SKIP412 1 /* This file is almost irrelevant for non-ANSI */
#endif

#include "flags.h"
#ifndef SKIP412
/*
 * 4.12 - Date and time
 */
#include <time.h>
#include <limits.h>
#include <string.h>
struct tm tm1, tm2, *ptm;
static time_t time_t1, time_t2, time_t3;
static void d4_12_1();
static void d4_12_2();
static void d4_12_3();
void d4_12()
	{
	Filename = "d412.c";
	d4_12_1();
	d4_12_2();
	d4_12_3();
	}

/*
 * 4.12.1 Components of time
 */
static void d4_12_1()
	{
	/* d4_12_1 is required by both d4_12_2 and d4_12_3 */
	/* make sure that all of the required fields are there */
	tm1.tm_sec = 0;
	tm1.tm_min = 49;
	tm1.tm_hour = 20;
	tm1.tm_mday = 12;
	tm1.tm_mon = 8;
	tm1.tm_year = 76;
	tm1.tm_wday = -99;
	tm1.tm_yday = -99;
	tm1.tm_isdst = 1;
	}

/*
 * 4.12.2 Time manipulation functions.
 */
static void d4_12_2()
	{
#ifndef SKIP4_122
	int sec;
	clock_t c;

	/* 4.12.2.1 clock
	 * system time. Also check existence of CLOCKS_PER_SEC macro
	 */
	sec = (c = clock()) / CLOCKS_PER_SEC;

	/* is the processor time available via "clock" ? */
	if (c == (clock_t)-1)
		complain(- __LINE__);

	/* 4.12.2.2 difftime
	 * compute the difference between two times
	 */
	time_t1 = mktime(&tm1);
	tm1.tm_sec = 5;
	time_t2 = mktime(&tm1);
	dequals(__LINE__, difftime(time_t1, time_t2), -5.0);

	/* 4.12.2.3 mktime
	 * convert a "struct tm" to a "time_t". Return
	 * (time_t)-1 if it is unrepresentable
	 */
	tm2.tm_sec = tm2.tm_min = tm2.tm_hour = tm2.tm_mday =
			tm2.tm_mon = tm2.tm_year = INT_MIN;
	checkthat( - __LINE__, mktime(&tm2) == (time_t)-1);

	/* now check that the day of the week for tm1 is Sunday. */
	iequals(__LINE__, tm1.tm_wday, 0);

	/* 4.12.2.4 time
	 * present time of day
	 */
	time_t1 = time_t2;
	time_t3 = time(&time_t1);

	/* check whether the time is available */
	if (time_t3 == (time_t)-1)
		complain(- __LINE__);
	dequals(__LINE__, difftime(time_t3, time_t1), 0.0);
	checkthat(__LINE__, difftime(time_t1, time_t2) > 0.0);
#endif /* SKIP4_122 */
	}

/*
 * 4.12.3 Time conversion functions
 */
static void d4_12_3()
	{
#ifndef SKIP4_123
	char s[64];
	char *p;

	/* 4.12.3.1 asctime
	 * convert a "struct tm" to a string
	 */
	p = asctime(&tm1);

	/* 4.12.3.2 ctime
	 * convert a "time_t" to a string
	 */
	time_t1 = mktime(&tm1);
	stequals(__LINE__, p, ctime(&time_t1));
	iequals(__LINE__, tm1.tm_wday, 0);	/* Sunday */
	iequals(__LINE__, tm1.tm_yday, 255);

	/* 4.12.3.3 gmtime
	 * convert a "time_t" to a Greenwich Mean Time string
	 */
	ptm = gmtime(&time_t1);

	/* 4.12.3.4 localtime
	 * convert a "time_t" to a "struct tm"
	 */
	ptm = localtime(&time_t1);
	time_t2 = mktime(ptm);
	/* this should be an isomorphic change */
	dequals(__LINE__, difftime(time_t1, time_t2), 0.0);

	/* 4.12.3.5 strftime
	 * encode time information in a locale dependant way.
	 * This test will make sure that the "C" locale works right.
	 */
	 iequals(__LINE__, strftime(s, 64, "XXX", &tm1), 3);
	 iequals(__LINE__, strftime(s,  2, "XXX", &tm1), 0);

	 #define ST(in, out) strftime(s, 64, in, &tm1); stequals(__LINE__, s, out);
	 #define ST2(in, out) strftime(s, 64, in, &tm1); stequals( - __LINE__, s, out);

	/* check each of the formatting directives */
	ST("%a", "Sun");
	ST("%A", "Sunday");
	ST("%b", "Sep");
	ST("%B", "September");
	{
		static char a[31] = {0};

		strftime(a, 30, "%c", &tm1);
		if (strchr(a, '\n') == 0)
			*strchr(a, '\0') = '\n';
		stequals(- __LINE__, a, asctime(&tm1));
	}
	ST("%d", "12");
	ST("%H", "20");
	ST("%I", "08");
	ST("%j", "256");
	ST("%m", "09");
	ST("%M", "49");
	ST("%p", "PM");
	ST("%S", "05");
	ST("%U", "37");
	ST("%w", "0");
	ST("%W", "36");
	ST2("%x", "Sun Sep 12, 1976");
	ST2("%X", "20:49:05");
	ST("%y", "76");
	ST("%Y", "1976");
	ST("%%", "%");
#endif /* SKIP4_123 */
	}

#else /* if SKIP412 */
void d4_12() { pr_skip("d4_12: SKIPPED ENTIRELY\n"); }
#endif
