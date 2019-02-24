/*
 * read or print machine-dependent data structures
 * generic non-cross-machine version
 */

#include "defs.h"

/*
 * read an ASCII expression
 */

extern char lastc;

WORD
ascval()
{
	long l;
	register char *p;
	register int i;

	l = 0;
	p = (char *)&l;
	i = sizeof(l);
	while (quotchar()) {
		if (--i >= 0)
			*p++ = lastc;
	}
	return ((WORD)l);
}

/*
 * read a floating point number
 * the result must fit in a WORD
 */

WORD
fpin(buf)
char *buf;
{
	union {
		WORD w;
		float f;
	} x;
	double atof();

	x.f = atof(buf);
	return (x.w);
}

/*
 * print a floating point number
 */

#define	FPWID	32

fpout(flag, va)
char flag;
char *va;
{
	char buf[FPWID+1];
	char *gcvt();

	if (flag == 'f')
		printf("%-16s", gcvt((double)*(float *)va, 9, buf));
	else
		printf("%-32s", gcvt(*(double *)va, 18, buf));
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
