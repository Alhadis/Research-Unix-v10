/*  convert from view2d to anim format by slicing in x or y  */

#include <stdio.h>
#include "view2d.h"
char *progname;

short timewarp;
double ts, te;
extern Rd2d rd;
int verbose;

typedef struct Frame {
  double time;
  short *p;
} Frame;
Frame frame;
Frame *here;

short nx, ny;  /* input grid */

main(argc, argv)
  int argc;
  char **argv;
{
  int i, j;
  int fd;
  int dir = -1;  /* 1=x, 0=y */
  double cut, atof();
  char *Malloc();

  timewarp = 0;
  verbose = 0;
  progname = argv[0];
  here = &frame;

  for(argc--, argv++; *argv; argv++){
    if(**argv == '-' ){
      switch(argv[0][1]) {
      case 'v':
        verbose++;
        break;
      case 'x':
        dir=1;
	cut=atof(&(argv[0][2]));
        break;
      case 'y':
        dir=0;
	cut=atof(&(argv[0][2]));
        break;
      }
    }else{
      if(fd) error("can only read one file");
      fd = Open(*argv,0);
    }
  }
  if(dir<0) error("must supply -x or -y cutpoint");

  rd2dh(fd,&nx,&ny);
  if((timewarp>0)&&(verbose))
     fprintf(stderr,"timewarp=%d ts=%g te=%g\n",timewarp,ts,te);
  i = nx*ny*sizeof(short);
  here->p = (short *)Malloc(i);

  while(rd2di(&here->time,here->p)){
    wrframe(here->time,here->p,dir,cut);
  }
  exit(0);
}


wrframe(t,p,dir,cut)
  double t;
  short *p;
  int dir;
  double cut;
{
  float *b;
  register short *q;
  int i, j, n, s;
  double t0, t1, v2, out, pow2();
  v2 = pow2(rd.v);
  out = (-BIG-rd.u)*v2;
  n = dir ? ny: nx;
  s = dir ? nx: 1;
  if(dir){ q = p +    (int)(cut*(nx-1));
    }else{ q = p + nx*(int)(cut*(ny-1));}
  if(verbose) fprintf(stderr,"time=%g\n",t,j);
  t0 = out;
  for(i=0; i<n; i++){
	t1 = (*q-rd.u)*v2;
	q += s;
	if((t1>out)&&(t0>out))
		printf("L%d: line %d %f %d %f\n",i-1,i-1,t0,i,t1);
	t0 = t1;
  }
  printf("click time\n");
}
