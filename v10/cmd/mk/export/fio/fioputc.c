#define	FIO_IMP
#include	"fio.h"
#include	<libc.h>

Fputc(fd, c)
{
	register Fbuffer *f;

	FIOSET(f, fd);
	if(f->next >= &f->buf[FIOBSIZE]){
		if(F_flush(f, fd))
			return(-1);
	}
	*f->next++ = c;
	f->end = f->next;
	f->offset++;
	f->flags |= FIO_WRITING;
	return(0);
}
