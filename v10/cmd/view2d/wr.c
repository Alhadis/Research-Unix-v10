#include <stdio.h>
#include "view2d.h"

view2d(fd,nx,ny,time,u,v,fixuv,pmin,pmax,p)
  short fd, nx, ny, u, v, fixuv, pmin, pmax, p[];
  double time;
{
  Header wr;
  wr.magic1 = MAGIC;
  wr.magic2 = 0;
  wr.ver = 3;
  wr.nx = nx;
  wr.ny = ny;
  wr.u = u;
  wr.v = v;
  wr.fixuv = fixuv;
  wr.pmin = pmin;
  wr.pmax = pmax;
  sprintf(wr.time, "%15.8g", time);
  write(fd,&wr,sizeof(wr));
  write(fd,p,nx*ny*sizeof(short));
}
