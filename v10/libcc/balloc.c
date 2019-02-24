#include <jstructs.h>

#define rox	r.origin.x
#define roy	r.origin.y
#define rcx	r.corner.x
#define rcy	r.corner.y

Bitmap *
balloc(r)
Rectangle r;
{
	char *malloc(), *calloc();
	register left, right;
	register Bitmap *bp;

	if ((bp=(Bitmap *)malloc(sizeof(Bitmap))) == (Bitmap *)0)
		return (Bitmap *)0;

	left = rox & ~WORDMASK;
	if ((right=rcx) & WORDMASK) {
		right |= WORDMASK;
		right ++;
	}
	bp->width = (right-left)>>WORDSHIFT;
	bp->rect = r;

	bp->base = (Word *)calloc(bp->width * (rcy-roy), sizeof(Word));
	if (bp->base == (Word *)0){
		free((char *)bp);
		return (Bitmap *)0;
	}
	bp->_null=0;
	return bp;
}

bfree(bp)
	register Bitmap *bp;
{
	if (bp) {
		free((char *)bp->base);
		free((char *)bp);
	}
}
