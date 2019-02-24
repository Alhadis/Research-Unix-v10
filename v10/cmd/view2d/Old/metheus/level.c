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
  register char *p;
  register int prange;
  register short *q;
  register int r, rmin;
  short *qend;
  char *pict;
  int ip;
  short *v;
  char *map;
  int black;
  int ncolor = 32;
  int ps, psx;
  int wid;
  int coldstart = 1;
  int blanking = 1;
  int singstep = 0;
  int staticd = 0;
  int holly = 0;
  int i, j;
  short nx, ny, fru, frv;
  int frame, nframe, fx, fy, fz;
  int mapr[256], mapg[256], mapb[256];
  int period = 5;
  int printcol = 0;
  long ring, ttim;
  int nf;
  double frtime;
  double tim[201];
  char *Malloc();
  FILE *terminal = NULL;
  char junk[150];
  double cleft, cright;
  timewarp = -1;
  verbose = 0;
  progname=argv[0];
  map = NULL;

  for(argc--, argv++; *argv && (**argv == '-' ); argv++){
    switch(argv[0][1]){
     case 'S':
       switch(argv[0][2]){
        case 'a': 
        case 'A': singstep = 1;
                  terminal=NULL;
                  break;
        case 'f': 
        case 'F': singstep = 1;
                  terminal = fopen("/dev/tty","r");
                  if(terminal==NULL) error("can't open /dev/tty");
                  break;
        case 'M': singstep = 1;
                  if(argv[0][3]!='\0') holly = atoi(&argv[0][3]);
                  else holly = 1;
                  break;
        case 'S': staticd = 1;
                  blanking = 1-blanking;
                  break;
       }
       break;
     case 'b':
       blanking = 1-blanking;
       break;
     case 'c':
       ncolor = atoi(&argv[0][2]);
       break;
     case 'm':
       map = &argv[0][2];
       break;
     case 'p':
       sscanf(&argv[0][2], "%d",&period);
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
     case 'w':
       coldstart = 0;
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
    if(holly) fprintf(stderr,"snap %d\n",holly);
    fprintf(stderr,"blanking=%d\n",blanking);
  }
  if((nx>1280)||(ny>1024)) error("picture too large %d,%d\n",nx,ny);
  v = (short *)Malloc(nx*ny*sizeof(short));
  pict = (char *)Malloc(nx*ny*sizeof(char));
  minit(coldstart);

  j = (4*nx>5*ny) ? (4*nx)/5 : ny;
  if     ( j>512 ){ ps=1024; wid= 1; }
  else if( j>256 ){ ps= 512; wid= 2; }
  else if( j>128 ){ ps= 256; wid= 4; }
  else if( j> 64 ){ ps= 128; wid= 8; }
  else            { ps=  64; wid=16; }
  psx = 5*ps/4;

  prange = ((int)rd.pmax)-rd.pmin;
  if(prange==0) prange = 1;
  black = cvmap(map,ncolor-1,mapr,mapg,mapb);
  if(printcol){
    for(j=0; j<=ncolor-1; j++){
      cleft  = rd.fmin +     j*(rd.fmax-rd.fmin)/ncolor;
      cright = rd.fmin + (j+1)*(rd.fmax-rd.fmin)/ncolor;
      fprintf(stderr,"color %4d%4d%4d = %g to %g\n",
          mapr[j], mapg[j], mapb[j], cleft, cright );
    }
  }

  frame=blanking;
  while(rd2di(&frtime,v)){
    if(verbose) fprintf(stderr,"time=%.9g frame %d\n",frtime,frame);
    if(frame<200) tim[frame]=frtime;
    if(coldstart!=0){
      fx = frame%wid;
      fy = wid-1 - (frame/wid)%wid;
      fz = frame/(wid*wid);
      if(singstep){ fx=1; fy=1; fz=1; }
      if(fz>0) error("too many frames %d  time=%g",frame,frtime);
      p = pict;
      q = v;
      rmin = rd.pmin;
      for(i=0; i<ny; i++){
        q = v+nx*(ny-1-i);
        qend = q+nx;
        for(; q<qend; q++){
          r = *q;
          if( r < -BIG ){ *p = black; }
           else{
            *p = ((r-rmin)*ncolor)/prange;
            if(*p>=ncolor) *p = ncolor-1;
           }
          p++;
        }
      }
      image(pict, psx*fx+(psx-nx)/2, ps*fy, psx*fx+(psx-nx)/2 + nx-1, ps*fy + ny-1);
      if(singstep){
        if(holly){
          sprintf(junk,"snap %d",holly);
          system(junk);
        }else if(terminal==NULL){
          fprintf(stderr,"time=%g frame %d\n",frtime,frame);
        }else{
          fprintf(stderr,"time=%g frame %d   GO? ",frtime,frame);
          fgets(junk,100,terminal);
          if((junk[0]=='s')||(junk[0]=='S')){
            system("snap");
            junk[0]=' ';
          }
          if(sscanf(junk,"%d",&j)==1){
            if(verbose) fprintf(stderr," skipping %d.. ",j);
            frame += j-1;
            rd2dj(j-1);
          }
        }
      }
    }
    frame++;
  }
  nframe = frame-blanking;
  if(verbose) fprintf(stderr,"%d frames\n%d colors\n",nframe,ncolor);
  if(singstep||staticd){ exit(0); }

  if(!staticd){
      j = (4*nx>5*ny) ? (4*nx)/5 : ny;
      if     ( j>512 ){ zoom(0); }
      else if( j>256 ){ zoom(1); }
      else if( j>128 ){ zoom(3); }
      else if( j> 64 ){ zoom(7); }
      else            { zoom(15); }
  }
  if(nframe>1){
    if(nframe>200) nframe = 200;
    period *= 1000;  /* convert to milliseconds */
    ttim = (tim[nframe]-tim[blanking]);
    if(ttim<=0){  /* if user gave unreasonable times, fake it */
      for(i=blanking+1; i<=nframe; i++){
        tim[i] = tim[i-1]+1;
      }
    }
    if(blanking) tim[0] = 2*tim[1]-tim[2];
    tim[nframe+1] = 2*tim[nframe]-tim[nframe-1];
    ttim = (tim[nframe]-tim[1] + (tim[2]-tim[1]+tim[nframe]-tim[nframe-1])/2);
  }
  frame=blanking;
  while(1){  /**** infinite loop ****/
    fx = frame%wid;
    fy = wid-1 - (frame/wid)%wid;
    fz = frame/(wid*wid);
    pan(psx*fx,ps*fy+ny);
    if(nframe==1) exit(0);
    ring = (frame==0)? 500: (period*(tim[frame+1]-tim[frame-1])) / (2*ttim);
      /* conceptually, display frame i from halfway between (i-1,i) until
         halfway between (i,i+1).    ring = delay time in ms. */
    ring = (ring*60)/1000;  /* convert to 1/60 sec ticks */
    while( ring>=120 ){
      nap(119);
      ring -= 119;
     }
    nap(ring);
    if(++frame>nframe-1+blanking) frame = 0;
  }
}
