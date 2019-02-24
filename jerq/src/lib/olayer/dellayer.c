#include <jerq.h>
#include <layer.h>

dellayer(l)
	register Layer *l;
{
	register Obscured *op;
	if(l==0)
		return;
	downback(l);
	Lgrey(l);
	/*
	 * Window to be deleted is now at rear; free the obscure's
	 */
	for(op=l->obs; op; op=op->next){
		bfree(op->bmap);
		free((char *)op);
	}
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
