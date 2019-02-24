#include <jerq.h>

#undef	Rpt
#define	Rpt(o,c)	o,c

Rectangle
rsupport(bits,r,bscratch)	/* returns "support" of rectangle in bitmap */
Bitmap *bits, *bscratch; Rectangle r;
{
	Rectangle rsup; Bitmap *bproj; int xsize, ysize; unsigned d; Word *w;

	rsup.origin.x  = rsup.origin.y  =
	rsup.corner.x  = rsup.corner.y  = 0;

	xsize = (r.corner.x-r.origin.x+1)/2;
	ysize =  r.corner.y-r.origin.y;

	if (xsize == 0 || ysize == 0) return rsup;

	bproj = bscratch;
	if (bproj == (Bitmap *)0) bproj=balloc(Rect(0,0,xsize,ysize));
	if (bproj == (Bitmap *)0) return r;

	bitblt(bits,Rpt(r.origin,Pt(r.origin.x+xsize,r.corner.y)),
		bproj,Pt(0,0),F_STORE);
	bitblt(bits,Rpt(Pt(r.origin.x+xsize,r.origin.y),r.corner),
		bproj,Pt(0,0),F_OR);

	while (xsize>1) {
		d=(xsize+1)/2;
		bitblt(bproj,Rect(d,0,xsize,ysize),bproj,Pt(0,0),F_OR);
		xsize=d;
	}

	w=addr(bproj,Pt(0,ysize));
	for (d=r.corner.y; d>r.origin.y; d--) {
		w -= bproj->width;
		if (*w & FIRSTBIT) {
			rsup.corner.y=d;
			break;
		}
	}

	if (rsup.corner.y == 0) return rsup;

	w=bproj->base;
	for (rsup.origin.y=r.origin.y; rsup.origin.y<r.corner.y; rsup.origin.y++) {
		if (*w & FIRSTBIT) break;
		w += bproj->width;
	}

	if (bscratch == (Bitmap *)0) bfree(bproj);

	rsup.origin.x=r.origin.x; rsup.corner.x=r.corner.x;

	xsize =  r.corner.x-r.origin.x;
	ysize = (r.corner.y-r.origin.y+1)/2;
	bproj = bscratch;
	if (bproj == (Bitmap *)0) bproj=balloc(Rect(0,0,xsize,ysize));
	if (bproj == (Bitmap *)0) return rsup;

	bitblt(bits,Rpt(r.origin,Pt(r.corner.x,r.origin.y+ysize)),
		bproj,Pt(0,0),F_STORE);
	bitblt(bits,Rpt(Pt(r.origin.x,r.origin.y+ysize),r.corner),
		bproj,Pt(0,0),F_OR);

	while (ysize>1) {
		d=(ysize+1)/2;
		bitblt(bproj,Rect(0,d,xsize,ysize),bproj,Pt(0,0),F_OR);
		ysize=d;
	}

	w=addr(bproj,Pt(xsize,0)); d=FIRSTBIT>>(xsize%WORDSIZE);
	for (rsup.corner.x=r.corner.x; rsup.corner.x>r.origin.x; rsup.corner.x--) {
		if ((d<<=1) == 0) { w--; d=LASTBIT; }
		if (*w & d) break;
	}

	w=bproj->base; d=FIRSTBIT;
	for (rsup.origin.x=r.origin.x; rsup.origin.x<r.corner.x; rsup.origin.x++) {
		if (*w & d) break;
		if ((d>>=1) == 0) { w++; d=FIRSTBIT; }
	}

	if (bscratch == (Bitmap *)0) bfree(bproj);
	return rsup;
}
