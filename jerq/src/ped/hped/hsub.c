/*		VAX ped utilities		*/
/*	Also functions that make objects  read from file on VAX	*/

#include "hped.h"


/*	error message	*/

rrecord(s,a1,a2,a3,a4,a5,a6) char *s;
{
	fprintf(frecord,"PED: ");
	fprintf(frecord,s,a1,a2,a3,a4,a5,a6);
	fprintf(frecord,"\n");
}

error(serious,s,a1,a2,a3,a4,a5,a6) char *s;
{
	fprintf(stderr,"PED: ");
	fprintf(stderr,s,a1,a2,a3,a4,a5,a6);
	fprintf(stderr,"\n");
	if(serious) done();
}

Free(st)
	char *st;
{
	if(st) free(st);
}

purge(){
	register j;
	register objP a;
	for(j=0;j<no_obj;j++){
		a = obj[j];
		if(!a) continue;
		obj[j] = 0;
		if(a->type=='t') Free(a->txt);
		if(a->type=='f') Free(a->mem);
		Free(a->p0);
		Free(a);
	}
	for(j=0;j<bn;j++){
		a = bb[j];
		if(!a) continue;
		bb[j] = 0;
		if(a->type=='t') Free(a->txt);
		if(a->type=='f') Free(a->mem);
		Free(a->p0);
		Free(a);
	}
	bn = 0;
	no_obj = 0;
}

/*	Functions that make objects  read from file on VAX	*/

/* -- -- make an OBJECT from collected points -- -- */
/* -- It is called only from rd_obj and rd_pic --  */

objP mk_obj(pbase,plst,gtype,pen,arhead,brush,opsp,psz)
	Point *pbase, *plst;
	char gtype,pen,arhead,brush,opsp;
	short psz;
{
	register n, j;
	objP aa;

	if(gtype=='m') gtype = 'p';
	n = plst - pbase;
	if(n<=1) return((objP)0);	/* we reject polygons and splines
					of size 1. n=0 is definetetly illegal
					but n=1 should be banned as well */
	aa = (objP)malloc((unsigned)(sizeof(objS)));
	if(!aa) error(1,"Bailout");
	aa->n = n;
	aa->p0 = (Point *)malloc((unsigned)(n*sizeof(Point)));
	if(aa->p0==0) error(1,"Bailout");
	for(j=0;j<n;j++)  aa->p0[j] = pbase[j];
#ifdef OLD_HIST
	if(arhead != ' ') aa->size = arhead;
	else aa->size = 0;
#else
	aa->size = arhead;
#endif
	aa->color = pen;
	aa->pen = brush;
	aa->ps = psz;
	aa->type = gtype;
	aa->status = 1;
	if(gtype == 's' && opsp == ' ') {
		if(aa->p0[0].t <1) aa->p0[0].t = 1;
		if(aa->p0[n-1].t <1) aa->p0[n-1].t = 1;
	}
	else if(gtype == 'S'){
		if(opsp=='w') aa->p0[0].t = 1;
		}
	if(gtype == 's' || gtype == 'S'){
		aa->txt = malloc((unsigned)(n+1));
		for(j=0;j<n;j++) aa->txt[j]='0' + aa->p0[j].t;
		aa->txt[n]=0;
	}
	ex_one(aa);
	return(aa);
}

objP mk_circ(pp, r, pen, brush, psz)
	Point *pp; int r;
	char pen, brush;
	short psz;
{
	objP aa;
	aa = (objP)malloc((unsigned)(sizeof(objS)));
	if(!aa) error(1,"Bailout");
	aa->type = 'c';
	aa->size = r;
	aa->ps = psz;
	aa->n = 1;
	aa->p0 = (Point *)malloc((unsigned)(2*sizeof(Point)));
	if(aa->p0==0) error(1,"Bailout");
	aa->p0[0] = *pp;
	aa->status = 1;
	aa->color = pen;
	aa->pen = brush;
	ex_one(aa);
	return(aa);
}

/* --  make text object -- */

objP mk_text(str, pp, font, tsize, brush, psz)
	char *str; 
	Point *pp;
	char font, tsize, brush;
	short psz;
{
	objP aa;
	register j;

	aa = (objP)malloc((unsigned)(sizeof(objS)));
	if(!aa) error(1,"Bailout");
	aa->n = 1;
	aa->p0 = (Point *)malloc((unsigned)(2*sizeof(Point)));
	if(aa->p0==0) error(1,"Bailout");
	aa->p0[0] = *pp;
	if(str[0] != '\\') j = 3; else j =1;
	aa->txt = malloc((unsigned)(strlen(str)+j));
	if(aa->txt==0) error(1,"Bailout");
	if(str[0] != '\\') {
		aa->txt[0]='\\'; aa->txt[1]='C';
		aa->txt[2] = 0;
		strcat(aa->txt,str);
	}
	else strcpy(aa->txt,str);
	aa->type = 't';
	aa->status = 1;
	aa->color = font;
	aa->pen = brush;
	aa->ps = psz;
	aa->size = tsize;
	ex_one(aa);
	return(aa);
}

objP mk_fam(xx,yy,n,bobj) short xx, yy, n; objP bobj[];
{
	objP aa;
	register i;
	aa = (objP)malloc((unsigned)(sizeof(objS)));
	if(!aa) error(1,"Bailout");
	aa->size = n;
	aa->n = 1;
	aa->p0 = (Point *)malloc((unsigned)(2*sizeof(Point)));
	if(aa->p0==0) error(1,"Bailout");
	(aa->p0[0]).x = xx;
	(aa->p0[0]).y = yy;
	aa->type = 'f';
	aa->status = 1;
	aa->mem = (objP *)malloc((unsigned)(n*sizeof(objP)));
	if(aa->mem==0) error(1,"Bailout");
	for(i=0;i<aa->size;i++) aa->mem[i] = bobj[i];
	ex_one(aa);
	return(aa);
}
/*	own index() for portability with Unix V	*/
char * pindex(s,c)
	char * s;
	char c;
{
	register char * p;
	for(p=s; *p; p++) if(*p==c) break;
	if(*p) return(p);
	else return((char *)0);
}
