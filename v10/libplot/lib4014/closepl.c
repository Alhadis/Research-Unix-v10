#include <stdio.h>
#include "tek.h"
closepl()
{
	fflush(stdout);
	e1=e0;
	move(5., 5.);
	putchar(PLOTOUT);
}
