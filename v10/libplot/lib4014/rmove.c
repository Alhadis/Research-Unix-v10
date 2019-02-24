#include <stdio.h>
#include "tek.h"
rmove(xx, yy) 
double	xx, yy; 
{
	e1->copyx += xx;
	e1->copyy += yy;
	move(e1->copyx, e1->copyy);
}
