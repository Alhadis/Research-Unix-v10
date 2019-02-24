/*		filled spline routine			*/
/*	do filling on host and send scan lines to DMD	*/

#include "hped.h"

#include "device.h"

#ifdef NO_INTER
extern float FAC;
extern int XBASE, YBASE;
#endif

/*
*	Definitions for filler
*	(Point is defined in hped.h as double x,y; short t;)
*/

typedef struct jPoint
	{
		short x, y;
	} jPoint;

char shade;

char smooth;

typedef struct seg {
	short x, y, X, Y;
	double c1, c2, c3;
	char type;
	char stat;
	}  segS, * segP;

#define MXPT	512
#define SILLY  -8000
#define ABSURD -16000
#define isbreak(A) (A->x < SILLY)
#define pequal(A,B) ( abs(A.x-B.x)<2 && abs(A.y-B.y)<2 )
#define sflip(A,B) { if(A.y > B.y) swpt(&A,&B); \
	else if(A.y==B.y && A.x > B.x) swpt(&A,&B); }

short bottom;	/* lowest y seen (for fill) */

segP sinp;
segP sinpp;

#define COMP(A)  if(p1->A>p2->A) return(-1); else if(p1->A<p2->A) return(1);

place(p1,p2) segP p1, p2;
{
	COMP(y) else { 
		COMP(x) else { 
			COMP(X) else { 
				COMP(Y) else return(0);
			}
		}
	}
}

proc_fill(a)
	objP a;
{
	register i, j;
	jPoint * jp;
	register jPoint *pp, *opp;
	short npts;
	short do_twice;
	char oshade;
	segP sfin;

	shade = a->color;	/* global variable */
#ifdef NO_INTER
	/* prepare textures for troff or ramtek output */
	if(a->size){
		if(a->type=='Q') pl_poly(a,a->p0,a->p0 + a->n-1);
		else if(a->type=='S') spline(a);
	}
	do_twice = 0;
	rpt_f:;
	switch(shade) {
		case 'L':
		case 'M': if(!do_twice) {
				oshade = shade;
				do_twice = 1;
				shade = 'F';
			}
			else {
				do_twice = 0;
				shade = 'G';
				rotate(a,0.,1.);
			}
			break;
		case 'K':
		case 'I': rotate(a,0.707,-0.707); break;
		case 'J':		
		case 'H': rotate(a,0.707,0.707); break;
		case 'G': rotate(a,0.,1.); break;
	}
#else
	/* start communication when in real ped */
	s_char('X');
#endif
	if(a->n<3) goto telos;

	sinp = (segP)malloc((unsigned)((2*a->n+8)*sizeof(segS)));
	if(!sinp) {
		rrecord("No memory\n");
		goto telos;
	}

	jp = (jPoint *)malloc((unsigned)((a->n*2)*sizeof(jPoint)));
	if(!jp) {
		rrecord("No memory\n");
		goto telos;
	}

	if(a->type=='s' || a->type=='S') smooth = 1;
	else smooth = 0;

	for(i=0, pp=jp; i<a->n; i++, pp++){
		if((a->p0+i)->t < 0) {
			pp->x = pp->y = ABSURD;
		}
		else {
#ifdef SROFF
			pp->x = FAC*(scale*(a->p0+i)->x + xshft - XBASE) + 0.5;
			pp->y = FAC*(scale*(a->p0+i)->y + yshft - YBASE) + 0.5;
#else
			pp->x = scale*(a->p0+i)->x + xshft + 0.5;
			pp->y = scale*(a->p0+i)->y + yshft + 0.5;
#endif
			if(smooth)
			if( (a->p0+i)->t>0 || *(a->txt+i) >'0' ) {
				pp++;
				pp->x = (pp-1)->x;
				pp->y = (pp-1)->y;
				rrecord("mult knot %d",pp-jp);
			}
		}
	}
	npts = pp - jp;

	rrecord("*** Making sides for filled spline with %d vtcs ***",a->n);

	bottom = 16000;	/* init. to too big a value for fill */
	sinpp = sinp;
	for(i=0,opp=pp=jp; i<npts; i++,pp++) {
		if(isbreak(pp)){
			Mk_sides(opp,pp);
			opp = pp + 1;
		}
	}
	Mk_sides(opp,pp);

	j = (sfin = sinpp) - sinp;
	qsort(sinp,j,sizeof(struct seg),place);
#ifdef DIAGN
	rrecord("%d sides found",j);
	fprintf(frecord,"side types:");
	for(sinpp=sinp;sinpp<sfin;sinpp++) fprintf(frecord,"%c",sinpp->type);
	fprintf(frecord,"\n");
#endif

	realfill(sinp,sfin);

	Free(jp);
	Free(sinp);
	telos:;
#ifndef NO_INTER
	s_char('X');
#else
	if(do_twice) {
		shade = oshade;
		goto rpt_f;
	} 
#endif
}

