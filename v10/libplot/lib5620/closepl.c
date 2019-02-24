#include <stdio.h>
#include "jcom.h"
#include "jplot.h"
#include <sgtty.h>

void
closepl()
{
	extern struct sgttyb
		savetty;			/* cooked tty modes */

	graphic(HOME);
	finish(mpx);
	ioctl(jerq, TIOCSETP, &savetty);
}
