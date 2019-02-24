/****** routines for reading view2d files ******

!!! Must be declared by calling program: !!!
 double ts;  starting time
 double te;  ending time
 short timewarp; == -1  ts, te unused;  avoids seeks if possible
                 ==  0  ts, te set from file
                 ==  1  ts set by calling program, te set from file
                 ==  2  ts, te set by calling  program

another global, declared here in rd.c (with structure definition in view2d.h)
 Rd2d rd;
       short rd.pmin == global min pixel value
       float rd.pmin == global max
       short rd.u == global u value  (origin)
       short rd.v == global v value  (exponent)
       (the following are not set if timewarp==-1)
       int   rd.nfr  == number of frames
       double rd.ts == time of first frame in file
       double rd.te == time of last frame in file

 if p is the pixel value and f is the corresponding user's function value,
       f = (p-u) * 2**v      p = u + f / 2**v
    if p<(-BIG) then the function value is undefined, and the pixel is black

 rd2dh(fd,pny,pny)
    int fd;            (on input) a file descriptor open for read and seek
    short *pnx, *pny;  (on output) size of the images
  Initializes the global variables ts, te, rd. Chooses global scale factor.
  Skips forward through file to time>=ts.

 int
 rd2di(ptime,p)
    double *ptime;   frame time
    short p[];       nx by ny array of pixel values
  Reads a frame of data.
  Returns 0 upon EOF or time>te.

 rd2dj(j)
    int j;
  jumps to j frames forward (or back, if j<0)

At Lynn's request, there used to be a warning message if "time" decreased.
Linda didn't like it, so I removed it.
*/


#include <stdio.h>
#include "view2d.h"
extern short timewarp;
extern double ts, te;
Rd2d rd;

Header hd;
Header2 hd2;
short filver;   /* normally 3;  old files may still be 2 */
double hdtime;  /* set by rdhd() */
long frsiz;  /* size of header and image */
int hdr_rd;   /* are we part way through reading a header? */
#define MAXN 1025

rd2dh(fd,pnx,pny)       /* initialize */
  int fd;
  short *pnx, *pny;
{
  long firstfr, imsiz;
  int i, j;
  float v2;
  int hdsiz;
  long Lseek();
  double pow2();

  /* read first header */
  rd.fd = fd;
  filver = 0;
  hdr_rd = 0;
  if(rdhd()==0) error("empty input\n");
  if(filver==3){ hdsiz = sizeof(hd); }
    else{ hdsiz = sizeof(hd2); }
  if( (hd.nx>MAXN) || (hd.ny>MAXN) ) error("nx,ny too big");
  *pnx = hd.nx;
  *pny = hd.ny;
  rd.siz = hd.nx * hd.ny;
  imsiz = rd.siz*sizeof(short);
  frsiz = hdsiz+imsiz;
  rd.ts = hdtime;
  rd.u = hd.u;
  rd.v = hd.v;
  rd.fixuv = hd.fixuv;
  rd.pmin = hd.pmin;
  rd.pmax = hd.pmax;
  if(timewarp>=0){
    Lseek(fd,-frsiz,2);
    rdhd();
    rd.te = hdtime;
    if     (timewarp==0){ te = rd.te; ts = rd.ts; }
    else if(timewarp==1){ te = rd.te; }
  }

  /* locate first active header */
  firstfr = 0;
  if(timewarp>0){
    Lseek(fd,firstfr,0);
    rdhd();
    while(hdtime<ts){
      firstfr = Lseek(fd,imsiz,1);
      if( rdhd() == 0 ) error("starting time is after EOF");
    }
  }

  /* get ready to start reading file */
  if( (timewarp>=0) || (rd.fixuv!=1) ){
    rd.nfr = Lseek(fd,0L,2)/frsiz;
    Lseek(fd,firstfr,0);
  }else if( rd.fixuv==1 ){
    hdr_rd = 1;    /* so that initial header needn't be reread */
  }

  /* global scaling */
  if(rd.fixuv!=1){
    g_range();
    Lseek(fd,firstfr,0);
  }else{
    v2 = pow2(rd.v);
    rd.fmin = (rd.pmin-rd.u)*v2;
    rd.fmax = (rd.pmax-rd.u)*v2;
  }
}



