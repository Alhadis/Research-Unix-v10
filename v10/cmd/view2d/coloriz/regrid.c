#include <stdio.h>
#include "view2d.h"
#define NMAX 1030
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
Frame *here, *tween, *there, *tframe;

short nx, ny;  /* input grid */
short mx, my;  /* output grid */
float x[NMAX], y[NMAX];
int detrend;

main(argc, argv)
  int argc;
  char **argv;
{
  int i, j;
  short *in;  /* input values */
  short *mid; /* intermediate values */
  int nfr;    /* number of frames to be drawn */
  int ifr;
  double tsi, tei;  /* starting and ending time to be imposed */
  int warpi = 0;  /* internal timewarp */
  double twtime, timestep, t;
  short *a, *b, *c;
  float *d;   /* trend plane */
  int fd;
  int tinterp = 0;
  int ulim = 0;
  int timebar = 0;
  int firstfr = 1;  /* reading first frame? */
  float ufmin, ufmax;
  char *Malloc();

  timewarp = 0;
  detrend = 0;
  verbose = 0;
  progname = argv[0];
  here  = frame+0;
  tween = frame+1;
  there = frame+2;
  nfr = -1; mx = -1; my = -1;   /* flag uninitialized values */

  for(argc--, argv++; *argv; argv++){
    if(**argv == '-' ){
      switch(argv[0][1]) {
      case 'b':
	i = sscanf(&argv[0][2], "%d", &timebar);
	if(i!=1) timebar = -1;
	break;
      case 'f':
        i = sscanf(&argv[0][2], "%d", &nfr);
        tinterp = 1;
        break;
      case 'm':
        i = sscanf(&argv[0][2], "%e, %e", &ufmin, &ufmax);
        if(i!=2) error("bad fmin,fmax");
        ulim++;
        break;
      case 'n':
        i = sscanf(&argv[0][2], "%hd, %hd", &mx, &my);
        if(i == 1) { my = mx; i = 2; }
        if((i!=2)||(mx<0)||(mx>NMAX)||(my<0)||(my>NMAX)) error("bad NX, NY");
        break;
      case 'r':
        detrend++;
        break;
      case 't':
        warpi = sscanf(&argv[0][2], "%E, %E", &tsi, &tei);
        if(warpi==0) error("bad TS,TE");
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

  if(!tinterp){  /* if not interpolating, can use rd.c's -t processing */
    timewarp=warpi;
    ts = tsi;
    if(timewarp==2) te = tei;
  }
  rd2dh(fd,&nx,&ny);
  if(warpi<=0) tsi = ts;
  if(warpi<=1) tei = te;
  if(ulim){
    rd.fmin = ufmin;
    rd.fmax = ufmax;
    g_rang2();
  }
  if(nfr==-1) nfr = rd.nfr;
  if((nx > NMAX) || (ny > NMAX)) error("n too large");
  if(mx==-1){ mx = nx; my = ny; }
  if(verbose){
    fprintf(stderr,"ts=%g te=%g\n",tsi,tei);
    fprintf(stderr,"nx=%d ny=%d nframes=%d\n",nx,ny,rd.nfr);
    fprintf(stderr,"mx=%d my=%d\n",mx,my);
    fprintf(stderr,"global starting_time=%g ending_time=%g\n",rd.ts,rd.te);
    fprintf(stderr,"fmin=%g fmax=%g\n",rd.fmin,rd.fmax);
    fprintf(stderr,"pmin=%d pmax=%d\n",rd.pmin,rd.pmax);
    fprintf(stderr,"u=%d v=%d\n",rd.u,rd.v);
  }
  if(tinterp&&(rd.nfr==1)) error("can't interpolate %d frames!",rd.nfr);
  if(timebar&&(rd.ts==rd.te)){
     fprintf(stderr,"couldn't determine boundary times for timebar\n");
     timebar = 0;
  }
  for(i=0; i<nx; i++) x[i] = i/(nx-1.);
  for(j=0; j<ny; j++) y[j] = j/(ny-1.);

  in = (short *)Malloc(nx*ny*sizeof(short));
  mid = (short *)Malloc(mx*ny*sizeof(short));
  if(timebar){
    if(timebar==-1) timebar = my/30;
    if(timebar<1) timebar = 1;
    i = mx*(my+3*timebar)*sizeof(short);
  }else{
    i = mx*my*sizeof(short);
  }
  j = (nx*ny*sizeof(short)+mx*ny*sizeof(short)+3*i)/1000;
  if(detrend) j += (8*mx*my)*sizeof(float)/1000;
  if(j>900) fprintf(stderr,"using %dkB workspace\n",j);
  here->p  = 3*timebar*mx+(short *)Malloc(i);
  tween->p = 3*timebar*mx+(short *)Malloc(i);
  there->p = 3*timebar*mx+(short *)Malloc(i);
  if(detrend) d = (float *)Malloc(mx*my*sizeof(float));

  if(tinterp){
    ifr = 1;
    twtime = tsi;
    timestep = (tei-tsi)/((nfr-1.)+1e-20);
    rdframe(here,in,mid);
    if(detrend){ fit(here->p,d); rmfit(here->p,d); }
    /* should fit first interpolated frame, but that's a nuisance */
    rdframe(there,in,mid);
    if(detrend) rmfit(there->p,d);
    while( ifr<=nfr ){
      while( twtime>there->time ){
        tframe = there; there = here; here = tframe; /* swap */
        if( rdframe(there,in,mid)==0 ) error("unexpected EOF!");
        if(detrend) rmfit(there->p,d);
      }
      t = (twtime-here->time)/((there->time-here->time)+1e-30);
      if(verbose)
        fprintf(stderr,"time=%g %g(%g,%g)\n",twtime,t,here->time,there->time);
      if( (t<0) || (t>1) ){
        fprintf(stderr,"here=%g there=%g\n",here->time,there->time);
        error("can't happen: extrap t=%g twtime=%g",t,twtime);
        }
      for(i=mx*my, a=here->p, b=there->p, c=tween->p; i>0; i--){
        if( (*a < -BIG)||(*b < -BIG) ){
          *c++ = -BIG-1;  a++; b++;
        }else{
          *c++ = (1-t) * *a++ + t * *b++;
        }
      }
      wrframe(twtime,tween->p,timebar);
      ifr++;
      twtime = tsi+(ifr-1)*timestep;
      if(twtime>te) twtime=te;
      if(twtime>rd.te) break;
    }
  }else{  /* no time interpolation */
    while( rdframe(here,in,mid) ){
      if(verbose) fprintf(stderr,"time=%g\n",here->time);
      if(detrend){
        if(firstfr){ fit(here->p,d); firstfr=0; }
        rmfit(here->p,d);
      }
      wrframe(here->time,here->p,timebar);
    }
  }
  exit(0);
}


wrframe(t,p,timebar)
  double t;
  short p[];
  int timebar;
{
  if(detrend){
    view2d(1,mx,my,t,rd.u,rd.v,0,0,0,p);
  }else{
    if(timebar){
      int i,j;
      int k = mx*(t-rd.ts)/(rd.te-rd.ts);
      short *q;
      short space = -BIG-1;  /* black */
      short bar = rd.pmin + .8*((int)rd.pmax-rd.pmin);   /* bar color */
      short base = rd.pmin;   /* base-bar color */
      q = &p[-3*timebar*mx];
      for(i=0;i<timebar;i++){
        for(j=0;j<k;j++){ *q++ = bar; }
        for(j=k;j<mx;j++){ *q++ = base; }
      }
      for(i=0;i<2*timebar*mx;i++){ *q++ = space; }
    }
    view2d(1,mx,my+3*timebar,t,rd.u,rd.v,1,rd.pmin,rd.pmax,&p[-3*timebar*mx]);
  }
}


int                           /* returns 0 on EOF */
rdframe ( h, u, v )
  Frame *h;
  short *u;
  short *v;
{
  int i, ii, j, k, m;
  float s, t;
  short *w;
  int umin, umax;

 if( rd2di( &h->time, u ) == 0 ) return(0);

 if((mx==nx)&&(my==ny)){
  w = h->p;
  m=nx*ny;
  for(i = 0; i < m; i++){ *w++ = *u++; }
 }else{
  /* interpolate in x */
  ii = 0;
  for(i = 0; i < mx; i++){
    t = i/(mx-1.);
    while( (ii<nx-1) && (t>=x[ii+1]) ) ii++;
    if(t == x[ii]) {
      for(j = 0; j < ny; j++) {
        v[i+j*mx] = u[ii+j*nx];
      }
    }else{
      s = (t-x[ii])/(x[ii+1]-x[ii]);
      for(j = 0; j < ny; j++){
        k = ii+j*nx;
        if( (u[k] < -BIG)||(u[k+1] < -BIG) ){
          v[i+j*mx] = -BIG-1;
         }else{
          v[i+j*mx] = (u[k] + s*(u[k+1]-u[k]));
         }
      }
    }
  }

  /* interpolate in y */
  w = h->p;
  ii = 0;
  for(i = 0; i < my; i++){
    t = i/(my-1.);
    while( (ii<ny-1) && (t>=y[ii+1]) ) ii++;
    if(t == y[ii]){
      for(j = 0; j < mx; j++){
        w[j+i*mx] = v[j+ii*mx];
      }
    }else{
      s = (t-y[ii])/(y[ii+1]-y[ii]);
      for(j = 0; j < mx; j++){
        k = j+ii*mx;
        if( (v[k] < -BIG)||(v[k+mx] < -BIG) ){
          w[j+i*mx] = -BIG-1;
         }else{
          w[j+i*mx] = (v[k] + s*(v[k+mx]-v[k]));
         }
      }
    }
  }
 }
  return(1);
}


fit(p,d)
  short *p;
  float *d;  /* l2 fit to p */
{
  float *z;  /* floating copy of first frame data */
  float *zz; /* pointer into z */
  short *l;  /* shares storage with z */
  float *work, plane[3];  /* workspace for tl2fit */
  float x, y;
  float u2, v2, pow2();
  int i, j;
  int imx, imy, imxmy;
  char *Malloc();

  i = mx*my*sizeof(float);
  z        = (float *)Malloc(i);
  work     = (float *)Malloc(6*i);
  imx = mx; imy = my;  imxmy = mx*my;
  u2 = rd.u;  v2 = pow2(rd.v);
  for( i=mx*my, zz=z; i>0; i--){
    *zz++ = ((*p++)-u2)*v2;
  }
  tl2fit_(&imx,&imy,&imxmy,z,work,plane);
  free(z); free(work);
  if(verbose) fprintf(stderr,"l2 plane: %g + %g x + %g y for 0<=x,y<=1\n",
    plane[0], plane[1], plane[2]);

  for(j=1; j<=my; j++){
    y = (j-1)/(my-1.);
    for(i=1; i<=mx; i++){
      x = (i-1)/(mx-1.);
      *d++ = (plane[0] + x*plane[1] + y*plane[2])/v2 + u2;
    }
  }
}

rmfit(p,d)            /*  p -= d */
  short *p;
  float *d;
{
  int i;
  float f;
  short *p0, *p1;
  p0 = p;
  p1 = p+mx*my-1;
  for(i=mx*my; i>0; i--, p++, d++){
    f = *p - *d;
    *p = (f>BIG)? BIG: ( (f<-BIG)? -BIG: f );
  }
}

