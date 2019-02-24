#include "jcom.h"
#include "jplot.h"

void
disc(x,y,r)
double x, y, r;
{
	register int ri;
	graphic(DISC);
	xysc(x, y);
	ri = r*e1->scalex;
	intout(ri);
	cmd_sent();
}
