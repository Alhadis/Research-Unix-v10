#include "jraw.h"
#include "box.h"

List *Box.copy()
{
	return (sel) ? (List *) new Box(R) : 0;
}

Box.down(Point p)
{
	Rectangle r;
	sort();
	if (ul == p)
		selborder(sel=3,R);
	else if (lr == p)
		selborder(sel=12,R);
	else if (ll == p)
		selborder(sel=9,R);
	else if (ur == p)
		selborder(sel=6,R);
	else if (p < R) {
		sel = 15;
		return (mod |= BOXHIT) | selbox(R);
	}
	else
		return mod;
	return mod |= BOXHIT;
}

Box.selleck(Rectangle r)
{
	if (R <= r) {
		sel = 15;
		mod |= BOXBOX;
	}
	return mod;
}

void drawbox(Rectangle R, Bitmap *b, int fc)
{
	if (b == 0)
		b = &display;
	segment(b,ul,ll,fc);
	segment(b,ll,lr,fc);
	segment(b,lr,ur,fc);
	segment(b,ur,ul,fc);
}

void Box.draw(Bitmap *b, int fc)	{drawbox(R,b,fc);}

Hit Box.typeat(Point p, Hit h)
{
	if (p <= R)
		h.type = BOXHIT | (h.type & LABHIT);
	else
		return h;
	if (p.x == X) {
		h.g = BR;
		h.f = LC;
	}
	else if (p.x == U) {
		h.g = BL;
		h.f = RC;
	}
	else if (p.y == Y) {
		h.g = BL;
		h.f = TC;
	}
	else if (p.y == V) {
		h.g = TL;
		h.f = BC;
	}
	else {
		h.f = BC;
		h.g = TC;
	}
	return h;
}

List *Box.unsel(int mask, register List *prev)
{
	if (sel)	/* might have been moving it */
		sort();
	if (X == U || Y == V)
		return remove(prev);
	else
		return List::unsel(mask,prev);
}

Macro.down(Point p)
{
	Rectangle r;
	sort();
	if (ul == p)
		selborder(sel=3,R);
	else if (lr == p)
		selborder(sel=12,R);
	else if (ll == p)
		selborder(sel=9,R);
	else if (ur == p)
		selborder(sel=6,R);
	else
		return mod;
	return mod |= BOXHIT;
}
Texture grey = {
	0xaaaa, 0x5555, 0xaaaa, 0x5555,
	0xaaaa, 0x5555, 0xaaaa, 0x5555,
	0xaaaa, 0x5555, 0xaaaa, 0x5555,
	0xaaaa, 0x5555, 0xaaaa, 0x5555,
};
void Macro.draw(Bitmap *b, int fc)
{
	Rectangle r = canon(R);
	r.c.x += 1; r.c.y += 1;
	texture(b,r,&grey,fc);
	texture(b,inset(r,1),&grey,fc);
}

Hit Macro.typeat(Point p, Hit h)
{
	if (p > R)
		return h;
	if (p.x == X) {
		h.g = BL;
		h.f = RC;
	}
	else if (p.x == U) {
		h.g = BR;
		h.f = LC;
	}
	else if (p.y == Y) {
		h.g = TL;
		h.f = BC;
	}
	else if (p.y == V) {
		h.g = BL;
		h.f = TC;
	}
	h.type = BOXHIT | (h.type & LABHIT);
	return h;
}
/* no measures
void Measure.draw(Bitmap *b, int fc)
{
	char buf[80];
	if (P != Q) {
		sprintf(buf," %d#%d",(U-X)/8,(Y-V)/8);
		string(&defont,buf,b,Q,fc);
	}
	Box::draw(b,fc);
}
 */
