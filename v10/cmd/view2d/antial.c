/* version of regrid to do antialiasing */

#include <stdio.h>
#include "view2d.h"
#define NMAX 1026
char *progname;

short timewarp;
double ts, te;
extern Rd2d rd;
int verbose;

typedef struct Frame {
  double time;
  short *p;
} Frame;
Frame frame[3];
Frame *here;

short nx, ny;  /* input grid */
short mx, my;  /* output grid */
int ncolor;

main(argc, argv)
  int argc;
  char **argv;
{
  int i, j;
  short *in;  /* input values */
  int nfr;    /* number of frames to be drawn */
  int ifr;
  double t;
  short *a, *b, *c;
  int fd;
  int ulim = 0;
  float ufmin, ufmax;
  char *Malloc();

  ncolor = 32;
  timewarp = 0;
  verbose = 0;
  progname = argv[0];
  here  = frame+0;
  nfr = -1;

  for(argc--, argv++; *argv; argv++){
    if(**argv == '-' ){
      switch(argv[0][1]) {
      case 'c':
        ncolor = atoi(&argv[0][2]);
        break;
      case 'v':
        verbose++;
        break;
      }
    }else{
      if(fd) error("can only read one file");
      fd = Open(*argv,0);
    }
  }

  rd2dh(fd,&nx,&ny);
  mx = nx/2; my = ny/2;
  if((timewarp>0)&&(verbose))
     fprintf(stderr,"timewarp=%d ts=%g te=%g\n",timewarp,ts,te);
  if(ulim){
    rd.fmin = ufmin;
    rd.fmax = ufmax;
    g_rang2();
  }
  if(nfr==-1) nfr = rd.nfr;
  if((nx > NMAX) || (ny > NMAX)) error("n too large");
  if(verbose){
    fprintf(stderr,"nx=%d ny=%d nframes=%d\n",nx,ny,rd.nfr);
    fprintf(stderr,"mx=%d my=%d\n",mx,my);
    fprintf(stderr,"global starting_time=%g ending_time=%g\n",rd.ts,rd.te);
    fprintf(stderr,"fmin=%g fmax=%g\n",rd.fmin,rd.fmax);
    fprintf(stderr,"pmin=%d pmax=%d\n",rd.pmin,rd.pmax);
    fprintf(stderr,"u=%d v=%d\n",rd.u,rd.v);
  }

  in = (short *)Malloc(nx*ny*sizeof(short));
  i = mx*my*sizeof(short);
  here->p  = (short *)Malloc(i);

  if(rdframe(here,in)==0) error("unexpected empty first frame");
  wrframe(here->time,here->p);
  while( rdframe(here,in) ){
    wrframe(here->time,here->p);
  }

  exit(0);
}


wrframe(t,p)
  double t;
  short p[];
{
    view2d(1,mx,my,t,rd.u,rd.v,1,0,255,p);
}


int                           /* returns 0 on EOF */
rdframe ( h, u )
  Frame *h;
  short *u;
{
  int i, ii, j, jj, hue;
  int ncolor1 = ncolor-1;
  int rmin = rd.pmin;
  int dark, sum;
  short *q, *w;
  short *qend;
  short a,b,c,d;  /* colors of 4 adjacent input pixels */
  int r;
  int prange = ((int)rd.pmax)-rd.pmin;
  int black=255;

  if( rd2di( &h->time, u ) == 0 ) return(0);
  if(verbose) fprintf(stderr,"time=%g\n",h->time);

  /* convert to color, between 0 and ncolor1 */
  q = u; qend=q+nx*ny;
  for(; q<qend; q++){
    r = *q;
    if(r < -BIG){
      *q = black;
    }else{
      *q = ((r-rmin)*ncolor1)/prange;
    }
  }

  w = h->p;
  for(j=0; j<mx; j++){
    for(i=0;  i<my; i++){
      ii = 2*i;
      jj = 2*j;
      q = &u[ii+nx*jj];
      dark = 0;
      sum = 0;
      if( (a=q[0]   )==black){dark++;}else{sum+=a;};
      if( (b=q[1]   )==black){dark++;}else{sum+=b;};
      if( (c=q[nx]  )==black){dark++;}else{sum+=c;};
      if( (d=q[nx+1])==black){dark++;}else{sum+=d;};
      if(dark==4){
        *w = black;
      }else{
        hue = (int)(sum*62.499/((4-dark)*ncolor1));
        *w = hue + dark*64;
      }
      w++;
    }
  }
  return(1);
}

