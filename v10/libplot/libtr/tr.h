#define SCX(A) ((A - e1->xmin)*e1->scalex  + e1->left)
#define SCY(A) ((A - e1->ymin)*e1->scaley + e1->bottom)
#define PUTFONT printf("\n.ft %s\n.sp -1\n",e1->pfont)
#define PUTSIZE printf("\n.ps %d\n.sp -1\n",e1->psize)
extern struct penvir {
	double left,  bottom;
	double xmin, ymin;
	double scalex, scaley;
	double sidex, sidey;
	double copyx, copyy;
	double quantum;
	double grade;
	double ninches;
	double pslant;
	int pgap;
	int pmode, psize, pbrush;
	char pfont[4];
	} *e0, *e1, *esave;
extern int CH;
#define sSTEP 120
#define NX(A) (int)(((A)*e1->scalex)+.5)
#define LX(A) ((int)((A)*e1->scalex))
#define LY(A) ((int)((A)*e1->scaley))
#define RESET  if(CH++>8) { CH=0; move(e1->copyx,e1->copyy); }
#define DASHSIZE (double)64.
extern int internal;
extern double linespace;
struct seg {
	int x, y, X, Y;
	char stat;
};
#define RADIANS 57.3
#define D202RES 972.
#define APSRES 723.
#define POSTRES	720.
