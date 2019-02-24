/*	double polygon (possibly open) from given	*/
/*	Ken Thompson's pgm, orininally for 202 fonts	*/

#include	"hped.h"
#include	<math.h>
#ifndef PI
#define PI 3.1415926536
#endif

#define	notest	1
#define	LARGE	1e5
#define	NPOINT	1500
double	xa[NPOINT];
double	ya[NPOINT];
int	npt;
int	snpt;
int	debug;
double	dist;
double	atx, aty, oatx, oaty;
double	crx, cry;
double dx, dy;

#define	EPS	1e-3

double	atof();
double	clean();

Point *dofor();
Point *doback();

short arhead;

double pyth(x1,y1,x2,y2)
	double x1, y1, x2, y2;
{
	return(sqrt((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2)));
}

fat(pol,typ,nn,arrow,pins,pout,ni,no,inside,outside,thick)
	Point *pol;
	char typ;
	short nn, arrow, *ni, *no;
	Point *pins, *pout;
	double inside, outside, thick;
{

	Point *pp;

	if(typ != 'p' && typ != 'P') return(0);

	inside /= scale; outside /= scale; thick /= scale;
	if(typ=='p') arhead = arrow;
	else arhead = 0;	/*for now*/
	if(make_xa(pol,nn,typ) < 0) return(-1);
	if(curve()) {
		dist = -inside;
		dotest();
		pp = dofor(pins);
		*ni = pp - pins;
		make_xa(pol,nn,typ);
		dist = outside;
		dotest();
		pp = doback(pout);
		*no = pp - pout;
	} else {
		dist = inside;
		dotest();
		pp = dofor(pins);
		*ni = pp - pins;
		make_xa(pol,nn,typ);
		dist = -outside;
		dotest();
		pp = doback(pout);
		*no = pp - pout;
	}
}

curve()
{
	register i;
	double x1, x2, x3;
	double y1, y2, y3;
	double a, b;
	double t1, t2;

	t1 = 0.0;
	t2 = 0.0;
	for(i=1; i<npt; i++) {
		x1 = xa[i-1];
		x2 = xa[i];
		x3 = xa[i+1];
		y1 = ya[i-1];
		y2 = ya[i];
		y3 = ya[i+1];
		a = atan2(y1-y2, x1-x2) - atan2(y3-y2, x3-x2);
		t1 += clean(a);
		t2 += clean(-a);
	}
	return(t1 > t2);
}


dotest()
{
	register i;

	if(notest)
		return;
loop:
	noshort();
	points(npt-1);
	oatx = atx;
	oaty = aty;
	for(i=1; i<npt; i++) {
		points(i);
		if(cross(i))
			goto badpoint;
		oatx = atx;
		oaty = aty;
	}
	return;

badpoint:
	if(i > 1) {
		oatx = xa[i-2];
		oaty = ya[i-2];
	} else {
		oatx = xa[npt-2];
		oaty = ya[npt-2];
	}
	atx = xa[i+1];
	aty = ya[i+1];
	cross(i);
	xa[i-1] = crx;
	ya[i-1] = cry;
	if(i == npt-1)
		i = 0;
	for(; i<npt; i++) {
		xa[i] = xa[i+1];
		ya[i] = ya[i+1];
	}
	npt--;
	goto loop;
}

Point * dofor(pa)
	Point * pa;
{
	register i;

	for(i=1; i<npt; i++) {
		points(i);
		if(i==1 && (arhead==1 || arhead==3) ){
			pa->t = 0;
			pa->x = atx + 2*dx; pa->y = aty + 2*dy; pa++;
		}
		pa->t = 0;
		pa->x = atx;	pa->y = aty;	pa++;
	}
	if(arhead==2 || arhead==3){
		pa->t = 0;
		pa->x = atx + 2*dx; pa->y = aty + 2*dy; pa++;
	}
	return(pa);
}

