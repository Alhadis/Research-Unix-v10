int	flags;
double	wlong, awlong, nlat, elev;
double	obliq, phi, eps, tobliq;
double	dphi, deps;
double	day, deld;
double	eday, capt, capt2, capt3, gst;
double	pi, radian, radsec, deltat;
double	erad, glat;
double	xms, yms, zms;
double	xdot, ydot, zdot;
double	ecc, incl, node, argp, mrad, anom, motion;
double	lambda, beta, rad, mag, semi;
double	alpha, delta, rp, hp;
double	ra, decl, semi2;
double	lha, decl2, lmb2;
double	az, el;
double	meday, seday, mhp, salph, sdelt, srad;
double	rah, ram, ras, dday, dmin, dsec;
long	agc;
double	da, dd, px, epoch;
char	line[100];
double	sin();
double	cos();
double	floor();
double	ltod();
double	sqrt();
double	pyth();
double	fmod();
double	atan2();
double	log();
double	fabs();
double	dist();
double	rise();
double	set();
double	melong();
double	solstice();
double	betcross();
int	evcomp();
long	time();
double	sunel();
double	readate();
double	convdate();
double	ltod();
double	atof();
double	cosadd();
double	sinadd();
char *	skip();
struct	obj1
{
	double	ra;
	double	decl2;
	double	semi2;
	double	az;
	double	el;
	double	mag;
};
struct	obj2
{
	char	*name;
	int	(*obj)();
	struct	obj1	point[12+2];
}
osun, omoon, oshad, omerc, ovenus,
omars, osat, ojup, ostar;
extern struct	obj2	*objlst[];
struct	obj3
{
	double	t1;
	double	t2;
	double	t3;
	double	t4;
	double	t5;
} occ;
extern int	dmo[];
extern	struct	_iobuf {
	int	_cnt;
	unsigned char	*_ptr;
	unsigned char	*_base;
	short	_flag;
	char	_file;
} _iob[20];
struct _iobuf	*fopen();
struct _iobuf	*fdopen();
struct _iobuf	*freopen();
long	ftell();
char	*fgets();
main(argc, argv)
char *argv[];
{
	register i, j;
	double d;
	pi = 3.1415926535897932;
	radian = pi/180.;
	radsec = radian/3600.;
	args(argc, argv);
	init();
	deld = 1.0/12;
loop:
	d = day;
	pdate(d);
	if(flags & 8) {
		printf(" ");
		ptime(d);
	}
	printf("\n");
	for(i=0; i<=12+1; i++) {
		setime(d);
		for(j=0; objlst[j]; j++) {
			(*objlst[j]->obj)();
			setobj(&objlst[j]->point[i]);
			if(flags & 8)
				output(objlst[j]->name, &objlst[j]->point[i]);
		}
		d += deld;
		if(flags & 8)
			exit(0);
	}
	search();
	day += 1.0;
	if(flags & 1)
		goto loop;
	exit(0);
}
args(argc, argv)
char *argv[];
{
	register char *p;
	long t;
	p = "";
	if(argc > 1)
		p = argv[1];
	while(*p)
	switch(*p++) {
	case 'd':
		flags |= 64;
		continue;
	case 'l':
		flags |= 128;
		continue;
	case 'c':
		flags |= 1;
		continue;
	case 'p':
		flags |= 8;
		continue;
	case 's':
		flags |= 4;
		continue;
	case 'a':
		flags |= 16;
		continue;
	case 't':
		flags |= 256;
		continue;
	case 'o':
		flags |= 2;
		continue;
	case 'k':
		flags |= 512;
		continue;
	case '-':
		continue;
	default:
		printf("unknown option '%c'\n", p[-1]);
	}
	t = time((int *)0);
	day = t/86400. + 25567.5;
	if(flags & 64)
		day = readate();
	awlong = (74. + 32./60.)*radian;
	nlat = (40. + 40./60.)*radian;
	elev = 0.;
	if(flags & 128)
		readlat();
}
double
readate()
{
	register i;
	double ifa[5];
	printf("year mo da hr min\n");
	rline((&_iob[0]));
	for(i=0; i<5; i++)
		ifa[i] = atof(skip(i));
	return(convdate(ifa));
}
double
convdate(ifa)
double ifa[];
{
	double y, d;
	register i;
	y = ifa[0];
	i = (int)ifa[1];
	d = ifa[2];
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
	y += ifa[3]/24. + ifa[4]/1440. - .5;
	return(y-2415020.);
}
readlat()
{
	register i;
	double ifa[3];
	printf("wlong nlat elev\n");
	rline((&_iob[0]));
	for(i=0; i<3; i++)
		ifa[i] = atof(skip(i));
	awlong = ifa[0] * radian;
	nlat = ifa[1] * radian;
	elev = ifa[2];
}
double
fmod(a, b)
double a, b;
{
	return(a - floor(a/b)*b);
}
