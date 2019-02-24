/* As per your license agreement, your distribution is not to be moved or copied outside the Designated Site
 * without specific permission from Plum Hall Inc.
 */

/*
 * CLIB - portable replacements for various C library functions
 */
#ifndef CLIB_H
#define CLIB_H

/* assumes you have #include'd either  "defs.h"  or  "hodefs.h" */

/*
 * IS_DIGIT
 */
int is_digit FN_DEF1(c,
	int c)
	{
	return '0' <= c && c <= '9';
	}

/*
 * MEM_CMP
 */
int mem_cmp FN_DEF3(ps1, ps2, n,
	const void *ps1,
	const void *ps2,
	int n)
	{
	const char *s1 = (const char *)ps1;
	const char *s2 = (const char *)ps2;
	for (;n > 0; --n)
		{
		if (*s1 != *s2)
			return (UCHAR)*s1 - (UCHAR)*s2;
		++s1, ++s2;
		}
	return 0;
	}

/*
 * STR_CMP
 */
int str_cmp FN_DEF2(s1, s2,
	const char *s1,
	const char *s2)
	{
	for (; *s1 == *s2; ++s1, ++s2)
		if (*s1 == '\0')
			return 0;
	return (UCHAR)*s1 - (UCHAR)*s2;
	}

/*
 * ST_NCMP
 *  Note: spelled  st_ncmp  to avoid 6-letter collision with st_ncpy
 */
int st_ncmp FN_DEF3(s1, s2, n,
	const char *s1,
	const char *s2,
	int n)
	{
	for (; n > 0 && *s1 == *s2; ++s1, ++s2, --n)
		if (*s1 == '\0')
			return 0;
	if (n == 0)
		return 0;
	else
		return (UCHAR)*s1 - (UCHAR)*s2;
	}

/*
 * STR_CPYE - copy string s2 to s1 (not an exact replacement for strcpy)
 */
char *str_cpye FN_DEF2(s1, s2,
	char *s1,
	const char *s2)
	{
	while ((*s1 = *s2++) != '\0')
		++s1;
	return s1;
	}

/*
 * STR_CAT - copy string s2 to end of s1
 */
char *str_cat FN_DEF2(s1, s2,
	char *s1,
	const char *s2)
	{
	char *s0 = s1;

	while (*s1 != '\0')
		++s1;
	while ((*s1 = *s2++) != '\0')
		++s1;
	return s0;
	}

/*
 * ST_NCPY - copy at most n chars of string s2 to s1
 */
char *st_ncpy FN_DEF3(s1, s2, n,
	char *s1,
	const char *s2,
	int n)
	{
	char *s0 = s1;
	while (n > 0 && (*s1 = *s2++) != '\0')
		--n, ++s1;
	while (n-- > 0)
		*s1++ = '\0';
	return s0;
	}

/*
 * STR_LEN - return the length of a string
 */
int str_len FN_DEF1(s,
	const char *s)
	{
	const char *t;

	for (t = s; *t != '\0'; ++t)
		;
	return t - s;
	}

/*
 * STR_STR - find occurrence of s2 in s1
 */
const char *str_str FN_DEF2(s1, s2,
	const char *s1,
	const char *s2)
	{
	register const char *ps1, *ps2;

	if (*s2 == '\0')
		return s1;
	for ( ; *s1 != '\0'; ++s1)
		{
		if (*s1 == *s2)
			{
			for (ps1 = s1, ps2 = s2; *ps1 && *ps2 && *ps1 == *ps2; ++ps1, ++ps2)
				;
			if (*ps2 == '\0')
				return s1;
			}
		}
	return 0;
	}
/*
 * STR_CSPN
 */
int str_cspn FN_DEF2(s1, s2,
	char *s1,
	char *s2)
	{
	register int i;
	register char *p;

	for (i = 0;  s1[i] != '\0';  ++i)
		{
		for (p = s2;  *p != '\0';  ++p)
			{
			if (*p == s1[i])
				return (i);
			}
		}
	return (i);
	}
#endif
