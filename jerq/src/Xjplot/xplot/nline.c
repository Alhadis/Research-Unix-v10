#include <stdio.h>
#include <math.h>
#include "jcom.h"
#include "jplot.h"

void
line(x0,y0,x1,y1)
double x0,y0,x1,y1;
{
	if (x1 == e1->copyx && y1 == e1->copyy) {
		move(x1, y1);
		vec(x0, y0);
		return;
	}
	move(x0, y0);
	vec(x1, y1);
	return;
}

void
vec(x1, y1)
double x1, y1;
{
	int t0x, t0y, t1x, t1y, t2x, t2y, t3x, t3y;
	int ymin;
	double deltax2, deltay2, denom, halfsq;
	register double radx, rady;
	register double x0, y0;
	if(e1->pen == 1){
		graphic(VEC);
		xysc(x1, y1);
		cmd_sent();
		return;
	}
	x0 = SCX(e1->copyx);
	y0 = SCY(e1->copyy);
	e1->copyx = x1;
	e1->copyy = y1;
	x1 = SCX(x1);
	y1 = SCY(y1);
	if(x0 == x1 && y0 == y1)return;
	if(x0 > x1){
		radx = x0;
		x0 = x1;
		x1 = radx;
		radx = y0;
		y0 = y1;
		y1 = radx;
	}
	deltax2 = x1 - x0;
	deltay2 = y1 - y0;
	deltax2 *= deltax2;
	deltay2 *= deltay2;
	denom = deltax2 + deltay2;
	halfsq = (float)e1->pen*(float)e1->pen;
	halfsq /= (float)4;
	radx = sqrt((deltay2*halfsq)/denom) + .5;
	rady = sqrt((deltax2*halfsq)/denom) + .5;
	if((t0y = y0 - rady) < 0)t0y = 0;
	t1y = y0 + rady;
	if((t2y = y1 - rady) < 0)t2y = 0;
	t3y = y1 + rady;
	if(y0 < y1){
		if(x1 != x0)
			radx = -radx;
		ymin = t0y;
	}
	else ymin = t2y;
	if(y0 == y1){
		t0x = t1x = x1;
		t2x = t3x = x0;
	}
	else {
		if((t0x = x0 - radx) < 0)t0x = 0;
		if((t1x = x0 + radx) < 0)t1x = 0;
		if((t2x = x1 - radx) < 0)t2x = 0;
		if((t3x = x1 + radx) < 0)t3x = 0;
	}
	graphic(FILL);
	intout(4);
	intout(ymin);
	xy(t1x,t1y);
	xy(t0x,t0y);
	xy(t3x,t3y);
	xy(t2x,t2y);
	if(y0 < y1){
		xy(t3x,t3y);
		xy(t1x,t1y);
		xy(t2x,t2y);
		xy(t0x,t0y);
	}
	else {
		xy(t1x, t1y);
		xy(t3x,t3y);
		xy(t0x,t0y);
		xy(t2x,t2y);
	}
	cmd_sent();
}
void
rvec(dx,dy)
double dx,dy;
{
	if(e1->pen == 1){
		graphic(VEC);
		xysc(e1->copyx+dx, e1->copyy+dy);
		cmd_sent();
	}
	else vec(e1->copyx+dx, e1->copyy+dy);
}
