#include	<CC/jerq.h>
#include	"pot.pri"

extern void circle(Bitmap *, Point, short, int);
extern void disc(Bitmap *, Point, short, int);

#define		P_H_STRING	0x8000
#define		P_H_VALUE	0x4000

Globepot.Globepot(Rectangle rect, short fl, short lat, short lon) : (rect, fl, lat)
{
	w = lon;
	ctop();
}

Globepot.Globepot(Rectangle rect, short fl, short lat, short lon, Point sp, char *s) : (rect, fl, lat)
{
	w = lon;
	ctop();
	flags |= P_H_STRING;
	stroffset = sp;
	str = s;
}

Globepot.Globepot(Rectangle rect, short fl, short lat, short lon, Point vp) : (rect, fl, lat)
{
	w = lon;
	ctop();
	flags |= P_H_VALUE;
	valoffset = vp;
}

Globepot.Globepot(Rectangle rect, short fl, short lat, short lon, Point vp, Point sp, char *s) : (rect, fl, lat)
{
	w = lon;
	ctop();
	flags |= P_H_STRING|P_H_VALUE;
	valoffset = vp;
	stroffset = sp;
	str = s;
}

void
Globepot.resize(Rectangle rect)
{
	r = rect;
	calc();
	ctop();
	draw();
}

void
Globepot.calc()
{
	radius = min(r.c.x-r.o.x, r.c.y-r.o.y)/2;
	offset = max(1, radius/4);
	center = div(add(r.o, r.c), 2);
}

#define	blob	(*(front? &disc:&circle))(&display, pt, 6, F_XOR)

void
Globepot.draw()
{
	Point a, b;
	long black = 0xFFFFFFFF, dashed = 0xF0F0F0F0;

	circle(&display, center, radius, F_OR);
	a = add(center, Pt(0, -radius));
	b = add(center, Pt(0, radius));
	elarc(&display, center, offset, radius, a, b, F_OR, black);
	elarc(&display, center, offset, radius, b, a, F_OR, dashed);
	a = add(center, Pt(-radius, 0));
	b = add(center, Pt(radius, 0));
	elarc(&display, center, radius, offset, a, b, F_OR, black);
	elarc(&display, center, radius, offset, b, a, F_OR, dashed);
	blob;
	if(flags&P_H_STRING) string(&defont, str, &display, r.o+stroffset, F_OR);
	if(flags&P_H_VALUE) prvals();
}

void
Globepot.mod(int but)
{
	Point p, lp;
	int d, outside = 0;

	while(button(but))
	{
		p = mouse.xy-center;
		d = max(1, sqrt(p.x*(long)p.x + p.y*(long)p.y));
		if(outside)
		{
			if(d < radius-20)
				outside = 0;
		}
		else
		{
			if(d > radius+20)
			{
				outside = 1;
				blob;
				front = !front;
				blob;
			}
		}
		if(d > radius)
		{
			p.x = muldiv(p.x, radius, d);
			p.y = muldiv(p.y, radius, d);
		}
		p = add(p, center);
		if(p != pt)
		{
			blob;
			pt = p;
			blob;
			if(flags&P_H_VALUE) prvals();
			ptoc();
			if(flags&P_H_VALUE) prvals();
			nap(2);
		}
	}
	outside = 0;
}

void
Globepot.setval(short lat, short lon)
{
	blob;
	if(flags&P_H_VALUE) prvals();
	val = lat;
	w = lon;
	ctop();
	if(flags&P_H_VALUE) prvals();
	blob;
}

#define		FUDGE		12

void
Globepot.ptoc()
{
	Point p = sub(pt, center);
	int q, sx, sy, fudge;

	q = sx = sqrt(radius*(long)radius - p.x*(long)p.x);
	sy = sqrt(radius*(long)radius - p.y*(long)p.y);
	fudge = muldiv(FUDGE, sy, radius);
	if(front == 0) q = -q;
	w = (450 - atan2(p.x, q) + fudge)%360;
	fudge = muldiv(FUDGE, sx, radius);
	val = atan2(sy, p.y);
	val = (val > 90)? (360-val):-val;
	if(front)
		if(val+fudge > 90)
			val = 180-val-fudge;
		else
			val += fudge;
	else
		if(val-fudge < -90)
			val = -180-val+fudge;
		else
			val -= fudge;
}

void
Globepot.ctop()
{
	Point p;
	int a, h, fudge;

	front = (w <= 90+FUDGE) || (w > 270+FUDGE);
	calc();
	fudge = FUDGE/2;
	if(front)
		if(val-fudge < -90)
			a = -180-val+fudge;
		else
			a = val-fudge;
	else
		if(val+fudge > 90)
			a = 180-val-fudge;
		else
			a = val+fudge;
	a = (a < 0)? -a:(360-a);
	p.y = muldiv(sin(a), radius, 1024);
	h = muldiv(radius, cos(a), 1024);
	p.x = muldiv(cos((450-w+fudge)%360), h, 1024);

#define	delta(z, rad)	muldiv(FUDGE, sqrt(rad*(long)rad-z*(long)z), rad)

	p.x -= delta(p.y, radius);
	p.y -= delta(p.x, h);
	pt = add(center, p);
}

void
Globepot.prvals()
{
	char buf[128];

	sprintf(buf, "lat=%d long=%d", val, w);
	string(&defont, buf, &display, add(r.o, valoffset), F_XOR);
}
