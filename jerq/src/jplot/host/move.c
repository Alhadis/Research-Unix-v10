#include "jcom.h"
#include "jplot.h"

void
move(xi,yi)
double xi,yi;
{
	graphic(MOVE);
	xysc(xi, yi);
	cmd_sent();
}
