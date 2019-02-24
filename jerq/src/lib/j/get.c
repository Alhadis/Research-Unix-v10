#include <jerq.h>
#include <jerqio.h>
#define _SYSREAD	5

getchar()
{
	return(getc(stdin));
}

getc(f)
register FILE *f;
{
	register int c;

	if (f->base == NULL)
		f->cp = f->base = alloc(BUFSIZ);
	if (f->count == 0) {
		f->count = _read(fileno(f),f->base,BUFSIZ);
		if (f->count == 0)
			return(EOF);
		f->cp = f->base;
	}
	f->count--;
	c = *f->cp++;
	c &= 0xff;
	return(c);
}

_read(i,buf,n)
char *buf;
{
	tag(_SYSREAD);
	jputshort(i);
	jflush();
	return(jgetbuf(buf,n));
}
