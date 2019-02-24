#include <jerq.h>
#include "dline.h"
static
code(p, r)
	Point p;
	Rectangle r;
{
	return( (p.x<r.origin.x? 01 : p.x>=r.corner.x? 02 : 0) |
		(p.y<r.origin.y? 04 : p.y>=r.corner.y?010 : 0));
}

/*
 * Modified clip-to-rectangle algorithm
 *	works in bitmaps
 *	Everything in SCREEN coordinates.
 *
 *	Newman & Sproull 124 (1st edition)
 */
Jdclipline(b, r, p0, p1, f)
	register Bitmap *b;
	Rectangle r;
	Point p0, p1;
{
	register c0, c1;
	register xl, xr, yt, yb;
	Point temp;
	int swapxy=0;

	if(!Jxmajor){
#define XYswap(p)	c0=p.x; p.x=p.y; p.y=c0
		XYswap(p0); XYswap(p1); XYswap(r.origin); XYswap(r.corner);
		swapxy++;
	}
	xl=r.origin.x; xr=r.corner.x;
	yt=r.origin.y; yb=r.corner.y;
	c0=code(p0, r);
	c1=code(p1, r);
	while(c0|c1){
		if(c0&c1)
			return;	/* no point of line in r */
		if(c0==0){	/* swap points */
			short t;
			temp=p0; p0=p1; p1=temp;
			t=c0; c0=c1; c1=t;
		}
		if(c0==0)
			break;
		if(c0&01)	/* push towards left edge */
			p0.y=Jminor(p0.x=xl);
		else if(c0&02)	/* push towards right edge */
			p0.y=Jminor(p0.x=xr-1);
		else if(c0&04){	/* push towards top edge */
			if(Jslopeneg)
				p0.x=Jmajor((p0.y=yt)-1)-1;
			else{
				p0.y=yt;
				p0.x=Jmajor(yt);
			}
		}else if(c0&010){	/* push towards bottom edge */
			if(Jslopeneg){
				p0.y=yb-1;
				p0.x=Jmajor(yb-1);
			}
			else{
				p0.y=yb-1;
				p0.x=Jmajor(yb)-1;
			}
		}
		c0=code(p0, r);
	}
	if(swapxy){
		XYswap(p0); XYswap(p1);
	}
	_dline(b, p0, p1, f);
}
