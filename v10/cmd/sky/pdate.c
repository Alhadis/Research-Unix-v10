#include "sky.h"

int	timezone =	5;
int	daylight =	1;

int	dmo[12]
= {
	0,31,59,90,120,151,181,212,243,273,304,334
};
double
dsrc(d, ifa, i)
double d;
double ifa[];
{
	double y;

	do {
		ifa[i] += 1.;
		y = convdate(ifa);
		ddd(d, y, ifa, '>');
	} while(d >= y);
	do {
		ifa[i] -= 1.;
		y = convdate(ifa);
		ddd(d, y, ifa, '<');
	} while(d < y);
	return(d - y);
}

ddd(d, y, ifa, ch)
double d, y, ifa[];
{

}

dtsetup(d, ifa)
double d;
double ifa[];
{

	if(flags & KITCH) {
		d -= timezone/24.;
		bdtsetup(d, ifa);
		if(lastsun(ifa, 4))
			return;
		if(!lastsun(ifa, 10))
			return;
		d += 1./24.;
	}
	bdtsetup(d, ifa);
}

lastsun(ifa, m)
double ifa[];
{

	if(ifa[1] < m)
		return(1);
	if(ifa[1] > m)
		return(0);
	if(ifa[2] < 24)
		return(1);
	return(0); /* fix up later */
}

bdtsetup(d, ifa)
double ifa[];
double d;
{
	double t;

	ifa[0] = floor(1900. + d/365.2425);
	ifa[1] = 1.;
	ifa[2] = 1.;
	ifa[3] = 0.;
	ifa[4] = 0.;
	ifa[1] = floor(1. + dsrc(d, ifa, 0)/30.);
	ifa[2] = floor(1. + dsrc(d, ifa, 1));
	dsrc(d, ifa, 2);

	d -= .5;
	t = (d - floor(d))*24.;
	ifa[3] = floor(t);
	ifa[4] = (t-ifa[3]) * 60.;
}

pdate(d)
double d;
{
	double ifa[5];

	dtsetup(d, ifa);
	printf("%.0f %.0f %.0f", ifa[0], ifa[1], ifa[2]);
}

ptime(d)
double d;
{
	double ifa[5];

	dtsetup(d, ifa);
	printf("%.0f:%.2f", ifa[3], ifa[4]);
}

prlat()
{
/*
	printf("%.4f N.  %.4f W.  %.0f m. above MSL\n", nlat, wlong, elev);
*/
}
