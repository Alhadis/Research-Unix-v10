/*
 * read or print machine-dependent data structures
 * stupid version, for vax debugging 68000
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
	register char *p;
	register int i;

	l = 0;
	p = (char *)&l;
	i = sizeof(l);
	while (quotchar()) {
		if (--i >= 0)
			*p++ = lastc;
	}
	switch (i) {
	case 4:
		return (0);

	case 3:
		return (l);	/* just the char */

	case 2:
		return ((WORD)swab((TSHORT)l));

	default:
		return ((WORD)swal(l));
	}
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
	printf("FCONST%X:%X", ((long *)va)[0], ((long *)va)[1]);
}

/*
 * print a date
 */

printdate(tvec)
WORD tvec;
{
	char *p;
	char *ctime();

	p = ctime((long *)&tvec);
	p[strlen(p)-1] = 0;		/* stomp on newline */
	printf("%-25s", p);
}
