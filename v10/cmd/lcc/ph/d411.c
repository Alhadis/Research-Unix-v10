/* The Plum Hall Validation Suite for C
 * Unpublished copyright (c) 1986-1991, Chiron Systems Inc and Plum Hall Inc.
 * VERSION: 4
 * DATE: 1993-01-01
 * The "ANSI" mode of this suite corresponds to official ANSI C, X3.159-1989.
 * As per your license agreement, your distribution is not to be moved or copied outside the Designated Site
 * without specific permission from Plum Hall Inc.
 */


#include "flags.h"
#ifndef SKIP411
#define LIB_TEST 1
#include "defs.h"

/*
 * 4.11 - String Handling
 */

#if ANSI
#include <stddef.h>
#include <string.h>
#else
#if ALL_STRING_FNS
char *memcpy();
char *memset();
char *strcpy();
char *strncpy();
char *strcat();
char *strncat();
char *memchr();
char *strchr();
char *strrchr();
char *strpbrk();
char *strtok();
int strspn();
int strcspn();
#endif
#endif

static void d4_11_2();
static void d4_11_3();
void d4_11b();









static char mem1[15] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14};
static char mem2[15] = {9,9,9,9,9,9,9,9,9,9,9,9,9,9,9};
static char string1[] = "aaaaaaaaaa";
static char string2[] = "bbbb\0bbbb";
static char string3[] = "cccccccccc";
static char string4[21] = "dddddddddd";
static char string5[] = "abcdefgX\0Y";
static char string6[] = "abcXabcX\0Y";
static char string7[] = "oneXtwoYXthreeYYXXZZfourGFGF";
static char string8[] = "oneXtwoYXthreeYYXXZZfourGFGF";
static char string9[60];
void d4_11()
	{

	Filename = "d411.c";
	d4_11_2();
	d4_11_3();
	d4_11b();
	}

/*
 * 4.11.2 - Copying functions
 */
static void d4_11_2()
	{
#ifndef SKIP4_112
	int i;
	char *p;

	/* 4.11.2.1 memcpy
	 * basic copy
	 */
	p = memcpy(mem2, mem1, 15);
	aequals(__LINE__, mem2, p);
	for (i = 0; i < 15; ++i)
		iequals(__LINE__, mem1[i], mem2[i]);

#if ANSI
	/* 4.11.2.2 memmove
	 * "correct" memory copy.
	 * First, shift left copy
	 */
	memmove(mem2, mem2+1, 14);
	for (i = 0; i < 14; ++i)
		iequals(__LINE__, mem2[i], mem1[i+1]);
	iequals(__LINE__, mem2[14], mem1[14]);

	/* Then shift right copy */
	memmove(mem2+1, mem2, 14);
	for (i = 1; i < 15; ++i)
		iequals(__LINE__, mem2[i], mem1[i]);
	iequals(__LINE__, mem2[0], mem2[1]);
#endif
	/* 4.11.2.3 strcpy
	 * string copy
	 */
	p = strcpy(string1+1, string2);
	aequals(__LINE__, p, string1+1);
	iequals(__LINE__, string1[0], 'a');
	for (i = 1; i < 10; ++i)
		if (i < 5)
			iequals(__LINE__, string1[i], 'b');
		else if (i == 5)
			iequals(__LINE__, string1[i], 0);
		else
			iequals(__LINE__, string1[i], 'a');
	iequals(__LINE__, string1[10], 0);

	/* 4.11.2.4 strncpy
	 * string copy with count
	 */
	p = strncpy(string1, string3, 9);
	aequals(__LINE__, p, string1);
	for (i = 0; i < 9; ++i)
		iequals(__LINE__, string1[i], 'c');
	iequals(__LINE__, string1[9], 'a');
	iequals(__LINE__, string1[10], 0);

	/* fill with nulls if too few */
	strncpy(string1, string2, 8);
	for (i = 0; i < 8; ++i)
		if (i < 4)
			iequals(__LINE__, string1[i], 'b');
		else
			iequals(__LINE__, string1[i], 0);
	iequals(__LINE__, string1[9], 'a');
	iequals(__LINE__, string1[10], 0);
#endif /* SKIP4_112 */
	}

/*
 * 4.11.3 - Concatenation functions
 */
static void d4_11_3()
	{
#ifndef SKIP4_113
	int i;
	char *p;

	/* 4.11.3.1 strcat
	 * string concatenation
	 */
	p = strcat(string4, string3);
	aequals(__LINE__, p, string4);
	for (i = 0; i < 20; ++i)
		if (i < 10)
			iequals(__LINE__, string4[i], 'd');
		else
			iequals(__LINE__, string4[i], 'c');
	iequals(__LINE__, string4[20], 0);

	/* 4.11.3.2 strncat
	 * string concatenation with maximum length
	 */
	string4[10] = 0;
	/* if there are not enough chars, then it is the same as strcat */
	p = strncat(string4, string3, 12);
	aequals(__LINE__, p, string4);
	for (i = 0; i < 20; ++i)
		if (i < 10)
			iequals(__LINE__, string4[i], 'd');
		else
			iequals(__LINE__, string4[i], 'c');
	iequals(__LINE__, string4[20], 0);
	string4[10] = 0;
	/* otherwise concatenate fewer characters */
	strncat(string4, string3, 4);
	for (i = 0; i < 14; ++i)
		if (i < 10)
			iequals(__LINE__, string4[i], 'd');
		else
			iequals(__LINE__, string4[i], 'c');
	iequals(__LINE__, string4[14], 0);
#endif /* SKIP4_113 */
	}

#else /* if SKIP411 */
void d4_11() { pr_skip("d4_11: SKIPPED ENTIRELY\n"); }
#endif
