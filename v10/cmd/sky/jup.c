#include "sky.h"

extern struct juptab
{
	float f[6];
	char c[3];
} juptab[];

jup()
{
	double pturbl, pturbb, pturbr;
	double lograd;
	double dele, enom, vnom, nd, sl;
	double q0, v0, t0, m0, j0 , s0, u0, n0;
	double lsun, elong, ci, dlong;
	double planp[9];
	struct juptab *pp = &juptab[0];
	double olong;
	double grin;
	double temp;

/*
 *	The arguments nnd coefficients are taken from
 *
 *	Here are the mean orbital elements.
 */

	object = "Jupiter     ";
	ecc = 0.04833748 + 1.63903e-4*capt - 0.4642e-6*capt2 - 1.593e-9*capt3;
	incl = 1.3086429 - 0.005696*capt + 3.89e-6*capt2;
	node = 99.4431901 + 1.0105300*capt + 3.522e-4*capt2 - 8.51e-6*capt3;
	argp = 13.4119487 + 0.21344495*capt + 7.466e-4*capt2 - 3.7946e-6*capt3;
	anom = 225.3350378 + 0.0830853474*eday - 8.332e-4*capt2 + 3.9876e-6*capt3;
	motion = .083091212;
	mrad = 5.202803;

	incl *= radian;
	node *= radian;
	argp *= radian;
	anom = fmod(anom, 360.)*radian;
	motion *= radian;

/*
 *	Longitudes of perturbing planets,
 *	They are of epoch Jan 0.5, 1900, and are
 *	referred to the fixed qquinox of that date.
 */

	q0 = 178.179 + 4.092338817*eday;
	v0 = 342.767 + 1.602130491*eday;
	t0 =  99.697 + 0.985609114*eday;
	m0 = 293.748 + 0.524032950*eday;
	j0 = 238.050 + 0.083091230*eday;
	s0 = 266.280 + 0.033459743*eday;
	u0 = 243.370 + 0.011731421*eday;
	n0 =  85.183 + 0.005987356*eday;

	q0 *= radian;
	v0 *= radian;
	t0 *= radian;
	m0 *= radian;
	j0 *= radian;
	s0 *= radian;
	u0 *= radian;
	n0 *= radian;

	grin = 5.*s0 - 2.*j0 - 8079.0*radsec*capt;

	planp[1] = q0;
	planp[2] = v0;
	planp[3] = t0;
	planp[4] = m0;
	planp[5] = j0;
	planp[6] = s0;
	planp[7] = u0;
	planp[8] = n0;

/*
 *	Computation of long period terms affecting the mean anomaly.
 */

	anom += 0.
	+(1192.85-6.076*capt-0.0400*capt2+0.00075*capt3)*radsec*sin(grin)
	+(-23.80-0.192*capt+0.0226*capt2-0.00080*capt3)*radsec*cos(grin)
	+(-11.04-0.060*capt-0.0072*capt2+0.00021*capt3)*radsec*sin(2.*grin)
	+(1.44-0.086*capt+0.0004*capt2+0.00006*capt3)*radsec*cos(2.*grin)

	 + (8.22-0.120*capt-0.002*capt2)*radsec*sin(2.*j0-6.*s0+3.*u0)
	 + (0.55 + 0.420*capt - 0.0079*capt2)*radsec*cos(2.*j0-6.*s0+3.*u0)
	;

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

	pturbl = 0.
		+(-83.79-1.222*capt+0.0097*capt2)*sin(grin-j0)
		+(137.08-1.508*capt-0.0069*capt2)*cos(grin-j0)
	;

	for(;;){
		if(pp->c[2]==0){
			pp++;
			break;
		}
		temp = planp[pp->c[2]]*pp->c[0] + j0*pp->c[1];
		pturbl += (pp->f[0]+pp->f[1]*capt+pp->f[2]*capt2)*sin(temp)
			+ (pp->f[3]+pp->f[4]*capt+pp->f[5]*capt2)*cos(temp);
		pp++;
	}

/*
 *	Perturbations in latitude.
 */

	pturbb = 0.;
/*
	for(;;){
		if(pp->f[0]==0.){
			pp++;
			break;
		}
		pturbb += pp->f[0]*cos(pp->f[1] + pp->c[0]*j0 + pp->c[1]*planp[pp->c[2]]);
		pp++;
	}
*/

/*
 *	Perturbations in log radius vector.
 */

	pturbr = 0.;
/*
	for(;;){
		if(pp->f[0]==0.){
			pp++;
			break;
		}
		pturbr += pp->f[0]*cos(pp->f[1] + pp->c[0]*j0 + pp->c[1]*planp[pp->c[2]]);
		pp++;
	}
*/
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

	mag = -8.93;

	semi = 98.57;

	helio();
	geo();

}
