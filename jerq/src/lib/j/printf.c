/*
 * Scaled down version of C Library printf.
 * Only %s %u %d (==%u) %o %x %D are recognized.
 */
#include <jerq.h>
#include <jerqio.h>
char *_doprnt();

printf(fmt, x1)
	char *fmt;
	unsigned x1;
{
	_doprnt(stdout, fmt, &x1);
}

fprintf(f, fmt, x1)
	FILE *f;
	char *fmt;
	unsigned x1;
{
	_doprnt(f, fmt, &x1);
}

char *
sprintf(buf, fmt, x1)
	char *buf, *fmt;
	unsigned x1;
{
	FILE fake;
	register FILE *f;
	f = &fake;
	f->cp = f->base = buf;
	f->flag = f->count = 0;
	_doprnt(f, fmt, &x1);
	putc('\0',f);
	return(f->cp);
}

char *
_doprnt(f, fmt, x1)
	register FILE *f;
	register char *fmt;
	unsigned *x1;
{
	register c;
	register unsigned int *adx;
	register long l;
	char *s;

	adx = x1;
	for(;;){
		while((c = *fmt++) != '%') {
			if(c == '\0')
				return(f->cp);
			putc(c,f);
		}
		c = *fmt++;
		if(c=='o' || c=='u' || c=='x')
			sprintn((long)*adx, c=='o'? 8: (c=='x'? 16:10), f);
		else if(c=='d'){	/* Signed, please */
			l= *(int *)adx;
			if(l<0){
				putc('-',f);
				l= -l;
			}
			sprintn(l, 10, f);
		}else if(c == 's') {
			s = *(char **)adx;
			while(c = *s++)
				putc(c,f);
			adx += (sizeof (char *)/sizeof(int)) -1;
		} else if (c == 'D') {
			l=*(long *)adx;
			if(l<0){
				putc('-', f);
				l=-l;
			}
			sprintn(l, 10, f);
			adx += (sizeof(long) / sizeof(int)) - 1;
		} else if (c == 'X') {
			sprintn(*(long *)adx, 16, f);
			adx += (sizeof(long) / sizeof(int)) - 1;
		} else if (c == 'c') {
			putc(*adx,f);
		} else
			putc(c, f);
		adx++;
	}
}

/*
 * Print an unsigned integer in base b.
 */
sprintn(n, b, f)
	unsigned long n;
	FILE *f;
{
	register long a;

	if(a = n/b)
		sprintn(a, b, f);
	putc("0123456789ABCDEF"[(int)(n%b)],f);
}
