/* Copyright (c) 1989, 1990 AT&T --- All Rights Reserved.              */
/* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T.                */
/* The copyright notice does not imply actual or intended publication. */
/* AUTHORS:                                            */
/*     H. S. Baird - ATT-BL MH - first versions        */
/*     T. J. Thompson - ATT-BL HO - improvements       */

/* Coord.c - function bodies for basic Coordinate geometry.
   See Coord.h for associated defines, typedefs, etc.
   */

#include <stdio.h>
#include <math.h>
#include <string.h>
#define ATAN_INCL 1
#include "CPU.h"
#include "boole.h"
#include "Units.h"
#include "Coord.h"

/* Added by Tim Thompson ... */
int Readvax = 0;

VOID
setreadvax()
{
	char *getenv();
	char *p = getenv("READVAX");
	/* any non-null value of READVAX will set it */
	if ( p!=NULL && strlen(p)>0 )
		Readvax = 1;
}

/* ... */

/* Convert a real value, a units specification, and a resolution
   into a value in scanner coordinates. */
int vto_scoor(v,u,r)
    double v;	/* value in special units */
    char u;	/* units: one of UNITS (see ric.h) */
    int r;	/* resolution in pixels/inch */
{   int c;
	switch(u) {
	    case 'i':	/* inches */
		c = (int)(v*r + 0.5);
		break;
	    case 'c':	/* cm */
		c = (int)((v*r/2.54) + 0.5);
		break;
	    case 'p':	/* points */
		c = (int)((v*INCHES_PER_POINT*r) + 0.5);
		break;
	    case 'P':	/* picas */
		c = (int)((v*12.0*INCHES_PER_POINT*r) + 0.5);
		break;
	    case 's':	/* scanner pixel (Scoor) */
	    case 'u':	/* basic unit */
		c = (int)(v + 0.5);
		break;
	    default:
		c = (int)(v + 0.5);
		break;
	    };
	return(c);
	}

/* Return pointer to empty Sp from malloc space */
Sp *alloc_sp()
{   Sp *spp;
	if((spp=(Sp *)malloc(sizeof(Sp)))==NULL)
		abort("alloc_sp: can't");
	*spp = zero_Sp;
	return(spp);
	}

/* Free Sp from malloc space */
free_sp(spp)
    Sp *spp;
{	free(spp);
	}

Sp *ato_sp(s)
    char *s;
{   char *d;
    static Sp p;
    char *p_x,*p_y;
#define SP_DELIM "(), "
	d = strdup(s);
	p_x = strtok(d,SP_DELIM);
	p_y = strtok((char *)0,SP_DELIM);
	if(p_x==NULL||p_y==NULL)
		{free(d); return(NULL);};
	p.x = atoi(p_x);
	p.y = atoi(p_y);
	free(d);
	return(&p);
	}

char *sp_toa(sp)
    Sp *sp;
#define sp_toa_distinct (5)
{   static char s[sp_toa_distinct][40];
    static int sp_toa_cur = -1;
	sp_toa_cur = (sp_toa_cur+1)%sp_toa_distinct;
	sprintf(s[sp_toa_cur],"(%d,%d)",sp->x,sp->y);
	return(s[sp_toa_cur]);
	}

/* Read a list of Sp's (in ascii) from file *fp, into set *spsp */
frda_sps(fp,spsp)
    FILE *fp;
    Sps *spsp;
#define MAX_SPS_LINE (80)
{   Sp *spp,**bb;
    long seek;
    char line[MAX_SPS_LINE+1];
	*spsp = empty_Sps;
	seek = ftell(fp);
	/* count all points quickly, in advance */
	while(!feof(fp)) {
		line[0] = '\0';
		fgets(line,MAX_SPS_LINE,fp);
		if(strlen(line)>0) {
			spsp->mny++;
			};
		};
	if((spsp->pa=(Sp **)malloc((spsp->mny+1)*sizeof(Sp *)))==NULL)
		abort("frda_sps: can't alloc spsp->pa[%d]",spsp->mny+1);
	/* next, reread points */
	fseek(fp,(long)seek,0);
	bb=spsp->pa;
	while(!feof(fp)) {
		line[0] = '\0';
		fgets(line,MAX_SPS_LINE,fp);
		if(strlen(line)>0) {
			*(spp = alloc_sp()) = *ato_sp(line);
			*(bb++) = spp;
			};
		};
	*bb = NULL;
	}

err_sps(p)
    Sps *p;
{   int i;
    Sp **s;
	fprintf(stderr,"Sps: %d points: ",p->mny);
	if(p->mny>0) for(s=p->pa; (*s)!=NULL; s++) {
		fprintf(stderr,"%s ",sp_toa(*s));
		};
	fprintf(stderr,"\n");
	}

