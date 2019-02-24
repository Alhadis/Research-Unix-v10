#define	FIO_IMP
#include	"fio.h"
#include	<libc.h>

int
Fclose(fd)
{
	int ret;
	register Fbuffer *f;

	FIOSET(f, fd);
	ret = Fflush(fd);
	if(f->flags&FIO_MALLOCED)
		free((char *)f);
	Ffb[fd] = 0;
	if(close(fd) < 0)
		ret = -1;
	return(ret);
}
