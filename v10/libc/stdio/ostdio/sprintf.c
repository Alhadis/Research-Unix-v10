/* @(#)sprintf.c	4.1 (Berkeley) 12/21/80 */
#include	<stdio.h>

int
sprintf(str, fmt, args)
char *str, *fmt;
{
	struct _iobuf _strbuf;
	register int n;

	_strbuf._flag = _IOWRT+_IOSTRG;
	_strbuf._ptr = (unsigned char *) str;
	_strbuf._cnt = 32767;
	n = _doprnt(fmt, &args, &_strbuf);
	putc('\0', &_strbuf);
	return(n);
}

int
snprintf(str, len, fmt, args)
char *str, *fmt;
{
	struct _iobuf _strbuf;
	register int n;

	_strbuf._flag = _IOWRT+_IOSTRG;
	_strbuf._ptr = (unsigned char *) str;
	_strbuf._cnt = len;
	n = _doprnt(fmt, &args, &_strbuf);
	putc('\0', &_strbuf);
	return(n);
}
