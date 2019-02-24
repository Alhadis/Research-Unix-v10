#include <varargs.h>

#define	SIZE	1024
extern int	printcol;

char	*doprint();

/* VARARGS 1 */
print(fmt, va_alist)
char *fmt;
va_dcl
{
	char buf[SIZE], *out;
	va_list ap;

	va_start(ap);
	out = doprint(buf, fmt, &ap);
	va_end(ap);
	return write(1, buf, (int)(out-buf));
}

/* VARARGS 2 */
fprint(fd, fmt, va_alist)
char *fmt;
va_dcl
{
	char buf[SIZE], *out;
	va_list ap;

	va_start(ap);
	out = doprint(buf, fmt, &ap);
	va_end(ap);
	return write(fd, buf, (int)(out-buf));
}

/* VARARGS 2 */
sprint(buf, fmt, va_alist)
char *buf;
char *fmt;
va_dcl
{
	char *out;
	int scol;
	va_list ap;

	scol = printcol;
	va_start(ap);
	out = doprint(buf, fmt, &ap);
	va_end(ap);
	printcol = scol;
	return out-buf;
}
