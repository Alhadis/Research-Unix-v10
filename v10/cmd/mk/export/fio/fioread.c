#define	FIO_IMP
#include	"fio.h"
#include	<libc.h>

#define		MAXREAD		0xff00		/* 64K - 4K */
#define		FIOSHORT	100

long
Fread(fd, addr, n)
	char *addr;
	long n;
{
	register unsigned char *p;
	register Fbuffer *f;
	COUNT k;
	char *oaddr = addr;
	char *goal = addr+n;

	FIOSET(f, fd);
again:
	k = f->end - f->next;
	if(n < k) k = n;
	memcpy(addr, (char *)f->next, k);
	f->next += k;
	f->lnext = f->next;	/* effectively disable undo */
	n -= k;
	addr += k;
	if((n > 0) && (n < FIOSHORT)){	/* buffer reads of less than FIOSHORT */
		if(f->next == f->end)
			f->next = f->end = f->buf;
		if(Ffillbuf(fd) < 0)
			return((long)(addr - oaddr));
		goto again;
	}
	if(n > 0)
		FIOFLUSH(f->oflush);
	while(n > 0){	/* buffer is empty so do real reads */
		k = n;
		if(k > MAXREAD) k = MAXREAD;
		if((k = read(fd, addr, k)) < 0)
			return(-1L);
		if(k == 0) break;
		addr += k;
		n -= k;
		f->offset += k;
	}
	return((long)(addr - oaddr));
}
