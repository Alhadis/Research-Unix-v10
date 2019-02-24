#include "hped.h"

#include <math.h>

rotate(a,cph,sph)
	objP a;
	double cph, sph;
{
	register i;
	double tx;
	Point * pp;

	if(!a) return(-1);
	if(a->type=='t' || a->type=='c') return(-1);
	if(a->type=='f') for(i=0;i<a->size;i++) rotate(a->mem[i],cph,sph);
	for(i=0, pp=a->p0; i<a->n; i++, pp++){
		if(pp->t<0) continue;
		tx = pp->x*cph - pp->y*sph;
		pp->y = pp->x*sph + pp->y*cph;
		pp->x = tx;
	}
}

unrot(tx1,tx2,y,cph,sph)
	double tx1, tx2, y;
	double cph, sph;
{
	double tx, ty1, ty2;
	ty1 = ty2 = y;
	tx = tx1*cph + ty1*sph;
	ty1 = -tx1*sph + ty1*cph;
	tx1 = tx;
	tx = tx2*cph + ty2*sph;
	ty2 = -tx2*sph + ty2*cph;
	tx2 = tx;
	pmove(tx1,ty1);
	pvecr(tx2-tx1,ty2-ty1);
}
