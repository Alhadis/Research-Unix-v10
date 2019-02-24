#include <jerq.h>
#include <font.h>
extern Point PtCurrent;
parabola(p0, p1, pb)
Point p0, p1, pb;
{
long x, y;
	long d1sq, d2sq;
	long x1, y1, x0, y0, xb, yb;
	Point p, db;
	long c0x, c0y, c1x, c1y, d1, d2, i;
	long dx, dy;
/*	segment(&display, p0, p1, F_OR);*/
	x0 = p0.x; y0 = p0.y;
	x1 = p1.x; y1 = p1.y;
	xb = pb.x; yb = pb.y;
	dx = xb - x0;
	dy = yb - y0;
	d1sq = dx*dx + dy*dy;
	dx = xb - x1;
	dy = yb - y1;
	d2sq = dx*dx + dy*dy;
	d1 = (long)sqrt(d1sq);
	d2 = (long)sqrt(d2sq);
	if(d1 <= 0 || d2 <= 0){
		segment(&display, p0, p1, F_OR);
		return;
	}

	c0x = x0 + x1 - (xb<<1);
	c1x =  (xb - x0)<<1;
	c0y = y0 + y1 - (yb<<1);
	c1y = (yb - y0)<<1;
	PtCurrent = p0;
	for(i=1;i<=d1/2; i++){
		x = x0 + (i*i*c0x)/d1sq + (i*c1x)/d1;
		y = y0 + (i*i*c0y)/d1sq + (i*c1y)/d1;
		p.x = x;
		p.y = y;
		segment(&display, PtCurrent, p, F_OR);
		PtCurrent = p;
	}
	for(;i<=d2; i++){
		x = x0 + (i*i*c0x)/d2sq + (i*c1x)/d2;
		y = y0 + (i*i*c0y)/d2sq + (i*c1y)/d2;
		p.x = x;
		p.y = y;
		segment(&display, PtCurrent, p, F_OR);
		PtCurrent = p;
	}
}
