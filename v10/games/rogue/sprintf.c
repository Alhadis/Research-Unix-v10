
/*LINTLIBRARY*/
#include	<stdio.h>

/*VARARGS2*/
char *
sprintf(str, fmt, args)
char *str, *fmt;
{
	struct _iobuf _strbuf;

	_strbuf._flag = _IOWRT+_IOSTRG;
	_strbuf._ptr = (unsigned char *)str;
	_strbuf._cnt = 0x7fffffff;
	_doprnt(fmt, &args, &_strbuf);
	putc('\0', &_strbuf);
	return(str);
}
