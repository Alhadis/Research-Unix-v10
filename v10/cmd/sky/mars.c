#include "sky.h"

extern struct marst
{
	float f[2];
	char c[3];
} marst[];

mars()
{
	double pturbl, pturbb, pturbr;
	double lograd;
	double dele, enom, vnom, nd, sl;
	double q0, v0, t0, m0, j0 , s0, u0;
	double lsun, elong, ci, dlong;
	double planp[8];
	struct marst *pp = &marst[0];
	double olong;
	double temp;

/*
 *	The arguments nnd coefficients are taken from
 *	Simon Newcomb, Tables of the Heliocentric Motion
 *	of Mars
 *	A.P.A.E. VI, part 4 (1895).
 *
 *	Here are the mean orbital elements.
 */

	object = "Mars        ";
	ecc = .09331290 + .000092064*capt - 0.077e-6*capt2;
	incl = 1.850333 - 6.75e-4*capt + 12.61e-6*capt2;
	node = 48.786442 + .770992*capt - 1.39e-6*capt2
	 - 5.33e-6*capt3;
	argp = 334.218203 + 1.840758*capt + 1.299e-4*capt2
	 - 1.19e-6*capt3;
	mrad = 1.52368840;
	anom = 319.529425 + .5240207666*eday + 1.808e-4*capt2
	 + 1.19e-6*capt3;
	motion = 0.5240711638;

	incl *= radian;
	node *= radian;
	argp *= radian;
	anom = fmod(anom, 360.)*radian;
	motion *= radian;

/*
 *	Conventional mean anomalies of perturbing planets.
 */

	q0 = 102.28  + 4.092334429*eday;
	v0 = 212.388 + 1.60211831*eday;
	t0 = 358.415  + .98559696*eday;
	m0 = 319.530 + .52402078*eday;
	j0 = 225.209 + .08307904*eday + 0.332*sin((134.4+38.5*capt)*radian);
	s0 = 175.533 + .03344747*eday - 0.808*sin((134.4+38.5*capt)*radian);
	u0 = 74.188 + 0.0117193*eday;

	q0 *= radian;
	v0 *= radian;
	t0 *= radian;
	m0 *= radian;
	j0 *= radian;
	s0 *= radian;
	u0 *= radian;

	planp[1] = q0;
	planp[2] = v0;
	planp[3] = t0;
	planp[4] = m0;
	planp[5] = j0;
	planp[6] = s0;
	planp[7] = u0;

/*
 *	Computation of long period terms affecting the mean anomaly.
 *		4*mars - 7*earth + 3*venus
 *		3*jupiter - 8*mars + 4*earth
 *		2*jupiter - - 6*mars + 3*earth
 *		2*saturn - 2*mars + earth
 *		jupiter - 2*mars + earth
 *		5*saturn - 2*jupiter
 */

	anom = anom
	 - (37.05 + 13.50*capt)*radsec
	 + 0.606*radsec*sin((212.87+119.051*capt)*radian)
	 + 52.490*radsec*sin((47.48+19.771*capt)*radian)
	 + 0.319*radsec*sin((116.88+773.444*capt)*radian)
	 + 0.130*radsec*sin((74.00+163.00*capt)*radian)
	 + 0.009*radsec*sin((325.00+753.67*capt)*radian)
	 + 0.280*radsec*sin((300.00+40.8*capt)*radian);

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

	pturbl = 0.043*sin(2.*anom);
	for(;;){
		if(pp->f[0]==0.){
			pp++;
			break;
		}
		pturbl += pp->f[0]*cos(pp->f[1] + pp->c[0]*m0 + pp->c[1]*planp[pp->c[2]]);
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
		pturbb += pp->f[0]*cos(pp->f[1] + pp->c[0]*m0 + pp->c[1]*planp[pp->c[2]]);
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
		pturbr += pp->f[0]*cos(pp->f[1] + pp->c[0]*m0 + pp->c[1]*planp[pp->c[2]]);
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
	mag = -1.30 + .01486*dlong;

	semi = 4.68;

	helio();
	geo();

}
