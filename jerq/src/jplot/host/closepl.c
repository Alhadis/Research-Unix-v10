#include <stdio.h>
#include "jcom.h"
#include "jplot.h"
#include <sgtty.h>

void
closepl()
{
	graphic(CLOSE);
	flush();
}
