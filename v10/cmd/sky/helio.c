#include "sky.h"

helio()
{
/*
 *	uses lambda, beta, rad, motion
 *	uses xms, yms, zms, xdot, ydot, zdot
 *	uses mag, semi, psi, tobliq
 *	sets alpha, delta
 *	sets geolam, geobet, georad
 */

/*
 *	helio converts from ecliptic heliocentric coordinates
 *	referred to the mean equinox of date
 *	to equatorial geocentric coordinates referred to
 *	the true equator and equinox
 */

	double xmp, ymp, zmp;
	double radtemp;

/*
 *	compute geocentric distance of object and
 *	compute light-time correction (i.i. planetary aberration)
 */

	xmp = rad*cos(beta)*cos(lambda);
	ymp = rad*cos(beta)*sin(lambda);
	zmp = rad*sin(beta);
	georad = sqrt((xmp+xms)*(xmp+xms) +
		(ymp+yms)*(ymp+yms) +
		(zmp+zms)*(zmp+zms));
	geolam = lambda - .0057756e0*georad*ldot;
	geobet = beta - .0057756*georad*bdot;
	radtemp = georad;
	georad = rad - .0057756*georad*rdot;

	xmp = georad*cos(geobet)*cos(geolam);
	ymp = georad*cos(geobet)*sin(geolam);
	zmp = georad*sin(geobet);

/*
 *	compute annual parallax from the position of the sun
 */

	xmp += xms;
	ymp += yms;
	zmp += zms;
	georad = sqrt(xmp*xmp + ymp*ymp + zmp*zmp);

/*
 *	compute annual (i.e. stellar) aberration
 *	from the orbital velocity of the earth
 *	(by an incorrect method)
 */

	xmp -= xdot*georad;
	ymp -= ydot*georad;
	zmp -= zdot*georad;

/*
 *	perform the nutation and so convert from the mean
 *	equator and equinox to the true
 */

	geolam = atan2(ymp, xmp);
	geobet = atan2(zmp, sqrt(xmp*xmp+ymp*ymp));
	geolam += psi;

/*
 *	change to equatorial coordinates
 */

	xmp = georad*cos(geolam)*cos(geobet);
	ymp = georad*(sin(geolam)*cos(geobet)*cos(tobliq) - sin(tobliq)*sin(geobet));
	zmp = georad*(sin(geolam)*cos(geobet)*sin(tobliq) + cos(tobliq)*sin(geobet));

	alpha = atan2(ymp, xmp);
	delta = atan2(zmp, sqrt(xmp*xmp+ymp*ymp));

	hp = 8.794e0*radsec/georad;
	semi /= georad;
	georad = radtemp;
	if(rad > 0. && rad < 2.e5)
		mag += 2.17*log(rad*georad);
}
