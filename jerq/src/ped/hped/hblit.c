/*	object manupulation on host, similar to manips on blit	*/

#include "hped.h"


/*duplicate object `a'  and return pointer to it*/

objP dupl_obj(a) objP a;
{
	register i;
	register objP b;
	register Point * ap, * bp;
	b = (objP)malloc((unsigned)(sizeof(objS)));
	b->size = a->size;
	b->color = a->color;
	b->pen = a->pen;
	b->ps = a->ps;
	b->type = a->type;
	b->status = 1;
	b->n = a->n;
	b->bounds = a->bounds;
	bp = b->p0 =
	(Point *)malloc((unsigned)(((a->n+1)*sizeof(Point))));
	ap = a->p0;
	bp = b->p0;
	for(i=0; i<a->n; i++) {
		*bp++ = *ap++;
	}
	b->txt = 0;
	switch(b->type){
	case 'f':
		b->mem = (objP *)malloc((unsigned)(b->size*sizeof(objP)));
		for(i=0;i<b->size;i++)
			b->mem[i] =  dupl_obj(a->mem[i]);
		break;
	case 's':
	case 'S':
	case 't':
		b->txt = malloc((unsigned)(strlen(a->txt)+1));
		strcpy(b->txt,a->txt);
		break;
	}
	return(b);
}


tr_obj(a,dx,dy)
	objP a;
	double dx, dy;
{
	Point * pp;
	register i;
	if(a->type=='f') {
		(a->p0)->x += dx;	(a->p0)->y += dy;
		for(i=0;i<a->size;i++){
			tr_obj(a->mem[i],dx,dy);
		}
	}
	else {
		for(pp=a->p0, i=0; i<a->n; i++, pp++){
			if(pp->t < 0) continue;
			pp->x += dx;	pp->y += dy;
		}
		a->bounds.origin.x += dx;
		a->bounds.origin.y += dy;
		a->bounds.corner.x += dx;
		a->bounds.corner.y += dy;
	}
}

/*	copy object		*/
cp_obj(aa,dx,dy)
	register objP aa;
	double dx, dy;
{
	register objP  bb;
	bb = dupl_obj(aa);
	tr_obj(bb,dx,dy);
	obj[no_obj++] = bb;
	return(0);
}
/* -- -- flip -- -- */

flip(a,cp,horz,vert)
	register objP a;
	Point cp;
{
	cp.x *= 2;	cp.y *= 2;
	f1(a,horz,vert,cp);
	return(0);
}

f1(a,h,v,c)
	register objP a;
	Point c;
{
	register j;
	register Point * pp;
	if(a->type=='f'){
		for(j=0;j<a->size;j++) f1(a->mem[j],h,v,c);
	}
	else {
		for(pp=a->p0,j=0; j<a->n; pp++,j++) {
			if(pp->t < 0) continue;
			if(h) pp->x = c.x - pp->x;
			if(v) pp->y = c.y - pp->y;
		}
	}
	return(0);
}

