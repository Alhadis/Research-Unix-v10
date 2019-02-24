#include <jerq.h>
#include <font.h>
#define	SPACING	14
Rectangle
drawmenu(menu, pt)
	register Menu *menu;
	Point pt;
{
	register i,xmax;
	register char **s;
	Point p;
	register Bitmap *b;
	Rectangle r;
	extern Font defont;

	s = menu->item;
	xmax = jstrwidth(*s++);
	for (i = 0; *s; i++)
		xmax = max(xmax,jstrwidth(*s++));
	xmax += 10;
	menu->n = i+1;
	if(menu->lasty <= 0)
		menu->lasty = SPACING/2;
	/* move p to origin of rectangle */
	p.x = max(0, pt.x-(xmax>>1));
	p.y = max(0, pt.y-menu->lasty);
	p.x = min(p.x, XMAX-xmax);
	p.y = min(p.y, YMAX-menu->n*SPACING-2);
	r.origin = r.corner = p;
	r.corner.x += xmax;
	r.corner.y += menu->n*SPACING+2;
	menu->b = b = balloc(r);
	cursinhibit();
	if(b)
		bitblt(&display, r, b, p, F_STORE);
	rectf(&display, r, F_OR);
	rectf(&display, inset(r, 1), F_CLR);
	p.y += 2;	/* careful with font ascent */
	for (s = menu->item; *s; s++) {
		string(&defont, *s, &display,
			Pt(p.x+((xmax-jstrwidth(*s))>>1), p.y), F_XOR);
		p.y += SPACING;
	}
	cursallow();
	return r;
}
menuhit(menu,but)
	register Menu *menu;
{
	register hit= -1,y=menu->lasty,oldhit;
	register Bitmap *b;
	Point p;
	Rectangle r;

	r = drawmenu(menu, mouse.xy);
	for (oldhit = -1; bttn(but); nap(2)) {
		if (ptinrect((p = mouse.xy),r)) {
			y = p.y - r.origin.y;
			hit = y/SPACING;
			if (hit<0) hit = 0;
			if (hit>=menu->n) hit = menu->n-1;
			if (hit!=oldhit) {
				/* A little hysteresis */
				if (oldhit>0 &&
				    abs(y-(SPACING/2+hit*SPACING)) > SPACING/3)
					hit = oldhit;
			}
		}
		else
			hit = -1;
		if (hit != oldhit) {
			flip(r,oldhit);
			flip(r,hit);
			oldhit = hit;
		}
	}
	if(b=menu->b){	/* assignment = */
		cursinhibit();
		screenswap(b, b->rect, b->rect);
		cursallow();
		bfree(b);
		menu->b=0;
	}
	menu->lasty = y;
	return(hit);
}

static flip(r,n)
Rectangle r;
register int n;
{
	if (n >= 0) {
		r.origin.y += SPACING*n+1;	/* +1 for the top line... */
		r.corner.y = r.origin.y+SPACING;
		r.origin.x++;
		r.corner.x--;
		rectf(&display, r, F_XOR);
	}
}
