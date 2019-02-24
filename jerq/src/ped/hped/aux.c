#include "beauty.h"
short * typrow;
#include "where.h"

extern short * lpstat;

/*	Set up data structures	*/

char * totasp;
char * begsp;
char * endsp;

char * Alloc(n,step)
	unsigned n;
{
	unsigned s;
	char * p;
	s = n*step;
	p = begsp;
	if(p+s >= endsp) return((char *)0);
	begsp = p + s;
	return(p);
}

#define FF 10

unsigned set_up(n,oa)
	short n;
	objP oa[];
{
	register unsigned NO, NP, SZ;
	unsigned total;
	register unsigned j;

	NP = SZ = find_size(oa,n);
	NP *= 2;	/* for good measure */
	NO = find_objn(oa,n)*2;
	total = (2*NO+NP)*sizeof(short)+
		4*NP*sizeof(long)+ 3*NP*sizeof(sides)+
		NP*sizeof(Point) + FF*NP*sizeof(eqnS) + 2*NP*sizeof(clust);
	rrecord("%ld bytes required",total);
	total = (total*5)/4;
	if( !(totasp = (char *)calloc(total,sizeof(char))) )
		error(1,"NO SPACE");
	begsp = totasp;
	endsp = totasp + total;

	if( !(offrow = (short *)Alloc(NO,sizeof(short)) ) )
		error(1,"No space for offrow");
	if( !(typrow = (short *)Alloc(NO,sizeof(short)) ) )
		error(1,"No space for typrow");
	if( !(nrow = (short *)Alloc(NO,sizeof(short)) ) )
		error(1,"No space for nrow");
	if( !(row = (Point *)Alloc(NP,sizeof(Point)) ) )
		error(1,"No space for row");
	if( !(mv_tol = (Point *)Alloc(NP,sizeof(Point)) ) )
		error(1,"No space for mv_tol");
	if( !(stt = (sides *)Alloc(NP,sizeof(sides)) ) )
		error(1,"No space for stt (side array)");
	if( !(cls = (clust *)Alloc(2*NP,sizeof(clust)) ) )
		error(1,"No space for cls (cluster array)");
	if( !(lp = (sideP **)Alloc(NP,sizeof( sideP *)) ) )
		error(1,"No space for lp");
	if( !(lpstat = (short *)Alloc(NP,sizeof(short)) ) )
		error(1,"No space for lpstat");

	/* allocate room for equations: assume FF*sides eqs */
	if( !(eq = (eqnS *)Alloc(NP*FF,sizeof(eqnS)) ) )
		error(1,"No space for line/line eqs");
	/* initialize penalty to zero */
	for(j=0; j<NP*FF; j++) (eq+j)->penalty = 0;

	/*	auxil. space for clustering	*/
	if( !(stp = (sideP *)Alloc(NP,sizeof(sideP)) ) )
		error(1,"No space for stp (side pointer array)");
	if( !(cmem = (sideP *)Alloc(NP*2,sizeof(sideP)) ) )
		error(1,"No space for cmem");
	if( !(srow = (sides *)Alloc(NP*2,sizeof(sides)) ) )
		error(1,"No space for srow");
	return(SZ);
}

clean_up()
{
	free(totasp);
}

/*	
*	Estimate size required to store polygons
*	(adapted from hcomm routine list_size()
*/

find_size(v,n)
	objP v[];
	short n;
{
	register i, k;
	register objP a;
	k = 0;
	for(i=0;i<n;i++) {
		a = v[i];
		if(a->type=='f') k += find_size(a->mem,(short)a->size);
		else k += a->n;
	}
	return(k);
}


find_objn(v,n)
	objP v[];
	short n;
{
	register i, k;
	register objP a;
	k = 0;
	for(i=0;i<n;i++, k++) {
		a = v[i];
		if(a->type=='f') k += find_objn(a->mem,(short)a->size);
	}
	return(k);
}

/*	Create row[] from formatt() arguments	*/

short obknt;
short ptknt;

mk_row(n,oa)
	objP oa[];
{
	register i;

	obknt = ptknt = 0;
	mk_row1(n,oa);
	rrecord("%d objects total, %d points",obknt,ptknt);
	return(ptknt);
}	

mk_row1(n,oa)
	objP oa[];
{
	register i, j, k;
	register objP a;

	for(j=0; j<n; j++){
		a = oa[j];
		if(a->type=='f') {
			mk_row1((short)a->size,a->mem);
			continue;
		}
		if( a->n<2 || (a->type != 'p' && a->type != 'P') ) {
			/* look only at polygons */
			offrow[obknt] = -1;
		}
		else {
			offrow[obknt] = ptknt;
			for(i=0;i<a->n;i++,ptknt++) row[ptknt] = *(a->p0+i);
		}
		typrow[obknt] = (a->type=='P' || a->type=='Q' || a->type=='S');
		nrow[obknt++] = a->n;
	}

}

/*	Create array sides[] from row[]	*/

#define isbreak(A) ((A)->t < 0)

double smalls, bigs;