int                           /* returns 0 upon EOF */
rd2di(ptime,p)                /* read an image */
  double *ptime;
  short p[]; 
{
  short *q;
  int shift;
  int i, j, k;
  int pmin=rd.pmin;
  int pmax=rd.pmax;

  if(hdr_rd==1){ hdr_rd=0; }
    else if(rdhd()==0) return(0);
  if( (timewarp>=2) && (hdtime>te) ) return(0);
  *ptime = hdtime;

  Read(rd.fd,p,rd.siz*sizeof(short));

  /* convert to global units */
  if( (hd.u!=rd.u) || (hd.v!=rd.v) ){
    shift = rd.v - hd.v;
    if(shift>15){
      for( i=rd.siz, q=p; i>0; i--, q++ ){
        *q = rd.u;
      }
    }else if(shift>=0){
      for(k=1; shift>0;){ shift--; k*=2; }
      for( i=rd.siz, q=p; i>0; i--, q++ ){
        if( *q < -BIG ) continue;
        j = ((*q-hd.u)/k) + rd.u;
        *q = (j<pmin)? pmin:
            ((j>pmax)? pmax: j);
      }
    }else{ /* shift<0 */
      for(k=1; shift<0;){ shift++; k*=2; }
      for( i=rd.siz, q=p; i>0; i--, q++ ){
        if( *q < -BIG ) continue;
        j = ((*q-hd.u)*k) + rd.u;
        *q = (j<pmin)? pmin:
            ((j>pmax)? pmax: j);
      }
    }
  }else{  /* hd.u=rd.u and hd.v==rd.v */
    for( i=rd.siz, q=p; i>0; i--, q++ ){
      if( *q < -BIG ) continue;
      if( *q<rd.pmin ){ *q = rd.pmin; }
      else if( *q>rd.pmax ){ *q = rd.pmax; }
    }
  }
  return(1);
}


rd2dj(j)
  int j;
{
  long Lseek();
  Lseek(rd.fd,j*frsiz,1);
}


int
rdhd()      /* returns 0 upon EOF */
{
  long Lseek();
  char tm[16];

  if( filver==0 ){   /* get version number first time through */
    if( EOFRead(rd.fd,&hd,sizeof(hd)) == 0 ) return(0);
    if(hd.magic1!=MAGIC) error("bad magic number %o at start",hd.magic1);
    filver = hd.ver;
    if(filver==2){ Lseek(rd.fd,0L,0); }
     else if(filver!=3){ error("bad version number %d",filver); }
  }else if(filver==3){
    if( EOFRead(rd.fd,&hd,sizeof(hd)) == 0 ) return(0);
  }

  if(filver==2){
    if( EOFRead(rd.fd,&hd2,sizeof(hd2)) == 0 ) return(0);
    hd.magic1 = hd2.magic1;
    hd.magic2 = hd2.magic2;
    hd.ver = hd2.ver;
    hd.nx = hd2.nx;
    hd.ny = hd2.ny;
    hd.u = hd2.u;
    hd.v = hd2.v;
    hd.fixuv = hd2.fixuv;
    hd.pmin = hd2.pmin;
    hd.pmax = hd2.pmax;
    sprintf(hd.time, "%15.8g", (double)hd2.time);
  }

  if(hd.magic1!=MAGIC){
    fprintf(stderr,"bad magic number %o\n",hd.magic1);
    fprintf(stderr,"file pointer now at %ld\n",Lseek(rd.fd,0L,1));
    exit(2);
  }
  if(hd.ver!=filver){ error("inconsistent version number"); }
  if(sscanf(hd.time,"%16E",&hdtime)!=1){
    swab(hd.time,tm,16);
    if(sscanf(tm,"%16E",&hdtime)!=1) error("can't parse time: %s",hd.time);
  }
  return(1);
}



g_range()
{    /* scan until last active header, updating range */
  float gmin, gmax, v2;
  double pow2();
  f_range(&rd.fmin,&rd.fmax);
  while( f_range(&gmin,&gmax) ){
    if( rd.v > hd.v ){
      rd.u = hd.u;
      rd.v = hd.v;
    }
    if( gmin < rd.fmin ) rd.fmin = gmin;
    if( gmax > rd.fmax ) rd.fmax = gmax;
  }
  g_rang2();
}


g_rang2()
{          /* choose global scaling */
  float gmin, gmax, v2;
  double pow2();
  gmax = (rd.fmax<0)?0:rd.fmax;
  gmin = (rd.fmin>0)?0:rd.fmin;
  v2 = pow2(rd.v);
  if( (gmin < (-BIG-rd.u)*v2) || (gmax > (BIG-rd.u)*v2) ){
    while( (gmax-gmin)/v2 >= 2*BIG-2 ){ rd.v++; v2 *= 2; }
    rd.u = (short)(-(gmin+gmax)/(2*v2));
  }
  rd.pmin = rd.u + (short)(rd.fmin/v2);
  rd.pmax = rd.u + (short)(rd.fmax/v2);
}


int
f_range ( pfmin, pfmax )
  float *pfmin, *pfmax;
{
  int i, j;
  short p[MAXN], *q;
  short pmin = BIG;
  short pmax = -BIG;
  double pow2();
  float u2, v2;
  if( rdhd()==0 ) return(0);
  if( (timewarp>=2) && (hdtime>te) ) return(0);
  u2 = hd.u;
  v2 = pow2(hd.v);
  for( i=hd.ny; i>0; i-- ){
    Read(rd.fd,p,hd.nx*sizeof(short));
    for( j=hd.nx, q=p; j>0; j--, q++ ){
      if( *q < -BIG ) continue;
      if( *q < pmin ){ pmin = *q; }
      if( *q > pmax ){ pmax = *q; }
    }
  }
  *pfmin = (pmin-u2)*v2;
  *pfmax = (pmax-u2)*v2;
  return(1);
}
