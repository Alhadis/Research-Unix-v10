#include <stdio.h>
#include <math.h>
#include "view2d.h"

char *progname;
int fd;
int verbose;

main(argc, argv)
  char **argv;
{
  int i, j;
  int npts;
  int nx, ny;
  float f, v2;
  short u, v;
  float fmin, fmax, frange, grange, gmax, gmin;
  char buf[512];
  int hi, lo;
  double time;
  int firstime = 1;
  short *data, *d;
  int rmax, rmin, r;
  char *errfil;
  char *Malloc();
  double pow2();
  progname = argv[0];
  verbose = 0;
  if((argv[1][0]=='-')&&(argv[1][1]=='v')) verbose = 1;

  /* open error message file */
  umask(0);
  errfil="/tmp/rks.err";
  if(access(errfil,2)!=0) close(Creat(errfil,0666));
  freopen(errfil,"a",stderr);

  /* open output file */
  fd = Creat(argv[1+verbose],0666);

  while( gets(buf) != NULL ){
    if(buf[0]==0) continue;

    i = sscanf(buf, "%E %d %d %f %f\n", &time, &nx, &ny, &fmin, &fmax);
    if( i != 5 ) error("file scrambled. %d items in:%s\n",i,buf);
    if(verbose) fprintf(stderr, "\n%g %d %d %g %g\n", time, nx, ny, fmin, fmax);
    npts = nx*ny;
    if(firstime){
      data = (short *)Malloc(npts*sizeof(short));
      firstime = 0;
    }

    gmax = (fmax<0)?0:fmax;
    gmin = (fmin>0)?0:fmin;
    grange = gmax - gmin;
    frange = fmax - fmin;
    if(grange<=0){ v=0; u=0; v2=1;}
    else{
      f = 2*BIG-2;
      v = log(grange)/log(2.)-14;
      v2 = pow2(v);
      while( 2*grange < f*v2 ){ v--; v2 /= 2; }
      f = -(gmin+gmax)/(2*v2);
      u = f;
      if( (f<-BIG)||(f>BIG) ){
        fprintf(stderr,"fmin=%g fmax=%g u=%d v=%d\n",fmin,fmax,u,v);
        error("u out of bounds f=%g",f);
      }
    }

    rmax = -9999;
    rmin = 9999;
    for(d = data, i = 0; i < npts; i++, d++){
      hi = getchar()-33;
      if(hi < 0) hi = getchar()-33;  /* discard newline */
      lo = getchar()-33;
      if( (hi=='~'-33) && (lo=='~'-33) ){
        *d = -BIG-1;  /* out of bounds */
      }else{
        r = hi*64+lo;
        if(r>rmax) rmax=r;
        if(r<rmin) rmin=r;
        *d = u + (short)((fmin + (hi*64+lo)*frange/4095)/v2);
      }
    }
    if(verbose) fprintf(stderr,"rmin=%d rmax=%d\n",rmin,rmax);
    view2d(fd,nx,ny,time,u,v,0,0,0,data);
  }
  exit(0);
}
