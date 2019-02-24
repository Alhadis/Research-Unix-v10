/*
  ...dump vector in two-digit base-64 compressed format...
*/

#include <stdio.h>
#include "../view2d.h"
#define INF 1e25

pack(n,f)
  int n;
  float f[];
{
  float *t;
  float fmin = INF, fmax = -INF;
  float floor = -INF;
  for(t=f+n; t!=f; t--){
    if( *t<fmin) fmin= *t;
    if( *t>fmax) fmax= *t;
    }
  printf("%d %e %e\n",n,fmin,fmax);
  v2pack(n,f,fmin,fmax,floor);
}

/* write compressed values */
v2pack(n,data,fmin,fmax,floor)
  int n;
  float *data;
  float fmin, fmax;  /* data range before compression */
  float floor;  /* value to flag out of bounds */
{
  int code, hi, lo, i;
  float frange, slope, t;
  frange = fmax - fmin;
  if(frange==0.) frange=1.;
  slope = 4095 / frange;
  for(i=1; i<=n; i++, data++){
    t = *data;
    if( t <= floor ){
      putchar('~');
      putchar('~');
    }else{
      code = slope*(t-fmin);
         if(code>4095){ code=4095; }else if(code<0){ code = 0; }
      lo = code%64;
      hi = code/64;
      putchar(hi+33);
      putchar(lo+33);
    }
    if(i%38==0) putchar('\n');
  }
  if(n%38!=0) putchar('\n');
}
