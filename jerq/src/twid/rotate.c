#include <jerq.h>
#include "twid.h"

static Point center;
static Bitmap *tb;
#define F_AND	((Code) 4)

Point
trans(p)
Point p;
{
	Point q;
	q.x = center.x + center.y - p.y;
	q.y = center.y + p.x - center.x;
	return(q);
}

rot1(b,r)
Bitmap *b;
Rectangle r;
{
	register quad;
	int w;
	Point oo,oq,qo,qq;
	Bitmap selfbm;
	register Bitmap *self = &selfbm,*mask,*temp;
	self->base = addr(b,r.origin);
	self->rect = r;
	self->width = b->width;
	self->_null = 0;
	w = r.corner.x - r.origin.x;
	quad = w/2;
	oo = r.origin;
	qo = oo; qo.x += quad;
	oq = oo; oq.y += quad;
	qq = qo; qq.y += quad;
	tb = balloc(r);
	mask = balloc(r);
	temp = balloc(r);
	if(tb==0 || mask==0 || temp==0){
		bfree(tb);		/* bfree(0) is safe */
		bfree(mask);
		bfree(temp);
		return;
	}
	rectf(mask,r,F_CLR);
	rectf(mask,Rpt(oo,qq),F_STORE);
	for (; quad >= 1; quad >>= 1) {
		qo = oo; qo.x += quad;
		oq = oo; oq.y += quad;
		qq = qo; qq.y += quad;
		/*
		 * left and right
		 */
		copy(mask,temp,oo,F_STORE);
		copy(mask,temp,oq,F_OR);
		copy(self,temp,oo,F_AND);
		copy(temp,self,oo,F_XOR);
		copy(self,temp,Pt(oo.x-quad,oo.y),F_XOR);
		copy(self,self,Pt(oo.x-quad,oo.y),F_OR);
		copy(temp,self,qo,F_XOR);
		/*
		 * flip diagonals
		 */
		copy(self,temp,oo,F_STORE);
		copy(self,temp,sub(oo,Pt(quad,quad)),F_XOR);
		copy(mask,temp,oo,F_AND);
		copy(temp,self,oo,F_XOR);
		copy(temp,self,qq,F_XOR);
		/*
		 * new mask
		 */
		copy(mask,mask,sub(oo,Pt(quad/2,quad/2)),F_AND);
		copy(mask,mask,qo,F_OR);
		copy(mask,mask,oq,F_OR);
	}
	bfree(mask);
	bfree(temp);
	bfree(tb);
}

rot2(b,r)
Bitmap *b;
Rectangle r;
{
	bitblt(tb,r,b,trans(Pt(r.origin.x,r.corner.y)),F_STORE);
}

Rectangle
rotate(b,r)
Bitmap *b;
Rectangle r;
{
	Rectangle rr;
	squareop(b,r,rot1);
	center = div(add(r.origin,r.corner),2);
	tb = Balloc(r);
	bitblt(b,r,tb,r.origin,F_STORE);
	rectf(b,r,F_CLR);
	squareop(b,r,rot2);
	bfree(tb);
	rr.origin = trans(Pt(r.origin.x,r.corner.y));
	rr.corner = trans(Pt(r.corner.x,r.origin.y));
	return(rr);
}

squareop(b,r,op)
Bitmap *b;
Rectangle r;
int (*op)();
{
	register n,m;
	Point p;
	m = min(r.corner.x-r.origin.x,r.corner.y-r.origin.y);
	for (n = 1; (n<<1) <= m; n <<= 1)
		;
	p = r.origin;
	while (p.x+n <= r.corner.x) {
		while (p.y+n <= r.corner.y) {
			(*op)(b,Rpt(p,add(p,Pt(n,n))));
			p.y += n;
		}
		if (p.y < r.corner.y)
			squareop(b,Rect(p.x,p.y,p.x+n,r.corner.y),op);
		p.x += n;
		p.y = r.origin.y;
	}
	if (p.x < r.corner.x)
		squareop(b,Rpt(p,r.corner),op);
}

copy(s,d,p,m)
Bitmap *s,*d;
Point p;
Code m;
{
	if (m == F_AND)	{
		bitblt(s,s->rect,tb,tb->rect.origin,F_STORE);
		rectf(tb,tb->rect,F_XOR);
		bitblt(tb,tb->rect,d,p,F_CLR);
	}
	else
		bitblt(s,s->rect,d,p,m);
}