Mk_sides(p1,p2)
	jPoint *p1, *p2;
{
	register jPoint * qq, * pp, * rr;
	jPoint K, M, V;
	segP pstart;

	if(p2<=p1) return(0);

	qq = p2-1;	/* last point */
	rr = p1+1;	/* next point */
	pstart = sinpp;

	if(smooth)
	for(pp=p1; pp<p2; pp++, rr++){
		if(rr>=p2) rr = p1;
		K.x = (qq->x + pp->x)/2;
		K.y = (qq->y + pp->y)/2;
		V.x = pp->x;
		V.y = pp->y;
		M.x = (rr->x + pp->x)/2;
		M.y = (rr->y + pp->y)/2;
		qq = pp;
		if( pequal(K,V) || pequal(M,V) ) lline(K,M);
		else arc(K,V,M);
	}
	else
	for(pp=p1;pp<p2;pp++) {
		lline(*qq,*pp);
		qq = pp;
	}

	ed_sides(pstart,sinpp);	/*sinpp is global variable*/
	return(0);
}

/*	edit horizontals	*/

ed_sides(sstart,sfin)
	segP sstart, sfin;
{
	register segP sp, sbf, snx;

	for(sp=sstart;sp<sfin;sp++){
		if(!(sp->stat)) continue;
		if(sp->y < bottom) bottom = sp->y;
		if(sp->Y < bottom) bottom = sp->Y;
		if(sp->y != sp->Y) continue;
		if(sp==sstart) sbf = sfin - 1; 
		else sbf = sp-1;
		if(sp<sfin-1) snx = sp+1;
		else snx = sstart;
		if(sbf->y==sp->y){	/* point before is below*/
			if(snx->y==sp->y){	/* both below */
				sp->stat = 0;
			}
			continue;
		}
		else if(sbf->Y==sp->y){	/*previous point above*/
			if(snx->Y==sp->y){	/* both above */
				sp->stat = 0;
			}
			continue;
		}
	}
}

swpt(a,b)
	jPoint *a, *b;
{
	jPoint t;
	t = *a;
	*a = *b;
	*b = t;	
}

lline(K,M)
	jPoint K, M;
{
	sflip(M,K)
	sinpp->x = K.x;	sinpp->y = K.y;
	sinpp->X = M.x;	sinpp->Y = M.y;
	sinpp->type = 'l';
	sinpp->stat = 1;
	sinpp++;
}

arc(K,V,M)
	jPoint K, V, M;
{
	double a, b, x0, c, d, y0;
	double e, f, t;
	double xx, yy, xx1, yy1, xx2, yy2;
	jPoint Q, R, S;
	short endoff1, endoff2;

	sflip(M,K)
	a = (double)K.x + M.x - 2*V.x;
	b = 2*(V.x - K.x);
	x0 = K.x;
	c = (double)K.y + M.y - 2*V.y;
	d = 2*(V.y - K.y);
	y0 = K.y;
	e = a*d-b*c;
	if(!e) {
		lline(K,M);
		return(0);
	}
	if(!c) {
		sinpp->type = 'h';
		goto work;
	}
	else if(!d) {
			sinpp->type = 'd';
			goto work;
		}
		else sinpp->type = 'v';
	t = - (d/c)/2;
	if(t<=0 || t >=1) goto work;
	f = -a*t - b;
	Q.x = x0 - t*f + 0.5;	/* values are>0 since they are in j-coord */
	Q.y = y0 - t*t*c + 0.5;
	R.x = x0 - (b*d/c)/4 + 0.5;
	R.y = Q.y;
	xx1 = V.x - M.x;
	yy1 = V.y - M.y;
	S.x = M.x + ((Q.y-M.y)*xx1)/yy1 + 0.5;	/*if den=0, then t=1*/
	S.y = Q.y;
	if( pequal(K,R) || pequal(R,Q) ) lline(K,Q);
	else arc(K,R,Q);
	if( pequal(Q,S) || pequal(S,M) ) lline(Q,M);
	else arc(Q,S,M);
	return(0);
	work:;	/* process arc */
	sinpp->x = K.x;	sinpp->y = K.y;
	sinpp->X = M.x;	sinpp->Y = M.y;
	switch(sinpp->type){
	case 'h':
		sinpp->c1 = (a/d)/d;
		sinpp->c2 = (b/d);
		sinpp->c3 = 0;
		break;
	case 'd':
		sinpp->c1 = a/c;
		sinpp->c2 = b/c;
		sinpp->c3 = c;
		break;
	case 'v':
		sinpp->c1 = a/c;
		sinpp->c2 = (e/c)*(d/c)/2;
		sinpp->c3 = (4/d)*(c/d); /* d!=0 in 'v' */
		break;
	default:
		rrecord("No coef. Unknown type %c",sinpp->type);
	}
	sinpp->stat = 1;
	sinpp++;
	return(0);
}

