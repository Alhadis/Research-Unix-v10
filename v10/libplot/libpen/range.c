#include "pen.h"
range(x0, y0, x1, y1)
double x0, y0, x1, y1;
{
	double dx, dy, step;
	if(noscale){
	dx = x1 - x0;
	dy = y1 - y0;
	if(dx <= 2. || dy <= 2.)
		e1->scale = 1000.;
	else if(dx <= 100. || dy <= 100.)
		e1->scale = 100.;
	else e1->scale = 1.;
	if(!res){
		dx *= e1->scale;
		dy *= e1->scale;
		step = (((dy*DX)/dx)>DY) ? DY/dy : DX/dx;	
		e1->p2x = e0->p1x + dx*step;
		e1->p2y = e0->p1y + dy*step;
		fprintf(pltout,"IP %8.2f,%8.2f %8.2f,%8.2f\n",
			  e1->p1x,e1->p1y,e1->p2x,e1->p2y);
			DX = e1->p2x - e1->p1x;
			DY = e1->p2y - e1->p1y;
			res = 1;
	}
	}
	e1->xmin = SCX(x0);
	e1->ymin = SCY(y0);
	e1->xmax = SCX(x1);
	e1->ymax = SCY(y1);
	fprintf(pltout,"SC %8.2f,%8.2f %8.2f,%8.2f\n",
		e1->xmin,e1->xmax,e1->ymin,e1->ymax);
	ckerr(1);
}
