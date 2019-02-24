#include "jerq.h"
#include "commands.h"
#include "io.h"

bspline(b, p, n, c) Bitmap *b; Point *p; int n; Code c;
{	int i;
	sendCommand(CCBSPLINE);
	sendToBitmap(b);
	sendInt(n);
	for (i=0; i<n; i++) sendPoint(*p++);
	sendCode(c);
}
