#include <stdio.h>
#include "tek.h"
restore()
{
	e1--;
	putchar(ESC);
	putchar(e1->ppink);
	move(e1->copyx, e1->copyy);
}
