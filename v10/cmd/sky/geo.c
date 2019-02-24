#include "sky.h"

geo()
{

/*
 *	uses alpha, delta, georad
 */

/*
 *	sets ra, decl, lha, decl2, az, el
 */

/*
 *	geo converts geocentric equatorial coordinates
 *	to topocentric equatorial and topocentric horizon
 *	coordinates.
 *	All are (usually) referred to the true equator.
 */

	double sel, saz, caz;
	double f;
	double sa, ca, sd;

/*
 *	convert to local hour angle and declination
 */

	lha = gst - alpha - glong;
	decl = delta;

/*
 *	compute diurnal parallax (requires geocentric latitude)
 */

	sa = cos(decl)*sin(lha);
	ca = cos(decl)*cos(lha) - erad*cos(glat)*sin(hp);
	sd = sin(decl)           - erad*sin(glat)*sin(hp);

	lha = atan2(sa, ca);
	decl2 = atan2(sd, sqrt(sa*sa+ca*ca));
	f = sqrt(sa*sa+ca*ca+sd*sd);
	semi2 = semi/f;
	ra = gst - lha - glong;
	while(ra > pi)
		ra -= 2*pi;
	while(ra < -pi)
		ra += 2*pi;

/*
 *	convert to horizon coordinates
 */

	sel = sin(nlat)*sin(decl2) + cos(nlat)*cos(decl2)*cos(lha);
	el = atan2(sel, sqrt(1.-sel*sel));
	saz = sin(lha)*cos(decl2);
	caz = cos(nlat)*sin(decl2) - sin(nlat)*cos(decl2)*cos(lha);
	az = pi + atan2(saz, -caz);

	az /= radian;
	el /= radian;

}
