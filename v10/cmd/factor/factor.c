#include <math.h>

extern double modf();

#ifdef pdp11
#define setfps() ldfps(0240)	/* double, chop */
#define ifdiv(d,n) if((n/d)*d==n)

#else
#define setfps()

#ifdef vax
#define ifdiv(d,n) junk = n/d;\
		   asm(" emodd _junk,$0,$0f1.0,r0,_junk");\
		   if(junk==0.)
#else
#define ifdiv(d,n) if(modf(n/d,&junk)==0.)
#endif

#endif

double wheel[] = {
	 2,10, 2, 4, 2, 4, 6, 2, 6, 4,
	 2, 4, 6, 6, 2, 6, 4, 2, 6, 4,
	 6, 8, 4, 2, 4, 2, 4, 8, 6, 4,
	 6, 2, 4, 6, 2, 6, 6, 4, 2, 4,
	 6, 2, 6, 4, 2, 4, 2,10,
};
double n;
double d;
double junk;
double s;

main(argc,argv)
char **argv;
{
	extern double atof();
	register double *wheelp;
	if(argc>1)
		n = atof(argv[1]);
	setfps();
	do {
		if(argc<=1 && scanf("%lf",&n)<=0)
			break;
		if(n<=0)
			break;
		printf("%.0f\n",n);
		s = sqrt(n) + 1;
		try(2.);
		try(3.);
		try(5.);
		try(7.);
		d = 1.;
		wheelp = &wheel[1];
		for(;;) {
			d += *wheelp;
			ifdiv(d,n)
				try(d);
			if(++wheelp >= wheel + sizeof(wheel)/sizeof(*wheel)) {
				wheelp = wheel;
				if(d>s)
					break;
			}
		}
		if(n>1)
			printf("     %.0f\n",n);
		printf("\n");
	} while(argc<=1);
	return(0);
}

try(p)
double p;
{
	static double quot;
	while(modf(n/p,&quot)==0) {
		printf("     %.0f\n",p);
		n = quot;
	}
	s = sqrt(n) + 1;
}
