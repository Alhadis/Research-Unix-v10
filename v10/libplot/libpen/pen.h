#include <stdio.h>
#include <math.h>
#include <sgtty.h>
#define UP 0
#define DOWN 1
#define QUANTUM 10
#define penUp()	{if(penstate==DOWN)fprintf(pltout,"PU "); penstate=UP;}
#define penDown()	{if(penstate==UP)fprintf(pltout,"PD ");penstate=DOWN;}
extern int penstate;
extern FILE *pltout, *pltin;
extern int res;
float theta();
extern struct penvir {
	float xmax, ymax;
	float xmin, ymin;
	float copyx, copyy;
	float p1x, p1y;
	float p2x, p2y;
	float charsz;
	float pslant;
	float scale;
	int pgap;
	int pentype;
	int pen;
	int brush;
} *e0, *e1, E[], *esave;
extern double DX, DY;
struct seg {
	int x, y, X, Y;
	char stat;
};
#define SCX(x)	x*e1->scale
#define SCY(y)	y*e1->scale
#define CH_RATIO	.67
float normx();
float normy();
#define RADIANS 57.3
int internal;
extern char *pens[];
extern int noscale;
#define CH_RATIO	.67
