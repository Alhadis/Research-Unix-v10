#define	FIO_IMP
#include	"fio.h"
#include	<libc.h>

long
Fwrite(fd, addr, nbytes)
	register char *addr;
	register long nbytes;
{
	register Fbuffer *f;
	register COUNT n;
	long nnbytes = nbytes;

	FIOSET(f, fd);
	f->flags |= FIO_WRITING;
	n = &f->buf[FIOBSIZE] - f->next;
	if(nbytes < n) n = nbytes;
	memcpy(f->next, addr, n);
	f->end = (f->next += n);
	f->offset += n;
	nbytes -= n;
	addr += n;
	if(nbytes){
		if(F_flush(f, fd))
			return(-1L);
		while(nbytes >= FIOBSIZE){
			if(write(fd, addr, (COUNT)FIOBSIZE) != FIOBSIZE)
				return(-1L);
			addr += FIOBSIZE;
			nbytes -= FIOBSIZE;
			f->offset += FIOBSIZE;
		}
		memcpy(f->buf, addr, (COUNT)(n = nbytes));
		f->end = f->next = f->buf+n;
		f->offset += n;
		f->flags |= FIO_WRITING;
	}
	return(nnbytes);
}
