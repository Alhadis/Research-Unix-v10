/*	Plotting Routines adapted for tped	*/
/*	this routine has to be different. however parts could	*/
/*	be made common in hped and tped				*/

#include "hped.h"
#include "device.h"

#include <math.h>

#define isbreak(A) (A->t<0)

typedef Point * pointP;

extern int compact;

/* plot all objects */

draw_all()
{
	register i;
	for(i=0;i<no_obj;i++) if(obj[i]) plot_one(obj[i]);
	return(0);
}

char * find_sep(s)
	char * s;
{
	register char * p;
	for(p=s; *p; p++) if(*p==SEPAR) break;
	if(*p) return(p);
	else return((char *)0);
}

/* plot one obj */

plot_one(a) objP a;
{
	register  i;
	objP b;
	char obfont[8];

	if(!a) return(-1);
	if(!a->status) return(0);
#ifdef RTKP
	Color = a->pen;	if(!Color) Color = 7;
	setp(FORE,Color);
#endif

	/* fprintf(stderr,"object type: %c ps=%d\n",a->type,a->ps); */
	/* possible bug */
	if(!fullob(a->color) && !(no_line(a)) ) 
	if(a->size=='a' || a->size=='b' || a->size=='c')
		plot_arrow(a,(char)a->size);

	switch(a->type){
	case 'f':
		for(i=0;i<a->size;i++) if(a->mem[i]) plot_one(a->mem[i]);
		break;
	case 'p':
	case 'P':
		if(a->n>1){
			if(a->color==HEAVY || a->color==DOUBLE) {
				heavy(a,&b);
				a = b;
				if(a->type=='Q') goto filplot;
			}
			else if(a->color=='i') {
				ispline(a,&b);
				a = b;
				goto splplot;
			}
		}
#ifdef TROFF
		if(compact) pl_pts(a);
		else
#endif
		pl_poly(a,a->p0,a->p0 + a->n-1);
		break;
	case 's':
	case 'S':
		splplot:;
#ifdef TROFF
		if(compact) pl_pts(a);
		else
#endif
		if(fullob(a->color))  proc_fill(a);
		else spline(a);
		break;
	case 'c':
		/* temporary cludge! only solid circles */
		if(fullob(a->color)) fcirc(*(a->p0),a->size,'E');
		else pcirc(*(a->p0),a->size);
		break;
	case 'Q':
		filplot:;
#ifdef TROFF
		if(compact) pl_pts(a);
		else
#endif
		proc_fill(a);
		break;
	case 't':
		if(a->color=='W') strcpy(obfont,"CW");
		else {
			obfont[0] = a->color;
			obfont[1] = 0;
		}
		pstring(*(a->p0),a->txt,(short)a->size,obfont);
		break;
	}
	return(0);
}

/*	find breaks and send compact descriptions to troff	*/
#ifdef TROFF
pl_pts(aa)
	objP aa;
{
	register pointP pa, pb;
	register pointP pp, opp;
	short kl;
	pa = aa->p0;
	pb = aa->p0 + aa->n;
	if(pa>=pb) return(-1);
	kl = (aa->type>='P' && aa->type<='S') ? 1: 0;
	set_pen(aa->color,(short)aa->size,aa->ps);
	for(opp=pp=pa;pp<pb;pp++) {
		if(isbreak(pp)) {
			c_points(opp,pp,kl,aa->type);
			opp = pp + 1;
		}
	}
	c_points(opp,pp,kl,aa->type);
	fl_pen(aa->color);
}
#endif

/* -- plot polygon -- */

