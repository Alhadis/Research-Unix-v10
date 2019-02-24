/* From alice!doyle Fri Feb  1 17:19 EST 1985 */
/*  modified slightly by ehg */
/*  based on mvefil.f by rksmith */
#include <stdio.h>
#include <math.h>
#include "view2d.h"

#define max(a,b) 		(a<b ? b : a)
#define min(a,b) 		(a>b ? b : a)
#define abs(x)			(x>=0 ? x : -(x))
#define inf 1.e30

moviefil(fd, nx, ny, time, outside, fa)
int fd, nx, ny;
float time, outside, fa[];
{
  int i;
  int npts;
  float f, v2;
  short u, v;
  float fmin, fmax, grange, gmax, gmin;
  double tim;
  short *data, *d;
  char *malloc();

    tim = time;
    npts = nx*ny;
    data = (short *)malloc((unsigned)npts*sizeof(short));
    if(data==NULL){fprintf(stderr,"out of memory!\n");exit(9);}

    fmin = inf;
    fmax = -inf;
    for (i=0;i<npts;i++) {
      if(fa[i]>outside){
    	fmin = min(fa[i], fmin);
    	fmax = max(fa[i], fmax);
      }
    }
	
    gmax = (fmax<0)?0:fmax;
    gmin = (fmin>0)?0:fmin;
    grange = gmax - gmin;
    if(grange<=0){ v = 0; u=0; }
    else{
      f = 2*BIG-2;
      v = log(grange)/log(2.)-14;
      v2 = pow(2.,(double)v);
      while( 2*grange < f*v2 ){ v--; v2 /= 2; }
      f = -(gmin+gmax)/(2*v2);
      u = f;
      if( (f<-BIG)||(f>BIG) ){
        fprintf(stderr,"fmin=%g fmax=%g u=%d v=%d\n",fmin,fmax,u,v);
        fprintf(stderr,"u out of bounds f=%g",f);
	exit(1);
      }
    }

    for(d = data, i = 0; i < npts; i++, d++){
      if(fa[i]<=outside){
        *d = -BIG-1;  /* out of bounds */
      }else{
        *d = u + fa[i]/v2;
      }
    }
    view2d(fd,nx,ny,tim,u,v,0,0,0,data);
    free((char *)data);
}


fmovie_(nx,ny,time,outside,f)
  int *nx, *ny;
  float *time, *outside, *f;
{
  moviefil(1,*nx,*ny,*time,*outside,f);
}
