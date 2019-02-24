extern double fabs(), floor(), ceil(), fmod();
extern double frexp(), ldexp(), modf();
extern double sqrt(), hypot(), atof(), strtod();
extern double sin(), cos(), tan(), asin(), acos(), atan(), atan2();
extern double exp(), log(), log10(), pow();
extern double sinh(), cosh(), tanh();
extern double gamma();
extern double besj0(), besj1(), besjn(), besy0(), besy1(), besyn();
extern double frand();
extern double erf(), erfc();

#define	MAXFLOAT	170141173319264429905852091742258462720.0
#define	MAXDOUBLE	170141183460469229370504062281061498880.0
#define	HUGE		MAXFLOAT	/* kludge */
#define	HUGE_VAL	MAXDOUBLE
#define PI	3.1415926535897932384626433832795028841971693993751
#define	PI2	(2*PI)
