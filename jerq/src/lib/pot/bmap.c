#include	<CC/jerq.h>
#include	<CC/font.h>
#include	"pot.pri"

static Rectangle r32 = Rectangle( 0,0, 32,32 );

class Bmapitem
{
public:
	Point stroffset;
	char *str;
	Bitmap *map;
	Bmapitem *next;
	Bmapitem(Point, char *, Bitmap *);
};

#define		P_B_STRING	0x8000

Bmappot.Bmappot(Rectangle rect, short fl, short v) : (rect, fl, v)
{
	items = 0;
	nitems = 0;
	corner.x = corner.y = 0;
}

Bmappot.Bmappot(Rectangle rect, short fl, Point p, char *s, short v) : (rect, fl, v)
{
	flags |= P_B_STRING;
	str = s;
	stroffset = p;
	items = 0;
	nitems = 0;
	corner.x = corner.y = 0;
}

void
Bmappot.draw()
{
	register Bmapitem *b;
	int n, style = flags&P_STYLE;
	Point p, step;

	if(flags&P_B_STRING) string(&defont, str, &display, r.o+stroffset, F_OR);
	if(nitems == 0) return;
	step.x = step.y = 0;
	if(style <= P_HORIZ)
	{
		if(flags&P_HORIZ)
			step.x = (corner.x)/(nitems-1);
		else
			step.y = (corner.y-14)/(nitems-1);
		for(n = 0, b = items, p = r.o; b; b = b->next, n++)
		{
			if(n == val)
				bitblt(b->map, b->map->rect, &display, r.o, F_STORE);
			string(&defont, b->str, &display, add(b->stroffset, p), F_OR);
			p = p+step;
		}
	}
	else if(style == P_CIRC)
	{
		Point center = div(add(r.o, r.c), 2);

		for(n = 0, b = items; b; b = b->next, n++)
		{
			if(n == val)
				bitblt(b->map, b->map->rect, &display, r.o, F_STORE);
			string(&defont, b->str, &display, b->stroffset+center, F_OR);
		}
	}
}

void
Bmappot.mod(int but)
{
	register Bmapitem *b;
	int style = flags&P_STYLE, sel, n = max(nitems, 1);

	if(style <= P_HORIZ)
	{
		int gap, horiz = style==P_HORIZ;

		gap = (horiz? (r.c.x-r.o.x):(r.c.y-r.o.y))/n;
		while(button(but))
		{
			sel = (horiz? (mouse.xy.x-r.o.x):(mouse.xy.y-r.o.y))/gap;
			sel = max(0, min(sel, nitems-1));
			if(sel != val)
			{
				val = sel;
				for(b = items; sel--; b = b->next);
				bitblt(b->map, b->map->rect, &display, r.o, F_STORE);
				nap(2);
			} 
		}
	}
	else if(style == P_CIRC)
	{
		Point p, center = div(add(r.o, r.c), 2);
		int ndeg = 360/n;

		while(button(but))
		{
			p = mouse.xy - center;
			sel = (atan2(p.x, p.y)/ndeg + 1)%n;
			if(sel != val)
			{
				val = sel;
				for(b = items; sel--; b = b->next);
				bitblt(b->map, b->map->rect, &display, r.o, F_STORE);
				nap(2);
			} 
		}
	}
	b = 0;
}

void
Bmappot.resize(Rectangle rect)
{
	r = rcenter(Rpt(Pt(0, 0), corner), rect);
	draw();
}

static Bitmap *
bt(Texture32 *t)
{
	Bitmap *b = balloc(Rect(0, 0, 32, 32));

	texture32(b, b->rect, t, F_OR);
	return(b);
}

void
Bmappot.item(Point p, char *s, Texture32 *t)
{
	item(p, s, bt(t));
}

void
Bmappot.item(Point p, char *s, Bitmap *bp)
{
	register Bmapitem *b, *bb;

	bb = new Bmapitem(p, s, bp);
	corner.x = max(corner.x, bp->rect.c.x);
	corner.y = max(corner.y, bp->rect.c.y);
	r = rcenter(Rpt(Pt(0, 0), corner), r);
	if(items == 0)
		items = bb;
	else
	{
		for(b = items; b->next; b = b->next)
			;
		b->next = bb;
	}
	nitems++;
}

Bmapitem.Bmapitem(Point p, char *s, Bitmap *bp)
{
	str = s;
	stroffset = p;
	map = bp;
	next = 0;
}
