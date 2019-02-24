#include "map.h"

/* Given two lat-lon pairs, find an orientation for the
   -o option of "map" that will place those two points
   on the equator of a standard projection, equally spaced
   about the prime meridian.   Both an erect and an inverted
   orientation are found.

   Option -t prints out a series of
   coordinates that follows the (great circle) track
   in the original coordinate system,
   followed by ".
   This data is just right for map -t.
*/
struct place pole;
struct coord twist;
int track;

extern void doroute(double, double, double, double, double);

void
dorot(double a, double b, double *x, double *y, void (*f)(struct place *))
{
	struct place g;
	deg2rad(a,&g.nlat);
	deg2rad(b,&g.wlon);
	(*f)(&g);
	*x = g.nlat.l/RAD;
	*y = g.wlon.l/RAD;
}

void
rotate(double a, double b, double *x, double *y)
{
	dorot(a,b,x,y,normalize);
}

void
rinvert(double a, double b, double *x, double *y)
{
	dorot(a,b,x,y,invert);
}

main(int argc, char **argv)
{
#pragma ref argv
	double an,aw,bn,bw;
	track = argc>1;
	for(;;){
		if(scanf("%lf%lf%lf%lf",&an,&aw,&bn,&bw)!=4)
			break;
		doroute(-90.,an,aw,bn,bw);
		if(!track)
			doroute(90.,an,aw,bn,bw);
	}
	return 0;
}

void
doroute(double dir, double an, double aw, double bn, double bw)
{
	double an1,aw1,bn1,bw1,pn,pw;
	double theta;
	double cn,cw,cn1,cw1;
	int i,n;
	orient(an,aw,0.);
	rotate(bn,bw,&bn1,&bw1);
/*	printf("b %f %f\n",bn1,bw1);*/
	orient(an,aw,bw1);
	rinvert(0.,dir,&pn,&pw);
/*	printf("p %f %f\n",pn,pw);*/
	orient(pn,pw,0.);
	rotate(an,aw,&an1,&aw1);
	rotate(bn,bw,&bn1,&bw1);
	theta = (aw1+bw1)/2;
/*	printf("a %f %f \n",an1,aw1);*/
	orient(pn,pw,theta);
	rotate(an,aw,&an1,&aw1);
	rotate(bn,bw,&bn1,&bw1);
	if(fabs(aw1-bw1)>180)
		if(theta<0.) theta+=180;
		else theta -= 180;
	orient(pn,pw,theta);
	rotate(an,aw,&an1,&aw1);
	rotate(bn,bw,&bn1,&bw1);
	if(!track) {
		printf("-o %.4f %.4f %.4f\n",pn,pw,theta);
		printf("A %.4f %.4f\n",an1,aw1);
		printf("B %.4f %.4f\n",bn1,bw1);
	} else {
		cn1 = 0;
		n = 1 + fabs(bw1-aw1)/.2;
		for(i=0;i<=n;i++) {
			cw1 = aw1 + i*(bw1-aw1)/n;
			rinvert(cn1,cw1,&cn,&cw);
			printf("%f %f\n",cn,cw);
		}
		printf("\"\n");
	}
}
