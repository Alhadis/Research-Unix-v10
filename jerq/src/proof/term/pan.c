#include <jerq.h>
#include "font.h"
#include "../comm.h"
#include "term.h"

Texture bullseye = {
	0x07E0, 0x1FF8, 0x399C, 0x63C6, 0x6FF6, 0xCDB3, 0xD99B, 0xFFFF,
	0xFFFF, 0xD99B, 0xCDB3, 0x6FF6, 0x63C6, 0x399C, 0x1FF8, 0x07E0,
};

#define		RX		100

Point
confine(p, r)
	Point p;
	Rectangle r;
{
	if(p.x < r.origin.x) p.x = r.origin.x;
	if(p.y < r.origin.y) p.y = r.origin.y;
	if(p.x > r.corner.x) p.x = r.corner.x;
	if(p.y > r.corner.y) p.y = r.corner.y;
	return(p);
}

static Rectangle
pan_it(but, tr, br)
	Rectangle br, tr;
{
	Rectangle rr;
	Point cent, dif, p;

	cent = div(add(tr.origin, tr.corner), 2);
	dif = sub(cent, tr.origin);
	rr.origin = add(br.origin, dif);
	rr.corner = add(br.corner, sub(cent, tr.corner));
	rectf(&display, tr, F_XOR);
	while(button(but))
	{
		p = confine(dif = mouse.xy, rr);
		if(!eqpt(p, dif)) cursset(p);
		if(!eqpt(cent, p))
		{
			rectf(&display, tr, F_XOR);
			tr = raddp(tr, sub(p, cent));
			rectf(&display, tr, F_XOR);
			cent = p;
		}
		nap(2);
	}
	rectf(&display, tr, F_XOR);
	return(tr);
}

static Point rorg;

static Rectangle
rmap(r)
	Rectangle r;
{
	int scale = arena.page.corner.x - arena.page.origin.x;

	r.origin.x = muldiv(r.origin.x, RX, scale);
	r.origin.y = muldiv(r.origin.y, RX, scale);
	r.corner.x = muldiv(r.corner.x, RX, scale);
	r.corner.y = muldiv(r.corner.y, RX, scale);
	return(raddp(r, rorg));
}

static Point
pimap(p)
	Point p;
{
	p.x = muldiv(p.x-rorg.x, arena.page.corner.x - arena.page.origin.x, RX);
	p.y = muldiv(p.y-rorg.y, arena.page.corner.x - arena.page.origin.x, RX);
	return(p);
}

Pan(but)
{
	Rectangle r, rr;
	Point lastm, ro, p;
	Bitmap *bp;
	Texture *t;

	t = cursswitch(&bullseye);
	if(arena.bp)
	{
		r = riscale(arena.layer);
		rr = rpage;
		rr.corner = add(rr.corner, sub(r.origin, r.corner));
		ro = r.origin;
		lastm = mouse.xy;
		if(own()&MOUSE) while(button(but))
		{
			ro = add(ro, sub(lastm, mouse.xy));
			lastm = mouse.xy;
			ro = confine(ro, rr);
			r = raddp(r, sub(ro, r.origin));
			bitblt(arena.bp, r, &display, Drect.origin, F_STORE);
			nap(2);
		}
		arena.layer = rscale(r);
	}
	else
	{
		/* center layer then r around cursor */
		rorg.x = rorg.y = 0;
		r = rmap(arena.layer);
		rorg = sub(mouse.xy, div(add(r.origin, r.corner), 2));
		rorg = confine(rorg, Rpt(Drect.origin, sub(Drect.corner, r.corner)));
		cursset(add(rorg, div(add(r.origin, r.corner), 2)));
		r = rmap(arena.page);
		if(bp = balloc(inset(r, -1)))
		{
			rectf(bp, bp->rect, F_OR);
			rectf(bp, r, F_XOR);
			screenswap(bp, bp->rect, bp->rect);
		}
		rr = rmap(arena.layer);
		if((own()&MOUSE) && button(but)) rr = pan_it(but, rr, r);
		if(bp)
		{
			screenswap(bp, bp->rect, bp->rect);
			bfree(bp);
		}
		p = pimap(rr.origin);
		arena.layer = raddp(rscale(rsubp(Drect, Drect.origin)), p);
		stipple(display.rect);
		cursor.x = cursor.y = 0;
		(void)cursswitch((Texture *)0);
		reader.start1 = reader.start2 = 0;
		pstate = S_DRAW;
	}
	(void)cursswitch(t);
}
