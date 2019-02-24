#include	<CC/jerq.h>
#include	<CC/font.h>
#include	"pot.pri"

extern void bord(Bitmap *, Rectangle, int, int);

#define		P_H_STRING	0x8000
#define		P_H_VALUE	0x4000

Rulepot.Rulepot(Rectangle rect, short fl, short m, short v) : (rect, fl, v)
{
	maxval = m;
}

Rulepot.Rulepot(Rectangle rect, short fl, short m, Point sp, char *s, short v) : (rect, fl, v)
{
	maxval = m;
	flags |= P_H_STRING;
	stroffset = sp;
	str = s;
}

Rulepot.Rulepot(Rectangle rect, short fl, short m, Point vp, short v) : (rect, fl, v)
{
	maxval = m;
	flags |= P_H_VALUE;
	valoffset = vp;
}

Rulepot.Rulepot(Rectangle rect, short fl, short m, Point vp, Point sp, char *s, short v) : (rect, fl, v)
{
	maxval = m;
	flags |= P_H_STRING|P_H_VALUE;
	valoffset = vp;
	stroffset = sp;
	str = s;
}

void
Rulepot.resize(Rectangle rect)
{
	r = rect;
	draw();
}

void
Rulepot.draw()
{
	int len, i, s, e, horiz = (flags&P_STYLE)==P_HORIZ;
	Point p;
	Rectangle bar;

	bar = inset(r, -1);
	if(horiz)
		bar.o.x++;
	else
		bar.o.y++;
	bord(&display, bar, 1, F_OR);
	bar = r;
	p = r.o;
	if(horiz)
	{
		len = r.c.x - r.o.x;
		s = r.o.x;
		e = r.c.x;
		for(i = 0; i <= 10; i++)
		{
			p.x = (s*(10-i) + e*i)/10;
			segment(&display, p, Pt(p.x, p.y+(i%5? 5:10)), F_OR);
		}
		rx = bar.c.x = bar.o.x + muldiv(val, len, maxval);
	}
	else
	{
		p.x = r.c.x;
		len = r.c.y - r.o.y;
		s = r.c.y;
		e = r.o.y;
		for(i = 0; i <= 10; i++)
		{
			p.y = (s*(10-i) + e*i)/10;
			segment(&display, p, Pt(p.x-(i%5? 5:10), p.y), F_OR);
		}
		rx = bar.o.y = bar.c.y - muldiv(val, len, maxval);
	}
	rectf(&display, bar, F_XOR);
	if(flags&P_H_STRING) string(&defont, str, &display, r.o+stroffset, F_OR);
	if(flags&P_H_VALUE) prval(r.o+valoffset, val);
}

confine(int x, int lo, int hi)
{
	return(min(max(lo, x), hi));
}

void
Rulepot.mod(int but)
{
	int x, len, horiz = (flags&P_STYLE)==P_HORIZ;
	Rectangle bar = r;

	len = horiz? (r.c.x - r.o.x):(r.c.y - r.o.y);
	bar = r;
	while(button(but))
	{
		x = horiz? confine(mouse.xy.x, r.o.x, r.c.x):confine(mouse.xy.y, r.o.y, r.c.y);
		if(rx != x)
		{
			if(flags&P_H_VALUE) prval(r.o+valoffset, val);
			if(horiz)
			{
				bar.o.x = min(x, rx);
				bar.c.x = max(x, rx);
				val = muldiv(x-r.o.x, maxval, len);
			}
			else
			{
				bar.o.y = min(x, rx);
				bar.c.y = max(x, rx);
				val = muldiv(r.c.y-x, maxval, len);
			}
			rx = x;
			rectf(&display, bar, F_XOR);
			if(flags&P_H_VALUE) prval(r.o+valoffset, val);
			nap(2);
		}
	}
}