mk_sides(n)
{
	register i, j, k, m;
	register sideP st;
	register ok, klast;

	m = 0;
	st = stt;	/*	stt[] is global	*/
	bigs = 0;
	smalls = HUGE;
	for(i=0;i<obknt;i++){	/* obknt is global */
		if(offrow[i]<0) continue;
		ok = k = offrow[i];
		klast = k + nrow[i];
		for(; k<klast; k++){
			if(isbreak(&row[k])){
				j = mk_part(&row[ok],&row[k],st,typrow[i]);
				rrecord("break: %d sides",j);
				st += j;
				ok = k + 1;
				m += j;
			}
		}
		j = mk_part(&row[ok],&row[k],st,typrow[i]);
		rrecord("final: %d sides",j);
		st += j;
		m += j;
	}
	rrecord("min side=%d max side=%d",(int)smalls,(int)bigs);
	return(m);
}

mk_part(p1,p2,sid,klosure)
	Point *p1, *p2;
	sideP sid;
	short klosure;
{
	register Point *qq, *pp;
	register sideP sp;
	short skount;	/* how many sides found */
	double dx, dy;

	if(p2<=p1) return(0);
	qq = pp = p1;
	if (p2-p1 >2 && klosure) qq = p2-1;	/* last point */
	else pp++; 
	for(sp=sid,skount=0; pp<p2; pp++){
		sp->nu = 0;
		sp->p1 = qq;
		sp->p2 = pp;
		dx = pp->x - qq->x;
		dy = pp->y - qq->y;
		sp->s = atan2(dy,dx);
		if(sp->s<0) sp->s += PI;
		if(sp->s > 0.9*PI) sp->s -= PI;
		sp->l = sqrt(dy*dy + dx*dx);
		if(sp->l<smalls) smalls = sp->l;
		else if(sp->l>bigs) bigs = sp->l;
		qq = pp;
		sp++; skount++;
	}
	return(skount);
}


extern char ranger[];

spill_row(KOUNT,N,OB)
	objP OB[];
{
	register i, j, m, u, uu;

#ifndef REALPED
	fd1 = stdout;
	/* fprintf(fd1,"o 8i\n%s",ranger); */
	fprintf(fd1,"o 8i\nr 0 1000 1000 0\n");
#endif
	m = obknt;
	obknt = 0;
	grab_row(N,OB);
#ifndef REALPED
	for(j=0;j<N;j++) {
		if(OB[j]->type=='t'){
			uu = strlen(OB[j]->txt);
			if(OB[j]->txt[uu-1]=='\n') OB[j]->txt[uu-1] = 0;
		}
		wr_one(OB[j]);
	}
	fprintf(fd1,"e\n");
#endif
	if(obknt != m) rrecord("Mess! %d should have been %d",obknt,m);
}

grab_row(nn,oba)
	objP oba[];
{
	register i, j, u;
	register objP a;

	for(j=0; j<nn; j++){
		a = oba[j];
		if(a->type=='f') {
			grab_row((short)a->size,a->mem);
			continue;
		}
		u = offrow[obknt++];
		if( a->n>1 && (a->type == 'p' || a->type == 'P') ) {
			if(u<0) continue;
			for(i=0;i<a->n;i++) *(a->p0+i) = row[u++];
		}
	}

}

/*	List contents of cluster array from C1 to C2	*/

list(C1,C2,base)
	clustP C1, C2;
	sideP base;
{
	register i;
	register sideP st;
	register clustP clj;

	for(clj=C1;clj<C2;clj++){
		if(clj->n<2) continue;
		for(i=0;i<clj->n;i++) {
			if( !(st = *(clj->mem+i)) ) continue;	/*good precaution*/
			fprintf(fq,"i=%2d: %g,%g(%d)->%g,%g(%d)",
			st-base,(st->p2)->x,(st->p2)->y,st->p2-row,
			(st->p1)->x,(st->p1)->y,st->p1-row);
			fprintf(fq," val=%6.3f cl=%d: %6.3f %d\n",
			st->t,clj-cls,clj->val,st->nu);
		}
		fprintf(fq,"\n");
	}
}

/*
*	Find signed distance of point cnt from line s
*	base is used only for diagnostics. should be removed later
*/

double cut(s,cnt,base)
	sideP s;
	Point cnt;
	sideP base;
{
	double dist;
	if(!s->l) {
		rrecord("cut: zero length side");
		return((double)(32000+rand()));
	}
	dist = ((s->p1)->x-cnt.x)*((s->p2)->y-cnt.y) -
		((s->p1)->y-cnt.y)*((s->p2)->x-cnt.x);
	dist /= s->l;
	if((s->p1)->y < (s->p2)->y) dist = - dist;
		else if((s->p1)->y==(s->p2)->y &&
		(s->p1)->x > (s->p2)->x ) dist = - dist;
	if(detail)
	fprintf(fq,"cut: %d at %g\n",s-base,dist);
	return(dist);
}

/*	find center of cluster pointed to by cp	*/

Point ccenter(cp)
	clustP cp;
{
	Point v;
	register sideP vp, *vpp;
	register i;

	v.x = v.y = 0;
	for(i=0, vpp = cp->mem; i<cp->n; i++, vpp++) {
		vp = *vpp;
		v.x += ( (vp->p1)->x + (vp->p2)->x );
		v.y += ( (vp->p1)->y + (vp->p2)->y );
	}
	i *= 2;
	if(i) {
		v.x /= i;	v.y /= i;
	}
	return(v);
}
