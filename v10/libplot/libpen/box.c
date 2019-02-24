#include "pen.h"
box(x1,y1,x2,y2)
double x1,y1,x2,y2;
{
	if(noscale){
	x1 = SCX(x1);
	y1 = SCY(y1);
	x2 = SCX(x2);
	y2 = SCY(y2);
	}
	penUp();
	fprintf(pltout,"PA %8.2f,%8.2f; PD; PA %8.2f,%8.2f %8.2f,%8.2f %8.2f,%8.2f %8.2f,%8.2f\n",
		x1,y1,x1,y2,x2,y2,x2,y1,x1,y1);
	penstate = DOWN;
	ckerr(0);
}
sbox(x1, y1, x2, y2)
double x1, y1, x2, y2;
{
	double ff[8], *fp[1];
	int i[2];
	ff[0] = ff[6] = x1;
	ff[1] = ff[3] = y1;
	ff[2] = ff[4] = x2;
	ff[5] = ff[7] = y2;
	i[0] = 4;
	i[1] = 0;
	fp[0] = ff;
	internal = 1;
	fill(i,fp);
	internal = 0;
}
