#include "sky.h"

extern struct merctab
{
	float f[2];
	char c[3];
} merctab[];

merc()
{
	double pturbl, pturbb, pturbr;
	double lograd;
	double dele, enom, vnom, nd, sl;
	double q0, v0, t0, m0, j0 , s0;
	double lsun, elong, ci, dlong;
	double planp[7];
	struct merctab *pp = &merctab[0];
	double olong;
	double temp;

/*
 *	The arguments nnd coefficients are taken from
 *	Simon Newcomb, Tables of the Heliocentric Motion
 *	of Mercury
 *	A.P.A.E. VI, part 2 (1895).
 *
 *	Here are the mean orbital elements.
 */

	object = "Mercury     ";
	ecc = .20561421 + .00002046*capt - 0.03e-6*capt2;
	ecc -= .00000250; /*empirical*/
	ecc -= .00000003; /*empirical*/
	incl = 7.0028806 + .0018608*capt - 18.3e-6*capt2;
	incl += .00025;/* empirical */
	node = 47.145944 + 1.185208*capt + .0001739*capt2;
	node -= 2.2/3600.; /* empirical */
	argp = 75.899697 + 1.555490*capt + .0002947*capt2;
	argp += 0.06/3600. + 1.89/3600.; /* empirical */
	mrad = .3870986;
	anom = 102.279381 + 4.0923344364*eday + 6.7e-6*capt2;
	anom += 2.07/3600. - 1.89/3600.; /* empirical */
	motion = 4.0923770233;

	incl *= radian;
	node *= radian;
	argp *= radian;
	anom = fmod(anom, 360.)*radian;
	motion *= radian;

/*
 *	Conventional mean anomalies of perturbing planets.
 */

	q0 = 102.28  + 4.092334429*eday;
	v0 = 212.536 + 1.602126105*eday;
	t0 = -1.45  + .985604737*eday;
	m0 = 319.66 + .524028480*eday;
	j0 = 225.36 + .083086735*eday;
	s0 = 175.68 + .033455441*eday;

	q0 *= radian;
	v0 *= radian;
	t0 *= radian;
	m0 *= radian;
	j0 *= radian;
	s0 *= radian;

	planp[1] = q0;
	planp[2] = v0;
	planp[3] = t0;
	planp[4] = m0;
	planp[5] = j0;
	planp[6] = s0;

/*
 *	Computation of long period terms affecting the mean anomaly.
 */

	anom = anom;

/*
 *	Computation of elliptic orbit.
 */

	enom = anom + ecc*sin(anom);
	do {
		dele = (anom - enom + ecc * sin(enom)) /
			(1. - ecc*cos(enom));
		enom += dele;
	} while(fabs(dele) > 1.e-8);
	vnom = 2.*atan2(sqrt((1.+ecc)/(1.-ecc))*sin(enom/2.),
			cos(enom/2.));
	rad = mrad*(1. - ecc*cos(enom));

/*
 *	Perturbations in longitude.
 */

	pturbl = 0.;
	for(;;){
		if(pp->f[0]==0.){
			pp++;
			break;
		}
		pturbl += pp->f[0]*cos(pp->f[1] + pp->c[0]*q0 + pp->c[1]*planp[pp->c[2]]);
		pp++;
	}

/*
 *	Perturbations in latitude.
 */

	pturbb = 0.;
	for(;;){
		if(pp->f[0]==0.){
			pp++;
			break;
		}
		pturbb += pp->f[0]*cos(pp->f[1] + pp->c[0]*q0 + pp->c[1]*planp[pp->c[2]]);
		pp++;
	}

/*
 *	Perturbations in log radius vector.
 */

	pturbr = 0.;
	for(;;){
		if(pp->f[0]==0.){
			pp++;
			break;
		}
		pturbr += pp->f[0]*cos(pp->f[1] + pp->c[0]*q0 + pp->c[1]*planp[pp->c[2]]);
		pp++;
	}
	pturbr *= 1.e-6;

/*
 *	reduce to the ecliptic
 */

	olong = vnom + argp + pturbl*radsec;
	nd = olong - node;
	lambda = node + atan2(sin(nd)*cos(incl), cos(nd));

	sl = sin(incl)*sin(nd);
	beta = atan2(sl, sqrt(1.-sl*sl)) + pturbb*radsec;

	lograd = pturbr*2.30258509;
	rad *= 1. + lograd;

/*
 *	Compute motion for planetary aberration.
 */

	temp = motion*mrad*mrad*sqrt(1.-ecc*ecc)/(rad*rad);
	ldot = temp*sin(2.*(lambda-node))/sin(2.*(olong-node));
	bdot = temp*sin(incl)*cos(lambda-node);
	rdot = motion*mrad*ecc*sin(olong-argp)/sqrt(1.-ecc*ecc);

/*
 *	Compute magnitude.
 */

	lsun = 99.696678 + 0.9856473354*eday;
	lsun *= radian;
	elong = lambda - lsun;
	ci = (rad - cos(elong))/sqrt(1. + rad*rad - 2.*rad*cos(elong));
	dlong = atan2(sqrt(1.-ci*ci), ci)/radian;
	mag = -.003 + .01815*dlong + .0001023*dlong*dlong;

	semi = 3.34;

	helio();
	geo();

}
