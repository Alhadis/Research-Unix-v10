#include <stdio.h>
#include <libc.h>
#include "3610op.h"

minit(cold)
{
  msnare();
  mcmd(INI);
  mcmd(WRMSKL); mlong(0xffffffff);
  mcmd(WRBANK); mbyte(15);
  mcmd(SELRES); mbyte(0xc0); mbyte(0);  /* 8-bit mode */
  mcmd(MOVP1); mword(0); mword(0);  /* blank screen */
  mcmd(MOVP2); mword(1279); mword(1023);
  mcmd(SETCLL); mbyte(0); mbyte(255); mbyte(255); mbyte(255);
  mcmd(FFILL);
}

colmap(maxi,mapr,mapg,mapb)
  int maxi;
  int mapr[],mapg[],mapb[]; /* colors, 0 through maxi */
{
  int i;
  mcmd(CMSEL); mbyte(0); mbyte(0);	/* write in cmap buffer 0 */
  mcmd(CMFILL);mword(0);mword(256);     /* initialize map to black */
    mbyte(0);mbyte(0);mbyte(0);mbyte(0);
  mcmd(CMLDM); mword(0);mword(maxi+1);	/* load entries at loc. 0 */
  for(i=0; i<=maxi; i++){
    mbyte(mapr[i]); mbyte(mapg[i]); mbyte(mapb[i]);
  }
  if((maxi+1)&1) mbyte(0);
  mcmd(CMACT);				/* make current buffer active */
  mflush();
}

image(in, x0, y0, x1, y1)
  char *in;
{
  int around=0;
  int y2;
  long n, saved, m;
  if(((int)in)&1){ error("odd address\n"); }
  n=(x1-x0+1)*(y1-y0+1);
  mcmd(MOVP1); mword(x0); mword(1023-y1);  /* upper left */
  mcmd(MOVP2); mword(x1); mword(1023-y0);  /* lower right */
  mcmd(WRR);
  mflush();
  Write(mfd, in, n);
  if(n&1) mbyte(0);
}

zoom(fact)
  int fact;
{
  mcmd(ZOOM);mbyte(fact);
}

pan(x,y)
  int x, y;  /* upper left corner */
{
  mcmd(MOVP1);mword(x);mword(1023-y);
  mcmd(PPAN);
  mflush();
}
