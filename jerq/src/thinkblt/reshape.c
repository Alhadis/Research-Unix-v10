#include <jerq.h>
#include <msgs.h>

#define INSET	3

reshape(newr, sendit)
Rectangle newr;
{
	Rectangle oldr; register Layer *l;
	oldr = P->layer->rect;
	dellayer(P->layer);
	if ((l = newlayer(newr)) == 0 && (l = newlayer(oldr)) == 0)
		kill(9);
	P->layer = l;
	Jdisplayp = (Bitmap *)l;
	if (l->rect.origin.x == 0 && l->rect.origin.y == 0 &&
	    l->rect.corner.x == 0 && l->rect.corner.y == 0) {
		Drect = P->rect = l->rect;
	} else {
		Drect = P->rect = inset(l->rect, INSET);
		border(&display, display.rect, INSET, F_STORE);
	}
	if (sendit)
		muxnewwind(P, C_RESHAPE);
}
