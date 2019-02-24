#include "paint.h"
#define	NSTACK	2048
Point stack[NSTACK];
/*
 * Fill the region in rectangle r of bitmap b containing the given seed point.
 * If PAINT is defined, the routine is broken so that it only works as used in the
 * paint program, but it will indicate its progress on the screen.
 * Remember: the most significant bit is on the left (i.e. shift left to move
 * left.)
 */
#define	PAINT
fill(b, r, seed)
Bitmap *b;
Rectangle r;
Point seed;
{
	Point p;
	register Word *a, *rp, mask;
	register left, right;
	register Point *sp=stack;
	register Bitmap *line=balloc(Rect(r.or.x, 0, r.co.x, 1));
	if(line==0)
		return(0);
	if(!ptinrect(seed, r))
		return(1);
	*sp++=seed;
	while(sp!=stack){
		seed = *--sp;
		/* scan left */
		a=addr(b, seed);
		mask=1<<(WORDSIZE-1-(seed.x&WORDMASK));
		for(left=seed.x;left>=r.or.x && (*a&mask)==0;--left){
			mask<<=1;
			if(mask==0){
				--a;
				mask=1;
			}
		}
		left++;			/* oops, overshot */
		/* scan right */
		a=addr(b, seed);
		mask=1<<(WORDSIZE-1-(seed.x&WORDMASK));
		for(right=seed.x;right<r.co.x && (*a&mask)==0;right++){
			mask=(mask>>1)&~(1<<(WORDSIZE-1));	/* logical shift */
			if(mask==0){
				a++;
				mask=1<<(WORDSIZE-1);
			}
		}
		rectf(b, Rect(left, seed.y, right, seed.y+1), F_XOR);
		if(seed.y+1!=r.co.y){
			/* look for seed points on the line below */
			rectf(line, line->rect, F_CLR);
			rectf(line, Rect(left, 0, right, 1), F_XOR);
			bitblt(b, Rect(left, seed.y+1, right, seed.y+2),
				line, Pt(left, 0), F_CLR);
			bitblt(line, Rect(left+1, 0, right+1, 1),
				line, Pt(left, 0), F_CLR);
			a=addr(line, Pt(left, 0));
			rp=addr(line, Pt(right, 0))+1;
			p.x=left&~WORDMASK;
			p.y=seed.y+1;
			while(a!=rp){
				if(*a){
					mask=1<<(WORDSIZE-1);
					while(mask){
						if(*a&mask && sp!=&stack[NSTACK])
							*sp++=p;
						p.x++;
						mask=(mask>>1)&~(1<<(WORDSIZE-1));
					}
				}
				else
					p.x+=WORDSIZE;
				a++;
			}
		}
		if(seed.y!=r.or.y){
			/* look for seed points on the line above */
			rectf(line, line->rect, F_CLR);
			rectf(line, Rect(left, 0, right, 1), F_XOR);
			bitblt(b, Rect(left, seed.y-1, right, seed.y),
				line, Pt(left, 0), F_CLR);
			bitblt(line, Rect(left+1, 0, right+1, 1),
				line, Pt(left, 0), F_CLR);
			a=addr(line, Pt(left, 0));
			rp=addr(line, Pt(right, 0))+1;
			p.x=left&~WORDMASK;
			p.y=seed.y-1;
			while(a!=rp){
				if(*a){
					mask=1<<(WORDSIZE-1);
					while(mask){
						if(*a&mask && sp!=&stack[NSTACK])
							*sp++=p;
						p.x++;
						mask=(mask>>1)&~(1<<(WORDSIZE-1));
					}
				}
				else
					p.x+=WORDSIZE;
				a++;
			}
		}
#ifdef	PAINT
		rectf(canvas->grn, Rect(left, seed.y, right, seed.y+1), F_XOR);
		if(button123()){
			bfree(line);
			return 0;
		}
		wait(CPU);
#endif
	}
	bfree(line);
	return 1;
}
texfill(im, seed)
Image *im;
Point seed;
{
	extern Texture dead;
	register Texture *t=cursswitch(&dead);
	register Bitmap *alf=balloc(im->rect);
	if(alf){
		bitblt(im->alf, im->rect, alf, im->rect.or, F_STORE);
		if(fill(alf, im->rect, seed)){
			bitblt(im->alf, im->rect, alf, im->rect.or, F_CLR);
			texcomp(im, im->rect, &tex, alf, C_AoverB);
		}
		else{
			bitblt(im->alf, im->rect, alf, im->rect.or, F_CLR);
			bitblt(alf, alf->rect, im->grn, im->rect.or, F_CLR);
		}
		bfree(alf);
	}
	cursswitch(t);
}
