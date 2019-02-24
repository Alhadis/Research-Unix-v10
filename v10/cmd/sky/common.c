#define APPARENT 1
#define OCCULT	2
#define LATUD	4
#define MEAN	8
#define NOSTAR	16
#define STCATL	32
#define KITCH	64
#define XYZ	128
#define GEO	256
#define HELIO	512
#define ECLIPTIC 1024
#define TOPO	2048

int flags;
int dmo[];

char	startab[20];
char	line[100];

double pi, radian, radsec;
double nlat, wlong, elev;
double glat, glong, erad;
double gst, psi, eps, dpsi, deps, obliq, tobliq;
double ecc, incl, argp, mrad, anom, motion;
double xms, yms, zms;
double node;
double lambda, beta, rad;
double semi, mag;
double xdot, ydot, zdot;
double ldot, bdot, rdot;
double day;
double eday, capt, capt2, capt3;
double deltat;
double ra, decl2, az, el;
double lha, alpha, delta, hp, semi2;
double decl;
double geolam, geobet, georad;
double epoch, da, dd, px;
double rah, ram, ras;
long agc;
int const;
double dday, dmin, dsec;
double rps, shra, shdecl, sunra, sundec, sunsd;
double moonra, moonde, moonsd;

char *object;

double sin(), cos(), atan();
double sqrt();
double log();
double atan2();
double atof();
double floor();
double fmod();
double fabs();
double readate();
double sin();
double cos();
double floor();
double convdate();
char *skip();
