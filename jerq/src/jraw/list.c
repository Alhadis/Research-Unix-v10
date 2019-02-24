#include "jraw.h"
#include "list.h"

List.down(Point p)		{p.x; return 0;}
List.selleck(Rectangle r)	{r.o.x; return 0;}
List.myborder(int mask, Rectangle r)	{mask; r.o.x; return 0;}
Hit List.typeat(Point p, Hit h)	{p.x; return h;}
List.type(int c)		{c; return 0;}
void List.draw(Bitmap *b, int fc)		{}

List *List.unsel(int mask, register List *prev)
{
	prev;		/* shut up!! */
	if ((mod &= ~mask) == 0)
		sel = 0;
	return this;
}

void List.move(Point dp)
{
	register i = sel;
	if (i == 0)
		return;
	draw(&display,F_XOR);
	if (i&1)
		X += dp.x;
	if (i&2)
		Y += dp.y;
	if (i&4)
		U += dp.x;
	if (i&8)
		V += dp.y;
	draw(&display,F_XOR);
}

void List.sort() {R = canon(R);}

List *List.append(List *l)
{
	register List *m;
	for (m = this; m->next != 0; m = m->next)
		;
	m->next = l;
	return this;
}
