#include "sky.h"

extern struct venust
{
	float f[2];
	char c[3];
} venust[];

venus()
{
	double pturbl, pturbb, pturbr;
	double lograd;
	double dele, enom, vnom, nd, sl;
	double q0, v0, t0, m0, j0 , s0;
	double lsun, elong, ci, dlong;
	double planp[7];
	struct venust *pp = &venust[0];
	double olong;
	double temp;
	double temp1;

/*
 *	The arguments nnd coefficients are taken from
 *	Simon Newcomb, Tables of the Heliocentric Motion
 *	of Venus
 *	A.P.A.E. VI, part 3 (1895).
 *
 *	Here are the mean orbital elements.
 */

	object = "Venus       ";
	ecc = .00682069 - .00004774*capt + 0.091e-6*capt2;
	incl = 3.393631 + .0010058*capt - 0.97e-6*capt2;
	node = 75.779647 + .89985*capt + .00041*capt2;
	argp = 130.163833 + 1.408036*capt - .0009763*capt2;
	mrad = .7233316;
	anom = 212.603219 + 1.6021301540*eday + .00128605*capt2;
	motion = 1.6021687039;

	incl *= radian;
	node *= radian;
	argp *= radian;
	anom = fmod(anom, 360.)*radian;
	motion *= radian;

/*
 *	Conventional mean anomalies of perturbing planets.
 */

	q0 = 102.35 + 4.092338439*eday;
	v0 = 212.60 + 1.602130154*eday;
	t0 = 358.63  + .985608747*eday;
	m0 = 319.74 + 0.524032490*eday;
	j0 = 225.43 + .083090842*eday;
	s0 = 175.8  + .033459258*eday;

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
 *		13*earth - 8.*venus
 *		4*mars - 7.*earth + 3.*venus
 *		saturn
 */

	anom +=
	  (2.761-0.022*capt)*radsec*sin((237.24+150.27*capt)*radian)
	 + 0.269*radsec*sin((212.2+119.05*capt)*radian)
	 - 0.208*radsec*sin((175.8+1223.5*capt)*radian);

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
		pturbl += pp->f[0]*cos(pp->f[1] + pp->c[0]*v0 + pp->c[1]*planp[pp->c[2]]);
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
		pturbb += pp->f[0]*cos(pp->f[1] + pp->c[0]*v0 + pp->c[1]*planp[pp->c[2]]);
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
		pturbr += pp->f[0]*cos(pp->f[1] + pp->c[0]*v0 + pp->c[1]*planp[pp->c[2]]);
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
	mag = -4.00 + .01322*dlong + .0000004247*dlong*dlong*dlong;

	semi = 8.41;

	helio();
	geo();

/*
 *	transit computation
*/

	if(!((flags&GEO)||(flags&HELIO))){
		temp1 = sin(sundec)*sin(decl2) + cos(sundec)*cos(decl2)
		 *cos(sunra-ra);
		temp1 = atan2(sqrt(1.-temp1*temp1),temp1)/radsec;
		temp1 = temp1/(semi2+sunsd);
		if(temp1 <= 1.0){
			 printf("Transit of Venus: Dist. = %.4f\n", temp1);
		}else if(temp1 < 1.1){
			 printf("Near Transit of Venus: Dist. = %.4f\n", temp1);
		}
	}

}
