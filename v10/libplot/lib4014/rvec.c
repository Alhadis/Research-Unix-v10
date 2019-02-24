#include <stdio.h>
#include "tek.h"
rvec(xx, yy) 
double	xx, yy; 
{
	e1->copyx += xx;
	e1->copyy += yy;
	vec(e1->copyx, e1->copyy);
}
