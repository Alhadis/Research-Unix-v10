#include <jerq.h>

/*
 *	Notes on strategies for magnification:
 *
 *	Most of what goes on in magnification is to pick up a byte
 *	by *(--from).  We then look up the corresponding bytes
 *	from bits, and place them at *(--to).  There are size bytes
 *	to be placed.  To speed things up a bit, all of the inner loops
 *	have been unrolled once.  What this means is that a loop like:
 *
 *	i = count;
 *	do
 *		loopbody;
 *	while (--i);
 *
 *	which executes loopbody count (>0) times, will be written
 *
 *	CountIsOdd = count & 1;			-- is count odd?
 *	i = (count+1)/2;			-- for efficiency, we use
 *						   >>1 instead of /2.
 *	if (CountIsOdd)
 *		goto SkipOnce;
 *	do
 *		loopbody;			-- for even iterations
 * SkipOnce:
 *		loopbody;			-- for odd iterations
 *	while (--i);
 *
 *	which chages nothing, but runs faster if count is modestly large.
 *	We could unroll more, but it starts to get complicated, and
 *	only pays for even larger counts.
 *
 *	The next idea is that we are expanding the bytes backwards, to
 *	make it easier to cope with the excess bits.  But mostly, we're
 *	simply trying to do the following to each byte:
 *
 *	bitsource = bits + *(--from) * size; 	-- appx. bits[*--from])
 *	k = size;
 *	do
 *		*(--to) = *(--bitsource);	-- copy from the array to dest
 *	while (--k);
 *
 *	Everything else is to loop on the horizontal and vertical
 *	directions; to avoid expanding too many bits; and to
 *	unroll loops.
 *
 *	Particular exceptions are flagged.
 */

magbyodd(from,to,fjump,tjump,hcount,vcount,bits,valid,size)
register char *to,*bits;
register unsigned char *from;
register int size;
int fjump, tjump, vcount, hcount, valid;
{
	register int k;
	register char *bitsource;
	char odd;
	char *bats;
	int excess,j,wcnt;

	bats = bits + valid;
	wcnt = size + 1;	/* unroll main loop */
	wcnt >>= 1;
	bits += size;
	excess = size - valid;
	odd = valid++ &1;	/* unroll valid bits loop */
	valid >>= 1;
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
		}while (--k);

		j = hcount;
		while (--j) {
			bitsource = bits + *(--from) * size;
			k = wcnt;
			goto magic;	/* size is known to be odd */
			do {
				*(--to) = *(--bitsource);
magic:
				*(--to) = *(--bitsource);
			}while (--k);
		}
		from += fjump;
		to += tjump;
	} while (--vcount);
}

