#define	FIO_IMP
#include	"fio.h"
#include	<libc.h>

long
Fseek(fd, n, cmd)
	long n;
{
	register Fbuffer *f;
	register long dest, k;

	FIOSET(f, fd);
	switch(cmd)
	{
	case 0:
		dest = n;
		break;
	case 1:
		/* allow for the fact we are beyond where we think we are */
		dest = f->offset + n + f->next-f->end;
		break;
	case 2:
		if(f->flags&FIO_WRITING)
			F_flush(f, fd);
		dest = SEEK(fd, n, cmd);
		FIORESET(f);
		f->offset = dest;
		break;
	}
	if(dest < 0)
		return(dest);
	k = f->end - ((f->flags&FIO_WRITING)? f->buf : f->lnext);
	if((dest >= f->offset) || (dest < f->offset-k)){
		if(f->flags&FIO_WRITING)
			F_flush(f, fd);
		FIORESET(f);
		f->offset = dest;
		SEEK(fd, f->offset, 0);
	} else {
		f->next = f->lnext + (dest-(f->offset-k));
		if(f->flags&FIO_WRITING){
			f->offset -= f->end - f->next;
			f->end = f->next;
		}
		SEEK(fd, f->offset, 0);
	}
	return(dest);
}
