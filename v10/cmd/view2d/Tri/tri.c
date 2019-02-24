/*
  ...read triangle-format movie file...
    nv
      vx(i), vy(i)   i=1,nv
    nt
      itri(1..3,it)    it=1,nt
    time, fmin, fmax
      fk(i)   i=1,nv    in compressed format

if nv=0 or nt=0, use same values as for previous frame

WARNING.  If vx,vy bounding box changes, so will grid. 
*/

#include <stdio.h>
#include "../view2d.h"
#define INF 1e25
char *progname;

int mx, my;  /* output grid */
float *tr, *f;
int nv, nt;
float *vx, *vy;
float *fk;
int *itri;
float xmin, xmax, ymin, ymax;
int verbose;

main(argc,argv)
  int argc;
  char **argv;
{
  int i, ifr;
  char *Malloc();

  progname = argv[0];
  mx = 5; my = 5;
  verbose = 0;
  for(argc--, argv++; *argv; argv++){
    if(**argv == '-' ){
      switch(argv[0][1]) {
      case 'n':
	i = sscanf(&argv[0][2], "%d,%d", &mx, &my);
	if(i==1) my = mx;
	else if(i!=2) error("options:  -nmx,my");
	break;
      case 'v':
        verbose++;
	break;
      }
    }
  }
  
  i = mx*my*sizeof(float);
  tr = (float *)Malloc(i);
  f  = (float *)Malloc(i);
  nv = 0;
  nt = 0;
  while(fr()){
    if(verbose)fprintf(stderr,"frame %d done\n",ifr++);
  }

  exit(0);
}

fr()
{
  int new_nv, new_nt;
  int i, j;
  float time, fmin, fmax, floor;
  char *Malloc();

  i = scanf("%d",&new_nv);
  if( i == EOF ) return(0);
  if(verbose)fprintf(stderr,"nv=%d\n",new_nv);
  if( nv==0 ){
    if(new_nv==0) error("nv must be nonzero for first frame");
  }else{
    if(new_nv!=0){
      free((char *)fk);
      free((char *)vx);
      free((char *)vy);
    }
  }
  if((nv==0)||(new_nv!=0)){
    nv = new_nv;
    i = nv*sizeof(float);
    fk = (float *)Malloc(i);
    vx = (float *)Malloc(i);
    vy = (float *)Malloc(i);
    xmin = INF; xmax = -INF;
    ymin = INF; ymax = -INF;
    for(j=0; j<nv; j++){
      i = scanf("%e %e",vx+j,vy+j);
      if( i == EOF ) error("unexpected EOF");
      if( i!=2 ) error("bad format");
      if(vx[j]<xmin) xmin=vx[j];
      if(vx[j]>xmax) xmax=vx[j];
      if(vy[j]<ymin) ymin=vy[j];
      if(vy[j]>ymax) ymax=vy[j];
    }
  }

  i = scanf("%d",&new_nt);
  if( i == EOF ) error("unexpected EOF");
  if(verbose)fprintf(stderr,"nt=%d\n",new_nt);
  if( nt==0 ){
    if(new_nt==0) error("automatic triangulation not implemented yet");
  }else{
    if(nt!=0){
      free((char *)itri);
    }
  }
  if((nt==0)||(new_nt!=0)){
    nt = new_nt;
    i = 3*nt*sizeof(int);
    itri = (int *)Malloc(i);
    for(j=0; j<nt; j++){
      i = scanf("%d %d %d",itri+3*j,itri+1+3*j,itri+2+3*j);
      if( i == EOF ) error("unexpected EOF");
      if( i!=3 ) error("bad format");
    }
    macbox_( &nv,vx,vy,&nt,itri,
       &mx,&xmin,&xmax,&my,&ymin,&ymax,tr);
    if(verbose){
      fprintf(stderr,"tr=");
      for(i=0;i<mx*my;i++){
        fprintf(stderr," %f",tr[i]);
      }
      fprintf(stderr,"\n");
    }
  }

  i = scanf("%e %f %f\n", &time, &fmin, &fmax);
  if( i != 3 ) error("bad time,fmin,fmax format  %d",i);

  unpack(nv,fk,fmin,fmax,floor);

  floor = -INF;
  intbox_(&mx,&xmin,&xmax,&my,&ymin,&ymax,f,tr,
     &nv,fk,&floor,vx,vy,itri);
  if(verbose){
    fprintf(stderr,"f=\n");
    for(j=0;j<my;j++){
      for(i=0;i<mx;i++){
        fprintf(stderr," %g",f[i+mx*j]);
      }
      fprintf(stderr,"\n");
    }
  }
  
  moviefil(1,mx,my,time,floor,f);

  return(1);
}

/* read compressed values */
unpack(npts,data,fmin,fmax,floor)
  int npts;
  float *data;
  float fmin, fmax;  /* data range before compression */
  float floor;  /* value to flag out of bounds */
/*  input:   npts,fmin,fmax,floor  */
/*  output:  data  */
{
  int hi, lo;
  float frange = fmax - fmin;
  for(; npts>0; npts--, data++){
    hi = getchar()-33;
    if(hi < 0) hi = getchar()-33;  /* discard newline */
    lo = getchar()-33;
    if( (hi=='~'-33) && (lo=='~'-33) ){
      *data = floor;
    }else{
      *data = fmin + (hi*64+lo)*frange/4095;
    }
  }
}
