#include	<jerq.h>
#include	<font.h>
#include	"rebecca.h"

Texture boxcurs = {
	0x43FF, 0xE001, 0x7001, 0x3801, 0x1D01, 0x0F01, 0x8701, 0x8F01,
	0x8001, 0x8001, 0x8001, 0x8001, 0x8001, 0x8001, 0x8001, 0xFFFF,
};
Texture bullseye = {
	0x07E0, 0x1FF8, 0x399C, 0x63C6, 0x6FF6, 0xCDB3, 0xD99B, 0xFFFF,
	0xFFFF, 0xD99B, 0xCDB3, 0x6FF6, 0x63C6, 0x399C, 0x1FF8, 0x07E0,
};

extern Rectangle dr, Nullrect;

static Point
dline(p1, dp, Code)
	Point p1, dp;
{	Point p;

	segment(&display, p1, p = add(p1, dp), Code);
	return(p);
}

outline(r, Code)
	Rectangle  r;
{	register dx = r.corner.x-r.origin.x-1, dy = r.corner.y-r.origin.y-1;
	Point p;

	p = dline(r.origin, Pt(dx, 0), Code);
	p = dline(p, Pt(0, dy), Code);
	p = dline(p, Pt(-dx,0), Code);
	p = dline(p, Pt(0,-dy), Code);
}

quadline(a, b, c, d)
	Point *a, *b, *c, *d;
{
	segment(&display, *a, *b, F_XOR);
	segment(&display, *b, *c, F_XOR);
	segment(&display, *c, *d, F_XOR);
	segment(&display, *d, *a, F_XOR);
}
	
static Rectangle
canon(p1, p2)
	Point p1, p2;
{	Rectangle r;

	r.origin.x = min(p1.x, p2.x);
	r.origin.y = min(p1.y, p2.y);
	r.corner.x = max(p1.x, p2.x);
	r.corner.y = max(p1.y, p2.y);
	return(r);
}

long
distance2(p1, p2)
	Point p1, p2;
{
	long dx = p1.x - p2.x;
	long dy = p1.y - p2.y;
	return (dx*dx + dy*dy);
}

closest(a, b, c, d)
	Point a, b, c, d;
{
	Point pt, tp[4];
	int i, j;
	long di, da;

	pt = mouse.xy;
	tp[0] = a; tp[1] = b;
	tp[2] = c; tp[3] = d;

	di = distance2(pt, a);
	for (i=1, j=0; i < 4; i++)
	{	da = distance2(pt, tp[i]);
		if (da < di)
		{	di = da;
			j = i;
		}
	}
	return j;
}

Getquad(rr, a, b, c, d)
	Rectangle *rr;
	Point *a, *b, *c, *d;
{
	int i;

	*rr = Nullrect;
	Getrect(3, rr);
	if (samerect(*rr, Nullrect)
	||  rr->origin.x == rr->corner.x
	||  rr->origin.y == rr->corner.y)
		return 0;
	cursswitch(&bullseye);
	a->x = rr->origin.x; a->y = rr->origin.y;
	b->x = rr->corner.x; b->y = rr->origin.y;
	c->x = rr->corner.x; c->y = rr->corner.y;
	d->x = rr->origin.x; d->y = rr->corner.y;
	quadline(a, b, c, d);

	while(!button123()) sleep(2);
	while (!button3())
	{	while (button12())
		{	i = closest(*a, *b, *c, *d);
			quadline(a, b, c, d);
			switch (i) {
			case 0: *a = mouse.xy; break;
			case 1: *b = mouse.xy; break;
			case 2: *c = mouse.xy; break;
			case 3: *d = mouse.xy; break;
			}
			quadline(a, b, c, d);
			wait(CPU);
		}
		while(!button123()) sleep(2);
	}
	quadline(a, b, c, d);
	cursswitch((Texture *)0);

	return 1;
}

Getrect(but, r)
	Rectangle *r;
{
	Point p1, p2;
	Rectangle rr;

	*r = Nullrect;
	cursswitch(&boxcurs);
	while(!button123()) nap(1);
	if(!button(but))
	{	cursswitch((Texture *)0);
		return 0;
	}
	p1 = mouse.xy;
	p2 = p1;
	rr = canon(p1, p2);
	outline(rr, F_XOR);
	for(; button(but); nap(2))
	{	outline(rr, F_XOR);
		p2 = mouse.xy;
		rr = canon(p1, p2);
		outline(rr, F_XOR);
	}
	outline(rr, F_XOR);	/* undraw for the last time */
	cursswitch((Texture *)0);
	*r = rr;
	return 1;
}

disp(s, pt)
	char *s;
{
	char str[128];
	strcpy(str, s); strcat(str, "  ");
	string(&defont, str, &display, pt, F_XOR);
}
