#include <jerq.h>

screenswap(bp, rect, screenrect)
	register Bitmap *bp;
	Rectangle rect;
	Rectangle screenrect;
{
	bitblt(&display, screenrect, bp, rect.origin, F_XOR);
	bitblt(bp, rect, &display, screenrect.origin, F_XOR);
	bitblt(&display, screenrect, bp, rect.origin, F_XOR);
}
