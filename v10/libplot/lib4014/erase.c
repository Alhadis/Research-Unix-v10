#include <stdio.h>
#include "tek.h"
erase()
{
	putchar(ESC);
	putchar(ERASE);
	ohiy = ohix = oextra = oloy = -1;
	fflush(stdout);
	sleep(2);
	return;
}
