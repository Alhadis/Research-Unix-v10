#include "pen.h"
double DX, DY;
int res = 0;
struct penvir E[9];
struct penvir *e0 = E, *e1 = &E[1];
struct penvir *esave;
int penstate = UP;
int noscale = 1;
FILE *pltin, *pltout;
char *pens[4] = {
	"black",
	"red",
	"green",
	"blue"
};
ckerr(doit)
{
	register int i;
	static int count;

	if(doit) count = 0;
	if(count >= 250) count = 0;
	if(count++ != 0) return;
	fprintf(pltout,"OE;\n");
	fflush(pltout);
	i = readInt(pltin);
	if(i > 0){
		fprintf(stderr,"plotter error %d\n",i);
		exit(1);
	}
}
float theta(x1,y1,x2,y2,xc,yc,r)
double x1,y1,x2,y2,xc,yc,r;
{
	float radians, degrees;
	radians = atan2((y2-yc), (x2-xc)) - atan2((y1-yc),(x1-xc));
	if(radians < 0)radians += 2*PI;
	if(r < 0)radians = (2*PI) - radians;
	degrees = (radians * 180.) / PI;
	return( (r<0)? -degrees : degrees);
}
int
readInt(f)
FILE *f;
{
	register i, c;
	int sign = 1;
	i = 0;
	for(;;) {
		c = getc(f);
		if(c == EOF) {
			fprintf(stderr, "eof on plotter\n");
			exit(1);
		}
		c &= 0177;
		if(c == '-'){
			sign = -1;
			c = getc(f) & 0177;
			break;
		}
		if(c >= '0' && c <= '9')
			break;
	}
	while(c >= '0' && c <= '9') {
		i = 10*i + c - '0';
		c = getc(f) & 0177;
	}
	i *= sign;
	return(i);
}
sscpy(f,t)
struct penvir *f, *t;
{
	t->scale = f->scale;
	t->xmax = f->xmax;
	t->ymax = f->ymax;
	t->xmin = f->xmin;
	t->ymin = f->ymin;
	t->copyx = f->copyx;
	t->copyy = f->copyy;
	t->p1x = f->p1x;
	t->p1y = f->p1y;
	t->p2x = f->p2x;
	t->p2y = f->p2y;
	t->charsz = f->charsz;
	t->pen = f->pen;
	t->brush = f->brush;
	t->pentype = f->pentype;
	t->pslant = f->pslant;
	t->pgap = f->pgap;
}
float
normx(x)
double x;
{
	return(x);
}
float
normy(y)
double y;
{
	return(y);
}
unorm(){}
