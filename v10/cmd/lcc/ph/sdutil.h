/* As per your license agreement, your distribution is not to be moved or copied outside the Designated Site
 * without specific permission from Plum Hall Inc.
 */

/*
 * SDUTIL - system-dependent utility routines for freestanding implementations
 *
 * These functions are used by UTIL.C.  They provide facilities that cannot
 * be implemented portably and must be rewritten for each freestanding imple-
 * mentation.  This sample implementation uses standard library functions
 * (provided by a hosted implementation).
 */

#ifndef FREESTANDING
#include <errno.h>
#include <stdio.h>
#endif /* FREESTANDING */

#include "defs.h"

extern char last_char_printed;

/*
 * PR_OK - printf a text line in ok situation (no error, just text)
 */
void pr_ok FN_DEF1(s,
	const char *s)
	{
	char c;
	fputs(s, stdout);
	while (*s != '\0')
		c = *s++;
	last_char_printed = c;
	fflush(stdout);
	}

/*
 * SETZERO - appear, to a compiler, as though Zero is unpredictably set
 */
void setzero()
	{
	FILE *fp;
	int save;

	save = errno;
	if ((fp = fopen("nonexistent", "r")) != 0)
		{
		fscanf(fp, "ignore format", &Zero);
		fclose(fp);
		}
	errno = save;
	}
