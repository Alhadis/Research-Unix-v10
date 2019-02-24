#include "pen.h"
arc(x1, y1, x2, y2, xc, yc, r)
double x1, y1, x2, y2, xc, yc, r;
{
	float th;
	if(noscale){
	x1 = SCX(x1);
	y1 = SCY(y1);
	x2 = SCX(x2);
	y2 = SCY(y2);
	xc = SCX(xc);
	yc = SCY(yc);
	r = SCX(r);
	}
	th = theta(x1,y1,x2,y2,xc,yc,r);
	penUp();
	fprintf(pltout,"PA %8.2f,%8.2f; PD; AA %8.2f,%8.2f,%8.2f;\n",
		x1,y1,xc,yc,th);
	penstate = DOWN;
	ckerr(0);
}