Sp *append_sp_sps(sp,sps)
   Sp *sp;
   Sps *sps;
{	if(sps->mny>0) {
		if((sps->pa=(Sp **)realloc(sps->pa,(sps->mny+2)*sizeof(Sp *)))==NULL)
			abort("append_sp_sps: can't realloc sps->pa[%d]",sps->mny+2);
		}
	else {	sps->mny=0;
		if((sps->pa=(Sp **)malloc((sps->mny+2)*sizeof(Sp *)))==NULL)
			abort("append_sp_sps: can't alloc sps->pa[%d]",sps->mny+2);
		};
	sps->pa[sps->mny++] = sp;
	sps->pa[sps->mny] = NULL;
	}

/* Return pointer to local static Sps which holds a duplicate of the
   given Sps (all contents are freshly malloc'ed) */
Sps *dup_sps(old)
    Sps *old;
{   static Sps new;
    Sp **o,**n;
	if(old->mny>0) {
		new.mny = old->mny;
		if((new.pa=(Sp **)malloc((new.mny+1)*sizeof(Sp *)))==NULL)
			abort("dup_sps: can't alloc new.pa[%d]",new.mny+1);
		for(o=old->pa,n=new.pa; *o!=NULL; o++,n++) {
			*(*n = alloc_sp()) = **o;
			};
		*n = NULL;
		}
	else new = empty_Sps;
	return(&new);
	}

free_sps(p)
   Sps *p;
{	if(p->pa!=NULL) free(p->pa);
	*p = empty_Sps;
	}

Sp *append_sp_spa(sp,spa)
   Sp *sp;
   Spa *spa;
{	if(spa->mny>0) {
		if((spa->a=(Sp *)realloc(spa->a,(spa->mny+1)*sizeof(Sp)))==NULL)
			abort("append_sp_spa: can't alloc spa->a[%d]",spa->mny+1);
		}
	else {	spa->mny=0;
		if((spa->a=(Sp *)malloc((spa->mny+1)*sizeof(Sp)))==NULL)
			abort("append_sp_spa: can't alloc spa->a[%d]",spa->mny+1);
		};
	spa->a[spa->mny++] = *sp;
	}

err_spa(p)
    Spa *p;
{   int i;
    Sp *sp;
	fprintf(stderr,"Spa: %d points: ",p->mny);
	for(i=0,sp=p->a;i<p->mny;i++,sp++) {
		fprintf(stderr,"%s ",sp_toa(sp));
		};
	fprintf(stderr,"\n");
	}

free_spa(p)
   Spa *p;
{	if(p->a!=NULL) free(p->a);
	*p = empty_Spa;
	}

/* Rotate the Scanner point `(x,y') about origin `*orp' by angle `ang',
   and return pointer to the result.  This is slow, but no matter. */
Sp *rotate_Sp(ang,orp,x,y)
    double ang;
    Sp *orp;
    int x,y;
{   static Sp res;
    Sp of;
	of.x = x - orp->x;  of.y = y - orp->y;
	res.x = (short)(((cos(ang)*of.x - sin(ang)*of.y)) + orp->x + 0.5);
	res.y = (short)(((sin(ang)*of.x + cos(ang)*of.y)) + orp->y + 0.5);
	return(&res);
	}

/* Horizontally shear the Scanner point `(x,y') about origin `*orp' by shear
   angle `ang', and return pointer to the result.  `Ang' is close to 90 degrees.
   This is slow, but no matter. */
Sp *hshear_Sp(ang,orp,x,y)
    double ang;
    Sp *orp;
    int x,y;
{   static Sp res;
    Scoor of_y,tr_x;
	res.x = (short)(orp->x - (y - orp->y)*tan(ang-(90.0*DtoR)) + 0.5);
	res.y = orp->y;
	return(&res);
	}

/* Return pointer to initialized Edge from malloc space */
Edge *alloc_edge()
{   Edge *ep;
	if((ep=(Edge *)malloc(sizeof(Edge)))==NULL)
		abort("alloc_edge: can't");
	*ep = empty_Edge;
	return(ep);
	}

/* Free Edge from malloc space */
free_edge(ep)
    Edge *ep;
{	free(ep);
	}

char *edge_toa(ep)
	Edge *ep;
{   static char s[40];
	strcpy(s,sp_toa(&(ep->a)));
	strcat(s,"-");
	strcat(s,sp_toa(&(ep->b)));
	return(s);
	}

