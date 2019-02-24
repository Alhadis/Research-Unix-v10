#include "jplot.h"
#include <stdio.h>
#include <math.h>
void
range(x0, y0, x1, y1)
double x0, y0, x1, y1;
{
	e1->xmin = x0;
	e1->ymin = y0;
	e1->scalex =  e1->sidex / (x1 - x0);
	e1->scaley =  e1->sidey / (y1 - y0);
	e1->quantum = e0->quantum / sqrt(e1->scalex*e1->scalex +
		e1->scaley*e1->scaley);
	if(e1->quantum < .5) e1->quantum = .5;
}
