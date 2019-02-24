#define SCX(A) (A - e1->xmin)*e1->scalex  + e1->left;
#define SCY(A) (A - e1->ymin)*e1->scaley + e1->bottom;
#define unorm(y)	(double)y
extern struct penvir {
	double left, bottom;
	double xmin, ymin;
	double scalex, scaley;
	double sidex, sidey;
	double copyx, copyy;
	double  quantum;
	double grade;
	int pgap;
	double pslant;
	char ppink, pbrush;
	} *e0, *e1, *e2, *esave;
extern int ohiy, ohix, oloy, oextra;
extern int internal;
char bcolor();
#define ESC	033
#define ERASE	014
#define PLOTOUT	037
#define PLOTIN	035
#define RADIAN 57.3
struct seg{
	int x, y, X, Y;
	char stat;
};
