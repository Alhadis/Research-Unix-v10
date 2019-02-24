#include <stdio.h>
#define	DEFINING
#include "cv.h"

int cv;
#define	NCVBUF	4096
char	cvbuf[NCVBUF];
char	*cvbufp=cvbuf;
int	ncvbuf=0;
int	cvdmamode=0;
cvput(x){
	if(ncvbuf >= NCVBUF-1)
		cvflush();
	*cvbufp++=x, ncvbuf++;
}
cvnput(n, p)
	register n;
	register char *p;
{
	while(n--)
		cvput(*p++);
}
cvmode(x)
{
	if(ioctl(cv, CVSETDMA, &x))
		perror("ioctl");
}
cvflush(){
	register n;
	if(ncvbuf&1)
		cvput(0);
	n=write(cv, cvbuf, ncvbuf);
	if(n!=ncvbuf)
		fprintf(stderr, "incomplete write got %d wanted %d\n", n, ncvbuf);
	if(n<0){
		perror("write");
		exit(1);
	}
	cvbufp=cvbuf;
	ncvbuf=0;
}
cvinit(cold){
	cv=open("/dev/cv0", 2);
	if(cv < 0){
		fprintf(stderr, "can't open /dev/cv0\n");
		exit(1);
	}
	if(ioctl(cv, CVSETDMA, &WR_CMD)){
		perror("ioctl");
		abort();
	}
	cvput(4);	/* wake up */
	cvflush();
	if(cold){
		cvput(0xFD);	/* cold */
		cvflush();
		cvput(4);	/* wake up */
		cvflush();
		cvput(0x9D);    /* write enable masks */
		cvput(0xFF);
		cvput(0x7);
		cvload(3,-256,-256); /* lower left screen = 0,0  (ehg) */
		cvflush();
	}
}
cvvalue(r, g, b)
{
	cvput(6);
	cvput(r);
	cvput(g);
	cvput(b);
}
cvflood()
{
	cvput(7);
}
cvload(n, val)
	Point val;
{
	cvput(160);
	cvput(n);
	cvput(val.x>>8);
	cvput(val.x);
	cvput(val.y>>8);
	cvput(val.y);
}

image(in, x0, y0, x1, y1, bank)
  char *in;
{
  int around=0;
  int y2;
  static char cmd[]={0x9D, 0xFF, 0x00, 0xAE};
  long n, saved, m;
  if(((int)in)&1){ error("odd address\n"); }
  n=(x1-x0+1)*(y1-y0+1);
  if(n>32767){
    y2 = y0 + 62;
    if(y2>y1) error("bad image: %d %d %d %d",x0,y0,x1,y1);
    n=(x1-x0+1)*(y2-y0);
    image(in,x0,y0,x1,y2-1,bank);
    image(in+n,x0,y2,x1,y1,bank);
  }else{
    if(n&1) n++;/* display will ignore extra byte */
    cvload(13, x0, y0);  /* window start (lower left) */
    cvload(14, x1, y1);  /* window end   (upper right)*/
    cmd[2]=bank;
    cvnput(sizeof cmd, cmd); /* WRMASK bank; DMA_write */
    cvflush();
    cvmode(WR_IMAGE);
    Write(cv, in, n);
    cvmode(WR_CMD);
  }
}

zoom(fact)
  int fact;
{
  cvput(0x34);
  cvput(fact);
}

lutrte(bank)
  int bank;
{
  cvput(0x1E);
  cvput(bank);
}
