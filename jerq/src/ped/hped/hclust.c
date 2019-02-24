/*	Clustering called by formatt() (in hform.c)	*/

#include "beauty.h"

extern clust * cls;
extern clustP pcls;	/* pointer to first available loc in cls[] */

extern Point * row;	/* where a copy of all coordinates is stored */
extern short * offrow;	/* used to go from row[] back to objects */
extern short * nrow;	/* number of points in same contour on row[] */
extern Point * mv_tol;	/* where maximum allowed dx, dy are stored */

extern sides * stt;	/* side list. used by everything */
extern sideP * stp;	/* pointers to stp for processing by cluster1() */
extern sideP * cmem;	/* store members of clusters */
extern sideP ** lp;	/* preliminary cluster pointers */
extern short * lpstat;	/* status of lp pointers */

extern sideP *pmem;

extern sides * srow;	/* make point list look like sides */

extern eqnS * eq;	/* side equations */
extern short neq, oeq;

/*	Make cluster and equations	*/

create(n,tolerence,type,sbase,base,label,action)
	sideP * sbase;
	sideP base;
	char * label;
	short (*action)();
{
	register i;

	pcls  = cls;
	pmem = cmem;
	oeq = neq;

	cluster1(n,sbase,tolerence,label);
	chk_neg(cls,pcls,action,*label);
	if(detail) list(cls,pcls,base);
	i = add_eq(cls,pcls,type);
	rrecord("%d clusters from %s., %d eqs",i,label,neq-oeq);
}

llcomp(p1,p2)
	sideP *p1, *p2;
{
	register sideP e1, e2;
	e1 = *p1;
	e2 = *p2;
	if(e1->t < e2->t) return(-1);
	else if(e1->t > e2->t) return(1);
	else return(0);
}


cluster1(n,v,tol,label)
	sideP v[];
	char * label;
{
	register sideP *vp, *vq;
	register sideP svp, svq, ovp;
	register i, j, k;
	short cmark;
	double d_span, d_adj;
	double t, s, v1, v2;
	short pc;
	double center, span;
	short abv;
	short ns, seeds[512];
	double *gp, *gp_end, gaps[4096];
	short mxgloc;
	double mxgap;

	d_span = tol*DELTA; 
	d_adj = tol*DELTA1;
	if(detail)
	fprintf(fq,"\nTry clustering by %s. Tol: span = %g, adj = %g\n",
	label,d_span,d_adj);

	qsort(v,n,sizeof(sideP),llcomp);

	vp = v;		svp = *vp++;
	gp = gaps;	*gp++ = d_span;
	for(i=1; i<n; i++,vp++){
		ovp = svp;	svp = *vp;
		*gp++ = svp->t - ovp->t;
	}
	*gp++ =  d_span;
	gp_end = gp;

	abv = 1;
	ns = 0;
	for(gp=gaps,i=0; gp<gp_end; gp++,i++) {
		if(abv != (*gp>=d_adj)) {
			abv = 1 - abv;
			seeds[ns++] = i - 1;
		}
	}

	cmark = 1;
	for(i=0;i<n;i++) lpstat[i] = 0;
	for(i=0;i<ns; i +=2) {
		span = (*(v+seeds[i+1]))->t - (*(v+seeds[i]))->t;
		if(detail && span > d_span) fprintf(fq,"Too big span %g\n",span);
		if(span > (d_span+d_adj)) {
			for(vp=v+seeds[i]; vp<= v+seeds[i+1]; vp++)
				(*vp)->nu = 1;	/* make untouchable */
			continue;
		}
		k  = -1;
		if(span > d_span) {
			mxgloc = j = seeds[i]+1;
			for(; j<seeds[i+1]; j++){
				if(gaps[j+1]>gaps[mxgloc]) mxgloc = j+1;
			}
			if(detail) fprintf(fq,"%d %g: ",mxgloc,gaps[mxgloc]);
			if(mxgloc==(seeds[i]+1)) seeds[i]++;
			else if(mxgloc==(seeds[i+1])) seeds[i+1]--;
			else k = mxgloc;
		}

		for(j=seeds[i]; j<=seeds[i+1];j++) {
			if(j==k) cmark++;
			lpstat[j] = cmark;
		}
		cmark++;
	}

	for(i=0;i<n;i++) fprintf(stderr,"(%d)",lpstat[i]);
	fprintf(stderr," LP\n");

	pc = 0;
	lp[pc++] = v;
	for(i=1;i<n;i++){
		if(!lpstat[i]) lp[pc++] = v+i;
		else if(lpstat[i-1] != lpstat[i]) lp[pc++] = v+i;
	}
	lp[pc++] = v+n;

	for(i=1; i<pc; i++){
		if(lp[i-1]>=lp[i]) continue;
		if(!lpstat[i]) {
			for(vp=lp[i-1];vp<lp[i];vp++) single(*vp);
			continue;
		}
		if(lp[i]-lp[i-1]<2) single(*(lp[i]-1)); /*one point cluster*/
		else {
			vq = lp[i-1];	svq = *vq;
			pcls->n = lp[i]-lp[i-1];
			pcls->mem = pmem;
			pmem += pcls->n;
			for(j=0;j<pcls->n;j++) *(pcls->mem+j) = *(vq+j); 
			pcls->val = ( (*lp[i-1])->t + (*(lp[i]-1))->t )/2;
			pcls++;
		}
	}
}

single(pp)
	sideP pp;
{
	pcls->n = 1;	pcls->mem = pmem;	pmem += 1;
	*(pcls->mem) = pp;
	pcls->val = pp->t;
	pcls++;
}

add_eq(C1,C2,kind)
	clustP C1, C2;
{
	register i, k;
	register clustP cli;
	register sideP st0;

	k = 0;
	for(cli=C1;cli<C2;cli++){
		if(cli->n<2) continue;
		k++;
		i = 0;
		while(i<cli->n) {
			st0 = *(cli->mem+i);
			i++;
			if(!st0->nu) break;
		}
		for(; i<cli->n; i++) {
			if( (*(cli->mem+i))->nu ) continue;
			eq[neq].s1 = st0;
			eq[neq].s2 = *(cli->mem+i);
			eq[neq++].u = kind;
		}
	}
	return(k);
}
