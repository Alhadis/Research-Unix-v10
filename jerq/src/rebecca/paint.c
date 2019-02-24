#include	"/usr/jerq/include/jerq.h"
#include	"instruments.h"
#include	"rebecca.h"

extern Rectangle Paint, Rul, Gitrec, Nullrect, dr;
extern int xoff, yoff, magn, res;
extern char anychange;
extern unsigned char *photo[SZ];
extern short have[SZ], l_side[SZ], r_side[SZ];
extern int Sem1, *ALIVE, QUIT;

int prevp1wx = 2*SZ;
int prevp1wy = 2*SZ;

unsigned char curval=0;

/*** Paint box ***/

clearpaint() { drawpaint(); }

makepaint(z)
	Point z;
{
	Paint.origin = z;
	Paint.corner = add(z, Pt(60, 40));
	drawpaint();
}

drawpaint()
{	Rectangle box, box2;

	box = realbox(Paint);
	box2.origin = add(box.origin, Pt(20, 0));
	box2.corner = sub(box.corner, Pt(20, 20));
	outline(box, F_XOR);
	segment(&display, box2.origin, add(box2.origin, Pt(0, 20)), F_XOR);
	segment(&display, box2.corner, sub(box2.corner, Pt(0, 20)), F_XOR);
	segment(&display, add(box.origin,Pt(0,20)),sub(box.corner,Pt(0,20)),F_XOR);
	Tag("+", Pt(box.origin.x+10, box.origin.y+10));
	Tag("=", Pt(box.origin.x+30, box.origin.y+10));
	Tag("-", Pt(box.origin.x+50, box.origin.y+10));
	Tag("sheet", Pt(box.origin.x+10, box.origin.y+30));
}

flashpaint(how)
{	Rectangle box;

	box = realbox(Paint);
	switch (how) {
	case 0:	box.corner.y -= 20; box.corner.x -= 40; break;
	case 1:	box.corner.y -= 20; box.origin.x += 20; box.corner.x -= 20; break;
	case 2: box.corner.y -= 20; box.origin.x += 40; break;
	case 3: box.origin.y += 20; break;
	}
	rectf(&display, box, F_XOR);
}

actpaint()
{	Point pt, ppt; char str, which;
	extern int Sem3, Sem6;

	pt = sub(mouse.xy, dr.origin);
	while (button1() && ptinrect(pt, Paint) && OK)
	{	which = 0;
		if (pt.x - Paint.origin.x > 20) which++;
		if (pt.x - Paint.origin.x > 40) which++;
		if (pt.y - Paint.origin.y > 20) which=3;
		flashpaint(which);
		sleep(3);
		flashpaint(which);
		pt = sub(mouse.xy, dr.origin);
	}
	if (!ptinrect(pt, Paint)) return;

	flashpaint(which);
	switch (which) {
	case 0: str = '+'; break;
	case 1: str = 'v'; break;
	case 2: str = '-'; break;
	case 3: Psem(&Sem6); flashpaint(which); return;
	}
	while (!button123() && OK) ;

	valhost(curval, str);
	do {
		pt = sub(mouse.xy, dr.origin);
		while (button13() && OK)
		{	if (ptinrect(pt, Rect(0, 0, SZ, SZ)))
				pixblot(whereami(pt), curval, which);
			wait(CPU);
			pt = sub(mouse.xy, dr.origin);
		}
		if (button2())
		{	if (ptinrect(pt, Rul))
			{	curval = pt.x/width;
				valhost(curval, str);
			} else
			if (ptinrect(pt, Rect(0, 0, SZ, SZ)))
			{	ppt = whereami(pt);
				curval = photo[ppt.y][ppt.x];
				valhost(curval, str);
			}
		}
		Sem3 = 0;
		wait(CPU);
	} while (!(button2() && ptinrect(pt, Paint)) && OK);
	flashpaint(which);
	flushit();
	while (button123()) ;
}

pixblot(p1, val, How)
	Point p1;
{
	Rectangle r;
	int i, j, k;

	if (button1())
	{	if (p1.x == prevp1wx && p1.y == prevp1wy) return;
		if (!magn)
		{	r.origin = p1;
			r.corner = add(p1, Pt(res,res));
			goto more;
		}
		if (!resident(p1.x, p1.y)) return;
		if (!(How == 1 && photo[p1.y][p1.x] == val)) ptohost(p1, 'b', 0);
		switch (How) {
		case 0:	photo[p1.y][p1.x] = min(255, photo[p1.y][p1.x]+val); break;
		case 1:	photo[p1.y][p1.x] = val; break;
		case 2:	photo[p1.y][p1.x] = max(0, photo[p1.y][p1.x]-val); break;
		}
		putdot(p1);
		prevp1wx = p1.x;
		prevp1wy = p1.y;
		anychange = 1;
	}
	if (button3())
	{
		Psem(&Sem1);
		r = Gitrec;
		if (samerect(r, Nullrect)) return;
		r.origin = whereami(sub(r.origin, dr.origin));
		r.corner = whereami(sub(r.corner, dr.origin));
more:		rectohost(r, 'r');
		flushit();
		crunch(r, How, val);
		anychange = 1;
	}
}

crunch(r, How, val)
	Rectangle r;
{
	register int i, j; int ves;

	for (i = r.origin.y; i < r.corner.y; i++)
	{	if (i >= SZ || !resident(r.origin.x, i))
			continue;
		for (j = r.origin.x; j < r.corner.x; j++)
		{	switch (How) {
			case 0:	photo[i][j] = min(255, photo[i][j]+val);
				break;
			case 1:	photo[i][j] = val; break;
			case 2:	photo[i][j] = max(0, photo[i][j]-val);
				break;
			}
			if (magn || (!(j%res) && !(i%res)))
				putdot(Pt(j, i));
		}
	}
}

putdot(p1)
	Point p1;
{
	Rectangle r;

	r.origin.x = (magn)?(p1.x-xoff)*res:p1.x;
	r.origin.y = (magn)?(p1.y-yoff)*res:p1.y;
	r.corner.x = min(SZ, (r.origin.x + res));
	r.corner.y = min(SZ, (r.origin.y + res));
	
	burp(photo[p1.y][p1.x], r, SHOW);
}

Point
whereami(pin)
	Point pin;
{	Point pt;

	pt = pin;
	if (!magn)
	{	pt.x += xoff;
		pt.y += yoff;
	} else
	{	pt.x = xoff+pt.x/res;
		pt.y = yoff+pt.y/res;
	}
	pt.x = max(0, min(SZ-1, pt.x));
	pt.y = max(0, min(SZ-1, pt.y));
	if (!magn)
	{	while (pt.x%res) pt.x--;
		while (pt.y%res) pt.y--;
	}
	return pt;
}

samerect(a, b)
	Rectangle a, b;
{
	return (   a.origin.x == b.origin.x
		&& a.origin.y == b.origin.y
		&& a.corner.x == b.corner.x
		&& a.corner.y == b.corner.y);
}
