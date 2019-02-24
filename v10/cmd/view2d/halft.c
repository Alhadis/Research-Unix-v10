#include <stdio.h>
#include <math.h>
#include "view2d.h"

extern Rd2d rd;
char *progname;
short timewarp;
double ts, te;
int verbose;

main(argc,argv)
  int argc;
  char **argv;
{
  register unsigned char *p;
  register int prange;
  register short *q;
  register int r, rmin, ncolor1;
  int i, j, k;
  unsigned char *pict;
  int ip;
  short *v;
  int ncolor = 10;
  short nx, ny;
  int frame, nframe;
  int nf;
  double frtime;
  char *Malloc();
  char junk[150];
  double cleft, cright;
  int printcol;
  int lim202 = 255;
  int inv_vid = 0;
  timewarp = -1;
  verbose = 0;
  progname=argv[0];

  for(argc--, argv++; *argv && (**argv == '-' ); argv++){
    switch(argv[0][1]){
     case 'S':
       break;
     case 'c':
       ncolor = atoi(&argv[0][2]);
       break;
     case 'i':
       inv_vid = 1;
       break;
     case 't':
       i = sscanf(&argv[0][2], "%he, %he", &ts, &te);
       if(i<=0) error("bad TS,TE");
       timewarp = i;
       break;
     case 'v':
       verbose++;
       if(argv[0][2]=='c') printcol++;
       break;
     case 'T':
     case 'g':
     case 's':
       break;
     default:
     err:
       fprintf(stderr,"unrecognized option: %s\n",argv[0]);
       exit(1);
     }
   }

  if((timewarp>=0)&&verbose)
    fprintf(stderr,"timewarp=%d ts=%g te=%g\n",timewarp,ts,te);
  rd2dh(0,&nx,&ny);
  if((timewarp>=0)&&verbose)
    fprintf(stderr,"timewarp=%d ts=%g te=%g\n",timewarp,ts,te);
  if(verbose){
    fprintf(stderr,"nx=%d ny=%d\n",nx,ny);
    fprintf(stderr,"fmin=%g fmax=%g\n",rd.fmin,rd.fmax);
  }
  v = (short *)Malloc(nx*ny*sizeof(short));
  pict = (unsigned char *)Malloc(nx*ny*sizeof(char));

  prange = ((int)rd.pmax)-rd.pmin;
  if(prange==0) prange = 1;
  if(printcol){
    for(j=0; j<=ncolor-1; j++){
      cleft  = rd.fmin +     j*(rd.fmax-rd.fmin)/ncolor;
      cright = rd.fmin + (j+1)*(rd.fmax-rd.fmin)/ncolor;
      fprintf(stderr,"%g to %g\n", cleft, cright );
    }
  }
  frame=1;
  while(rd2di(&frtime,v)){
    if((timewarp>=0)&&(frtime>te)) break;
    if(verbose) fprintf(stderr,"time=%.9g frame %d\n",frtime,frame);
    p = pict;
    rmin = rd.pmin;
    ncolor1 = ncolor-1;
    for(j=ny-1; j>=0; j--){
      q = v+j*nx;
      for(i=0; i<nx; i++,q++,p++){
        r = *q;
        if( r < -BIG ){ *p = 0; }
         else{
          k = ((r-rmin)*ncolor)/prange;
          if(k>=ncolor) k = ncolor1;
          k = (k*lim202)/ncolor1;
          *p = k;
         }
        if(inv_vid) *p = lim202 - *p;
      }
    }
    Write(1,pict,nx*ny*sizeof(unsigned char));
    frame++;
  }
  nframe = frame-1;
  exit(0);
}
