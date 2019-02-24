#include "frame.h"

/*
 * The code here and elsewhere requires that strings not be gcalloc()ed
 */

#define	CHUNK	16
#define	ROUNDUP(n)	((n+CHUNK)&~(CHUNK-1))
long	clicks;
uchar *
allocstr(n)
	unsigned n;
{
	register uchar *p;
	p=(uchar *)alloc(ROUNDUP(n));
	if(p==0)
		panic("out of memory");
	return p;
}
insure(b, n)
	register Box *b;
	unsigned n;
{
	register uchar *p;
	if(b->len<0)
		panic("insure");
	if(ROUNDUP(b->len)>n)	/* > guarantees room for terminal NUL */
		return;
	p=allocstr(n);
	copystr(b->ptr, p);
	freestr(b->ptr);
	b->ptr=p;
}
freestr(s)
	uchar *s;
{
	free((char *)s);
}
copystr(from, to)
	register uchar *from, *to;
{
	do;while(*to++=*from++);
}
