#include "jraw.h"
#include "list.h"
#define GMASK	(~7)		/* for gridding mbb's */

Rectangle mbblist(register List *l)
{
	Rectangle r = Rect(1000, 1000, -1000, -1000);
	for (; l != 0; l = l->next)
		r = l->mbb(r);
	return r;
}

Rectangle canon(Rectangle r)
{
	Rectangle s = r;
	if (r.c.x < r.o.x) {
		s.o.x = r.c.x;
		s.c.x = r.o.x;
	}
	if (r.c.y < r.o.y) {
		s.o.y = r.c.y;
		s.c.y = r.o.y;
	}
	return s;
}

edge(register mask, Point p, Rectangle r)
{
	if (!(p <= r))
		return 0;
	if ((mask&1) && p.x == r.o.x || (mask&4) && p.x == r.c.x)
		return 1;
	if ((mask&2) && p.y == r.o.y || (mask&8) && p.y == r.c.y)
		return 2;
	return 0;
}

List *cut(List **lp)		/* alters *lp, returns the removed portion */
{
	register List *l,*m,*in=0,*out=0;
	for (l = *lp; l != 0; l = m) {
		m = l->next;
		if (l->sel==15 || (l->mod&ALLHIT))
			out = l->cons(out);
		else
			in = l->cons(in);
	}
	*lp = in;
	return out;
}

List *copy(register List *l)
{
	register List *m=0,*n;
	for (; l != 0; l = l->next)
		if (n = l->copy())
			m = n->cons(m);
	return m;
}

void removelist(List *l)
{
	register List *m;
	for (; l != 0; l = m) {
		m = l->next;
		delete l;
	}
}

void unselect(register mask)
{
	register List *l,*m;
	for (l = dl, m = (List *) &dl; l != 0; l = m->next)
		m = l->unsel(mask,m);
}

void snapsel()
{
	register List *l;
	for (l = dl; l != 0; l = l->next)
		l->snap();
}

Point collist(Point p, Wire *w)
{
	register List *l,*m;
	Point q = p;
	for (l = dl, m = (List *) &dl; l != 0; l = m->next)
		m = l->collinear(&q,w,m);
	return q;
}

void movem(int i)
{
	register List *l;
	while (button123() == i)
		if (fixit())
			for (l = dl; l != 0; l = l->next)
				l->move(dp);
}

void drawlist(register List *l, Bitmap *b, int fc)
{
	if (b == 0)
		b = &display;
	for (; l != 0; l = l->next)
		l->draw(b,fc);
}

void displacelist(register List *l, Point p)
{
	for (; l != 0; l = l->next)
		l->R = l->R + p;
}

void scroll()
{
	Point dp0;
	extern void drawdot();
	fixit();
	while (!button123())
		if (fixit()) {
			dp0 = dp;
			rectf(&display,Drect,F_CLR);
			drawicon();
			fixit();
			dp0 += dp;
			displacelist(dl,dp0);
			offset = offset + dp0;
			drawdot();
			drawlist(dl,&display,F_XOR);
		}
	while (button123())
		;
}

Hit textcode(Point p)
{
	Hit h = Hit(0,BL,TL);
	register List *l;
	for (l = dl; l != 0; l = l->next)
		h = l->typeat(p,h);
	return h;
}

selborder(int mask, Rectangle r)
{
	register i;
	register List *l;
	for (l = dl, i = 0; l != 0; l = l->next)
		i |= l->myborder(mask,r);
	return i;
}

selbox(Rectangle r)
{
	register i;
	register List *l;
	for (l = dl, i = 0; l != 0; l = l->next)
		i |= l->selleck(r);
	return i;
}

selpt(Point p)
{
	register i;
	register List *l;
	for (l = dl, i = 0; l != 0; l = l->next)
		i |= l->down(p);
	return i;
}
