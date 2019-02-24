#include <jerq.h>

/* See the comments in magodd.c.  We ignore the valid bits problem here,
 * because we can never be wrong by more than one byte, which will keep
 * us inside the right bitmap.  We are also going to move pairs of bytes
 * by using short *to, instead of char *to, since 2 is even.
 */

magby2(from,to,fjump,tjump,hcount,vcount,bits)
register unsigned short *to,*bits;
register unsigned char *from;
int fjump, tjump, hcount;
register int vcount;
{
	register int j;
	register char odd;
	tjump >>=1;
	odd = hcount++ & 1;
	hcount >>=1;
	do {
		j = hcount;
		if (odd)
			goto magic;
		do {
			*(--to) = bits[*(--from)];
magic:
			*(--to) = bits[*(--from)];
		} while (--j);
		from += fjump;
		to += tjump;
	} while (--vcount);
}

