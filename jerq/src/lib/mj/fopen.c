#include <jerq.h>
#include <jerqio.h>
#define _SYSOPEN	1
#define _SYSCLOSE	2
#define _SYSACCESS	8

access(file,mode)
char *file;
{
	tag(_SYSACCESS);
	jputstr(file);
	jputshort(mode);
	jflush();
	return(jgetshort());
}

FILE *
_open(file,mode,f,i)
char *file,*mode;
FILE *f;
{
	tag(_SYSOPEN);
	jputshort(i);
	jputstr(file);
	jputchar(*mode);
	jflush();
	if (i != jgetshort())
		return((FILE *) NULL);
	f->file = i;
	if (*mode == 'r') {
		f->count = 0;
		f->flag |= _IOREAD;
	}
	else {
		f->count = BUFSIZ;
		f->flag |= _IOWRT;
	}
	f->base = f->cp = NULL;
	return(f);
}

FILE *
fopen(file,mode)
char *file,*mode;
{
	register int i;
	register FILE *f;
	extern FILE *_lastbuf;
	for (f = _iob, i = 0; f->flag & (_IOREAD | _IOWRT); f++, i++)
		if (f >= _lastbuf)
			return((FILE *) NULL);
	return(_open(file,mode,f,i));
}

FILE *
freopen(file,mode,f)
char *file,*mode;
FILE *f;
{
	fclose(f);
	return(_open(file,mode,f,fileno(f)));
}

fclose(f)
FILE *f;
{
	if (f->flag != 0) {
		fflush(f);
		free(f->base);
		f->base = f->cp = 0;
		f->flag = 0;
		tag(_SYSCLOSE);
		jputshort(fileno(f));
		jflush();
		return(jgetshort());
	}
	return -1;
}

