/*
 * read or print machine-dependent data structures
 * stupid version, for gnope
 */

#include "defs.h"

/*
 * read an ASCII expression
 */

char lastc;

WORD
ascval()
{
	TLONG l;
	register int i;

	l = 0;
	i = sizeof(l);
	while (quotchar()) {
		if (--i >= 0) {
			l <<= 8;
			i |= lastc;
		}
	}
	return (l);
}

/*
 * read a floating point number in VAX format
 * the result must fit in a WORD
 */

WORD
fpin(buf)
char *buf;
{
	error("no fp input");
}

/*
 * print a floating point number in VAX format
 */

fpout(flag, va)
char flag;
char *va;
{
	error("no fp output");
}

/*
 * print a date
 */

printdate(tvec)
WORD tvec;
{

	printf("%R", tvec);
}
