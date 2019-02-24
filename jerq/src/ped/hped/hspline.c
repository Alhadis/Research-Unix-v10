/*	(a) convert circle into spline	*/
/*	(b) find interpolating spline	*/

#include "hped.h"
#include <math.h>

cir_to_sp(a,bb)
	register objP a;
	objP *bb;
{
	register i;
	double sf = 0.414127 /*tan22.5*/, rf, xc, yc;
	Point pbuf[8];

	rf = sf*a->size;
	xc = (a->p0)->x;
	yc = (a->p0)->y;
	pbuf[0].x = pbuf[3].x = xc + rf;
	pbuf[1].x = pbuf[2].x = xc + a->size;
	pbuf[4].x = pbuf[7].x = xc - rf;
	pbuf[5].x = pbuf[6].x = xc - a->size;
	pbuf[0].y = pbuf[7].y = yc + a->size;
	pbuf[1].y = pbuf[6].y = yc + rf;
	pbuf[2].y = pbuf[5].y = yc - rf;
	pbuf[3].y = pbuf[4].y = yc - a->size;
	for(i=0;i<8;i++) pbuf[i].t = 0;
	*bb = mk_obj(pbuf,pbuf+8,'S',SOLID,' ',a->pen,' ',a->ps);
}

#define NN 1024

double qx[NN];
double qy[NN];
double px[NN];
double py[NN];

ispline(a, bb)
	register objP a;
	objP *bb;
{
	register i;
	register objP b;
	if(a->n<3 || a->n>NN) return(-1);
	if(a->type != 'P' && a->type != 'p') return(-1);
	b = dupl_obj(a);
	b->type += 3;	/*'s'-'p'*/
	b->color =  SOLID;
	b->txt = malloc((unsigned)(a->n+1));
	for(i=0;i<b->n;i++) b->txt[i] = '0';
	b->txt[b->n] = 0;
	for(i=0;i<b->n;i++) {
		qx[i] = b->p0[i].x;
		qy[i] = b->p0[i].y;
	}
	if(b->type=='s') {
		jsolve(b->n,qx,px);
		jsolve(b->n,qy,py);
	}
	else {
		isolve(b->n,qx,px);
		isolve(b->n,qy,py);
	}
	for(i=0;i<b->n;i++) {
		b->p0[i].x = px[i];
		b->p0[i].y = py[i];
	}
	*bb = b;
	return(0);
}

jsolve(k,q,p)
	double q[], p[];
{
	register i;
	static double a[NN];
	static double f[NN];
	static double b[NN];
	static double g[NN];

	f[0] = 2.*q[0];
	f[k-1] = 2.*q[k-1];
	for(i=1;i<k-1;i++) f[i] = 8*q[i];
	a[0] = a[k-1] = 1.;
	for(i=1;i<k-1;i++) a[i] = 6;

	b[0] = a[0];
	g[0] = f[0];
	for(i=1;i<k;i++) {
		b[i] = a[i] - 1./b[i-1];
		g[i] = f[i] - g[i-1]/b[i-1];
	}
	p[k-1] = g[k-1]/b[k-1];
	for(i=k-2;i>=0;i--) p[i] = (g[i] - p[i+1])/b[i];

}

/* iterative solution for closed splines */

#define F 1.333333

isolve(n,q,p)
	double q[], p[];
{
	register i;
	static double d[NN];
	double big;
#ifdef ITER
	register it=0;
#endif

	for(i=0;i<n;i++) p[i] = q[i];
	do{
#ifdef ITER
	it++;
#endif
		d[0] = F*q[0] - p[0] - (p[n-1] + p[1])/6;
		p[0] += d[0];
		for(i=1;i<n-1;i++) {
			d[i] = F*q[i] - p[i] - (p[i-1] + p[i+1])/6;
			p[i] += d[i];
		}
		d[n-1] = F*q[n-1] - p[n-1] - (p[n-2] + p[0])/6;
		p[n-1] += d[n-1];
		big = 0.;
		for(i=0;i<n-1;i++) if(fabs(d[i])>big) big = fabs(d[i]);
	} while(big>0.02);
#ifdef ITER
	printf("n=%d\titer=%d\n",n,it);
#endif
}