Point * doback(pa)
	Point * pa;
{
	register i;

	for(i=npt-1; i>0; i--) {
		points(i);
		if(i==(npt-1) && (arhead==2 || arhead==3)){
			pa->t = 0;
			pa->x = atx + 2*dx; pa->y = aty + 2*dy; pa++;
		}
		pa->t = 0;
		pa->x = atx;	pa->y = aty;	pa++;
	}
	if(arhead==1 || arhead==3){
		pa->t = 0;
		pa->x = atx + 2*dx; pa->y = aty + 2*dy; pa++;
	}
	return(pa);
}

cross(i)
{
	double a1, a2, b1, b2, c1, c2, d;

	a1 = atx - xa[i];
	a2 = oatx - xa[i-1];
	b1 = ya[i] - aty;
	b2 = ya[i-1] - oaty;

	d = a1*b2 - a2*b1;
	if(fabs(d) < 1e-9) {
		crx = atx;
		cry = aty;
		return(0);
	}
	c1 = ya[i]*a1 + xa[i]*b1;
	c2 = ya[i-1]*a2 + xa[i-1]*b2;
	crx = (a1*c2 - a2*c1)/d;
	cry = (c1*b2 - c2*b1)/d;
	if(b(i, atx, aty, crx, cry) && b(i-1, oatx, oaty, crx, cry))
		return(1);
	return(0);
}

b(i, ax, ay, x, y)
double x, y;
double ax, ay;
{

	if(b1(xa[i], ax, x) && b1(ya[i], ay, y))
		return(1);
	return(0);
}

b1(a, b, c)
double a, b;
double c;
{

	if(a > b) {
		if(c > a)
			return(0);
		if(c < b)
			return(0);
	} else {
		if(c < a)
			return(0);
		if(c > b)
			return(0);
	}
	return(1);
}

points(n)
{
	double x1, x2, x3;
	double y1, y2, y3;
	double a, b, g, h;

	x1 = xa[n-1];
	x2 = xa[n];
	x3 = xa[n+1];
	y1 = ya[n-1];
	y2 = ya[n];
	y3 = ya[n+1];

	a = atan2(y1-y2, x1-x2);
	b = atan2(y3-y2, x3-x2);
	g = sin((a-b)/2.);
	if(fabs(g) < EPS)
		if(g < 0)
			g = -EPS; else
			g = EPS;
	h = dist/g;
	g = (a+b)/2.;
	aty = y2 + (dy = h*sin(g));
	atx = x2 + (dx = h*cos(g));
#ifdef BUGGG
	fprintf(frecord,"%d: dx=%g, dy=%g\n",n,dx,dy);
#endif
}

double
clean(a)
double a;
{
	double circ;

	circ = 2.*PI;
	while(a < 0.)
		a += circ;
	while(a >= circ)
		a -= circ;
	return(a);
}

doline()
{
	register i;
	double d;

	d = dist;
	dist = dist/2.;
	for(i=npt-1; i>=0; i--) {
		xa[i+1] = xa[i];
		ya[i+1] = ya[i];
	}
	xa[0] = xa[1]-(xa[2]-xa[1]);
	ya[0] = ya[1]-(ya[2]-ya[1]);
	xa[npt+1] = xa[npt]+(xa[npt]-xa[npt-1]);
	ya[npt+1] = ya[npt]+(ya[npt]-ya[npt-1]);
	for(i=1; i<=npt; i++)
		points(i);
	dist = -dist;
	for(i=npt; i>=1; i--)
		points(i);
	dist = d;
	points(1);
	printf("1e6\n");
}

noshort()
{
	double x, y, l;
	register i;

loop:
	xa[npt-1] = xa[0];
	ya[npt-1] = ya[0];
	for(i=0; i<npt-1; i++) {
		x = xa[i] - xa[i+1];
		y = ya[i] - ya[i+1];
		l = sqrt(x*x + y*y);
		if(l < .2)
			goto delete;
	}
	return;

delete:
	for(; i<npt; i++) {
		xa[i] = xa[i+1];
		ya[i] = ya[i+1];
	}
	npt--;
	goto loop;
}

