#include "pen.h"
frame(xs, ys, xf, yf)
double xs, ys, xf, yf;
{
	e1->p1x = e0->p1x+(xs*DX);
	e1->p1y = e0->p1y+(ys*DY);
	e1->p2x = e0->p1x+(xf*DX);
	e1->p2y = e0->p1y+(yf*DY);
	fprintf(pltout,"IP %8.2f,%8.2f %8.2f,%8.2f\n",
		e1->p1x,e1->p1y,
		e1->p2x,e1->p2y);
	fprintf(pltout,"SC %8.2f,%8.2f,%8.2f,%8.2f\n",
		e1->xmin,e1->xmax,e1->ymin,e1->ymax);
	ckerr(0);
}
