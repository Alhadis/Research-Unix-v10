#include "pen.h"
circle(x, y, r)
double x, y, r;
{
	if(noscale){
	x = SCX(x);
	y = SCY(y);
	r = SCX(r);
	}
	penUp();
	fprintf(pltout,"PA %8.2f,%8.2f CI %8.2f\n",
		x, y, r);
	ckerr(0);
}
disc(x, y, r)
double x, y, r;
{
	circle(x, y, r);
}