Edge *ato_edge(s)
    char *s;
{   char *d;
    static Edge e;
    char *a_x,*a_y,*b_x,*b_y;
#define EDGE_DELIM "(),- "
	d = strdup(s);
	a_x = strtok(d,EDGE_DELIM);
	a_y = strtok((char *)0,EDGE_DELIM);
	b_x = strtok((char *)0,EDGE_DELIM);
	b_y = strtok((char *)0,EDGE_DELIM);
	if(a_x==NULL||a_y==NULL||b_x==NULL||b_y==NULL)
		{free(d); return(NULL);};
	e.a.x = atoi(a_x);
	e.a.y = atoi(a_y);
	e.b.x = atoi(b_x);
	e.b.y = atoi(b_y);
	free(d);
	return(&e);
	}

/* Return pointer to empty Bbx from malloc space */
Bbx *alloc_bbx()
{   Bbx *bxp;
	if((bxp=(Bbx *)malloc(sizeof(Bbx)))==NULL)
		abort("alloc_bbx: can't");
	*bxp = empty_Bbx;
	return(bxp);
	}

/* Free Bbx from malloc space */
free_bbx(bxp)
    Bbx *bxp;
{	free(bxp);
	}

char *bbx_toa(bxp)
	Bbx *bxp;
{   static char s[40];
	strcpy(s,sp_toa(&(bxp->a)));
	strcat(s,sp_toa(&(bxp->b)));
	return(s);
	}

Bbx *ato_bbx(s)
    char *s;
{   char *d;
    static Bbx bx;
    char *a_x,*a_y,*b_x,*b_y;
#define BBX_DELIM "(), "
	d = strdup(s);
	a_x = strtok(d,BBX_DELIM);
	a_y = strtok((char *)0,BBX_DELIM);
	b_x = strtok((char *)0,BBX_DELIM);
	b_y = strtok((char *)0,BBX_DELIM);
	if(a_x==NULL||a_y==NULL||b_x==NULL||b_y==NULL)
		{free(d); return(NULL);};
	bx.a.x = atoi(a_x);
	bx.a.y = atoi(a_y);
	bx.b.x = atoi(b_x);
	bx.b.y = atoi(b_y);
	free(d);
	return(&bx);
	}

Bbx *expand_bbx(bxp,X)
    Bbx *bxp;
    int X;		/* expansion code */
{   static Bbx xbx;
	xbx = *bxp;
	switch(X) {
	    case -3:
		xbx.a.x += 2;
		xbx.a.y += 2;
		xbx.b.x -= 1;
		xbx.b.y -= 1;
		break;
	    case -2:
		xbx.a.x += 1;
		xbx.a.y += 1;
		xbx.b.x -= 1;
		xbx.b.y -= 1;
		break;
	    case -1:
		xbx.a.x += 1;
		xbx.a.y += 1;
		break;
	    case 0:
		break;
	    case 1:
		xbx.b.x += 1;
		xbx.b.y += 1;
		break;
	    case 2:
		xbx.a.x -= 1;
		xbx.a.y -= 1;
		xbx.b.x += 1;
		xbx.b.y += 1;
		break;
	    case 3:
		xbx.a.x -= 1;
		xbx.a.y -= 1;
		xbx.b.x += 2;
		xbx.b.y += 2;
		break;
	    default:
		break;
	    };
	return(&xbx);
	}

/* Predicate:  Is most of Bbx 1 inside Bbx 2?  ``Most'' is defined
   as more than half of the first Bbx's area inside BBx 2.*/
boolean bbx_inside_most(b1,b2)
    Bbx *b1,*b2;
{   struct {		/* overlap between blob box and selection box */
	Bbx bx;		/* Bbx of intersection */
	Sp sd;		/* side-lengths of overlap box, >=0 */
	long area;	/* area is square pixels, >=0 */
	} ov;
	if(!bbx_inside_any(b1,b2)) return(F);
	else if(bbx_inside_all(b1,b2)) return(T);
	else {	/* compute overlap box */
		ov.bx.a.x = ((b1)->a.x>(b2)->a.x)? (b1)->a.x: (b2)->a.x;
		ov.bx.a.y = ((b1)->a.y>(b2)->a.y)? (b1)->a.y: (b2)->a.y;
		ov.bx.b.x = ((b1)->b.x<(b2)->b.x)? (b1)->b.x: (b2)->b.x;
		ov.bx.b.y = ((b1)->b.y<(b2)->b.y)? (b1)->b.y: (b2)->b.y;
		ov.sd.x = bbx_wid(&ov.bx);  if(ov.sd.x<0) ov.sd.x=0;
		ov.sd.y = bbx_hgt(&ov.bx);  if(ov.sd.y<0) ov.sd.y=0;
		ov.area = ov.sd.x*ov.sd.y;
		return( (ov.area>0) && ( 2*ov.area > bbx_area(b1) ) );
		};
	}

