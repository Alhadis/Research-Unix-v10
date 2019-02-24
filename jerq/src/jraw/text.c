#include <CC/jerq.h>
#include <CC/font.h>
#include "jraw.h"
#include "text.h"
extern Font tiny;

Text.Text(Point p, int ff, int gg)
{
	P = p;
	f = ff;
	g = gg;
	sel = 15;
	which = 0;
	s = new char[20];
	t = new char[20];
	point(&display,p,F_XOR);
}

Text.Text(Point p, char *ss, int ff, char *tt, int gg)
{
	P = p;
	s = new char[20];
	strcpy(s,ss);
	t = new char[20];
	strcpy(t,tt);
	f = ff;
	g = gg;
	which = 0;
	sel = 15;
}

Rectangle bb(char *s, int f, Point p)
{
	register h,w;
	Rectangle r;
	r.o = p;
	h = tiny.height;
	w = strwidth(&tiny,s);
	if (f & (BC|TC))
		r.o.x -= w/2;
	if (f & (BR|RC|TR))
		r.o.x -= w+1;
	if (f & (LC|RC))
		r.o.y -= h/2;
	if (f & (BL|BC|BR))
		r.o.y -= h;
	r.o.x++;
	r.c.x = r.o.x+w;	/* r.c = r.o + Pt(w,h) */
	r.c.y = r.o.y+h;	/* but this is 32 ^%#$& bytes less! */
	return r;
}

Rectangle Text.mbb(Rectangle r)
{
	if (*s != 0)
		r = r.mbb(bb(s,f,P));
	if (*t != 0)
		r = r.mbb(bb(t,g,P));
	return r;
}

List *Text.copy()
{
	if (sel == 0)
		return 0;
	return (List *) new Text(P,s,f,t,g);
}

Text.down(Point p)
{
	if (p == P) {
		sel = 15;
		mod |= LABHIT;
	}
	which = 0;
	return mod;
}

Text.selleck(Rectangle r)
{
	if (P <= r) {
		sel = 15;
		mod |= LABBOX;
	}
	return mod;
}

Text.myborder(int mask, Rectangle r)
{
	if (sel |= edge(mask,P,r))
		mod |= LABBOX;
	return mod;
}

Hit Text.typeat(Point p, Hit h)
{
	if (p == P) {
		sel = 15;
		h.type |= LABHIT;
	}
	return h;
}

Text.type(int c)
{
	register i=0;
	register char *a,**aa;
	if (sel == 0)
		return 0;
	aa = which ? &t : &s;
	draw(&display,F_XOR);
	if (c == '\027')	/* ^W */
		**aa = 0;
	else if (c == '\r')
		which = 1-which;	/* switch on CR */
	else {
		for (a = *aa; *a && i<18; a++, i++)	/* stupidest possible algorithm */
		;
		if (c == '\b' && a > *aa)
			a--;
		else
			*a++ = c;
		*a = 0;
	}
	draw(&display,F_XOR);
	return 1;
}

void Text.draw(Bitmap *b, int fc)
{
	point(b,P,fc);
	if (*s != 0)
		string(&tiny,s,b,bb(s,f,P).o,fc);
	if (*t != 0)
		string(&tiny,t,b,bb(t,g,P).o,fc);
}

List *Text.unsel(int mask, register List *prev)
{
	if (*s == 0 && *t == 0) {
		delete s;
		delete t;
		return remove(prev);
	}
	else
		return List::unsel(mask,prev);
}

void Text.snap()
{
	Hit h = Hit();
	if (sel) {
		h = textcode(P);
		if (h.f != f || h.g != g) {
			draw(&display,F_XOR);
			f = h.f;
			g = h.g;
			draw(&display,F_XOR);
		}
	}
}
