/*
*	Beautification program	
*	n=number of object pointers in oa[]
*
*	it coisiders two kinds of relations:
*	those that are nonlinear, they create an equation
*	for each cluster member and use the cluster value
*	(e.g mean slope)
*	linear relations pair the first cluster member
*	with all the others, and create one less equ.
*	than cluster members. the cluster value is ignored
*
*	point relations are treated through pseudosides
*	so that we can use the same clustering routines.
*/

#include "beauty.h"
#include "locus.h"

#ifdef REALPED
short detail=0;
char flags[128] = {"SELXY"};
#else
extern char flags[];
#endif

#define find(A) (pindex(flags,A))

FILE *fq;

extern double cut();
extern Point ccenter();
extern short scomp();
extern short xycomp();
extern short idle();

clust * cls;
clustP pcls;	/* pointer to first available loc in cls[] */

Point * row;	/* where a copy of all coordinates is stored */
short * offrow;	/* used to go from row[] back to objects */
short * nrow;	/* number of points in same contour on row[] */
Point * mv_tol;	/* where maximum allowed dx, dy are stored */

sides * stt;	/* side list. used by everything */
sideP * stp;	/* pointers to stp for processing by cluster1() */
sideP * cmem;	/* store members of clusters */
sideP ** lp;	/* preliminary cluster pointers */
short * lpstat;	/* status of lp pointers */

sideP *pmem;

sides * srow;	/* make point list look like sides */

eqnS * eq;	/* side equations */
short neq, oeq;

/*	preferred slope values	*/
#define NEAR_G 0.1
short GN = 5;
double gslope[6] = { 0, QPI, HPI, TPI, PI };

short epsilon;

/*
	major routine called by other parts of program
	from main() in fped
	from hwork() in ped
*/

formatt(n,oa)
	short n;
	objP oa[];
{
	register  i, j;
	register sideP st;
	register sideP *pp;
	register clustP clj;
	register Point * pt;
	clustP opcls, topcls;
	Point pc;
	short ktotal;
	short nsides;
	short k;
	double q;
	clustP cnx;

	rrecord("%d objects to be formatted",n);
#ifdef REALPED
	if( (fq = fopen("ped.sort","a")) == NULL) {
		rrecord("Cannot print ped.sort");
		fq = stderr;
	}
	else setbuf(fq,NULL);
#else
	fq = stderr;
#endif

	fprintf(fq,"\nNEW RUN\n");
	set_up(n,oa);
	ktotal = mk_row(n,oa);	/* Build `row' and forget about objects	*/
	if(!ktotal) goto telos;
	nsides = mk_sides(n);	/*	Create array sides[] from row[]	*/
	rrecord("%d sides found",nsides);	
	if(nsides<1) goto telos;

	if(smalls < SMALL_MV) smalls = SMALL_MV;
	epsilon = smalls+0.5;
	/* desparation move on 1/31	*/
	epsilon *= 2;
	for(i=0;i<ktotal;i++){
		mv_tol[i].y = mv_tol[i].x = epsilon;
	}	

	neq = 0;	/* start with zero eqs */
	/*
	*	cluster sides according to various ways
	*/

	pcls = cls;
	pmem = cmem;

	for(j=0,st=stt,pp=stp; j<nsides; j++) *pp++ = st++;

	/* By slope */

	for(j=0,st=stt; j<nsides; j++,st++) st->t = st->s;
	cluster1(nsides,stp,1,"slopes");

	/*	check negative conditions	*/
	chk_neg(cls,pcls,scomp,' ');

	/*	fix slopes to good values	*/
	for(clj=cls; clj<pcls; clj++){
		for(k=0;k<GN;k++){
			if( fabs(clj->val-gslope[k]) < NEAR_G){
 				clj->val = gslope[k];
				if(clj->n==1){ /*make eqs right away*/
					if(detail) rrecord("%d ->eq",clj-cls);
					st = *(clj->mem);
					if(st->nu) continue;
					st->s = clj->val;
					eq[neq].s1 = eq[neq].s2 = st;
					eq[neq++].u = SLOPE;
				}
				break;
			}
		}
	}

	if(detail) list(cls,pcls,stt);

	/*	make slope eqs	*/
	k = 0;
	cnx = cls;
	for(clj=cls;clj<pcls;clj++){
		/* look for right angles	*/
		q = clj->val + HPI;
		if(cnx <= clj) cnx = clj+1;
		for(;cnx<pcls; cnx++){
			if(fabs(cnx->val-q) < DELTA){
				if(cnx->n<2) {
					st = *(cnx->mem);
					if(st->nu) continue;
					st->s = q;
					eq[neq].s1 = eq[neq].s2 = st;
					eq[neq++].u = SLOPE;
				}
				if(clj->n<2){
					if( (*(clj->mem))->nu ) continue;
					eq[neq].s1 = eq[neq].s2 = *(clj->mem);
					eq[neq++].u = SLOPE;
				}
				break;
			}
		}

		if(clj->n<2) continue;
		k++;
		for(i=0;i<clj->n;i++) {
			if( (st = *(clj->mem+i))->nu ) continue;
			st->s = clj->val;
			eq[neq].s1 = st;
			eq[neq].s2 = st;
			eq[neq++].u = SLOPE;
			fprintf(stderr,"sl_eq: %g,%g to %g,%g sl=%g\n",
			(st->p1)->x,(st->p1)->y,(st->p2)->x,(st->p2)->y,st->s);
		}
	}

	rrecord("%d clusters from slope, %d eqs",k,neq);

	if(find('E')){	/*	make extension equations	*/
		oeq = neq;
		opcls = pcls;
		k = 0;
		for(clj=cls; clj<opcls; clj++) {
			if(clj->n<2) continue;
			pc = ccenter(clj);
			for(j=0;j<clj->n;j++){
				st = *(clj->mem+j);
				st->t = cut(st,pc,stt);
			}
			topcls = pcls;
			cluster1(clj->n,clj->mem,epsilon,"extension");
			k += add_eq(topcls,pcls,EXTENSION);
		}

		if(detail) list(opcls,pcls,stt);
		rrecord("%d clusters form exten., %d eqs",k,neq-oeq);
	}

	if(find('L')) {	/* find length clusters */
		for(st=stt,j=0; j<nsides; j++, st++) st->t = st->l;
		create(nsides,epsilon,EQUAL,stp,stt,"lengths",idle);
	}

	/*find point clusters */
	for(i=0,st=srow,pt=row; i<ktotal; i++,st++,pt++) {
		st->p1 = st->p2 = pt;
		st->nu = 0;
	}
	for(i=0,st=srow,pp=stp; i<ktotal; i++) *pp++ = st++;
	if(find('X')){
		for(i=0,st=srow;i<ktotal;i++,st++) st->t = (st->p1)->x;
		create(ktotal,epsilon,SAMEX,stp,srow,"x coinc",xycomp);
	}
	if(find('Y')){
		for(i=0,st=srow;i<ktotal;i++,st++) st->t = (st->p1)->y;
		create(ktotal,epsilon,SAMEY,stp,srow,"y coinc",xycomp);
	}
	if(find('A')) goto telos;

	varinit (ktotal);
	solve (eq, neq);

	spill_row(ktotal,n,oa);

	telos: clean_up();

#ifdef REALPED
	fclose(fq);
#endif
}

