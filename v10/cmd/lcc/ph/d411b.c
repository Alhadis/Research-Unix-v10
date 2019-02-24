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
 * 4.11(b) - String Handling (continued)
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

static char mem1[15] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14};
static char mem2[15] = {9,9,9,9,9,9,9,9,9,9,9,9,9,9,9};
static char string1[] = "aaaaaaaaaa";
static char string2[] = "aaaa\0bbbb";
static char string3[] = "cccccccccc";
static char string4[21] = "dddddddddd";
static char string5[] = "abcdefgX\0Y";
static char string6[] = "abcXabcX\0Y";
static char string7[] = "oneXtwoYXthreeYYXXZZfourGFGF";
static char string8[] = "oneXtwoYXthreeYYXXZZfourGFGF";
static char string9[60];
static void d4_11_4();
static void d4_11_5();
static void d4_11_6();
void d4_11b()
	{
	Filename = "d411b.c";
	d4_11_4();
	d4_11_5();
	d4_11_6();
	}


/*
 * 4.11.4 - Comparison functions
 */
static void d4_11_4()
	{
#ifndef SKIP4_114
	int i;
	char *p;

	/* 4.11.4.1 memcmp
	 * memory compare
	 */
	checkthat(__LINE__, memcmp(string4, string3, 5) > 0);
	checkthat(__LINE__, memcmp(string3, string4, 5) < 0);
	checkthat(__LINE__, memcmp(string1, string2, 4) == 0);

	/* 4.11.4.2 strcmp
	 * string compare
	 */
	checkthat(__LINE__, strcmp("bbc", "abc") > 0);
	checkthat(__LINE__, strcmp("abc", "bbc") < 0);
	checkthat(__LINE__, strcmp("hi ma", "hi ma") == 0);

	/* 4.11.4.4 strncmp
	 * string compare with length
	 */
	checkthat(__LINE__, strncmp("bbc", "abc", 1) > 0);
	checkthat(__LINE__, strncmp("abc", "bbc", 1) < 0);
	checkthat(__LINE__, strncmp("hi max", "hi may", 5) == 0);

#if ANSI8612
	/* 4.11.4.5 strxfrm
	 * make the string collatable through strcmp, memcmp.
	 * This is implementation dependent. For the "C" locale, this 
	 * just copies the source into the target, if space permits.
	 */

	 checkthat(__LINE__, strxfrm(string9, string8, 60) > 0);
	 iequals(- __LINE__, strcmp(string9, string8), 0);

	/* 4.11.4.3 strcoll
	 * implementation dependent
	 */
	iequals(- __LINE__, strcoll(string9, string8), 0);
#if ANSI8706
	checkthat(__LINE__, strxfrm(string9, "abc", 2) == 3);	/* ANSI8706: now returns required length */
#endif
#endif
#endif /* SKIP4_114 */
	}


/*
 * 4.11.5 - Search functions
 */
static void d4_11_5()
	{
#ifndef SKIP4_115
	int i;
	char *p;

	/* 4.11.5.1 memchr
	 * search memory
	 */
	p = memchr(string5, 'X', 10);
	aequals(__LINE__, p, string5+7);
	p = memchr(string5, 'X', 3);
	aequals(__LINE__, p, (char *)0);
	p = memchr(string5, 'Y', 10);
	aequals(__LINE__, p, string5+9);
	p = memchr(string5, 'Z', 8);
	aequals(__LINE__, p, (char *)0);

	/* 4.11.5.2 strchr
	 * search strings
	 */
	p = strchr(string5, 'X');
	aequals(__LINE__, p, string5+7);
	p = strchr(string5, 'Y');
	aequals(__LINE__, p, (char *)0);

	/* 4.11.5.3 strcspn
	 * count number of leading characters not in set
	 */
	iequals(__LINE__, strcspn(string5, string5), 0);
	iequals(__LINE__, strcspn(string5, "ABC"), 8);
	iequals(__LINE__, strcspn(string5, "ABCX"), 7);

	/* 4.11.5.4 strpbrk
	 * get pointer to first character in the set
	 */
	p = strpbrk(string5, string5);
	aequals(__LINE__, p, string5);
	p = strpbrk(string5, "ABC");
	aequals(__LINE__, p, (char *)0);
	p = strpbrk(string5, "ABCX");
	aequals(__LINE__, p, string5+7);

	/* 4.11.5.5 strrchr
	 * locate last occurrence of character in a string
	 */
	p = strrchr(string6, 'Y');
	aequals(__LINE__, p, (char *)0);
	p = strrchr(string6, 'X');
	aequals(__LINE__, p, string6+7);

	/* 4.11.5.6 strspn
	 * length of segment consisting of characters in set
	 */
	iequals(__LINE__, strspn(string6, "ABC"), 0);
	iequals(__LINE__, strspn(string6, "cba"), 3);
	iequals(__LINE__, strspn(string6, "cXba"), 8);

	/* 4.11.5.7 strstr
	 * locate a substring
	 */
#if ANSI
	aequals(__LINE__, strstr(string8, "one"), string8+0);
	aequals(__LINE__, strstr(string8, "YX"), string8+7);
	aequals(__LINE__, strstr(string8, "GFGF"), string8+24);
	aequals(__LINE__, strstr(string8, "not there"), NULL);
	aequals(__LINE__, strstr(string8, ""), string8);	/* ANSI8703: was previously vague */

#endif
	/* 4.11.5.8 strtok
	 * string parsing
	 */
	p = strtok(string7, "YX");
	checkthat(__LINE__, strcmp(p, "one") == 0);
	p = strtok((char *)0, "YZ");
	checkthat(__LINE__, strcmp(p, "two") == 0);
	p = strtok((char *)0, "ZYX");
	checkthat(__LINE__, strcmp(p, "three") == 0);
	p = strtok((char *)0, "ZYXFGHIJ");
	checkthat(__LINE__, strcmp(p, "four") == 0);
	/* this should detect that despite the "space", no token remains */
	p = strtok((char *)0, "FGHIJ");
	aequals(__LINE__, p, (char *)0);
	/* re-initialize */
	p = strtok(string8, "XY");
	checkthat(__LINE__, strcmp(p, "one") == 0);
	/* check for nothing available at first parse */
	p = strtok("XYZ", "XYZ");
	aequals(__LINE__, p, (char *)0);
#endif /* SKIP4_115 */
	}

/*
 * 4.11.6 - Miscellaneous functions
 */
static void d4_11_6()
	{
#ifndef SKIP4_116
	int i;
	char *p;

	/* 4.11.6.1 memset
	 * memory fill
	 */
	memcpy(mem2, mem1, 15);
	p = memset(mem2+1, 99, 4);
	aequals(__LINE__, mem2+1, p);
	for (i = 0; i < 15; ++i)
		if (1 <= i && i <= 4)
			iequals(__LINE__, mem2[i], 99);
		else
			iequals(__LINE__, mem1[i], mem2[i]);
#if ANSI
	/* 4.11.6.2 strerror
	 * map to an error message string. Since the string
	 * is implementation defined, we can only check for existance.
	 */
	p = strerror(0);
#endif
	/* 4.11.6.3 strlen
	 * find string length
	 */
	iequals(__LINE__, strlen("123456789"), 9);
	iequals(__LINE__, strlen(""), 0);
#endif /* SKIP4_116 */
	}


#endif /* SKIP411 */
