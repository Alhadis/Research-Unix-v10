#include "sky.h"

extern struct nuttab
{
	float f[2];
	char c[5];
} nuttab[];

nutate()
{

/*
 *	uses radian, radsec, eday
 *	sets psi, eps, dpsi, deps, obliq, gst, tobliq
 */

	double msun, mnom, noded, dmoon;
	struct nuttab *pp;
	double arg;
	double Eday;
	double capT, capT2, capT3;

/*
 *	nutation of the equinoxes is a irregular wobble of the
 *	pole of the earths rotation whose magnitude is about
 *	9 seconds of arc and whose period is about 18.6 years.
 *
 *	it depends upon the pull of the sun and moon on the
 *	equatorial bulge of the earth.
 *
 *	psi and eps are the two angles which specify the
 *	true pole with respect to the mean pole.
 *
 *	all coeffieients are from Supplement to A.E. 1984
 */

	pp = &nuttab[0];
	Eday = eday - 36525;
	capT = capt-1;
	capT2 = capT*capT;
	capT3 = capT2*capT;

	mnom = 134.96298139 + 13.06499294724*Eday + 8.6972e-3*capT2
		 + 17.8e-6*capT3;
	msun = 357.52772333 + 0.98560028309*Eday - 0.16028e-3*capT2
		 - 3.33e-6*capT3;
	noded = 93.27191028 + 13.22935024060*Eday - 3.6825e-3*capT2
		 + 3.05e-6*capT3;
	dmoon = 297.85036306 + 12.19074911650*Eday - 1.91417e-3*capT2
		 + 5.28e-6*capT3;
	node = 125.04452222 - .05295376484*Eday + 2.07083e-3*capT2
		 + 2.22e-6*capT3;
	mnom *= radian;
	msun *= radian;
	noded *= radian;
	dmoon *= radian;
	node *= radian;

/*
 *	long period terms
 */


	psi = -(17.1996+.01742*capT)*sin(node);
	eps = (9.2025+0.00089*capT)*cos(node);
	for(;;){
		if(pp->f[0]==0.){
			pp++;
			break;
		}
		arg = pp->c[0]*mnom + pp->c[1]*msun + pp->c[2]*noded
			+ pp->c[3]*dmoon + pp->c[4]*node;
		psi += pp->f[0]*sin(arg);
		eps += pp->f[1]*cos(arg);
		pp++;
	}

/*
 *	short period terms
 */

	dpsi = 0.;
	deps = 0.;
	if((flags&APPARENT)==0){
		for(;;){
			if(pp->f[0]==0.){
				pp++;
				break;
			}
			arg = pp->c[0]*mnom + pp->c[1]*msun + pp->c[2]*noded
				+ pp->c[3]*dmoon + pp->c[4]*node;
			dpsi += pp->f[0]*sin(arg);
			deps += pp->f[1]*cos(arg);
			pp++;
		}
	}


	psi = (psi+dpsi)*radsec;
	eps = (eps+deps)*radsec;
	dpsi *= radsec;
	deps *= radsec;

	obliq = 23.43929111 - .013004167*capT - 0.164e-6*capT2
		 + 0.5036e-6*capT3;
	obliq *= radian;
	tobliq = obliq + eps;

	gst = 100.460618375 + 360.98564736629*Eday + 0.3882667e-3*capT2
		 - 0.025e-6*capT3;
	gst -= 180.;
	gst = fmod(gst, 360.);
	if(gst < 0.)
		gst += 360.;
	gst *= radian;
	gst += psi*cos(tobliq);

/*
 * print true obliquity, mean obliquity, mean GST, and true GST.
 */

	if(flags&GOOBIE){
		printf("Mean G. Sid. Time: ");
		prhms(gst - psi*cos(obliq), 4);
		printf("\n");

		printf("App. G. Sid. Time: ");
		prhms(gst,4);
		printf("\n");

		printf("Goobie: %6.3f %6.3f %6.3f %6.3f\n", psi/radsec,
			eps/radsec, dpsi/radsec, deps/radsec);

		printf("Obliquity: ");
		prdms(tobliq,3);
		printf("\n");
	}

}