Bbx *translate_bbx(bxp,off)
    Bbx *bxp;
    Sp off;
{   static Bbx res;
	res.a.x = bxp->a.x + off.x;
	res.a.y = bxp->a.y + off.y;
	res.b.x = bxp->b.x + off.x;
	res.b.y = bxp->b.y + off.y;
	return(&res);
	}

/* Free Bbxs (but not the contents) */
free_bbxs(bxsp)
    Bbxs *bxsp;
{   Bbx **b;
	if(bxsp->mny>0) {
		if(bxsp->pa!=NULL) free(bxsp->pa);
		};
	*bxsp = empty_Bbxs;
	}

/* Free Bbxs (and the individually-malloc'ed contents) */
free_bbxs_etc(bxsp)
    Bbxs *bxsp;
{   Bbx **b;
	if(bxsp->mny>0) {
		for(b=bxsp->pa; (*b)!=NULL; b++) free(*b);
		if(bxsp->pa!=NULL) free(bxsp->pa);
		};
	*bxsp = empty_Bbxs;
	}

/* Return pointer to local static Bbxs which holds a duplicate of the
   given Bbxs (all contents are freshly malloc'ed) */
Bbxs *dup_bbxs(old)
    Bbxs *old;
{   static Bbxs new;
    Bbx **o,**n;
	if(old->mny>0) {
		new.mny = old->mny;
		new.alloc = old->alloc;
		new.incr = old->incr;
		if((new.pa=(Bbx **)malloc((new.alloc)*sizeof(Bbx *)))==NULL)
			abort("dup_bbxs: can't alloc new.pa[%d]",new.alloc);
		for(o=old->pa,n=new.pa; *o!=NULL; o++,n++) {
			*(*n = alloc_bbx()) = **o;
			};
		*n = NULL;
		}
	else new = empty_Bbxs;
	return(&new);
	}

Bbx *append_bbx(bxp,bxsp)
   Bbx *bxp;
   Bbxs *bxsp;
{	if(bxsp->mny>0) {
		if((bxsp->mny+2)>bxsp->alloc) {
			bxsp->alloc += bxsp->incr;
			if( ( bxsp->pa =
			      (Bbx **)realloc(bxsp->pa,bxsp->alloc*sizeof(Bbx *)) )
			    ==NULL )
				abort("append_bbx: can't alloc bxsp->pa[%d]",bxsp->alloc);
			};
		}
	else {	bxsp->mny=0;
		if(bxsp->pa!=NULL) free(bxsp->pa);
		bxsp->alloc=0;
		while((bxsp->mny+2)>bxsp->alloc) bxsp->alloc += bxsp->incr;
		if((bxsp->pa=(Bbx **)malloc(bxsp->alloc*sizeof(Bbx *)))==NULL)
			abort("append_bbx: can't alloc bxsp->a[%d]",bxsp->alloc);
		};
	bxsp->pa[bxsp->mny++] = bxp;
	bxsp->pa[bxsp->mny] = NULL;
	}

frda_bbxs(fp,bxsp)
    FILE *fp;
    Bbxs *bxsp;
#define MAX_BBX_LINE (80)
{   Bbx *bxp,**bb;
    long seek;
    char line[MAX_BBX_LINE+1];
	*bxsp = empty_Bbxs;
	seek = ftell(fp);
	/* count all boxes quickly, in advance */
	while(!feof(fp)) {
		line[0] = '\0';
		fgets(line,MAX_BBX_LINE,fp);
		if(strlen(line)>0) {
			bxsp->mny++;
			};
		};
	if((bxsp->pa=(Bbx **)malloc((bxsp->mny+1)*sizeof(Bbx *)))==NULL)
		abort("frda_bbxs: can't alloc bxsp->pa[%d]",bxsp->mny+1);
	bxsp->alloc = bxsp->mny+1;
	/* next, reread boxes */
	fseek(fp,(long)seek,0);
	bb=bxsp->pa;
	while(!feof(fp)) {
		line[0] = '\0';
		fgets(line,MAX_BBX_LINE,fp);
		if(strlen(line)>0) {
			*(bxp = alloc_bbx()) = *ato_bbx(line);
			*(bb++) = bxp;
			};
		};
	*bb = NULL;
	}

fwra_bbxs(fp,bxsp)
    FILE *fp;
    Bbxs *bxsp;
{   Bbx **b;
	if(bxsp->mny>0) for(b=bxsp->pa; *b!=NULL; b++) {
		fputs(bbx_toa(*b),fp);
		fputs("\n",fp);
		};
	fflush(fp);
	}