realfill(sstart,sfin)
	segP sstart, sfin;
{
	register i, n;
	register segP  sp;
	segP s1, s2, st;
	short yb;

	sp = sstart;
	n = sfin -sstart;
	i = 0;
	s1 = sp; 
	yb = s1->y;
	for(;yb<=sp->y;sp++,i++) ;
	s2 = sp - 1;
	for(;yb>=bottom;yb--){
		draw(yb,s1,s2);
		for(st=s1;st<=s2;st++){
			if(!(st->stat)) continue;
			if(yb<=st->Y) st->stat=0;
		}
		if(sp >= sfin) continue; /*nomore*/
		if(i>=n) continue;
		for(; yb<=(sp->y+1)&&i<n; sp++,i++){
			/*we have gone below next element */
			for(st=s1;st<=s2;st++){
				if(sp->y==st->Y){
					if(sp->x==st->X) st->stat=0;
					else if(sp->Y==sp->y&&sp->X==st->X) st->stat=0;
				}
			}
		}
		/*cleanup*/
		s2 = sp - 1;
		while(s1<sfin && !(s1->stat)) s1++;
	}
}

#define NCUTS 32
#define NCUTS_1 31

#ifndef NO_INTER
#define paint(left,right,height,color) { s_char(color); s_short(left); \
		s_short(right); s_short(height); }
#endif

extern double xcut();

draw(y,pr1,pr2) short y; segP pr1, pr2;
{
	register segP pr;
	register i, j, k, n;
	double xtmp, x[NCUTS];

	for(pr=pr1,n=0; pr<=pr2 && n<NCUTS_1 ; pr++)
		if(pr->stat && pr->y!=pr->Y) x[n++] = xcut(y,pr);

	if(n<2) return(0);
	do {
		k = 0;
		for(i=1;i<n;i++){
			if(x[i-1]>x[i]) {
				xtmp = x[i-1]; 
				x[i-1] = x[i]; 
				x[i] = xtmp; 
				k=1;
			}
		}
	} while(k);
#ifdef NO_INTER
	for(i=1;i<n;i +=2) paint(x[i-1],x[i],y,shade);
#else
	for(i=1;i<n;i +=2) paint((short)(x[i-1]+0.5),(short)(x[i]+0.5),y,shade);
#endif
	return(1);
}

#include <math.h>

double xcut(y,sgp)
	segP sgp;
{
	double dx, dy, xy;
	double v;
	double x, x1, y1, x2, y2;
	x1 = sgp->x;	y1 = sgp->y;
	x2 = sgp->X;	y2 = sgp->Y;

	if(y1==y2) {	/* no matter what is the type */
		x = x1>x2 ? x1: x2;
		return(x);
	}

	switch(sgp->type) {
	case 'l':
		dy = y1-y2; 
		dx = x1-x2; 
		xy = y1*x2-x1*y2;
		x = (y*dx+xy)/dy;	/*dy non zero because of prev. check*/
		break;
	case 'h':
		dy = y - y1;
		x = x1 + (sgp->c1*dy + sgp->c2)*dy;
		break;
	case 'd':
		dy = y - y1;
		v = sgp->c3*dy;
		if(v<0) {
			rrecord("sqrt arg<0 %f",v);
			x = (x1+x2)/2;
			}
		else
		x = x1 + sgp->c1*dy - sgp->c2*sqrt(v);
		break;
	case 'v':
		dy = y - y1;
		v = 1 + sgp->c3*dy;
		if(v<0) {
			rrecord("sqrt arg<0 %f",v);
			x = (x1+x2)/2;
			}
		else
		x = x1 + sgp->c1*dy + sgp->c2*(1 - sqrt(v));
		break;
	default:
		rrecord("unknown curve: %c",sgp->type);
		x = (x1+x2)/2;
	}
	return(x);
}
