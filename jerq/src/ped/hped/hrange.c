/*	finding where and how big things are	*/

#include "hped.h"

#define maxmin(A) if((A).x < txmin) txmin = (A).x; if((A).y < tymin) tymin = (A).y;\
	if((A).x > txmax) txmax = (A).x; if((A).y > tymax) tymax = (A).y;
#define Minn(A) if((A).x < xmin) xmin = (A).x; if((A).y < ymin) ymin = (A).y;
#define Maxx(A) if((A).x > xmax) xmax = (A).x; if((A).y > ymax) ymax = (A).y;

ptinrec(p,r)
	Point p;
	Rectangle r;
{
	if(p.x < r.origin.x) return(0);
	if(p.y < r.origin.y) return(0);
	if(p.x > r.corner.x) return(0);
	if(p.y > r.corner.y) return(0);
	return(1);
}

f_ins_l(r1,r2)
	Rectangle r1, r2;
{
	if(!ptinrec(r1.origin,r2)) return(0);
	if(!ptinrec(r1.corner,r2)) return(0);
	return(1);
}

inters(r1,r2)
	Rectangle r1, r2;
{
	if(r1.corner.x < r2.origin.x) return(0);
	if(r2.corner.x < r1.origin.x) return(0);
	if(r1.corner.y < r2.origin.y) return(0);
	if(r2.corner.y < r1.origin.y) return(0);
	return(1);
}

/* find range of data and output reversing y */
rmaxmin(p,r)
	Point p;
	Rectangle *r;
{
	if(p.x < r->origin.x) r->origin.x = p.x;
	if(p.y < r->origin.y) r->origin.y = p.y;
	if(p.x > r->corner.x) r->corner.x = p.x;
	if(p.y > r->corner.y) r->corner.y = p.y;
	return(0);
}

/*	find dimensions of object	*/

ex_one(a) objP a;
{
	register i;
	register char *nlp, *nlp0, *txtp;
	Point pp, *qq;
	short w, w0, mde;
	double h, h0, g0;
	double txmin = 8000, txmax = -8000, tymin = 8000, tymax = -8000;
	if(!a) return(-1);
	if(!a->status) return(-1);
	if(!a->n) return(-1);

	switch(a->type){
	case 'f':
		for(i=0;i<a->size;i++) {
#ifdef CAUTION
			rrecord("f_range %d",i);
#endif
			ex_one(a->mem[i]);
			maxmin(a->mem[i]->bounds.origin);
			maxmin(a->mem[i]->bounds.corner);
		}
		break;
	case 'c':	/*clutzy*/
		pp = *(a->p0);
		pp.x += a->size;
		maxmin(pp);
		pp.x -= (2*a->size);
		maxmin(pp);
		pp.x += a->size;
		pp.y -= a->size;
		maxmin(pp);
		pp.y += (2*a->size);
		maxmin(pp);
		break;
	case 't':
		w0 = 0;
		txtp = a->txt;
		mde = 0;
		if(*txtp=='\\'){
			txtp++;
			if(*txtp=='C') mde = 1;
			else if(*txtp=='R') mde = 2;
			txtp++;
		}
		for(i=0,nlp0=nlp=txtp;*nlp;nlp++){
			if(*nlp!=SEPAR) continue;
			w = nlp - nlp0;
			if(w>w0) w0=w;
			nlp0 = nlp + 1;
			i++;
		}
		w = nlp - nlp0;
		if(w>w0) w0 = w;
		i++;
		/* w0 is max. width and i is height */
		g0 = 9*(a->size/10.);
		w0 *= g0;	/* approx. */
		h0 = g0;	/*for now*/
		h = h0*i;	/* approx. */
		tymin  = (a->p0)->y - (h/2);
		tymax = tymin + h;
		txmin  = (a->p0)->x;
		if(!mde) txmax = txmin + w0;
		else if(mde==2){
				txmax = txmin;
				txmin -= w0;
			}
			else if(mde==1){
					txmax = txmin + w0/2;
					txmin -= w0/2;
				}
		break;
	default:
		for(i=0,qq=a->p0;i<a->n;i++,qq++){
			if(qq->t < 0) continue;
			maxmin(*qq);
		}
	}
	a->bounds.origin.x = txmin;
	a->bounds.corner.x = txmax;
	a->bounds.origin.y = tymin;
	a->bounds.corner.y = tymax;
	return(0);
}

ob_range(r,o,n)
	Rectangle * r;
	objP o[];
	short n;
{
	register j;
	double txmin = 8000, txmax = -8000, tymin = 8000, tymax = -8000;
	for(j=0;j<n;j++){
		if(!o[j]) continue;
		if( !(o[j]->status) || !(o[j]->n) ) continue;
		ex_one(o[j]);	/*wasteful but safe*/
		maxmin(o[j]->bounds.origin);
		maxmin(o[j]->bounds.corner);
	}
	r->origin.x = txmin;
	r->origin.y = tymin;
	r->corner.x = txmax;
	r->corner.y = tymax;
}

tot_range(all)
{
	register  j;
	Rectangle spread;
	if( !(all && no_obj) && !bn ) return(-1);
	xmin = ymin = 8000;
	xmax = ymax = -8000;
	if(all)	{
		ob_range(&spread,obj,no_obj);
		prirec("obj rect",spread);
		Minn(spread.origin);
		Maxx(spread.corner);
		fprintf(frecord,"obj_range= %g,%g to %g,%g\n",xmin,ymin,xmax,ymax);
	}
	if(bn) {
		ob_range(&spread,bb,bn);
		prirec("bb rect",spread);
		Minn(spread.origin);
		Maxx(spread.corner);
		fprintf(frecord,"Tot_range= %g,%g to %g,%g\n",xmin,ymin,xmax,ymax);
	}
	return(0);
}

prirec(s,r)
	char * s;
	Rectangle r;
{
	fprintf(frecord,"%s %g,%g to %g,%g\n",
	s, r.origin.x, r.origin.y, r.corner.x, r.corner.y);
}
