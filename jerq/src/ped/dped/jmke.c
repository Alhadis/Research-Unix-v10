/*	Functions that make objects  drawn	*/

#include "jped.h"

/*duplicate object `a' between pts `p1' and `p and return pointer to it*/

objP dupl_obj(a,p1,p2) objP a; pointP p1, p2;
{
	register i;
	register objP b;
	register pointP ap, lp;
	short m;
	if(!a) return((objP)0);
	if(no_line(a->type)) { 	/* specials */
		p1 = a->p0;
		p2 = (a->p0) + (a->n-1);
	}
	if(p1) ap = p1; else ap = a->p0;
	if(p2) lp = p2; else lp = (a->p0) + (a->n-1);
	if(p1> a->p0 || lp < (a->p0 + a->n-1) ){
		m = lp - p1 + 1;
	}
	else m = a->n;
	b = 
	mk_obj(ap,a->txt,(short)a->type,
		(short)a->color,m,a->size,(short)a->pen,(short)a->ps);
	if(!b) return((objP)0);
	if(b->type=='f'){
		b->mem = (objP *)Alloc((unsigned)(b->size*sizeof(objP)));
		if(!b->mem) return((objP)0);
		for(i=0;i<b->size;i++){
			b->mem[i] =  dupl_obj(a->mem[i],(pointP)0,(pointP)0);
			if(!b->mem[i]) return((objP)0);
		}
	}
	return(b);
}

/*	make an object (any type!)	*/

objP mk_obj(pbase,sbase,type,color,n,size,pen,ps)
	pointP pbase;
	char *sbase;
	short type, color;
	short n, size, pen, ps;
{
	register objP aa;
	register i;
	register pointP pp1, pp2;

	old_fam = 0;
	if(n<=0) return((objP)0);
	aa = (objP)Alloc((unsigned)(sizeof(objS)));
	if(!aa) return((objP)0);
	aa->status = 11;
	aa->type = type;
	aa->color = color;
	aa->pen = pen;
	aa->ps = ps;
	aa->n = n;
	aa->size = size;
	aa->p0 = (pointP)Alloc((unsigned)(((n+1)*sizeof(pointS))));
	if(!aa->p0) return((objP)0);
	for(i=0, pp1= aa->p0, pp2= pbase; i<n; i++) *pp1++ = *pp2++;
	if(sbase) {
		aa->txt = Alloc((unsigned)(strlen(sbase)+1));
		if(!aa->txt) return((objP)0);
		strcpy(aa->txt,sbase);
	}
	else aa->txt = 0;
	return((objP)aa);
}

/*	group formation routines	*/

/*
	now all transfer is between obj[] and mem[]
	we should allow transfers between mem[] of different levels
	major difficulty is size of mem[]
*/

/* assemble objects */

short mk_assemb(topos,kount)
	objP topos[]; short *kount;
{
	Rectangle sweep;
	register objP aa;
	register j;
	*kount = 0;
	jtag(0," Enclose by rectangle");
	if(ownrec(&sweep)) return(0);
	for(j=0;j<no_obj;j++){	/*this range is always the same*/
		aa = obj[j];
		if(!aa) continue;
		if(!aa->status) continue;
		if(ptinrect(*(aa->p0),sweep)) {
			aa->status = 11;
			topos[(*kount)++] = aa;
			dr_link(topos[0],aa);
			obj[j] = 0;
		}
	}
	return(*kount);
}


objP tmpobf[MXOBJ];

short mk_fam(v,k)
	objP v[];
	short k;
{
	register i;
	register objP a;
	Point cp;	/*that's the family location!*/

	old_fam = 0;
	cp = *(v[0]->p0);
	a = mk_obj(&cp,(char *)0,'f',SOLID,(short)1,k,(short)0,(short)0);
	if(!a) return(0);
	a->mem = (objP *)Alloc((unsigned)(k*sizeof(objP)));
	if(!a->mem) return(0);
	for(i=0;i<k;i++) a->mem[i] = v[i];
	/*	pack uses tmpobf, but it OK. mk_fam already copied	*/
	pack(obj,&no_obj);
	obj[no_obj++] = a;
	return(0);
}

/*	disolve family		*/

short dis_fam(a)
	register objP a;
{
	register i;
	short k;

	if(a->type != 'f') return(-1);
	if(no_obj+a->size>=MXOBJ) bailout();	/*stays*/
	k = no_obj;
	for(i=0;i<a->size;i++) {
		dr_link(a,a->mem[i]);
		 obj[no_obj++] = a->mem[i];
	}
	a->status = 0;
	return(k);
}

short pack(v,N) objP v[]; short *N;
{
	register i, j;
	for(i=0;i< *N;i++) tmpobf[i] = v[i];
	for(i=0,j=0;i< *N;i++){
		if(tmpobf[i]) v[j++] = tmpobf[i];
	}
	*N = j;
}
