#include "sky.h"

/*
 *	References:
 *	Brown,
 *	Improved Lunar Ephemeris
 *	Supplement to A.E. 1968
 *	Transformation corrections.
 */

double k1, k2, k3, k4;
double mnom, msun, noded, dmoon;
double sinx();
double cosx();

extern struct	moontab
{
	float	f;
	char	c[4];
} moontab[];

extern struct moont1
{
	float f1[2];
	char c1[7];
} moont1[];

moon()
{
	register struct moontab *mp;
	register struct moont1 *mp1;
	double dlong, lsun, psun;
	double eccm, eccs, chp, cpe;
	double q0, v0, t0, m0, j0, sn0, l0;
	double arg1, arg2, arg3, arg4, arg5, arg6, arg7;
	double arg8, arg9, arg10, arg11, arg12, arg13;
	double arg14, arg15, arg16, arg17;
	double arg18, arg19, arg20, arg21, arg22;
	double dgamma, k5, k6;
	double lterms, sterms, cterms, nterms, pterms, spterms;
	double gamma1, gamma2, gamma3, arglat;
	double xmp, ymp, zmp;
	double temp1, temp2;
	double shsd;
	double obl2;
	double planp[7];

	object = "moon        ";

/*
 *	the fundamental elements - all referred to the epoch of
 *	Jan 0.5, 1900 and to the mean equinox of date.
 */

	dlong = 270.434164 + 13.1763965268*eday - .001133*capt2
		 + 1.9e-6*capt3;
	dlong -= .000086; /* empirical*/
	argp = 334.329556 + .1114040803*eday - .010325*capt2
		 - 12.5e-6*capt3;
	node = 259.183275 - .0529539222*eday + .002078*capt2
		 + 2.2e-6*capt3;
	node += .000100; /* empirical */
	lsun = 279.696678 + .9856473354*eday + .000303*capt2;
	psun = 281.220844 + .0000470684*eday + .000453*capt2
		 + 3.3e-6*capt3;

	dlong = fmod(dlong, 360.);
	argp = fmod(argp, 360.);
	node = fmod(node, 360.);
	lsun = fmod(lsun, 360.);
	psun = fmod(psun, 360.);

	eccm = 22639.550;
	eccs = .01675104 - .00004180*capt;
	incl = 18461.400;
	cpe = 124.986;
	chp = 3422.451;

/*
 *	some subsidiary elements - they are all longitudes
 *	and they are referred to the epoch 1/0.5 1900 and
 *	to the fixed mean equinox of 1850.0.
 */

	q0 = 177.481153 + 4.0923388020*eday;
	v0 = 342.069128 + 1.6021304820*eday;
	t0 =  98.998753 + 0.9856091138*eday;
	m0 = 293.049675 + 0.5240329445*eday;
	j0 = 237.352319 + 0.0830912295*eday;
	sn0 = 265.869508 + 0.03345974279*eday;
	l0 = 269.736239 +13.1763583100*eday;

/*
 *	the following are periodic corrections to the
 *	fundamental elements and constants.
 *	arg4 is the "Great Venus Inequality".
 */

	arg1 = 41.1 + 20.2*(capt+.5);
	arg2 = dlong - argp + 33. + 3.*t0 - 10.*v0 - 2.6*(capt+.5);
	arg3 = dlong + 3.*argp - 4.*lsun + 67. - 23.*t0 + 25.*m0;
	arg4 = dlong - argp + 151.1 + 16.*t0 - 18.*v0 - (capt+.5);
	arg5 = node;
	arg6 = node + 276.2 - 2.3*(capt+.5);
	arg7 = 152. + 119.*(capt+0.5);
	arg8 = dlong + argp - 2.*lsun + 105. + t0 - 3.*q0;
	arg9 = 313.9 + 13.*t0 - 8.*v0;
	arg10 = dlong - argp + 112.0 + 29.*t0 - 26.*v0;
	arg11 = dlong - argp + 21.*t0 - 21.*v0;
	arg12 = dlong + argp - 2.*lsun + 273. + 21.*t0 - 20.*v0;
	arg13 = dlong + argp - 2.*lsun + 303. + 8.*t0 - 12.*v0;
	arg14 = 2.*lsun - 2.*node + 270. + 6.*t0 - 5.*v0;
	arg15 = dlong + 2.*lsun - 3.*argp + 24.*t0 - 24.*v0;
	arg16 = dlong - lsun + 262. + 12.*t0 - 15.*v0;
	arg17 = dlong - lsun + 190. + 25.*t0 - 23.*v0;
	arg18 = 43. - 8.*t0 + 15.*m0;
	arg19 = node - lsun + 165. + 2.*m0;
	arg20 = node + 290.1 - 0.9*(capt+.5);
	arg21 = 115. + 38.5*(capt+.5);
	arg22 = 216.0 - 8.*t0 + 15.*m0;
	arg1 *= radian;
	arg2 *= radian;
	arg3 *= radian;
	arg4 *= radian;
	arg5 *= radian;
	arg6 *= radian;
	arg7 *= radian;
	arg8 *= radian;
	arg9 *= radian;
	arg10 *= radian;
	arg11 *= radian;
	arg12 *= radian;
	arg13 *= radian;
	arg14 *= radian;
	arg15 *= radian;
	arg16 *= radian;
	arg17 *= radian;
	arg18 *= radian;
	arg19 *= radian;
	arg20 *= radian;
	arg21 *= radian;
	arg22 *= radian;

	dlong += (
		   0.84 *sin(arg1)
		 +  0.31 *sin(arg2)
		 +  0.04 *sin(arg3)
		 + 14.27 *sin(arg4)
		 +  7.261*sin(arg5)
		 +  0.282*sin(arg6)
		 +  0.04 *sin(arg7)
		 +  0.075*sin(arg8)
		 +  0.237*sin(arg9)
		 +  0.108*sin(arg10)
		 +  0.030*sin(arg11)
		 +  0.126*sin(arg12)
		 +  0.033*sin(arg13)
		 +  0.054*sin(arg14)
		 +  0.010*sin(arg15)
		 +  0.013*sin(arg16)
		 +  0.013*sin(arg17)
		 +  0.026*sin(arg18)
		 + 0.017*sin(arg19)
		 )/3600.;

	argp += (
		 - 2.10 *sin(arg1)
		 -  0.118*sin(arg4)
		 -  2.076*sin(arg5)
		 -  0.840*sin(arg6)
		  - 0.100*sin(arg7)
		 -  0.593*sin(arg9)
		 -  0.065*sin(arg18)
		)/3600.;

	node += (
		   0.63*sin(arg1)
		 +  0.17*sin(arg4)
		 + 95.96*sin(arg5)
		 + 15.58*sin(arg6)
		 +  1.86*sin(arg20)
		 )/3600.;

	t0 += (
		-6.40*sin(arg1)
		-0.27*sin(arg7)
		-1.89*sin(arg9)
		+0.20*sin(arg22)
		 )/3600.;

	lsun += (
		-6.40*sin(arg1)
		-0.27*sin(arg7)
		-1.89*sin(arg9)
		+0.20*sin(arg22)
		)/3600.;

	dgamma = -  4.318*cos(arg5)
		 -  0.698*cos(arg6)
		 -  0.083*cos(arg20);

	j0 +=
		   0.33*sin(arg21);

	sn0 +=
		 -  0.83*sin(arg21);

/*
 *	the following factors account for the fact that the
 *	eccentricity, solar eccentricity, inclination and
 *	parallax used by Brown to make up his coefficients
 *	are both wrong and out of date.  Brown did the same
 *	thing in a different way.
 */

	k1 = eccm/22639.500;
	k2 = eccs/.01675104;
	k3 = 1. + 2.708e-6 + .000108008*dgamma;
	k4 = cpe/125.154;
	k5 = chp/3422.700;

/*
 *	the principal arguments that are used to compute
 *	perturbations are the following differences of the
 *	fundamental elements.
 */

	mnom = dlong - argp;
	msun = lsun - psun;
	noded = dlong - node;
	dmoon = dlong - lsun;

/*
 *	solar terms in longitude
 */

	lterms = 0.0;
	mp = moontab;
	for(;;) {
		if(mp->f == 0.0){
			mp++;
			break;
		}
		lterms += sinx(mp->f,
			mp->c[0], mp->c[1],
			mp->c[2], mp->c[3], 0.0);
		mp++;
	}

	lterms +=
		(294.e-9*eday - 9193./1296000.*dgamma + .0013)*sinx(1.0,0,0,0,2,0.)
		+(-220.e-9*eday + 5282./1296000.*dgamma)*sinx(1.0,1,0,0,-2,0.);

	planp[1] = q0;
	planp[2] = v0;
	planp[3] = t0;
	planp[4] = m0;
	planp[5] = j0;
	planp[6] = sn0;

/*
 *	planetary terms in longitude
 */

	mp1 = moont1;
	for(;;){
		if(mp1->f1[0] == 0.){
			mp1++;
			break;
		}
		lterms += sinx(mp1->f1[0], mp1->c1[0], mp1->c1[1],
			mp1->c1[2], mp1->c1[3],
			mp1->c1[4]*t0+mp1->c1[5]*planp[mp1->c1[6]]+mp1->f1[1]);
		mp1++;
	}

	lterms += sinx(-.189, 0,0,0,0, node) /*IAU*/
		+ sinx(-.013, -1,0,0,0, node) /*IAU*/
		+ sinx(-.013, 1,0,0,0, node); /*IAU*/
	lterms += sinx(0.019, 0,0,0,0, 5.*t0-3.*v0+node+216.0);
	lterms += sinx(0.016, 0,0,0,0, -5.*t0+3.*v0+node+075.0);
	lterms += sinx(-.038, 0,0,0,0, 2.*node);

/*
 *	solar terms in latitude
 */

	sterms = 0.0;
	for(;;) {
		if(mp->f == 0.0){
			mp++;
			break;
		}
		sterms += sinx(mp->f,
			mp->c[0], mp->c[1],
			mp->c[2], mp->c[3], 0.0);
		mp++;
	}

	cterms = 0.0;
	for(;;) {
		if(mp->f == 0.0){
			mp++;
			break;
		}
		cterms += cosx(mp->f,
			mp->c[0], mp->c[1],
			mp->c[2], mp->c[3], 0.0);
		mp++;
	}

	nterms = 0.0;
	for(;;) {
		if(mp->f == 0.0){
			mp++;
			break;
		}
		nterms += sinx(mp->f,
			mp->c[0], mp->c[1],
			mp->c[2], mp->c[3], 0.0);
		mp++;
	}

/*
 *	planetary terms in latitude
 */

	pterms = 0.;
	for(;;){
		if(mp1->f1[0] == 0.){
			mp1++;
			break;
		}
		pterms += sinx(mp1->f1[0], mp1->c1[0], mp1->c1[1],
			mp1->c1[2], mp1->c1[3],
			mp1->c1[4]*t0+mp1->c1[5]*planp[mp1->c1[6]]+mp1->f1[1]);
		mp1++;
	}

	pterms +=
		  sinx(0.014, 0,0,0,0, -2.*t0+2.*v0+l0+285.0)
		 + sinx(0.027, 0,0,0,0, -1.*t0+1.*v0+l0+285.0)
		 + sinx(0.015, 0,0,0,0, t0-v0+l0+105.0)
		 + sinx(0.077, 0,0,0,0, 5.*t0-3.*v0+l0+215.6)
		 + sinx(0.025, 0,0,0,0, -6.*t0+4.*v0+l0+255.0)
		 + sinx(0.074, 0,0,0,0, -5.*t0+3.*v0+l0+051.6)
		 + sinx(0.018, 0,0,0,0, -4.*t0+2.*v0+l0+075.0)
		 + sinx(0.010, 0,0,0,0, -3.*t0+v0+l0+075.0)
		 + sinx(0.030, 0,0,0,0, 8.*t0-5.*v0+l0+125.0);
	pterms +=
		 sinx(0.010, 0,0,0,0, -t0+2.*m0+l0+345.0)
		 + sinx(0.035, 0,0,0,0, 2.*j0+l0+168.0)
		 + sinx(0.018, 0,0,0,0, -2.*j0+l0+024.0)
		 + sinx(-.017, 0,0,0,0, l0)
		 + sinx(0.083, 0,0,1,0, 2.*node)
		 + sinx(0.215, 0,0,0,0, dlong) /*IAU*/
		 + sinx(-.012, -1,0,0,0, dlong) /*IAU*/
		 + sinx(0.011, 1,0,0,0, dlong); /*IAU*/

/*
 *	solar terms in parallax
 */

	spterms = 3422.700;
	for(;;) {
		if(mp->f == 0.0){
			mp++;
			break;
		}
		spterms += cosx(mp->f,
			mp->c[0], mp->c[1],
			mp->c[2], mp->c[3], 0.0);
		mp++;
	}

/*
 *	planetary terms in parallax
 */

	for(;;){
		if(mp1->f1[0] == 0.){
			mp1++;
			break;
		}
		spterms += cosx(mp1->f1[0], mp1->c1[0], mp1->c1[1],
			mp1->c1[2], mp1->c1[3],
			mp1->c1[4]*t0+mp1->c1[5]*planp[mp1->c1[6]]+mp1->f1[1]);
		mp1++;
	}

/*
 *	computation of longitude
 */

	lambda = (dlong + lterms/3600.)*radian;

/*
 *	computation of latitude
 */

	arglat = (noded + sterms/3600.)*radian;
	gamma1 = 18519.700 * k3;
	gamma2 = -6.241 * k3*k3*k3;
	gamma3 = 0.004 * k3*k3*k3*k3*k3;

	k6 = (gamma1 + cterms) / gamma1;

	beta = k6 * (gamma1*sin(arglat) + gamma2*sin(3.*arglat)
		 + gamma3*sin(5.*arglat) + nterms)
		 + pterms;
	if(flags & OCCULT)
		beta -= 0.6;
	beta *= radsec;

/*
 *	computation of parallax
 */

	spterms = k5 * spterms *radsec;
	hp = spterms + (spterms*spterms*spterms)/6.;

	rad = hp/radsec;
	georad = 1.;
	semi = .0799 + .272453*(hp/radsec);
	if(dmoon < 0.)
		dmoon += 360.;
	mag = dmoon/360.;

/*
 *	change to equatorial coordinates
 */

	lambda += psi;
	obl2 = obliq + eps;
	xmp = georad*cos(lambda)*cos(beta);
	ymp = georad*(sin(lambda)*cos(beta)*cos(obl2) - sin(obl2)*sin(beta));
	zmp = georad*(sin(lambda)*cos(beta)*sin(obl2) + cos(obl2)*sin(beta));

	alpha = atan2(ymp, xmp);
	delta = atan2(zmp, sqrt(xmp*xmp+ymp*ymp));

/*
 *c	lunar eclipse computation
 */

	shsd = 1.0183*hp/radsec - 969.85/rps;
	temp1 = sin(shdecl)*sin(delta) + cos(shdecl)*cos(delta)
	 *cos(shra - alpha);
	temp2 = atan2(sqrt(1.-temp1*temp1),temp1)/radsec;
	temp2 = semi + shsd - temp2;
	temp2 = temp2/(2.*semi);
	if(temp2 >= 0.){
		if(temp2 < 1.)
			printf("Partial Lunar Eclipse: Mag. = %.4f\n", temp2);
		else
			printf("Total Lunar Eclipse: Mag. = %.4f\n", temp2);
	}


	geolam = lambda;
	geobet = beta;

	geo();

/*
 *	solar eclipse computation
*/

	if(!((flags&GEO)||(flags&HELIO))){
		temp1 = sin(sundec)*sin(decl2) + cos(sundec)*cos(decl2)
		 *cos(sunra-ra);
		temp1 = atan2(sqrt(1.-temp1*temp1),temp1)/radsec;
		if(temp1 <= (semi2+sunsd)){
			temp2 = (semi2+sunsd-temp1)/(2.*sunsd);
			if(temp1 >= fabs(sunsd-semi2))
				printf("Partial Solar Eclipse: Mag. = %.4f\n", temp2);
			else if(temp2 > 1.)
				printf("Total Solar Eclipse: Mag. = %.4f\n", temp2);
			else
				printf("Annular Solar Eclipse: Mag. = %.4f\n", temp2);
		}
	}

/*
 *	constants for occultation computations
 */

	moonra = ra;
	moonde = decl2;
	moonsd = semi2;

}

double
sinx(coef,i,j,k,m,angle)
double coef, angle;
{
	double x;

	x = i*mnom + j*msun + k*noded + m*dmoon + angle;
	x = coef*sin(x*radian);
	if(i < 0)
		i = -i;
	for(; i>0; i--)
		x *= k1;
	if(j < 0)
		j = -j;
	for(; j>0; j--)
		x *= k2;
	if(k < 0)
		k = -k;
	for(; k>0; k--)
		x *= k3;
	if(m & 1)
		x *= k4;

	return(x);
}

double
cosx(coef,i,j,k,m,angle)
double coef, angle;
{
	double x;

	x = i*mnom + j*msun + k*noded + m*dmoon + angle;
	x = coef*cos(x*radian);
	if(i < 0)
		i = -i;
	for(; i>0; i--)
		x *= k1;
	if(j < 0)
		j = -j;
	for(; j>0; j--)
		x *= k2;
	if(k < 0)
		k = -k;
	for(; k>0; k--)
		x *= k3;
	if(m & 1)
		x *= k4;

	return(x);
}
