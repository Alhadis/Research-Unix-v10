#include "cbt.h"
mvgbt(to, from, ln) register char *from, *to;
{
	if(from > to)
		while(ln-- > 0) *to++ = *from++;
	else if(from < to)
	{	from += ln-1;
		to += ln-1;
		while(ln-- > 0) *to-- = *from--;
	}
}

prnode(b)
hdr *b;
{	int i;
	char *p;
	dkey *d;

	printf("kcnt %d htype %d hlev %d nfree %d ndsz %d\n", b->kcnt, b->htype,
		b->hlev, nfree(b), NDSZ);
	for(i = 0, p = (char *)(b+1); i < b->kcnt; i++) {
		d = (dkey *)p;
		prdkey(d);
		p += d->dlen;
	}
	putchar('\n');
}

prdkey(d)
dkey *d;
{	int i;
	printf("(%d,%d,", d->dlen, d->dcom);
	for(i = 0; i < MAXKLEN &&i < d->dlen - DKEYSZ; i++) /* dlen unsigned */
		putchar(d->dkey[i]);
	printf("),");
}
