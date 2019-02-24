#define	FIO_IMP
#include	"fio.h"
#include	<libc.h>

Ffillbuf(fd)
{
	register COUNT n;
	register Fbuffer *f = Ffb[fd];

	if(n = &f->buf[FIOBSIZE]-f->end){
		FIOFLUSH(f->oflush);
		n = read(fd, f->end, n);
		if(n <= 0)
			return(-1);
		f->end += n;
		f->offset += n;
	}
	return(1);
}