pl_poly(aa,pa,pb) objP aa; pointP pa, pb;
{
	register pointP pp, qq, opp, pp1;
	register j, k,  klisto;
	short mv_first, mv_last;
	/* klisto = 1 when polygon is closed */

	if(aa == 0) return(-1);
	klisto = klosed(aa->type);
	if(fullob(aa->type)) klisto = 1;
	pp1 = aa->p0 + aa->n - 1;

	if(pa < aa->p0) {
		pa = aa->p0;
		mv_first = 1;
	}
	else if(isbreak(pa)) { pa++; mv_first = 1; }
		else mv_first = 0;
	if(pb > pp1) {
		pb = pp1;
		mv_last = 1;
	}
	else if(isbreak(pb)) { pb--; mv_last = 1; }
		else mv_last = 0;

	if(mv_last) {
		pdline(aa->color,*pa,*pb);
		if(klisto){
			for(qq=pa-1;qq>=aa->p0;qq--){
				if(isbreak(qq)) break;
			}
			qq++;
			pdline(aa->color,*pb,*qq);
		}
	}
	else if(mv_first) {
		pdline(aa->color,*pa,*pb);
		if(klisto) {
			for(qq=pb+1;qq<=pp1;qq++){
				if(isbreak(qq)) break;
			}
			qq--;
			pdline(aa->color,*pa,*qq);
		}
	}
	else { 	/* mv_last = mv_first = 0 */
		k = pb - pa + 1;
		qq = pa;
		pp = qq + 1;
		opp = qq;
		if(pa != aa->p0 || pb != pp1) klisto = 0;
		for(j=1;j<k;j++,pp++,opp++) {
			if(isbreak(pp)) {	/* break in contour */
				if(klisto) {
					pdline(aa->color,*qq,*opp);
					qq = pp + 1;
				}
				j++; pp++, opp++;
			}
			else pdline(aa->color,*opp,*pp);
		}
		if(klisto) {
			pdline(aa->color,*opp,*qq);
		}
	}
	return(0);
}

/* spline plotting routine */

/* draw lines around double points */
#ifdef TROFF
#define DSTEP 15
ssline(p1,p2,k)
	Point p1, p2;
	short k;
{
	Point pm;
	double llen, dx, dy;
	register i, j;

	pm.x = (p1.x+p2.x)/2;
	pm.y = (p1.y+p2.y)/2;
	llen = (p2.x-pm.x)*(p2.x-pm.x) + (p2.y-pm.y)*(p2.y-pm.y);
	llen = sqrt(llen);
	if(llen<1) llen = 1;
	dx = (p2.x-pm.x)/llen;
	dy = (p2.y-pm.y)/llen;
	if(k==DASH) {
		for(i=0; i<llen;){
			for(j=0; j<DSTEP && i<llen; j++, i++){
				pline(pm,pm);
				pm.x += dx;
				pm.y += dy;
			}
			i + = DSTEP;
			pm.x += DSTEP*dx;
			pm.y += DSTEP*dy;
		}
	}
	else {
		for(i=0;i<llen;i++){
			pline(pm,pm);
			pm.x += dx;
			pm.y += dy;
		}
	}
}
#else
ssline(p1,p2,k)
	Point p1, p2;
	short k;
{
	Point pm;
	pm.x = (p1.x+p2.x)/2;
	pm.y = (p1.y+p2.y)/2;
	pdline((char)k,pm,p2);
}
#endif

#ifdef TROFF
extern double copxx, copyy;

#endif

spline(aa) objP aa;
{
	register pointP pa, pb;
	register pointP pp, opp;
	short kl;
	if(aa->type != 's' && aa->type != 'S') return(-1);
	pa = aa->p0;
	pb = aa->p0 + aa->n;
	if(pa>=pb) return(-1);
#ifdef TROFF
#define CHSIZE(A) { printf("\n.ps%s\n.sp -1\n",A); \
	pmove(copxx,copyy); }
	if(!compact) CHSIZE(" 2")
#endif
	kl = aa->type=='S'? 1: 0;
	for(opp=pp=pa;pp<pb;pp++) {
		if(isbreak(pp)) {
			dspline(opp,pp,kl,aa->color);
			opp = pp + 1;
		}
	}
	dspline(opp,pp,kl,aa->color);
#ifdef TROFF
	if(!compact) CHSIZE("")
#endif
}

