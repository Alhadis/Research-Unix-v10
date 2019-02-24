#include <jerq.h>
/*
 * Garbage-compacting version: all Bitmap's come from here
 */
static compact();
#define	STARTAREA	(Word *)(90*1024)
#define	ENDAREA	(Word *)(156*1024-1024)	/* -1024 is for stack (not much!) */
static Word *nextword=STARTAREA;
#define	NBTAB	400
static Bitmap btab[NBTAB];
#define	Height(bp)	(bp->rect.corner.y-bp->rect.origin.y)
/*VARARGS*/
Bitmap *
	/* A Rectangle......! */
balloc(rox, roy, rcx, rcy)
	register short rox, roy, rcx, rcy;
{
	register short left, right;
	register retry=0;
	register Bitmap *bp;
	register unsigned nwords;
	for(bp=btab; bp<&btab[NBTAB]; bp++)
		if(bp->base==(Word *)NULL)
			goto Foundit;
	return (Bitmap *)NULL;
  Foundit:
	left=rox&~WORDMASK;
	if((right=rcx)&WORDMASK){
		right|=WORDMASK;
		right++;
	}
	bp->width=(right-left)>>WORDSHIFT;
	bp->rect.origin.x=rox; bp->rect.origin.y=roy;
	bp->rect.corner.x=rcx; bp->rect.corner.y=rcy;
	/* Now we'll trash rcy */
	nwords=bp->width*(rcy-roy);
	while(ENDAREA-nextword<nwords){
		if(retry++)
			return (Bitmap *)NULL;
		compact();
	}
	bp->base=nextword;
	bp->_null=0;
	nextword+=nwords;
	rectf(bp, bp->rect, F_OR);
	return bp;
}
static
compact(){
	register Word *w, *x;
	register Bitmap *cursor, *this;
	register n;
	w=STARTAREA;
	for(;;){
		x=nextword;
		for(cursor=btab; cursor<&btab[NBTAB]; cursor++)
			if(w<=cursor->base && cursor->base<x){
				x=cursor->base;
				this=cursor;
			}
		if(x>=nextword)
			break;
		this->base=w;
		n=this->width*Height(this);
		do
			*w++= *x++;
		while(--n);
	}
	nextword=w;
}
bfree(bp)
	register Bitmap *bp;
{
	if(btab<=bp && bp<=&btab[NBTAB])
		bp->base=0;
}