/*	check negative conditions	*/
chk_neg(C1, C2, action,how)
	clustP C1, C2;
	short (* action)();
	char how;
{
	register i, j;
	register sideP st;
	register clustP clj;

	for(clj=C1; clj<C2; clj++){
		for(i=0;i<clj->n-1;i++){
			st = *(clj->mem+i);
			for(j=i+1;j<clj->n;j++)	action(st,*(clj->mem+j),how);
		}
	}
}

short scomp(sda, sdb, ignore)
	sideP sda, sdb;
	char ignore;
{
	double d1, d2, d3, d4;
	if( !(d1 = cut(sda,*(sdb->p1),stt)) ) goto zap;
	if( !(d2 = cut(sda,*(sdb->p2),stt)) ) goto zap;
	if( !(d3 = cut(sdb,*(sda->p1),stt)) ) goto zap;
	if( !(d4 = cut(sdb,*(sda->p2),stt)) ) goto zap;
	if(d1*d2<0 && d3*d4<0) goto zap;
	return;
	zap:
		if(detail) fprintf(fq,"freeze pair of sides\n");
		sda->nu = sdb->nu = 1;
		eq[neq].s1 = eq[neq].s2 = sda;
		eq[neq].u = SLOPE;
		eq[neq++].penalty = -100;
		eq[neq].s1 = eq[neq].s2 = sdb;
		eq[neq].u = SLOPE;
		eq[neq++].penalty = -100;
}

short idle() {}

short xycomp(sda, sdb, what)
	sideP sda, sdb;
	char what;
{
	double dx, dy, span;
	register ioff, joff;

	span = epsilon*DELTA1;	/* what cluster1 uses for d_adj */
	dx = fabs((sda->p1)->x - (sdb->p1)->x);
	dy = fabs((sda->p1)->y - (sdb->p1)->y);
	if(what=='y' && dx > span) return;
	else if(what=='x' && dy > span) return;
	sda->nu = sdb->nu = 1;
	ioff = sda->p1 - row;
	joff = sdb->p1 - row;
	if(detail) fprintf(fq,"constraint points %d and %d\n",ioff,joff);
	mv_tol[ioff].x = mv_tol[joff].x = dx/3;
	mv_tol[ioff].y = mv_tol[joff].y = dy/3;
}
