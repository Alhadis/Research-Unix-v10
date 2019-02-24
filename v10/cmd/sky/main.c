#include "sky.h"
#include <stdio.h>

main(argc,argv)
int argc;
char *argv[];
{

	pi = 3.1415926535897932;
	radian = pi/180.;
	radsec = radian/3600.;
	args(argc,argv);
	init();


	for(;;){
		day = readate();
		printf("Date: ");
		pdate(day);
		printf("\n");
		printf("Time: ");
		ptime(day);
		printf("\n");
		if(day < 0.)
			printf("Julian date %.4f\n", day+2415020.);
		setime(day);

	if(flags&TOPO){
		printf("               rt. ascens.      decl.       az.     elev.    semi.\n");
	}

		if((flags&HELIO)==0){
			lambda = 0.;
			beta = 0.;
			rad = 0.;
			ldot = 0.;
			rdot = 0.;
			bdot = 0.;
		}else{
			object = "earth       ";
		}
		helio();

/*
 *	Shadow computation for lunar eclipse.
 */

		rps = georad;
		shra = alpha + 180.*radian;
		shdecl = -delta;

		geo();

/*
 *	Computation for solar eclipse.
 */

		sunra = ra;
		sundec = decl2;
		sunsd = semi2;

		output();

		moon();
		output();

		merc();
		output();

		venus();
		output();

		mars();
		output();

		jup();
		output();

		sat();
		output();

		if((flags&NOSTAR)==0){
			stars();
		}

	}

}

args(argc,argv)
int argc;
char *argv[];
{
	register char *p, *q;

	p = "";
	if(argc>1)
		p = argv[1];
	while(*p)
	switch(*p++){

/*
 *	Meanings of options.
 *
 *	APPARENT: Catalogue apparent place omits
 *	short period terms of nutation for direct
 *	comparison with "Apparent Places of
 *	Fundamental Stars".
 *
 *	OCCULT: Changes the semidiameters of the
 *	sun and moon, and moves the moon slightly.
 *
 *	LATUD: prompts for longitude, latitude and elevation.
 *
 *	MEAN: omits all nutation, for direct comparison
 *	with star catalogs. Also omits the E-terms of nutation.
 *
 *	NOSTAR: Solar system only.
 *
 *	STCATL: Prompts for name of file containing star catalog.
 *
 *	XYZ: Prints x,y,z, coordinates vice angles.
 *
 *	GEO: Reports apparent geocentric place.
 *
 *	HELIO: Reports heliocentric place.
 *
 *	TOPO: Reports topocentric place.Default location
 *	is the hollyhock in Morris's back yard.
 *
 *	ECLIPTIC: Reports places in ecliptic vs. equatorial coords.
 *
 *	KITCH: Input times are interpreted as local time
 *	vs. Greenwich time (but still E.T.)
 */

	case 'a':
		flags |= APPARENT;
		continue;

	case 'e':
		flags |= OCCULT;
		continue;

	case 'l':
		flags |= LATUD;
		continue;

	case 'm':
		flags |= MEAN;
		continue;

	case 'n':
		flags |= GOOBIE;

	case 'p':
		flags |= NOSTAR;
		continue;

	case 's':
		flags |= STCATL;
		continue;
	case 'x':
		flags |= XYZ;
		continue;

	case 'g':
		flags |= GEO;
		continue;

	case 'h':
		flags |= HELIO;
		continue;

	case 'c':
		flags |= ECLIPTIC;
		continue;

	case '-':
		continue;

	default:
		printf("Unknown option '%c'\n",p[-1]);
	}
	if(!((flags&GEO)||(flags&HELIO))){
		flags |= TOPO;
	}

}

readlat()
{
	register i;
	double ifa[3];

	printf("NLat(deg) WLong(deg) elev(meters)\n");
	rline(stdin);
	for(i=0; i<3; i++)
		ifa[i] = atof(skip(i));
	nlat = ifa[0] * radian;
	wlong = ifa[1] * radian;
	elev = ifa[2];
}

readcat()
{
	char *gets();
	char *p;

	printf("Enter name of star catalog: ");
	p = startab;
	gets(p);
}

init()
{

	register char *p, *q;

	wlong = (74.+32./60.)*radian;
	nlat = (40.+40./60.)*radian;
	elev = 0.;
	if(flags & LATUD)
		readlat();
	prlat();
	glat = nlat - (692.74*radsec)*sin(2.*nlat)
		 + (1.16*radsec)*sin(4.*nlat);
	erad = .99832707 + .00167644*cos(2.*nlat)
		 - 0.352e-5*cos(4.*nlat)
		 + 0.001e-5*cos(6.*nlat)
		 + 0.1568e-6*elev;
	q = "/usr/lib/startab";
	for(p = startab; *p++ = *q++;)
		;
	if(flags & STCATL)
		readcat();
/*
	printf("%s\n", startab);
*/
}
rline(f)
FILE *f;
{
	register char *p;
	register c;

	p = line;
	do {
		c = getc(f);
		if(c < 0)
			return(1);
		*p++ = c;
	} while(c != '\n');
	return(0);
}

char*
skip(n)
{
	register i;
	register char *cp;

	cp = line;
	for(i=0; i<n; i++) {
		while(*cp == ' ' || *cp == '\t')
			cp++;
		while(*cp != '\n' && *cp != ' ' && *cp != '\t')
			cp++;
	}
	while(*cp == ' ' || *cp == '\t')
		cp++;
	return(cp);
}
double
readate()
{
	register i;
	double ifa[5];

	printf("year mo da hr min\n");
	if(rline(stdin) != 0)
		exit(0);
	for(i=0; i<5; i++)
		ifa[i] = atof(skip(i));
	return(convdate(ifa));
}

double
convdate(ifa)
double ifa[];
{
	double y, d, temp;
	register i;

	y = ifa[0];
	i = ifa[1];
	d = ifa[2];
	temp = 0.;
	if(d>28.){
		temp = d - 28.;
		d = 28.;
	}
	while(i < 1) {
		i += 12;
		y -= 1.;
	}
	while(i > 12) {
		i -= 12;
		y += 1.;
	}
	if(y < 0.)
		y += 1.;
	y += 4712.;
	if(fmod(y, 4.) == 0 && i > 2)
		d += 1.;
	y = y*365. +
		floor((y+3.)/4.) +
		dmo[i-1] + d - 1.;
	if(y > 2361232.)
		y -= floor((y-1794167.)/36525.) -
			floor((y-1721117.)/146100.);
	y += ifa[3]/24. + ifa[4]/1440. - .5 + temp;
	return(y-2415020.);
}
setime(day)
double day;
{


	eday = day;
	deltat = eday * .00167;
	glong = wlong;
	if(flags & OCCULT)
		glong += 15.*deltat*radsec;
	capt = eday/36525.;
	capt2 = capt*capt;
	capt3 = capt2*capt;
	nutate();

	aberr();

	sun();
	xms = rad*cos(beta)*cos(lambda);
	yms = rad*cos(beta)*sin(lambda);
	zms = rad*sin(beta);
}

double
fmod(x, y)
double x, y;
{
	extern double modf();
	double d;

	if (y == 0.0)
		return (x);
	modf(x/y, &d);
	return (x - d * y);
}
