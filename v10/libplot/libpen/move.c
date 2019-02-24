#include "pen.h"
move(x,y)
double x, y;
{
	if(noscale){
	x = SCX(x);
	y = SCY(y);
	}
	penUp();
	fprintf(pltout,"PA %8.2f,%8.2f\n",x,y);
	ckerr(0);
}
rmove(x, y)
double x, y;
{
	if(noscale){
	x = SCX(x);
	y = SCY(y);
	}
	penUp();
	fprintf(pltout,"PR %8.2f,%8.2f\n",x,y);
	ckerr(0);
}
