#include	<CC/jerq.h>
#include	<CC/font.h>
#include	"pot.pri"

Pot *base = 0;

Pot.Pot(Rectangle rect, short fl, short v)
{
	r = rect;
	flags = fl&P_MASK;
	val = v;
	help = "";
	next = base;
	base = this;
}

void
Pot.draw()
{
	rectf(&display, r, F_OR);
}

void
Pot.mod(int i)
{
	rectf(&display, r, F_OR);
	i++;	/* shut CC up */
}

void
Pot.resize(Rectangle rect)
{
	r = rect;
}

hitpot(int but)
{
	register Pot *pot;
	Point p;

	p = mouse.xy;
	for(pot = base; pot; pot = pot->next)
		if(ptinrect(p, inset(pot->r, -25))) break;
	if(pot == 0) return(0);
	pot->mod(but);
	return(1);
}

void
helppot(int but)
{
	register Pot *pot;
	Point p;

	p = mouse.xy;
	for(pot = base; pot; pot = pot->next)
		if(ptinrect(p, inset(pot->r, -25))) break;
	if(pot && pot->help && pot->help[0])
	{
		int x = strwidth(&defont, pot->help);
		Bitmap *b;

		if((p.x = pot->r.o.x-x-100) > 0)
			p.y = (pot->r.o.y+pot->r.c.y)/2-7;
		else if((p.x = pot->r.c.x+100)+x < XMAX)
			p.y = (pot->r.o.y+pot->r.c.y)/2-7;
		if(b = balloc(raddp(Rect(0, 0, x+6, 20), p)))
		{
			rectf(b, b->rect, F_OR);
			rectf(b, inset(b->rect, 1), F_CLR);
			string(&defont, pot->help, b, add(b->rect.o, Pt(3, 3)), F_OR);
			screenswap(b, b->rect, b->rect);
			while(button(but)) nap(2);
			screenswap(b, b->rect, b->rect);
			bfree(b);
		}
	}
}

void
drawpot()
{
	register Pot *pot;

	for(pot = base; pot; pot = pot->next)
		pot->draw();
}

void
reshapepot(Rectangle oldr, Rectangle newr)
{
	register Pot *pot;
	Rectangle r;
	Point o = oldr.c-oldr.o, n = newr.c-newr.o;

	for(pot = base; pot; pot = pot->next)
	{
		r.o.x = muldiv(pot->r.o.x-oldr.o.x, n.x, o.x);
		r.o.y = muldiv(pot->r.o.y-oldr.o.y, n.y, o.y);
		r.c.x = muldiv(pot->r.c.x-oldr.o.x, n.x, o.x);
		r.c.y = muldiv(pot->r.c.y-oldr.o.y, n.y, o.y);
		pot->resize(r);
	}
}

void
prval(Point p, short v)
{
	char buf[20];

	sprintf(buf, "%d", v);
	string(&defont, buf, &display, p, F_XOR);
}

Rectangle
rcenter(Rectangle r, Rectangle q)
{
	Point qc = div(add(q.o, q.c), 2);
	Point rc = div(add(r.o, r.c), 2);

	return(raddp(r, sub(qc, rc)));
}

extern void
bord(Bitmap *bp, Rectangle r, int i, int c)
{
	rectf(bp, Rectangle(r.o.x, r.o.y, r.c.x, r.o.y+i), c);
	rectf(bp, Rect(r.o.x, r.o.y+i, r.o.x+i, r.c.y-i), c);
	rectf(bp, Rect(r.c.x-i, r.o.y+i, r.c.x, r.c.y-i), c);
	rectf(bp, Rect(r.o.x, r.c.y-1, r.c.x, r.c.y), c);
}
