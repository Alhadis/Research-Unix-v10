#include <jerq.h>
/*	Fill a disc of radius r centered at x1,y1
 *	The boundary is a sequence of vertically, horizontally,
 *	or diagonally adjacent points that minimize 
 *	abs(x^2+y^2-r^2).
 *
 *	The circle is guaranteed to be symmetric about
 *	the horizontal, vertical, and diagonal axes
 */

disc(b, p, r, f)
	Bitmap *b;
	Point p;
{
	int eps,exy,dxsq,dysq;
	register x0,y0,x1,y1;
	r--;
	eps = 0;
	dxsq = 1;
	dysq = 1 - 2*r;
	x0 = p.x-1;
	x1 = p.x+1;
	y0 = p.y-r-1;
	y1 = p.y+r;
	while(y1 > y0) {
		exy = eps + dxsq + dysq;
		if(-exy <= eps+dxsq) {
			rectf(b, Rect(x0, y0, x1, y0+1), f);
			rectf(b, Rect(x0, y1, x1, y1+1), f);
			y1--;
			y0++;
			eps += dysq;
			dysq += 2;
		}
		if(exy <= -eps) {
			x1++;
			x0--;
			eps += dxsq;
			dxsq += 2;
		}
	}
}
