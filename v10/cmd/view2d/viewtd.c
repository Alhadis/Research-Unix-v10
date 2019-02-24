/*  convert from view2d to td format  */

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
      }
    }else{
      if(fd) error("can only read one file");
      fd = Open(*argv,0);
    }
  }

  rd2dh(fd,&nx,&ny);
  if((timewarp>0)&&(verbose))
     fprintf(stderr,"timewarp=%d ts=%g te=%g\n",timewarp,ts,te);
  if( nx != ny ) error("picture must be square");
  i = nx*ny*sizeof(short);
  here->p = (short *)Malloc(i);

  rd2di(&here->time,here->p);
  wrframe(here->time,here->p);

  exit(0);
}


wrframe(t,p)
  double t;
  short *p;
{
  float *b;
  short r;
  int i,j,s;
  char *malloc();
  s = nx*ny*sizeof(float);
  b = (float *)malloc(s);
  if(!b){fprintf(stderr,"no memory\n");exit(1);}
  for(j=0; j<ny; j++){
    for(i=0; i<nx; i++){
	r = *p++;
	if(r < -BIG) r=rd.pmin-1;
	b[i*ny+j] = r;
    }
  }
  write(1,b,s);
  free((char *)b);
}
