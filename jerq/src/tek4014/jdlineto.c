#include <jerq.h>
#include "dline.h"
static x0, y0;

dsegment(b, p, q, f, mask)
	Bitmap *b;
	Point p, q;
	Word mask;
{
	DotMask = mask;
	if ((f != F_XOR) && (b = &display)) {
		cursinhibit();
		dlsegment(b,p,q,f);
		cursallow();
	} else
		dlsegment(b, p, q, f);
}

floor(x,y)
	register long x;
	register y;
{
	if (y<=0) {
		if (y==0) return((short) x);
		y= -y; x= -x;
	}
	if (x<0) x -= y-1;
	return((short) (x/y));
}

ceil(x,y)
	register long x;
	register y;
{
	if (y<=0) {
		if (y==0) return((short) x);
		y= -y; x= -x;
	}
	if (x>0) x += y-1;
	return((short) (x/y));
}
Jminor(x)
	register x;
{
	register y;
	y=floor(2*(long)(x-x0)*Jdminor+Jdmajor, 2*Jdmajor)+y0;
	return Jslopeneg? -y : y;
}
Jmajor(y)
	register y;
{
	register x;
	x=ceil(2*(long)((Jslopeneg? -y : y)-y0)*Jdmajor-Jdminor, 2*Jdminor)+x0;
	if(Jdminor)
		while(Jminor(x-1)==y)
			x--;
	return x;
}
Point
Jdsetline(p, q)
	Point p, q;
{
	register dx, dy, t;
	Point endpt;
	short swapped=0;
	Jxmajor=1;
	Jslopeneg=0;
	dx=q.x-p.x;
	dy=q.y-p.y;
	if(abs(dy) > abs(dx)){	/* Steep */
		Jxmajor=0;
#define XYswap(p)	t=p.x; p.x=p.y; p.y=t
		XYswap(p);
		XYswap(q);
#define Swap(x, y)	t=x; x=y; y=t
		Swap(dx, dy);
	}
	if(dx<0){
		swapped++;
		Swap(p.x, q.x); Swap(p.y, q.y);
		dx= -dx; dy= -dy;
	}
	if(dy<0){
		Jslopeneg++;
		dy= -dy; p.y= -p.y; q.y= -q.y;
	}
	Jdminor=dy;
	Jdmajor=dx;
	x0=p.x;
	y0=p.y;
	endpt.x=swapped? p.x+1 : q.x-1;
	endpt.y=Jminor(endpt.x);
	if(!Jxmajor){
		XYswap(endpt);
	}
	return(endpt);
}
Jsetdda(x)
	register x;
{
	register y;
	y=Jminor(x);
	if(Jslopeneg)
		y= -y;
	return (short)((2*(x-x0)+2)*(long)Jdminor
		-(2*(y-y0)+1)*(long)Jdmajor);
}
