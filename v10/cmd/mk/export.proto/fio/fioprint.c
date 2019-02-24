#define	FIO_IMP
#include	<varargs.h>
#include	<fio.h>

/* VARARGS 2 */
Fprint(f, fmt, va_alist)
int f;
char *fmt;
va_dcl
{
	char buf[FIOBSIZE], *out;
	register long n;
	va_list ap;
	extern char *doprint();

	va_start(ap);
	out = doprint(buf, fmt, &ap);
	va_end(ap);
	n = out-buf;
	if(Fwrite(f, buf, n) != n)
		return(-1);
	else
		return((int)n);
}
