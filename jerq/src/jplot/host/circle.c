#include "jcom.h"
#include "jplot.h"

void
circle(x,y,r)
double x, y, r;
{
	register int ri;
	graphic(CIRCLE);
	xysc(x, y);
	ri = r*e1->scalex;
	intout(ri);
	cmd_sent();
}
