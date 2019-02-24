#include "jcom.h"
#include "jplot.h"
#include <stdio.h>
#include <math.h>
void
sbox(x0, y0, x1, y1)
double x0, y0, x1, y1;
{
	graphic(SBOX);
	xysc(x0, y1);
	xysc(x1, y0);
	cmd_sent();
}
