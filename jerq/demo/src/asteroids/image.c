#include <jerq.h>
/*
 * Images are encoded as arrays of characters, and interpreted into
 * bitmaps by buildimage().  The coordinate system has (0,0) at the
 * "location" of the object.
 *
 * Operators are:
 *	'R'	minimum enclosing rectangle  (must be first!)
 *	'p'	point
 *	'r'	rectf
 *	's'	segment
 *	'm'	moveto
 *	'l'	lineto
 *	0	end
 *
 * All objects are drawn with F_OR.
 */
Point
pscale(p)
	Point p;
{
#define	o	Drect.origin
#define	c	Drect.corner
	p.x=muldiv(c.x-o.x, p.x, XMAX);
	p.y=muldiv(c.y-o.y, p.y, YMAX);
#undef	c
#undef	o
	return p;
}
Rectangle
rscale(r)
	Rectangle r;
{
	r.origin=pscale(r.origin);
	r.corner=pscale(r.corner);
	return r;
}
/*
 * Reduce *p to be on the screen
 */
onscreen(p)
	register Point *p;
{
	p->x%=XMAX;
	p->y%=YMAX;
	if(p->x < 0)
		p->x+=XMAX;
	if(p->y < 0)
		p->y+=YMAX;
}
Bitmap *
buildimage(s)
	register char *s;
{
	register Bitmap *b=0;
	Rectangle r;
	Point p, curpt;
	if(sext(*s++) != 'R')
		quit();
	r.origin.x= sext(*s++);
	r.origin.y= sext(*s++);
	r.corner.x= sext(*s++);
	r.corner.y= sext(*s++);
	b=balloc(rscale(r));
	if(b==0)
		quit();
	rectf(b, b->rect, F_CLR);
	for(;;){
		switch(sext(*s++)){
		case 'p':
			p.x= sext(*s++);
			p.y= sext(*s++);
			point(b, pscale(p), F_OR);/**/
			break;
		case 'r':
			r.origin.x= sext(*s++);
			r.origin.y= sext(*s++);
			r.corner.x= sext(*s++);
			r.corner.y= sext(*s++);
			rectf(b, rscale(r), F_OR);/**/
			break;
		case 's':
			r.origin.x= sext(*s++);
			r.origin.y= sext(*s++);
			r.corner.x= sext(*s++);
			r.corner.y= sext(*s++);
			r=rscale(r);
			segment(b, r.origin, r.corner, F_OR);/**/
			break;
		case 'm':
			p.x= sext(*s++);
			p.y= sext(*s++);
			curpt=pscale(p);/**/
			break;
		case 'l':
			p.x= sext(*s++);
			p.y= sext(*s++);
			p=pscale(p);
			segment(b, curpt, p, F_OR);/**/
			curpt=p;
			break;
		case 0:
			return b;
		}
	}
}

sext(c)
	register c;
{
	if(c&0x80) c |= 0xFFFFFF00;
	return(c);
}
