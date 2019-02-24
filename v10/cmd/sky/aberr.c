#include "sky.h"

aberr()
{

	double lsun, ljup, lsat;
	double msun, mven, mjup, msat;
	double dmoon;
	double c,d,k,e;

/*
	To find the aberration, it is necessary to compute
	the velocity of the observer in an inertial frame.

	This program does the computation for the center
	of the earth w/r the center of mass of the Solar System
	and includes enough terms to limit the error to
	about 0".003.

	uses capt, eday
	sets xdot, ydot, zdot
*/

	lsun = 279.696678 + 0.9856473354*eday + .000300*capt2;
	ljup = 237.942344 + .083129433*eday;
	lsat = 266.56527 + 0.03351336*eday;

	msun = 358.475845 + .9856002670*eday - .000150*capt2;
	mven = 212.603219 + 1.602130154*eday;
	mjup = 225.22165 + .083085369*eday;
	msat = 175.47630 + 0.03345972*eday;
	dmoon = 350.737681+12.1907491914*eday-.001436*capt2;

	lsun = lsun * radian;
	ljup = ljup*radian;
	lsat = lsat*radian;
	msun = msun*radian;
	mven = mven*radian;
	mjup = mjup*radian;
	msat = msat*radian;
	dmoon = fmod(dmoon,360.)*radian;

/*
	Discover the velocity of the Earth around the Sun.
*/

	xdot =
	 -20.4916*sin(lsun)
	 - 0.3433*sin(lsun+msun)
	 + 0.0009*capt*sin(lsun+msun)
	 - 0.0065*sin(lsun+2.*msun)
	 - 0.0007*sin(4.159+2.*mven-2.*msun+lsun)
	 - 0.0007*sin(4.7042+msun-mjup+lsun);

	ydot =
	 20.4901*cos(lsun)
	 + 0.3433*cos(lsun+msun)
	 - 0.0009*capt*cos(lsun+msun)
	 + 0.0065*cos(lsun+2.*msun)
	 + 0.0007*cos(4.159+2.*mven-2.*msun+lsun)
	 + 0.0007*cos(4.7042+msun-mjup+lsun);

/*
	Discover the velocity of the Earth around the center
	of the Earth-Moon system.
*/

	xdot = xdot
	- 0.0079*sin(lsun+dmoon);

	ydot = ydot
	 + 0.0079*cos(lsun+dmoon);

/*
 *	Discover the velocity of the sun about the center of mass.
 */

	xdot = xdot
	 - 0.0086*sin(ljup)
	 - 0.0004*sin(ljup+mjup)
	 - 0.0019*sin(lsat)
	 - 0.0001*sin(lsat+msat);

	ydot = ydot
	 + 0.0086*cos(ljup)
	 + 0.0004*cos(ljup+mjup)
	 + 0.0019*cos(lsat)
	 + 0.0001*cos(lsat+msat);

/*
 *	Compute aberrational day numbers just for fun.
 */

	k = 20.496;
	e = .01675104 - 4.180e-5*capt - 1.26e-7*capt2;
	c = -ydot*cos(obliq) - k*e*cos(lsun-msun+pi)*cos(obliq);
	d = xdot - k*e*sin(lsun-msun+pi);

	if(flags&GOOBIE){
		printf("C: %.4f  D: %.4f\n", c, d);
	}

/*
	Convert to absolute velocity (v/c).
*/

	xdot = xdot * radsec;
	ydot = ydot * radsec;
	zdot = 0.;
}
