#define	FIO_IMP
#include	"fio.h"
#include	<libc.h>

Fflush(fd)
{
	register Fbuffer *f;

	FIOSET(f, fd);
	if(f->flags&FIO_WRITING)
		return(F_flush(f, fd));
	else
		return(0);
}

F_flush(f, fd)
	register Fbuffer *f;
{
	register COUNT n;

	f->flags &= ~FIO_WRITING;
	if(n = f->next - f->buf){
		if(write(fd, f->buf, n) != n)
			return(-1);
		f->next = f->buf;
	}
	return(0);
}
