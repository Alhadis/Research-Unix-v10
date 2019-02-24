#include <stdio.h>
#include "tek.h"
circle(xc, yc, r) 
double	xc, yc, r;
{
	if (r > 0) 
		r = -r;
	arc(xc + r, yc, xc + r, yc, xc, yc, r);
}
