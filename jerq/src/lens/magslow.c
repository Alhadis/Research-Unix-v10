#include <jerq.h>

#define BYTESIZE 8		/* better be a power of 2 */
#define BYTEMASK (BYTESIZE-1)
#define BYTESHIFT 3		/* log2(BYTESIZE) */
#define NUMCHARS 256		/* 2**BYTESIZE */

magslow(from,to,fjump,tjump,hcount,vcount,bits,valid,size)
register char *to;
register unsigned char *from;
register int size;
int fjump, tjump, hcount, valid;
char *bits;
int vcount;
{
	register int i,j,k;
	int m, excess8, excess,size8,initial,acc;

	excess = size-valid;
	to -= excess;
	tjump -= excess;
	size8 = size<<BYTESHIFT;
	excess8 = excess<<BYTESHIFT;
	for ( ; vcount>0; --vcount) {		/* loop on lines */
		initial = excess8;
		for (m = hcount; m>0; --m) {	/* pick up bytes */
			i = *--from;
			for (j = initial; j < size8; j += BYTESIZE) {
				acc = 0;
				for (k = 0; k < BYTESIZE; k++)
					if (i & (1<<((k+j)/size)))
						acc |= (1<<k);
				*--to = acc;
			}
			initial = 0;
		}
		from += fjump;
		to += tjump;
	}
}
