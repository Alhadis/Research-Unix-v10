#define	FIO_IMP
#include	"fio.h"
#include	<libc.h>

void
Fclose(fd)
{
	register Fbuffer *f;

	FIOSET(f, fd);
	Fflush(fd);
	if(f->flags&FIO_MALLOCED)
		free((char *)f);
	Ffb[fd] = 0;
	close(fd);
}
