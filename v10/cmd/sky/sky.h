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
#define GOOBIE 4096

extern int flags;
extern int dmo[];

extern char	startab[20];
extern char	line[100];

extern double pi, radian, radsec;
extern double nlat, wlong, elev;
extern double glat, glong, erad;
extern double gst, psi, eps, dpsi, deps, obliq, tobliq;
extern double ecc, incl, argp, mrad, anom, motion;
extern double xms, yms, zms;
extern double node;
extern double lambda, beta, rad;
extern double semi, mag;
extern double xdot, ydot, zdot;
extern double ldot, bdot, rdot;
extern double day;
extern double eday, capt, capt2, capt3;
extern double deltat;
extern double ra, decl2, az, el;
extern double lha, alpha, delta, hp, semi2;
extern double decl;
extern double geolam, geobet, georad;
extern double epoch, da, dd, px;
extern double rah, ram, ras;
extern long agc;
extern int const;
extern double dday, dmin, dsec;
extern double rps, shra, shdecl, sunra, sundec, sunsd;
extern double moonra, moonde, moonsd;

extern char *object;

extern double sin(), cos(), atan();
extern double sqrt();
extern double log();
extern double atan2();
extern double atof();
extern double floor();
extern double fmod();
extern double fabs();
extern double readate();
extern double sin();
extern double cos();
extern double floor();
extern double convdate();
extern char *skip();
