#include "pen.h"
vec(x, y)
double x, y;
{
	if(noscale){
	x = SCX(x);
	y = SCY(y);
	}
	penDown();
	fprintf(pltout,"PA %8.2f,%8.2f\n",x,y);
	ckerr(0);
}
rvec(x, y)
double x, y;
{
	x = SCX(x);
	y = SCY(y);
	penDown();
	fprintf(pltout,"PR %8.2f,%8.2f\n",x,y);
	ckerr(0);
}
