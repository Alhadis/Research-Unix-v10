#define	FIO_IMP
#include	"fio.h"
#include	<libc.h>

static Fdie()
{
	Fexit(0);
}

void
Finit(fd, buf)
	char *buf;
{
	register Fbuffer *f;
	static setfioexit = 1;

	if(setfioexit){
		setfioexit = 0;
		atexit(Fdie);
	}
	fd &= 0x7f;
	if(buf){
		Ffb[fd] = (Fbuffer *)buf;
		Ffb[fd]->flags = 0;
	} else if(!Ffb[fd]){
		Ffb[fd] = (Fbuffer *)FIOMALLOC((COUNT)sizeof(Fbuffer));
		Ffb[fd]->flags = FIO_MALLOCED;
	}
	f = Ffb[fd];
	FIORESET(f);
	f->offset = SEEK(fd, 0L, 1);
	if(f->offset < 0)
		f->offset = 0;		/* file not been written yet */
}

#ifdef	GNOT
#define	NOFILE	128
#endif
#ifndef	GNOT
#include	<sys/param.h>
#endif

Fbuffer *Ffb[NOFILE];

Fexit(type)
{
	register n;

	for(n = 0; n < NOFILE; n++)
		if(Ffb[n])
			if(type == 0)
				Fflush(n);
			else
				Fclose(n);
}
