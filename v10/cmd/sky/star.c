#include "sky.h"

star()
{
	double capt0, capt1, capt12, capt13;
	double sl, sb, cl;
	double zt, zee, tht;
	double sa, ca, sd;

	capt0 = epoch/36524.220;
	capt1 = (eday - epoch)/36524.220;
	capt12 = capt1*capt1;
	capt13 = capt12*capt1;

	da = da*radsec*15.*100.;
	dd = dd*radsec*100.;

/*
 *	remove E-terms of aberration
 *	except when finding catalog mean places
 *	See Exp. Supp. p. 144 and 48
 *
 *	I have omitted an unimportant time dependence which depends
 *	in essence on the change in the eccentricity
 *	and the perihelion position of the earths orbit.
 */

	alpha = alpha + .341*radsec*sin(alpha+168.8*radian)
	  /cos(delta);
	delta = delta + .341*radsec*cos(alpha+168.8*radian)
	  *sin(delta) + .029*radsec*cos(delta);

/*
 *	Correct for proper motion.
 *
 *	This computation is a second order approximation which
 *	causes no perciptible error in a millenium.
 */

	alpha = alpha + capt1*da
	 + da*dd*sin(delta)/cos(delta)*capt1*capt1;
	delta = delta + capt1*dd
	 - 0.25*da*da*sin(2.*delta)*capt1*capt1;

/*
 *	The effect of radial velocity has been omitted.
 *	The only bright stars perceptibly affected are
 *	Sirius, alpha Cent, and Altair.
 */

/*
 *	convert mean places at epoch of startable to current
 *	epoch (i.e. compute relevant precession)
 *	Coefficients are from Exp. Supp. p. 30.
 */

	zt = (2304.250 + 1.396*capt0)*capt1 + 0.302*capt12
	 + 0.018*capt13;
	zee = zt + 0.791*capt12;
	tht = (2004.682 - 0.853*capt0)*capt1 - 0.426*capt12
	 - 0.042*capt13;

	zt = zt*radsec;
	zee = zee*radsec;
	tht = tht*radsec;

	sa = cos(delta)*sin(alpha+zt);
	ca = cos(tht)*cos(delta)*cos(alpha+zt) - sin(tht)*sin(delta);
	sd = cos(tht)*sin(delta) + sin(tht)*cos(delta)*cos(alpha+zt);

	alpha = atan2(sa,ca) + zee;
	delta = atan2(sd, sqrt(sa*sa+ca*ca));

/*
 *	convert to mean ecliptic system of date
 */

	cl = cos(delta)*cos(alpha);
	sl = cos(delta)*sin(alpha)*cos(obliq) + sin(delta)*sin(obliq);
	sb = -cos(delta)*sin(alpha)*sin(obliq) + sin(delta)*cos(obliq);
	lambda = atan2(sl,cl);
	beta = atan2(sb,sqrt(cl*cl+sl*sl));
	if(px > 1.0) px = 0.;
	if(px != 0.) rad = 1./(radsec*px);
	if(px <= 0.) rad = 1.e9;
	semi = 0.;
	ldot = 0.;
	bdot = 0.;
	rdot = 0.;

	helio();
	geo();
}
