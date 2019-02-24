#include "jcom.h"
#include "jplot.h"
#include <stdio.h>
#include <math.h>
void
ppause(){
	graphic(PAUSE);
	flush();
	t = term();
	counter = 0;
}
