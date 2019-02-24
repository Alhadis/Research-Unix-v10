#include "map.h"
#include <stdio.h>

/* development of a loxodrome from a unit sphere onto a plane
   the loxodrome passes through 0,0 on the sphere,
   inclined a (alpha) from the equator

   latitude p (phi) positive north
   longitude t (theta) positive east - opposite to "map"
   all angles in radians

   x,y are coords of developed curve
   c (chi) is angle of inclination of developed curve from x axis

   dx/dp = cos(c)/sin(a)
   dy/dp = sin(c)/sin(a)
   dc/dp = tan(p)/tan(a)
   dt/dp = 1/(cos(p)*tan(a))

   cc lox.c -lport -lF77 -lI77
   a.out alpha		here alpha is in degrees
*/
extern char *realloc();
static double renorm();
static bsearch(), getxy(), calcf(), solve();
#define DEG (180/PI)
#define MAX 85

/* curlicue is an ordered sequence of points
along the midline of the spiral, in both geographic
and developed coordinates */
struct curl {
	double phi,x,y,theta,chi;
} *curlicue;	/* the midline */
int nc;		/* number of points on the midline */

struct coord a;	/* alpha, angle from equator */
static signa = 1;
static double z[4];		/* x==z[0],y==z[1],t=z[2],c==z[3] */

/* inputs to dodes, port library integration routine */

static int n = 4;
static double z0[4];		/* initial values = 0,0,0,a */
static double p0;		/* initial value of p = 0 */
static double p1 = 86*PI/180;	/* final value, 89.99+ degrees */
static double dt;		/* current time step */
static double deg = PI/180;	/* 1 degree */
static float errpar[2] = {1.e-7,1.e-7};

static
f(p,z,n,r)
double *p, *z, *r;
int *n;
{
	r[0] = cos(z[3])/a.s;
	r[1] = sin(z[3])/a.s;
	r[2] = a.c/(cos(*p)*a.s);
	r[3] = sin(*p)*r[2];
}
static
handle(t0,z0,t1,z1,n,dt,ts,e)
double *t0,*z0,*t1,*z1,*dt,*ts;
float *e;
int *n;
{
	if(*dt > deg) *dt = deg;
	if(fabs(z1[3]-z0[3])>.1 && *dt >.00001)
		*dt *= .1/fabs(z1[3]-z0[3]);
	if(*t1 + *dt >= PI/2)
		*dt = (PI/2 - *t1)/2;
	if(*t1 + *dt > *ts)
		*dt = *ts - *t1 + .00001;
	curlicue = (struct curl*)realloc((char*)curlicue,
			(nc+1)*sizeof(*curlicue));
	curlicue[nc].phi = *t1;
	curlicue[nc].x = z1[0];
	curlicue[nc].y = z1[1];
	curlicue[nc].theta = z1[2];
	curlicue[nc].chi = z1[3];
	nc++;
	/*printf("%f %f %f %f %f\n",*t1*DEG,
		z1[0]*DEG,z1[1]*DEG,z1[2]*DEG,z1[3]*DEG);
	fflush(stdout);*/
	if(*t0==*t1) abort();
}
static
struct place negplace(place)
struct place *place;
{
	struct place iplace;
	iplace = *place;
	iplace.nlat.l = -place->nlat.l;
	iplace.nlat.s = -place->nlat.s;
	iplace.wlon.l = -place->wlon.l;
	iplace.wlon.s = -place->wlon.s;
	return iplace;
}
static
Xloxodromic(place, x, y)
struct place *place;
float *x, *y;
{
	double u1 = place->nlat.l;
	double v1 = -place->wlon.l;
	double d,p,t;
	if(fabs(u1) > MAX*RAD) return 0;
	if(u1 < 0) {
		struct place iplace;
		int ret;
		iplace = negplace(place);
		ret = Xloxodromic(&iplace,x,y);
		*x = -*x;
		*y = -*y;
		return ret;
	}
	v1 = renorm(u1,bsearch(u1,0),v1);
	solve(u1,v1,&d,&p,&t);
	/*printf("%f %f %f %f %f %f\n",
		u,v,v1*DEG,d*DEG,p*DEG,t*DEG);
	fflush(stdout);*/
	getxy(d,p,x,y);
	/*printf("%f %f\n",x*DEG,y*DEG);
	fflush(stdout);*/
	return 1;
}
int (*loxodromic(par))()
float par;
{
	deg2rad(par, &a);
	if(par < 0)
		signa = -1;
	z0[3] = a.l;
	dt = deg;
	handle(&deg,z0,&p0,z0,&n,&deg,&p1,errpar);
	dodes_(f,z0,&n,&p0,&p1,&dt,errpar,handle);
	return Xloxodromic;
}

