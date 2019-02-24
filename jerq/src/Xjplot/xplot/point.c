#include "jcom.h"
#include "jplot.h"

void
point(x, y)
double x, y;
{
	graphic(POINT);
	xysc(x,y);
	cmd_sent();
}
