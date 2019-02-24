#include "jplot.h"

void
box(x0, y0, x1, y1)
double x0, y0, x1, y1;
{
	move(x0,y0);
	vec(x0,y1);
	vec(x1,y1);
	vec(x1,y0);
	vec(x0,y0);
}