dspline(pa,pb,kk,cc)	/*	[pa,pb)	*/
	pointP pa, pb;
	char cc;
{
	register pointP pp;
	Point p1, p2, p3;
	double midp;
	double X1, X2, X3, Y1, Y2, Y3;
	double Xm, Ym, Xz, Yz;
	double total;

		
	if(kk) p2 = *(pb-1);
	else p2 = *pa;
	p3 = *pa;
	pp = pa;
	if(!pa->t && !kk) { pp++; p3 = *pp; }
	for(; pp<pb; pp++){
		p1 = p2;
		p2 = p3;
		if(pp->t>0) ssline(p1,p2,cc);
		if(pp != pb-1) p3 = *(pp+1);
		else {
			if(kk) p3 = *pa;
			else break;	/* nothing more to plot */
		}
		if(pp->t>0) ssline(p3,p2,cc);
		else {
			X1 = 0.5*(p3.x + p1.x) - p2.x;
			X2 = p2.x - p1.x;
			X3 = 0.5*(p2.x + p1.x);

			Y1 = 0.5*(p3.y + p1.y) - p2.y;
			Y2 = p2.y - p1.y;
			Y3 = 0.5*(p2.y + p1.y);

			Xm = .25*(p3.x + p1.x) + .75*p2.x;
			Ym = .25*(p3.y + p1.y) + .75*p2.y;
			Xz = 0.5*(p2.x + p3.x);
			Yz = 0.5*(p2.y + p3.y);

			midp = (X3-Xm)*(X3-Xm) + (Y3-Ym)*(Y3-Ym);
			midp = sqrt(midp);
			total =  (Xz-Xm)*(Xz-Xm) + (Yz-Ym)*(Yz-Ym);
			total = sqrt(total);
			total += midp;
			total /= 2;
			if(cc==DASH) dspl_arc(X1,Y1,X2,Y2,X3,Y3,total);
			else spl_arc(X1,Y1,X2,Y2,X3,Y3,total);
		}
	}
	return(0);
}

spl_arc(X1,Y1,X2,Y2,X3,Y3,total)
	double X1,Y1,X2,Y2,X3,Y3, total;
{
	long n, n2;
	short LEN = 2;
	Point Ps, Pe;
	register u;

	n = total + 0.5;
	if(n<LEN) n = LEN;
	n2 = n*n;
	Pe.x = X3;		Pe.y = Y3;
	for(u=0;u<=n;u++){
		Ps = Pe;
		Pe.x = ( (X1*u + X2*n)*u ) / n2 + X3;
		Pe.y = ( (Y1*u + Y2*n)*u ) / n2 + Y3;
#ifdef RTKP
		pline(Ps,Pe);
#else
		if(Ps.x != Pe.x || Ps.y != Pe.y) pline(Ps,Ps);
#endif
	}
}

short nxtu(X1,Y1,X2,Y2,n,oldu)
	double X1,Y1,X2,Y2, n;
	short oldu;
{
	double xx, yy, rr;
	short ds;
	xx = (2*X1*oldu)/n + X2;
	yy = (2*Y1*oldu)/n + Y2;
	if(xx<0) xx = -xx;
	if(yy<0) yy = -yy;
	if(!xx && !yy) return(oldu+2);
	rr = sqrt(xx*xx + yy*yy)/15.;
#ifdef CAUTION
	fprintf(stderr,"d=%g ",rr);
#endif
	if(rr<1) rr = 1;
	ds = n/rr;	/*assumes dash length one*/
	if(ds<2) ds = 2;
	return(oldu+ds);
}

dspl_arc(X1,Y1,X2,Y2,X3,Y3,total)
	double X1,Y1,X2,Y2,X3,Y3, total;
{
	long n, n2;
	short LEN = 2;
	Point Ps, Pe;
	register u;
	short u0, u1;

	n = total + 0.5;
	if(n<LEN) n = LEN;
#ifdef CAUTION
	fprintf(stderr,"n=%ld\n",n);
#endif
	n2 = n*n;
	Pe.x = X3;		Pe.y = Y3;
	u0 = 0;
	while(u0<=n){
		u1 = nxtu(X1,Y1,X2,Y2,total,u0);	/* to draw */
		if(u1>n) u1 = n;
#ifdef CAUTION
		fprintf(stderr,"(%d,%d)",u0,u1);
#endif
		for(u=u0;u<=u1;u++){
			Ps = Pe;
			Pe.x = ( (X1*u + X2*n)*u ) / n2 + X3;
			Pe.y = ( (Y1*u + Y2*n)*u ) / n2 + Y3;
#ifdef RTKP
			if(u>u0) pline(Ps,Pe);
#else
			if(u>u0 && (Ps.x != Pe.x || Ps.y != Pe.y) ) pline(Ps,Ps);
#endif
		}
		u0 = nxtu(X1,Y1,X2,Y2,total,u1);	/* to skip */
#ifdef CAUTION
		fprintf(stderr,"\n");
#endif
	}
}


/* -- dash line plotter -- */

double lng(p,q)
	Point p, q;
{
	double xx, yy, rr;
	xx = p.x - q.x;
	yy = p.y - q.y;
	rr = sqrt(xx*xx + yy*yy);
#ifdef CAUTION
	fprintf(stderr,"dx=%g dy=%g rr=%g\n",xx,yy,rr);
#endif
	return(rr);
}

