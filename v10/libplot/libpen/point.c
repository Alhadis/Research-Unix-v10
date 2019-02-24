#include "pen.h"
point(x, y)
double x, y;
{
	if(noscale){
	x = SCX(x);
	y = SCY(y);
	}
	penUp();
	fprintf(pltout,"PA %8.2f,%8.2f PD; PU;\n",x,y); 
	penstate = UP;
	ckerr(0);
}
