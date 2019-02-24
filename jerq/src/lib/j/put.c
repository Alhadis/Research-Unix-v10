#include <jerq.h>
#include <jerqio.h>
#define _SYSWRITE	3

putchar(c)
{
	putc(c,stdout);
}

putc(c,f)
register FILE *f;
{
	register i = 0;

	if (f->base == NULL)
	{
		f->cp = f->base = alloc(BUFSIZ);
		if (f->base == NULL)
			return(-1);
		f->count = BUFSIZ;
	}
	*f->cp++ = c;
	if (--f->count == 0 || ((c == '\n') && isatty(f)))
		i = fflush(f);
	return(i);
}

fflush(f)
register FILE *f;
{
	register int n, r;

	if ((f->flag & _IOWRT) && ((n = f->cp - f->base) > 0))
		r = _write(fileno(f),f->base,n);
	f->cp = f->base;
	f->count = BUFSIZ;
	return((r == n) - 1);
}

puts(s)
register char *s;
{
	fputs(s,stdout);
}

fputs(s,f)
register char *s;
register FILE *f;
{
	register int c;
	while (c = *s++)
		putc(c,f);
}

_write(fd,buf,n)
char *buf;
{
	tag(_SYSWRITE);
	jputshort(fd);
	jputbuf(buf,n);
	return(jgetshort());
}

