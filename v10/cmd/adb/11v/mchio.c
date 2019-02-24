/*
 * read or print machine-dependent data structures
 * this version for a vax running on a vax
 */

#include "defs.h"

/*
 * read an ASCII expression
 */

char lastc;

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
	return ((WORD)swal(l));
}

/*
 * read a floating point number in VAX format
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
 * print a floating point number in VAX format
 */

fpout(flag, va)
char flag;
char *va;
{
	double d;
	char buf[20];
	char *gcvt();

	if (*(int *)va == 0x00008000) {
		prints("illegal float   ");
		return;
	}
	if (flag == 'f')
		d = *(float *)va;
	else
		d = *(double *)va;
	prints(gcvt(d, 16, buf));
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

/*
 * swap words in a longword
 */

long
swal(l)
{

	return (((l >> 16) & 0xffff) | ((l & 0xffff) << 16));
}
