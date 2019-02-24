#include <jerq.h>
#include "layer.h"

Layer *
newlayer(r)
	Rectangle r;
{
	register Layer *newl;
	newl=(Layer *)alloc(sizeof (Layer));
	if(newl==0)
		return 0;
	newl->obs=balloc(r);
	if (newl->obs==0) {
		free((char *)newl);
		return 0;
	}
	newl->rect=r;
	newl->someobs = 1;
	newl->nonevis = 0;
	newl->front=lback;
	newl->back=0;
	if(lback)
		lback->back=newl;
	if(lfront==0)
		lfront=newl;
	lback=newl;
	/*
	 * Make it look like a Bitmap
	 */
	newl->base=addr(&display, r.origin);
	newl->width=display.width;
	/*
	 * Pull it to the front and clear it!
	 */
	upfront(newl);
	Lbox(newl);
	return(newl);
}
