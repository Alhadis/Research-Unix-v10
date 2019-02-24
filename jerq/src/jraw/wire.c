#include "jraw.h"
#include "wire.h"

Rectangle Wire.mbb(Rectangle r)
{
	r.o.x = min(r.o.x,U);	/* wires are not sorted */
	r.o.y = min(r.o.y,V);
	r.c.x = max(r.c.x,X);
	r.c.y = max(r.c.y,Y);
	return List::mbb(r);
}

List *Wire.copy()
{
	return (sel) ? (List *) new Wire(P,Q) : 0;
}

Wire.down(Point p)
{
	register prop = 0;
	if (P == p)
		sel |= 3;
	else if (Q == p)
		sel |= 12;
	else if (Y == p.y && Y == V && (p.x-X)*(U-p.x) > 0)
		prop = 1;
	else if (X == p.x && X == U && (p.y-Y)*(V-p.y) > 0)
		prop = 1;
	else
		return mod;
	mod |= WIREHIT;
	if (prop && sel != 15) {	/* grab any endpoint goobies */
		sel = 15;
		return mod | selbox(canon(R));
	}
	return mod;
}

Wire.selleck(Rectangle r)
{
	if (P <= r) {
		sel |= 3;
		mod |= WIREBOX;
	}
	if (Q <= r) {
		sel |= 12;
		mod |= WIREBOX;
	}
	return mod;
}

Wire.myborder(int mask, Rectangle r)
{
	if ((sel |= edge(mask,P,r)) || (sel |= (edge(mask,Q,r)<<2)))
		mod |= WIREBOX;
	return mod;
}

char fpt[] = {BL, BC, BR, TC, BL, BC};	/* codes for wire orientations */
char gpt[] = {TL, TL, TR, BL, TL, TL};	/* codes for wire orientations */

Hit Wire.typeat(Point p, Hit h)
{
	register i,j,m;
	if (h.type & BOXHIT)		/* boxes have precedence */
		return h;
	m = h.type;
	if (p == P)
		j = 3;
	else if (p == Q)
		j = 12;
	else	/* no chance */
		return h;
	if (Y == V) {			/* I am horizontal */
		i = (X < U) ? 0 : 2;
		m |= WIREHIT;
	}
	else if (X == U && (m&WIREHIT) == 0) {	/* I am vertical and wimpy */
		i = (Y < V) ? 1 : 3;
		m |= WIREHIT;
	}
	else
		return h;
	h.type = m;
	h.f = (j == 3) ? fpt[i] : fpt[i+2];
	h.g = (j == 3) ? gpt[i] : gpt[i+2];
	return h;
}

void Wire.draw(Bitmap *b, int fc)
{
	segment(b,P,Q,fc);
}

void Ortho.draw(Bitmap *b, int fc)
{
	Point p = P;
	register dx=U-X,dy=V-Y;
#define abs(x)		((x < 0) ? -x : x)
	if (abs(dx) < abs(dy))
		p.y = V;
	else
		p.x = U;
	segment(b,P,p,fc);
	segment(b,p,Q,fc);
}

List *Wire.unsel(int mask, register List *prev)
{
	if (P == Q)
		return remove(prev);
	else
		return List::unsel(mask,prev);
}

void Wire.snap()
{
	Point p,q;
	if (!sel || P == Q)
		return;
	if (X == U) {
		p = Pt(min(Y,V),max(Y,V));
		q = collist(p,this);
		if (p != q) {
			draw(&display,F_XOR);
			Y = q.x;
			V = q.y;
			draw(&display,F_XOR);
		}
	}
	else if (Y == V) {
		p = Pt(min(X,U),max(X,U));
		q = collist(p,this);
		if (p != q) {
			draw(&display,F_XOR);
			X = q.x;
			U = q.y;
			draw(&display,F_XOR);
		}
	}
}

List *Wire.collinear(Point *p, register Wire *w, List *prev)
{
	register i,j;
	if (this == w)		/* hey, I STARTED this! */
		return this;
	if ((i=w->X) == w->U && i == X && i == U) {		/* same x */
		i = min(Y,V);
		j = max(Y,V);
olaptest:
		if (max(i,p->x) <= min(j,p->y)) {		/* they overlap */
			draw(&display,F_XOR);
			*p = Pt(min(i,p->x),max(j,p->y));	/* ugh! */
			return remove(prev);	/* but we must return a List */
		}
		else
			return this;
	}
	else if ((i=w->Y) == w->V && i == Y && i == V) {	/* same y */
		i = min(X,U);
		j = max(X,U);
		goto olaptest;
	}
	else
		return this;
}