dline(p1,p2,dd)
	Point p1, p2;
	float dd;
{
	Point pa, pb;
	long nst;
	double llen, ldx, ldy;
	double xa, ya, xb, yb;
	short slen;
	register i;

	llen = lng(p1,p2);
	nst = llen/20. + 0.5;
#ifdef CAUTION
	fprintf(stderr,"SL llen=%g dd=%g n=%ld\n",llen,dd,nst);
#endif
	if(!(nst%2)) nst++;
	if(nst<2) {
		uline(p1,p2);
		return;
	}
	ldx = (p2.x-p1.x)/nst;
	ldy = (p2.y-p1.y)/nst;
	pa = p1;
	pb.x = pa.x + ldx/2;
	pb.y = pa.y + ldy/2;
	for(i=0; i<nst; i++){
		pline(pa,pb);
		pa.x += ldx;	pa.y += ldy;
		pb.x += ldx;	pb.y += ldy;
	}
	pline(pa,p2);

}

/*	arrow plotter		*/

#define muldiv(A,B,C) (A/C)*B

plot_arrow(a,c)
	objP a; char c;
{
	double dx, dy, v, u, ll;
	double x1, y1, x2, y2;
	double f1, f2, f3, f4;
	Point Ps, Pe;

	if(a->n<=1 || c==' ') return(-1);
	arrow2:;
	if(c=='a' || c=='c')	{
		x1 = (a->p0)->x;	y1 = (a->p0)->y;
		x2 = ((a->p0)+1)->x;	y2 = ((a->p0)+1)->y;
	}
	else {
		if(c=='b') {
			x1 = (a->p0[a->n-1]).x;	y1 = (a->p0[a->n-1]).y;
			x2 = (a->p0[a->n-2]).x;		y2 = (a->p0[a->n-2]).y;
		}
	}
	/* scale here */
	x1 = scale*x1 + xshft;
	y1 = scale*y1 + yshft;
	x2 = scale*x2 + xshft;
	y2 = scale*y2 + yshft;
	dx = x1 - x2;
	dy = y1 - y2;
	if(a->type=='S' || (a->type=='s' && *(a->txt)=='0') ) {
		x1 = (x1+x2)/2;
		y1 = (y1+y2)/2;
		dx /= 2;
		dy /= 2;
	}
	ll = sqrt(dx*dx+dy*dy);
	if(ll<4) return(0);		/* too small */
	u = ll/4; if(u>7) u = 7;
	v = 2*u;
	f1 = muldiv(u,dy,ll);
	f2 = muldiv(v,dx,ll);
	f3 = muldiv(u,dx,ll);
	f4 = muldiv(v,dy,ll);
	Ps.x = x1;
	Ps.y = y1;
	Pe.x = x1 + f1 - f2;
	Pe.y = y1 - f3 - f4;
	uline(Ps,Pe);
	Pe.x = x1 - f1 - f2;
	Pe.y = y1 + f3 - f4;
	uline(Ps,Pe);
	if(c=='c') {
		c = 'b';
		goto arrow2;
	}
	return(0);
}

no_line(c) char c;
{
	if(c=='c' || c=='t' || c=='f') return(1);
	else return(0);
}

klosed(c) char c;
{
	if(c>='P' && c<='S') return(1);
	else return(0);
}

fullob(c) char c;
{
	if(c=='Q') return(1);
	if(c>='E' && c<='P') return(1);
	else return(0);
}


pdline(c,p1,p2)
	char c;
	Point p1, p2;
{

	if(c==DASH) {
		p1.x = scale*p1.x + xshft;
		p1.y = scale*p1.y + yshft;
		p2.x = scale*p2.x + xshft;
		p2.y = scale*p2.y + yshft;
		dline(p1,p2,15.);
	}
	else pline(p1,p2);
}

/* Filling circles on the host */

fcirc(cp,rr,c)
	Point cp;
	double rr;
	char c;
{
	double xl, xr, xx;
	register y;
	short y1, y2;
	double  yy;

	rr *= scale;
	cp.x = scale*cp.x + xshft;
	cp.y = scale*cp.y + yshft;
	y1 = cp.y - rr + 1.5;
	y2 = cp.y + rr + 0.5;
	rr = rr*rr;
	for(y=y1;y<y2;y++){
		yy = y - cp.y;
		xx = sqrt(rr - yy*yy);
		xl = cp.x - xx;
		xr = cp.x + xx;
		paint(xl,xr,y,c);
	}
}
