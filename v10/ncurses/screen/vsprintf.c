/*	@(#) vsprintf.c: 1.1 10/15/83	(1.1)	*/
/*LINTLIBRARY*/
#include <stdio.h>
#include <varargs.h>
#define MAXINT 32767

extern int _doprnt();

int
vsprintf(string, format, ap)
char *string, *format;
va_list ap;
{
	register int count;
	FILE siop;

	siop._cnt = MAXINT;
	siop._file = _NFILE;
	siop._flag = _IOWRT;
	siop._base = siop._ptr = (unsigned char *)string;
	count = _doprnt(format, ap, &siop);
	*siop._ptr = '\0'; /* plant terminating null character */
	return(count);
}
