#include <jerq.h>

/* See the comments in madodd.c */

magby3(from,to,fjump,tjump,hcount,vcount,bits,valid)
register char *to,*bits;
register unsigned char *from;
int fjump, tjump, hcount, valid;
int vcount;
{
	register int j;
	register char *bitsource;
	register char *bats;
	int excess;
	bats = bits + valid;
	bits += 3;
	excess = 3 - valid;
	to -= excess;
	tjump -= excess;
	do {
		bitsource = bats + *(--from) * 3;
		j = valid;
		*(--to) = *(--bitsource);
		if (--j) {
			*(--to) = *(--bitsource);
			if (--j)
				*(--to) = *(--bitsource);
		}
		j = hcount;
		while (--j) {
			bitsource = bits + *(--from) * 3;
			*(--to) = *(--bitsource);
			*(--to) = *(--bitsource);
			*(--to) = *(--bitsource);
		}
		from += fjump;
		to += tjump;
	} while (--vcount);
}

