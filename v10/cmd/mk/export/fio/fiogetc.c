#define	FIO_IMP
#include	"fio.h"
#include	<libc.h>

Fgetc(fd)
{
	register Fbuffer *f;

	FIOSET(f, fd);
	if(f->next >= f->end){
		if(f->end >= &f->buf[FIOBSIZE])
			f->end = f->next = f->buf;
		else
			f->next = f->end;
		if(Ffillbuf(fd) <= 0)
			return(-1);
	}
	f->lnext = f->next;
	f->flags &= ~(FIO_RDLAST|FIO_WRITING);
	return(*f->next++);
}
