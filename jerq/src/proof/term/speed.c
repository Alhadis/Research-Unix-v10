#include <jerq.h>
#include "font.h"
#include "../comm.h"
#include "term.h"

extern Texture bullseye;
extern Point confine();

long ntime;
int gap;

#define		DX		300
#define		DY		50

static
gimap(g)
{
	return(muldiv(100, DX, g+50) - DX);
}

static
gmap(x)
{
	return(muldiv(100, DX, x+DX) - 50);
}

static
push_it(but, rr)
	Rectangle rr;
{
	Rectangle r;
	Point p, lastp;

	r = rr;
	r.origin.y += 21;
	r.corner.y -= 2;
	r.corner.x = r.origin.x + gimap(gap);
	lastp = r.corner;
	cursset(lastp);
	nap(1); /* wait for cursset to take effect */
	rectf(&display, r, F_XOR);
	while(button(but))
	{
		p = confine(mouse.xy, rr);
		if(eqpt(p, lastp))
		{
			cursset(p);
			nap(1);
		}
		else
		{
			gap = gmap(p.x - r.origin.x);
			rectf(&display, r, F_XOR);
			r.corner.x = r.origin.x + gimap(gap);
			rectf(&display, r, F_XOR);
			cursset(lastp = p);
			nap(2);
		}
	}
	rectf(&display, r, F_XOR);
}

Speed(but)
{
	Rectangle r;
	Point p;
	Bitmap *bp;
	int x;

	r = raddp(Rect(0,0,DX,DY), Pt(0,0));
	x = gimap(gap);
	p = confine(mouse.xy, Rect(Drect.origin.x+x+2, Drect.origin.y+DY/2,
		Drect.corner.x-DX+x-2, Drect.corner.y-DY/2));
	r = raddp(r, Pt(p.x-x, p.y));
	if(bp = balloc(inset(r, -2)))
	{
		rectf(bp, bp->rect, F_OR);
		rectf(bp, inset(r, -1), F_XOR);
		str("SPEED", 0, bp, bp->rect, bp->rect.origin.y+1);
		str("0", -1, bp, bp->rect, bp->rect.origin.y+6);
		str("186,282", 1, bp, bp->rect, bp->rect.origin.y+6);
		screenswap(bp, bp->rect, bp->rect);
		push_it(but, r);
		screenswap(bp, bp->rect, bp->rect);
		bfree(bp);
	}
}

delay()
{
	int t;

	if((t = (ntime+gap) - realtime()) > 0) sleep(t);
	ntime = realtime();
}

str(s, adj, b, r, y)
	char *s;
	Bitmap *b;
	Rectangle r;
{
	register w, x;

	w = strwidth(&defont, s);
	switch(adj)
	{
	case -1:	/* left adjust */
		x = r.origin.x + 1;
		break;
	case 0:		/* centre */
		x = (r.origin.x+r.corner.x-w)/2;
		break;
	case 1:		/* right adjust */
		x = r.corner.x-1-w;
		break;
	}
	string(&defont, s, b, Pt(x, y), F_XOR);
}
