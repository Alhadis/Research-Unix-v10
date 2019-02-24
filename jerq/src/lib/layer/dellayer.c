#include <jerq.h>
#include "layer.h"

dellayer(l)
	register Layer *l;
{
	if(l==0)
		return;
	downback(l);
	Lgrey(l);
	/*
	 * Window to be deleted is now at rear; free the obscure
	 */
	bfree(l->obs);
	/*
	 * Remove layer from list
	 */
	lback=l->front;
	if(l->front)
		l->front->back=0;
	if(lfront==l)
		lfront=0;
	free((char *)l);
}
