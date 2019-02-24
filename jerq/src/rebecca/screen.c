#include	"/usr/jerq/include/jerq.h"
#include	"instruments.h"
#include	"rebecca.h"

extern Rectangle dr, Image;
extern int xoff, yoff, magn, res;
extern char anychange;
extern unsigned char *photo[SZ];
extern short have[SZ], l_side[SZ], r_side[SZ];

int prevp1x = 2*SZ;
int prevp1y = 2*SZ;

checkscreen()
{	Point pt;

	if (!magn) return;
	pt = sub(mouse.xy, dr.origin);
	while (button1() && button2() && ptinrect(pt, Image))
	{	pixselect();
		wait(CPU);
		pt = sub(mouse.xy, dr.origin);
	}
	flushost();
}

pixselect()
{	Rectangle r;
	Point p1;
	int i, j;

	p1 = whereami(sub(mouse.xy, dr.origin));

	if ((p1.x == prevp1x && p1.y == prevp1y) || p1.y+1 >= SZ)
		return;

	ptohost(p1, 's', 1);
	r.origin.x = (magn)?(p1.x-xoff)*res:p1.x;
	r.origin.y = (magn)?(p1.y-yoff)*res:p1.y;
	r.corner.x = r.origin.x + res;
	r.corner.y = r.origin.y + res;
	if (resident(p1.x, p1.y))
	{	photo[p1.y][p1.x] = blend(p1);
		burp(photo[p1.y][p1.x],  r, SHOW);
		prevp1x = p1.x;
		prevp1y = p1.y;
		anychange = 1;
	}
}

#define clamp(n)	((n<0)?0:((n>=SZ)?SZ-1:n))

blend(pt)
	Point pt;
{	register x, y, z=0, n=0, zx, zy;

	for (y = -1; y <= 1; y++)
	for (x = -1; x <= 1; x++)
	{	zy = clamp(pt.y + y);
		zx = clamp(pt.x + x);
		if (resident(zx, zy))
		{	z += photo[zy][zx];
			n++;
		}
	}
	return (n>0)?(z/n):0;
}
