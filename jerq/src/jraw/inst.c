#include "jraw.h"
#include "inst.h"

Master.Master(List *l)
{
	dl = l;
	kbdstr(s);
	init();
}

void Master.put(FILE *f)
{
	Point p = b->rect.o&~7;
	displacelist(dl,-p);
	fprintf(f,"m %s@\n",s);
	putlist(dl,f);
	fprintf(f,"e\n");
	displacelist(dl,p);
}

void Master.init()
{
	Rectangle r;
	if (dl != 0) {
		r = mbblist(dl);
		r.c = r.c + Pt(1,1);
		bfree(b);
		b = balloc(r);
		drawlist(dl,b,F_OR);	/* don't have to undraw here */
		mbbcnt = 1;
	}
}

int Master.edit(Point p)	/* translate to p before editing */
{
	Point q = b->rect.o;
	p = p - q + (q&7);
	displacelist(dl,p);
	drawlist(dl,0,F_XOR);
	dl = editlist(dl);
	drawlist(dl,0,F_XOR);		/* erase chicken scratchings */
	init();
	return 1;
}

Inst.Inst(register Master *mm, Point p)
{
	m = mm;
	P = Q = p;
	sel = 15;
	mbb(R);
}

Rectangle Inst.mbb(Rectangle r)
{
	if (m->mbbcnt == 0)		/* master is unintialized */
		m->init();
	Q = m->b->rect.c - ((m->b->rect.o&~7)-P) - Pt(1,1);	/* just in case */
	return List::mbb(r);
}

List *Inst.copy()
{
	return sel ? (List *) new Inst(m,P) : 0;
}

Inst.down(Point p)
{
	if (p < R) {
		sel = 15;
		mod |= BOXHIT;
		return mod | selbox(R);
	}
	return mod;
}

Inst.selleck(Rectangle r)
{
	if (R <= r) {
		sel = 15;
		mod |= BOXBOX;
	}
	return mod;
}

Hit Inst.typeat(Point p, Hit h)
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

void Inst.draw(Bitmap *b, int fc)
{
	Point p = m->b->rect.o;
	p.x &= 7;
	p.y &= 7;
	bitblt(m->b,m->b->rect,b,P+p,fc);
}
