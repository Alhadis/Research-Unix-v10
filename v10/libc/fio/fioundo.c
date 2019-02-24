#define	FIO_IMP
#include	"fio.h"
#include	<libc.h>

void
Fundo(fd)
{
	register Fbuffer *f;

	FIOSET(f, fd);
	if(f->flags&FIO_WRITING)
		return;
	if(f->flags&FIO_RDLAST)
		f->next[-1] = '\n';
	f->next = f->lnext;
}
