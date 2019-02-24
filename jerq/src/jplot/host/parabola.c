#include <stdio.h>
#include <math.h>
#include "jplot.h"
#include "jcom.h"
parabola(x0, y0, x1, y1, xb, yb) 
double	x0, y0, x1, y1, xb, yb; 
{
	graphic(PAR);
	xysc(x0, y0);
	xysc(x1, y1);
	xysc(xb, yb);
	cmd_sent();
}

