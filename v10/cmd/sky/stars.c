#include "sky.h"
#include <stdio.h>

stars()
{
	char starname[13];
	register i;
	double temp1;
	FILE *f;

	starname[12] = 0;
	object = starname;


/*
 *	read epoch of star table
 *	and convert to internal format
 *	epoch 1900.0 is J.D. 2415020.313
 */

	f = fopen(startab, "r");
	if(f == NULL) {
		printf("%s?\n", startab);
		return;
	}
	rline(f);
	epoch = atof(line);
	epoch = (epoch-1900.0) * 365.2422 + 0.313;

/*
 *	read mean places of stars at epoch of star table
 */

loop:
	if(rline(f)) {
		fclose(f);
		return;
	}
	for(i=0;i<12;i++){
		starname[i] = line[i];
	}
	agc = (long)atof(line+12);
	rah = atof(line+17);
	ram = atof(line+20);
	ras = atof(line+23);
	da = atof(line+30);
	dday = atof(line+37);
	dmin = atof(line+41);
	dsec = atof(line+44);
	dd = atof(line+50);
	px = atof(line+57);
	mag = atof(line+64);
	const = (int)atof(line+70);

/*
 *	convert rt ascension and declination to internal format
 */

	alpha = rah + ram/60. + ras/3600.;


	delta = fabs(dday) + dmin/60. + dsec/3600.;
	if(dday < 0.)
		delta = -delta;
	alpha *= 15.*radian;
	delta *= radian;

	star();
	output();
/*
 *	Check for occultation.
 */

	if(!((flags&GEO)||(flags&HELIO))){
		temp1 = sin(decl2)*sin(moonde) + cos(decl2)*cos(moonde)*cos(ra-moonra);
		temp1 = atan2(sqrt(1.-temp1*temp1),temp1)/radsec;
		temp1 = temp1/moonsd;
		if(temp1 <= 1.0){
			printf("Occultation: Dist. = %.4f\n", temp1);
		}else if(temp1 <= 1.2){
			printf("Near occultation: Dist. = %.4f\n", temp1);
		}
	}

	goto loop;
}
