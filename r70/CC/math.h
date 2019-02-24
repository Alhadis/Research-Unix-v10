extern "C" {
	extern double fabs(double),
		floor(double),
		ceil(double),
		modf(double, double*),
		frexp(double, int*),
		ldexp(double, int),
		sqrt(double),
		hypot(double, double),
		cabs(double, double),
		atof(const char*),
		sin(double),
		cos(double),
		tan(double),
		asin(double),
		acos(double),
		atan(double),
		atan2(double, double),
		exp(double),
		log(double),
		log10(double),
		pow(double, double),
		sinh(double),
		cosh(double),
		tanh(double),
		gamma(double),
		besj0(double),
		besj1(double), besjn(int, double), y0(double),
		besy1(double), besyn(int, double),
		erf(double),
		erfc(double),
		frand(),
		fmod(double, double);
}


#define	MAXFLOAT	170141173319264429905852091742258462720.0
#define	MAXDOUBLE	170141183460469229370504062281061498880.0
#define	HUGE		MAXFLOAT	/* kludge */
#define	HUGE_VAL	MAXDOUBLE
#define PI	3.1415926535897932384626433832795028841971693993751
#define	PI2	(2*PI)
