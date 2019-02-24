#define	FIO_IMP
#include	<fio.h>

Fprint(f, fmt, args)
	char *fmt;
{
	char buf[FIOBSIZE], *out;
	register long n;
	extern char *doprint();

	out = doprint(buf, fmt, (char *)&args);
	n = out-buf;
	if(Fwrite(f, buf, n) != n)
		return(-1);
	else
		return((int)n);
}
