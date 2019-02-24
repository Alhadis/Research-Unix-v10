#define	FIO_IMP
#include	"fio.h"

void
Ftie(ifd, ofd)
{
	register Fbuffer *f;

	FIOSET(f, ifd);
	f->oflush = ofd;
}
