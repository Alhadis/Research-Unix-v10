#include "pen.h"
line(x1, y1, x2, y2)
double x1, y1, x2, y2;
{
	if(noscale){
	x1 = SCX(x1);
	y1 = SCY(y1);
	x2 = SCX(x2);
	y2 = SCX(y2);
	}
	penUp();
	fprintf(pltout,"PA %8.2f,%8.2f PD %8.2f,%8.2f\n",
		x1, y1, x2, y2);
	penstate = DOWN;
	ckerr(0);
}
