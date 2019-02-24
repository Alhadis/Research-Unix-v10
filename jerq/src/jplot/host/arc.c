#include "jcom.h"
#include "jplot.h"

void
arc(x1, y1, x2, y2, x0, y0, r)
double x1, y1, x2, y2, x0, y0, r;
{

	graphic(ARC);
	xysc(x0, y0);
	if(r < 0){
		xysc(x2,y2);
		xysc(x1,y1);
	}
	else {
		xysc(x1, y1);
		xysc(x2,y2);
	}
	cmd_sent();
}
