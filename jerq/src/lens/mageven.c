#include <jerq.h>

/* See the comments in magodd.c.  Since size is known to be even,
 * we're going to move pairs of bytes at a time, so that adds some
 * divisions by two.
 */

magbyeven(from,to,fjump,tjump,hcount,vcount,bits,valid,size)
register unsigned short *to,*bits;
register unsigned char *from;
register int size;
int fjump, tjump, vcount, hcount, valid;
{
	register int k;
	register unsigned short *bitsource;
	char odd,vodd;
	unsigned short *bats;
	int excess,j,wcnt;
	size >>= 1;	/* Divide by sizeof(short), which equals 2 */
	vodd = size&1;
	wcnt = (size + 1)>>1;
	tjump >>= 1;
	valid = (valid + 1)>>1;
	bats = bits + valid;
	excess = size - valid;
	odd = valid++ &1;
	valid >>= 1;
	bits += size;
	to -= excess;
	tjump -= excess;
	do {

		bitsource = bats + *(--from) * size;
		k = valid;
		if (odd)
			goto vmagic;
		do {
			*(--to) = *(--bitsource);
vmagic:
			*(--to) = *(--bitsource);
		} while (--k);

		j = hcount;
		while (--j) {
			bitsource = bits + *(--from) * size;
			k = wcnt;
			if (vodd)
				goto magic;
			do {
				*(--to) = *(--bitsource);
magic:
				*(--to) = *(--bitsource);
			} while (--k);
		}
		from += fjump;
		to += tjump;
	} while (--vcount);
}

