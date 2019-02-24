#include <jerq.h>
#include <jerqio.h>
#define _SYSPOPEN	6
#define _SYSPCLOSE	7

FILE *
popen(s,mode)
char *s,*mode;
{
	register int i;
	register FILE *f;
	extern FILE *_lastbuf;
	for (f = _iob, i = 0; f->flag & (_IOREAD | _IOWRT); f++, i++)
		if (f >= _lastbuf)
			return((FILE *)0);
	tag(_SYSPOPEN);
	jputshort(i);
	jputstr(s);
	jputchar(*mode);
	jflush();
	if (i != jgetshort())
		return((FILE *)0);
	f->file = i;
	if (*mode == 'r') {
		f->count = 0;
		f->flag = _IOREAD;
	}
	else {
		f->count = BUFSIZ;
		f->flag = _IOWRT;
	}
	f->cp = f->base = NULL;
	return(f);
}

pclose(f)
FILE *f;
{
	fflush(f);
	free(f->base);
	f->cp = f->base = NULL;
	f->flag = 0;
	tag(_SYSPCLOSE);
	jputshort(fileno(f));
	jflush();
	return(jgetshort());
}

