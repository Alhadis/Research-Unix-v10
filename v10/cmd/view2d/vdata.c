#include	<stdio.h>
#include	<math.h>
#include	"view2d.h"
#define MAXPTS 4096

int NX = 16, NY = 16;
char *progname;

typedef struct Vector
{
	float x, y, z;
} Vector;

typedef struct Frame
{
	Vector data[MAXPTS];
	double time;
        int npts;
} Frame;
Frame frame[40];
int interp = 1;

float grid[200][200];
float xmin, xmax, ymin, ymax;
float xstep, ystep;
short *data;
int verbose;

main(argc, argv)
	char **argv;
{
	register i, j;
	int nframes;
	float x;

	verbose = 0;
	progname = *argv;
	for(argc--, argv++; **argv == '-'; argv++)
		switch(argv[0][1])
		{
		case 'n':
			NY = -1;
			sscanf(&argv[0][2], "%d,%d", &NX, &NY);
			if(NY < 0) NY = NX;
			break;
		case 'c':   /* piecewise constant */
			interp = 2;
			break;
		case 's':   /* scatterplot on black background */
			interp = 3;
			break;
		case 'i':
			interp = 0;
			break;
		case 'v':
			verbose = 1;
			break;
		default:
			error("usage: vdata -c -s -i -v -nNX,NY file");
		}
	if(*argv)
		if(freopen(*argv, "r", stdin)==NULL)
			error("freopen(%s) failed",*argv);
	if((NX>200)||(NY>200)) error("NX, NY must be <= 200");
	data = (short *)Malloc(NX*NY*sizeof(short));

	if(interp)
	{
		xmax = ymax = -HUGE;
		xmin = ymin = HUGE;
	}
	for(nframes = 0; readf(&frame[nframes]); nframes++){
	  if(nframes>38) error("too many frames");
        }
	if(nframes<=0) error("empty input");
	if(verbose)fprintf(stderr, "nx=%d ny=%d nframes=%d\n", NX, NY, nframes);

	if(interp)
	{
		xstep = (xmax-xmin)/(NX-1);
		ystep = (ymax-ymin)/(NY-1);
		if(verbose)fprintf(stderr, "%.5g<=x<=%.5g  %.5g<=y<=%.5g\n",
			xmin, xmax, ymin, ymax);
	}
	else
	{
		xmin = 0; xstep = 1; xmax = xmin + (NX-1);
		ymin = 0; ystep = 1; ymax = ymin + (NY-1);
	}
	for(i = 0; i < nframes; i++){
		fit(&frame[i]);
		out(frame[i].time,frame[i].npts);
	}
	exit(0);
}

readf(s)
	Frame *s;
{
	register Vector *v;
	register i;
	float f;

	if(scanf("%d %E", &s->npts, &s->time) != 2) return(0);
	if(verbose) fprintf(stderr,"%d %g\n",s->npts,s->time);
	if(s->npts>MAXPTS) error("too many data points");
	if(interp)
		for(v = s->data, i = 0; i < s->npts; i++, v++)
		{
			scanf("%f %f %f", &v->x, &v->y, &v->z);
			if(v->x < xmin) xmin = v->x;
			if(v->x > xmax) xmax = v->x;
			if(v->y < ymin) ymin = v->y;
			if(v->y > ymax) ymax = v->y;
		}
	else
		for(v = s->data, i = 0; i < s->npts; i++, v++)
		{
			scanf("%f", &v->z);
		}
	return(1);
}

fit(s)
	Frame *s;
{
	register Vector *v;
	Vector h;
	float g, x, y;
	register i, j, k;
	Vector *c1, *c2, *c3;
	float d, d1, d2, d3;

	if( (interp==1) || (interp==2) ) {
		for(j = 0; j < NY; j++)
			for(i = 0; i < NX; i++) {
				x = xmin + i*xstep;
				y = ymin + j*ystep;
				g = grid[i][j];
				d1 = d2 = d3 = HUGE;
				for(v = s->data, k = 0; k < s->npts; k++, v++) {
					d = (v->x - x)*(v->x - x) +
						(v->y - y)*(v->y - y);
					if(d < d1) {
						c3 = c2; d3 = d2;
						c2 = c1; d2 = d1;
						c1 = v; d1 = d;
					} else if(d < d2) {
						c3 = c2; d3 = d2;
						c2 = v; d2 = d;
					} else if(d < d3) {
						c3 = v; d3 = d;
					}
				}
				/* c1 c2 c3 three closest points */
				if(interp==1) {
					h.x = x; h.y = y; h.z = g;
					interp_(&h, c1, c2, c3);
					grid[i][j] = h.z;
				} else if(interp==2)
					grid[i][j] = c1->z;
			}
	} else if( interp==3 ) {
		for(j = 0; j < NY; j++)
			for(i = 0; i < NX; i++)
				grid[i][j] = -1e26;
		for(v = s->data, k = 0; k < s->npts; k++, v++) {
			i = (NX-1)*(v->x-xmin)/(xmax-xmin);
			j = (NY-1)*(v->y-ymin)/(ymax-ymin);
			if((i<0)||(i>=NX)||(j<0)||(j>=NY))
				error("can't: i,j=%d,%d\n",i,j);
			grid[i][j] = v->z;
		}
	} else {  /* option -i */
		for(v = s->data, j = 0; j < NY; j++)
			for(i = 0; i < NX; i++, v++)
				grid[i][j] = v->z;
	}
}

out(tim,npt)
  double tim;
  int npt;
{
	register short *d, i, j;
	register float zmax, zmin, zrange, f, v2;
	short u, v;
	extern double pow2();

	zmin = HUGE;
	zmax = -HUGE;
	for(j = 0; j < NY; j++)
		for(i = 0; i < NX; i++){
			if(grid[i][j] < -1e25 ) continue;
			if(grid[i][j] < zmin) zmin = grid[i][j];
			if(grid[i][j] > zmax) zmax = grid[i][j];
		}

	if(verbose) fprintf(stderr, "time=%g npts=%d %g<=z<=%g\n",
		 tim, npt, zmin, zmax);
	zmax = (zmax<0)?0:zmax;
	zmin = (zmin>0)?0:zmin;
	zrange = zmax - zmin;
	if(zrange<=0)
		v=0, u=0, v2=1, f=0;
	else{
		f = 2*BIG-2;
		v = log(zrange)/log(2.)-14;
		v2 = pow2(v);
		while( 2*zrange < f*v2 ){ v--; v2 /= 2; }
		f = -(zmin+zmax)/(2*v2);
		u = f;
		if( (f<-BIG)||(f>BIG) ){
			if(verbose)fprintf(stderr,"zmin=%g zmax=%g u=%d v=%d\n",
				zmin,zmax,u,v);
			error("u out of bounds f=%g",f);
		}
	}
	d = data;
	for(j = 0; j < NY; j++)
		for(i = 0; i < NX; i++)
			if(grid[i][j] < -1e25){ *d++ = -BIG-1; }
			 else{ *d++ = f + grid[i][j]/v2; }
	view2d(1, NX, NY, tim, u, v, 0, 0, 0, data);
}