make_xa(poly,n,typp)
	Point * poly;
{
	register i, j, k;
	Point * pp;
	double ds, dm;

	npt = 1;
	for(i=0, pp=poly;  i<n;  i++, pp++){
		if(pp->t<0) return(-1);
		if(i) if(pp->x==(pp-1)->x && pp->y==(pp-1)->y) continue;
		xa[npt] = pp->x;
		ya[npt] = pp->y;
		npt++;
	}
	if(typp=='P'){
		xa[0] = xa[npt-1];
		ya[0] = ya[npt-1];
		xa[npt] = xa[1];
		ya[npt] = ya[1];
	}
	else {	/* this assures that we move along right angle */
		xa[0] = 2*xa[1] - xa[2];
		ya[0] = 2*ya[1] - ya[2];
		xa[npt] = 2*xa[npt-1] - xa[npt-2];
		ya[npt] = 2*ya[npt-1] - ya[npt-2];
	}
	if(arhead) {
		if(arhead==1 || arhead==3) {
			j = 1;	k = 2;
			dm = pyth(xa[j],ya[j],xa[k],ya[k]);
			if(dm<=0) return(-1);
			ds = 1. - (14./(scale*dm));
			if(ds < 0.6) ds = 0.6;
			xa[j] = ds*xa[j] + (1.-ds)*xa[k];
			ya[j] = ds*ya[j] + (1.-ds)*ya[k];
		}
		if(arhead==2 || arhead==3) {
			j = npt - 1;	k = npt - 2;
			dm = pyth(xa[j],ya[j],xa[k],ya[k]);
			if(dm<=0) return(-1);
			ds = 1. - (14./(scale*dm));
			if(ds < 0.6) ds = 0.6;
			xa[j] = ds*xa[j] + (1.-ds)*xa[k];
			ya[j] = ds*ya[j] + (1.-ds)*ya[k];
		}
	}
	return(0);
}

extern double sqrt();

heavy(a,bb)
	objP a,  *bb;
{
	register i;
	register Point * pp, * qq;
	objP b;
	Point pins[512], pout[512];
	short ni, no, arh;

	if(a->type != 'p' && a->type != 'P') return(0);
	if(a->n<2) return(0);
	for(i=0,pp=a->p0;i<a->n;i++,pp++) if(pp->t<0) return(-1);
	b = dupl_obj(a);
	b->size = ' ';	/*arrows are handled separatedly*/
	arh = 0;
	if(a->type=='p'){
		if(a->size=='a') arh = 1;
			else if(a->size=='b') arh = 2;
				else if(a->size=='c') arh = 3;
	}

	fat(a->p0,a->type,a->n,arh,pins,pout,&ni,&no,2.,2.,2.);
	Free(b->p0);
	b->n = ni + no;
#ifdef DANGER
	rrecord("fat: ar=%d n0 = %d -> %d + %d",arh,a->n,ni,no);
#endif
	b->p0 = (Point *)malloc((unsigned)((b->n+4)*sizeof(Point)));
	pp = b->p0;
	if(arh==1 || arh==3) {
		*pp++ = *(a->p0);
		b->n++;
	}
	for(i=0,qq=pins;i<ni;i++,pp++,qq++) *pp = *qq;
	if(a->type=='P') {
		b->n++;
		pp->t = -1;
		pp++;
	}
	else if(arh==2 || arh==3){
		*pp++ = *(a->p0 + a->n-1);
		b->n++;
	}
	for(qq=pout,i=0;i<no;i++,pp++,qq++) *pp = *qq;

	if(a->color==DOUBLE) { b->color = SOLID; b->type = 'P'; }
	else if(a->color==HEAVY) { b->color = 'E'; b->type = 'Q'; }

	*bb = b;	/* it should not matter where it is */

#ifdef DANGER
	rrecord("fat+: ni=%d no=%d b->n=%d",ni,no,b->n);
#endif
}
