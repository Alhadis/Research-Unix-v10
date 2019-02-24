#include "pen.h"
parabola(x0, y0, x1, y1, xb, yb)
double x0, y0, x1, y1, xb, yb;
{
	register double t, x, y;
	double d1, d2, dt;
	double c0x,c0y,c1x,c1y;
	if(noscale){
	x0 = SCX(x0);
	y0 = SCY(y0);
	x1 = SCX(x1);
	y1 = SCY(y1);
	xb = SCX(xb);
	yb = SCY(yb);
	}
	d1=sqrt((xb-x0)*(xb-x0)+(yb-y0)*(yb-y0));
	d2=sqrt((xb-x1)*(xb-x1)+(yb-y1)*(yb-y1));
	if (d1<=QUANTUM || d2<=QUANTUM) {
		penUp();
		fprintf(pltout,"PA %8.2f,%8.2f PD %8.2f,%8.2f",
			x0,y0,x1,y1);
		penstate = DOWN;
	}
	else {
		c0x = x0 + x1 - 2.*xb; 
		c0y = y0 + y1 - 2.*yb; 
		c1x = 2. * (xb - x0);
		c1y = 2. * (yb - y0);
		penUp();
		fprintf(pltout,"PA %8.2f,%8.2f PD",x0,y0);	
		penstate = DOWN;
		dt = QUANTUM/d1;
		for (t=dt; t < 1.0; t += dt) {	
			x = (c0x*t + c1x)*t + x0;
			y = (c0y*t + c1y)*t + y0;
			fprintf(pltout," %8.2f,%8.2f",x,y);
		}
		fprintf(pltout," %8.2f,%8.2f\n",x1,y1);
	}
	ckerr(0);
}
