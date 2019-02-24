/* undocumented feature:
   -clx,x,x,x  sets contour levels at x, x, x, and x.   up to 8 levels allowed.
*/
#include <stdio.h>
#include <math.h>
#include "view2d.h"

extern Rd2d rd;
char *progname;
short timewarp;
double ts, te;

main(argc,argv)
  int argc;
  char **argv;
{
  float *p, *pict;
  float u2, v2, pow2();
  short *v, *q;
  char *map;
  int prange;
  int ncontour = 6;
  int i, j, r;
  short nx, ny, fru, frv;
  int nf;
  double frtime;
  int zero;
  int g2nx, g2ny;
  float g2s[6], g2d[4], g2c[300];
  float NaN;
  int explcont = 0;
  timewarp = 0;
  progname=argv[0];

  for(argc--, argv++; *argv && (**argv == '-' ); argv++){
    switch(argv[0][1]){
     case 'c':
       if(argv[0][2]=='l'){
         ncontour = sscanf(&(argv[0][3]),"%f,%f,%f,%f,%f,%f,%f,%f",
            g2c+0,g2c+1,g2c+2,g2c+3,g2c+4,g2c+5,g2c+6,g2c+7,g2c+8);
         explcont = 1;
         break;
       }
       ncontour = atoi(&argv[0][2]);
       break;
     case 't':
       i = sscanf(&argv[0][2], "%hd, %hd", &ts, &te);
       if(i==0) error("bad TS,TE");
       timewarp = i;
       break;
     case 'T':
       break;
     default:
     err:
       fprintf(stderr,"unrecognized option: %s\n",argv[0]);
       exit(1);
     }
   }

  rd2dh(0,&nx,&ny);
  v = (short *)Malloc(nx*ny*sizeof(short));
  pict = (float *)Malloc(nx*ny*sizeof(float));

  g2open_();
  g2d[0] = 0;
  g2d[1] = 0;
  g2d[2] = nx-1;
  g2d[3] = ny-1;
  i = 4;
  g2sce_(g2s,g2d,&i);
  g2nx = nx;
  g2ny = ny;
  fprintf(stderr,"contours at:\n");
  for( i=1; i<=ncontour; i++){
    if(!explcont)
      g2c[i-1] = rd.fmin + i*(rd.fmax-rd.fmin)/(ncontour+1);
    fprintf(stderr," %g\n",g2c[i-1]);
  }

  /* for a not-a-number flag, choose a value below all the data */
  NaN = rd.fmin;
  if(NaN > -1) NaN = -1;
  NaN *= 2;

  u2 = rd.u;
  v2 = pow2(rd.v);
  while(rd2di(&frtime,v)){
    p = pict;
    q = v;
    for(i=0; i<nx*ny; i++){
      r = *q;
      if( r < -BIG ){ *p = NaN; }
       else{
        *p = (r-u2)*v2;
       }
      p++; q++;
    }
    g2ff_();
    g2box_(g2s,g2d);
    g2co_(g2s,g2d,&g2nx,&g2ny,pict,&ncontour,g2c,&NaN);
  }
  g2clos_();
}
