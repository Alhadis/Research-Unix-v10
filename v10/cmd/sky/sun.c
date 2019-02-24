#include "sky.h"

extern struct suntab
{
	float f[2];
	char c[3];
} suntab[];
/*
 *	WARNING: Requires sign extension of characters.
 */

sun()
{
	double mmerc, mven, merth, mmars, mjup, msat;
	double dmoon, mmoon, gmoon;
	double pturbb, pturbl, pturbr, lograd;
	double planp[7];
	struct suntab *pp;
	double Eday, capT, capT2, capT3;
	pp = &suntab[0];
	Eday = eday - 36525.;
	capT = Eday/36525.;
	capT2 = capT*capT;
	capT3 = capT2*capT;

/*
 *	The arugments and coefficients are taken from
 *	Simon Newcomb, "Tables of the Motion of the Earth upon
 *	its Axis and About the Sun", A.P.A.E. VI (1895)
 */

	object = "sun         ";
	incl = 0.;
	ecc = .01670911 - 4.205e-5 * capT - 1.26e-7*capT2;
	node = 0.;
	argp = 282.938352 + .0000470774*Eday + .000462*capT2
		 + .000003*capT3;
	mrad = 1.0;
	anom = 357.527723 + .985600283*Eday - .000159*capT2
		 - .000003*capT3;
	motion = .9856473354;

	argp -= 0.00085; /*empirical*/
	anom += 0.00085; /*empirical*/

	dmoon = 350.737681+12.1907491914*eday-.001436*capt2;
	gmoon = 11.250889 + 13.2293504490*eday - .003212*capt2;
	mmoon = 296.104608 + 13.0649924465*eday + 9.192e-3*capt2;
	mmerc = 102.19 + 4.092329959*eday + .19;
	mven  = 212.448 + 1.602121635*eday + .17;
	merth = 358.476 + 0.985600267*eday + .19;
	mmars = 319.590 + .524024095*eday + .11;
	mjup  = 225.269 + .083082362*eday + .34;
	msat  = 175.593 + .033450794*eday + .24;
/*	muran = 72.248 + .011722663*eday + .19;*/

	dmoon = fmod(dmoon, 360.)*radian;
	gmoon = fmod(gmoon, 360.)*radian;
	mmoon = fmod(mmoon, 360.)*radian;
	mmerc *= radian;
	mven  *= radian;
	merth *= radian;
	mmars *= radian;
	mjup *= radian;
	msat  *= radian;


/*
	long period terms in the mean anomaly - the arguments
	of the terms are:
		4*mars - 7*earth + 3*venus
		3*jup - 8*mars + 4*earth
		13*earth - 8*venus
		8*earth - 15*mars
*/

	anom = anom
	  + 0.266/3600.*sin((31.8 + 119.0*capt)*radian)
	  + 6.40 /3600.*sin((231.19 + 20.20*capt)*radian)
	  + 1.870/3600.*sin((57.24+150.27*capt)*radian);

	incl *= radian;
	node *= radian;
	argp *= radian;
	anom = fmod(anom, 360.)*radian;

/*
 *	computation of elliptic orbit
 */

	lambda = anom + argp;

	pturbl = (2.*ecc - ecc*ecc*ecc/4.)*sin(anom)
		+ (ecc*ecc*5./4. - ecc*ecc*ecc*ecc*11./24.)*sin(2.*anom)
		+ (ecc*ecc*ecc*13./12.)*sin(3.*anom)
		+ (ecc*ecc*ecc*ecc*103./96.)*sin(4.*anom);

	pturbl -= 0.27*radsec*sin(anom);

	lambda += pturbl;

	beta = 0.;

	lograd = (ecc*ecc/4. + ecc*ecc*ecc*ecc/32.)
		 + (-ecc + ecc*ecc*ecc*3./8.)*cos(anom)
		 + (-ecc*ecc*3./4. + ecc*ecc*ecc*ecc*11./24.)*cos(2.*anom)
		 + (-ecc*ecc*ecc*17./24.)*cos(3.*anom)
		 + (-ecc*ecc*ecc*ecc*71./96.)*cos(4.*anom);

	lograd += 0.00000035; /*empirical*/
	lograd += 0.00000070*cos(anom); /*empirical*/

	lograd = lograd/2.30258509;

/*
 *	Computation of perturbations to elliptic orbit.
 */

	planp[1] = mmerc;
	planp[2] = mven;
	planp[3] = merth;
	planp[4] = mmars;
	planp[5] = mjup;
	planp[6] = msat;

	pturbl = 0.;
	for(;;){
		if(pp->f[0]==0.){
			pp++;
			break;
		}
		pturbl += pp->f[0]*cos(pp->f[1] + pp->c[0]*merth + pp->c[1]*planp[pp->c[2]]);
		pp++;
	}

	pturbl = pturbl
	  + 6.454*sin(dmoon)
	  + 0.015*sin(dmoon) /* empirical */
	  + 0.013*sin(3.*dmoon)
	  + 0.177*sin(dmoon + mmoon)
	  - 0.424*sin(dmoon - mmoon)
	  + 0.039*sin(3.*dmoon - mmoon)
	  - 0.064*sin(dmoon + merth)
	  + 0.172*sin(dmoon - merth)
/*
	  - 0.013*sin(dmoon-mmoon-merth)
	  - 0.013*sin(2.*gmoon-2.*dmoon)
*/
	;

	pturbl *= radsec;


	pturbb = 0.;
	for(;;){
		if(pp->f[0]==0.){
			pp++;
			break;
		}
		pturbb += pp->f[0]*cos(pp->f[1] + pp->c[0]*merth + pp->c[1]*planp[pp->c[2]]);
		pp++;
	}


	pturbb = pturbb
	  + 0.576*sin(gmoon)
	  + 0.016*sin(gmoon + mmoon)
	  - 0.047*sin(gmoon - mmoon)
	  + 0.021*sin(2.*dmoon - gmoon)
/*
	  + 0.005*sin(2.*dmoon-gmoon-mmoon)
	  + 0.005*sin(gmoon+merth)
	  + 0.005*sin(gmoon-merth)
*/
	;

	pturbb *= radsec;


	pturbr = 0.;
	for(;;){
		if(pp->f[0]==0.){
			pp++;
			break;
		}
		pturbr += pp->f[0]*cos(pp->f[1] + pp->c[0]*merth + pp->c[1]*planp[pp->c[2]]);
		pp++;
	}


	pturbr = pturbr
	  +13.360e-6*cos(dmoon)
	  + 0.030e-6*cos(3.*dmoon)
	  + 0.370e-6*cos(dmoon + mmoon)
	  - 1.330e-6*cos(dmoon - mmoon)
	  + 0.080e-6*cos(3.*dmoon - mmoon)
	  - 0.140e-6*cos(dmoon + merth)
	  + 0.360e-6*cos(dmoon - merth)
/*
	  - 0.030e-6*cos(dmoon-mmoon-merth)
	  + 0.030e-6*cos(2.*gmoon-2.*dmoon)
*/
	;
	lambda += pturbl;
	if(lambda > 2.*pi)
		lambda -= 2.*pi;

	beta += pturbb;

	lograd = (lograd+pturbr) * 2.30258509;
	rad = 1. + lograd * (1. + lograd * (.5 + lograd/6.));

	motion *= radian*mrad*mrad/(rad*rad);

	semi = 961.182;
	if(flags & OCCULT)
		semi = 959.63;
	mag = -26.5;
}
