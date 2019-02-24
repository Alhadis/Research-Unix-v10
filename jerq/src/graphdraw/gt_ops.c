#include "defs.h"
#include <font.h>

drawarc(p1, p2)
Point p1, p2; {
	Point midpt, center;
	int x, y, c = 1;
/*
	int m, d=50;
*/

	midpt = div( add(p1, p2), 2 );
/*
	m =-1/((p1.y - p2.y)/(p1.x - p2.x));
	center.x = d/sqrt(m*m + 1) + midpt.x;
	center.y = d/sqrt(m*m + 1) + midpt.y;
*/
	x = p1.x - p2.x;
	y = p1.y - p2.y;
	center.y = c*x + midpt.y;
	center.x = c*y + midpt.x;

	if (abs(x) > abs(y)) /*more horizontal; do p1, p2 -- goes ccw from p1 to p2*/
		arc(&display, center, p1, p2, f2);
	else  /*more vertical; do p2, p1*/
		arc(&display, center, p2, p1, f2);
}


loop(pnt)
Point pnt; {
	pnt.y += 18;
	circle(&display, pnt, loopcsize, f2);
}


Point looplabpt(pt)
Point pt; {
	Point labpnt;

	labpnt = pt;
	labpnt.y += 36;
	return(labpnt);
}


Point arclabpt(p1, p2)
Point p1, p2; {
	return(p1);
}


heavyloop(pt)
Point pt; {
	return;
}

heavyarcs(p1, p2)
Point p1, p2; {
	return;
}