/* find the index for latitude (i=0) or longitude (i=1)
in array curlicue.  In case of longitude, u must have
been normalized by renorm() */
static
bsearch(u,i)
double u;
{
	int top = nc;
	int bot = 0;
	int mid;
	while(bot<top-1) {
		mid = (bot+top)/2;
		if(i==0&&curlicue[mid].phi<=u||
		   i==1&&(signa>0&&curlicue[mid].theta<=u||
		   	  signa<0&&curlicue[mid].theta>=u))
			bot = mid;
		else
			top = mid;
	}
	return bot;
}
/* wind v thru multiples of 2*PI until it lies within PI
of the (linearly interpolated) midline */
static double
renorm(u,n,v)
double u,v;
{
	double t,fract;
	fract = (u-curlicue[n].phi)/
		(curlicue[n+1].phi-curlicue[n].phi);
	t = curlicue[n].theta*(1-fract) +
		curlicue[n+1].theta*fract;
	if(signa > 0) while(v<t-PI)
		v += 2*PI;
	else while(v>t+PI)
		v -= 2*PI;
	return v;
}
/* given the height (d) and foot (p,t) of the perpendicular projection
of a point onto the midline, return the x-y coordinates of the point*/
static
getxy(d,p,xp,yp)
float d,p,*xp,*yp;
{
	int n, sign = 1;
	double fract, x0, y0, c0;
	if(p < 0) {
		p = -p;
		sign = -1;
	}
	n = bsearch(p,0);
	fract = (p-curlicue[n].phi)/
		(curlicue[n+1].phi-curlicue[n].phi);
	x0 = sign*(curlicue[n].x*(1-fract) +
		curlicue[n+1].x*fract);
	y0 = sign*(curlicue[n].y*(1-fract) +
		curlicue[n+1].y*fract);
	c0 = curlicue[n].chi*(1-fract) +
		curlicue[n+1].chi*fract;
	*xp = x0 - d*sin(c0);
	*yp = y0 + d*cos(c0);
}	

/*
	solve simultaneously for d,p,t (dist to midline,
					phi lat on midline,
					theta lon on midline)
	given a, u, v (rhumb angle from parallels, lat, east lon)

	sin d sin a = cos u sin(t-v)		law of sines
	sin u = cos d sin p + sin d cos p cos a	law of cosines
	t tan a = log tan (p/2 + PI/4)		eqn of rhumb line

	initial guess: d=u-p, t=v, p from table lookup in curlicue
*/
static double su,cu,v0;
static int n3 = 3, jmax = 50;
static double acc = 1e-6;
static
calcf(n,x,nf,g)
int *n, *nf;
double *x,*g;
{
	double f[3], sd;
	if(x[1]>=PI/2) {
		*nf = 0;
		return;
	}
	sd = sin(x[0]);
	f[0] = sd*a.s-cu*sin(x[2]-v0);
	f[1] = su - cos(x[0])*sin(x[1]) - sd*cos(x[1])*a.c;
	f[2] = x[2]*a.s/a.c - log(tan(x[1]/2+PI/4));
	/*printf("calcf: d p t %g %f %f",
		x[0]*DEG,x[1]*DEG,x[2]*DEG);
	printf(" resid %f %f %f\n",f[0],f[1],f[2]);
	fflush(stdout);*/
	*g = f[0]*f[0]+f[1]*f[1]+f[2]*f[2];
}
/* given renormed lat-lon (u,v) find the height (*dp) and foot
(*pp,*tp) of a perpendicular to the midline. precondition: u>=0 */
static
solve(u,v,dp,pp,tp)
double u,v,*dp,*pp,*tp;
{
	double z[3];	/* d, p, t */
	int n;
	if(u < 0) {
		abort();
		solve(-u,-v,dp,pp,tp);
		*dp = -*dp;
		*pp = -*pp;
		*tp = -*tp;
		return;
	}
	n = bsearch(signa*fabs(v),1);
	su = sin(u);
	cu = cos(u);
	z[1] = curlicue[n].phi;	/* initial guess */
	if(signa*v < 0) z[1] = -z[1];
	z[0] = u - z[1];
	z[2] = v0 = v;
	/*printf("inputs (deg) %f %f\n",u*DEG,v*DEG);
	printf("interval %d guess (deg) %f %f %f\n",n,
		z[0]*DEG,z[1]*DEG,z[2]*DEG);
	fflush(stdout);*/
	dsmnf_(&n3,z,calcf,&jmax,&acc);
	*dp = z[0];
	*pp = z[1];
	*tp = z[2];
}
loxcut(g,og,cutlon)
struct place *g, *og;

{
	double glon, oglon;
	double sign = g->nlat.l>=0? 1: -1;
	double nlat = sign*g->nlat.l;
	if(nlat > MAX*RAD)
		return 0;
	glon = sign*renorm(nlat, bsearch(nlat,0),
				-sign*g->wlon.l);
	sign = og->nlat.l>=0? 1: -1;
	nlat = sign*og->nlat.l;
	if(nlat > MAX*RAD)
		return 0;
	oglon = sign*renorm(nlat, bsearch(nlat,0),
				-sign*og->wlon.l);
	return fabs(glon-oglon) < PI;
}
