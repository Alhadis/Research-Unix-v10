#include <stdio.h>
#include <signal.h>
#include "pico.h"

extern struct	SRC src[MANY];
extern short	CURSCRATCH, CUROLD;
extern void	onquit();

#define lowbyte(A) (A)&0377
#define higbyte(A) ((A) >> 8) & 07

int om_fd;
char om_buf[1024];

char * setxy(p,x,y)
	char *p;
{
	*p++ = x&0377;
	*p++ = (x >> 8) & 07;
	*p++ = y&0377;
	*p++ = (y >> 8) & 07;
	return(p);
}

om_open(dev)
{	char buf[10];
	extern char metheus;

	sprintf(buf, "/dev/om%d", dev);
	if ((om_fd = open(buf, 2)) == -1)
	{	perror("om_open");
		metheus = 0;
		return 0;
	}
	reloadmaps();
	return 1;
}

om_close()
{
	close(om_fd);
}

om_flush(pp)
	char *pp;
{
	if ((pp-om_buf)%2) *pp++ = 0;	
	write(om_fd, om_buf, pp-om_buf);
}

metreset()
{	register char *pp = om_buf;

	*pp++ = 'O';
	*pp++ = 0377;	/* WRMASK */
	signal(SIGINT, SIG_IGN);
	metfill(0, Old->sx, Old->sy, Old->ex, Old->ey);
	om_flush(pp);
}

metfill(col, x1, y1, x2, y2)
{
	register char *pp = om_buf;

	*pp++ = 'N';
	*pp++ = col;
	*pp++ = 'R'; pp = setxy(pp, x1, y1);
	*pp++ = 'S'; pp = setxy(pp, x2, y2);
	*pp++ = 'd'; om_flush(pp);
}

set_color(n)
{
	register char *pp;
	pp = om_buf;
	*pp++ = 'N';
	*pp++ = n;
	om_flush(pp);
}

metwi(row, length, xoff, height)
	unsigned char *row;
	short length, xoff, height;
{
	unsigned char mcode[16];
	short xend;

	xend = xoff + length;

	mcode[0] = 0x4F;
	mcode[1] = 0377;	/* WRMASK */

	mcode[2]  = 0x52;	/* set P1 */
	mcode[3]  = lowbyte(xoff);	mcode[4]  = higbyte(xoff);

	mcode[5] = mcode[10] = lowbyte(height);
	mcode[6] = mcode[11] = higbyte(height);
	mcode[7]  = 0x53;	/* set P2 */
	mcode[8]  = lowbyte(xend);	mcode[9]  = higbyte(xend);

	mcode[12] = 0;
	mcode[13] = 'o';

	signal(SIGINT, SIG_IGN);
	write(om_fd, mcode, 14);
	write(om_fd, row, length);
	signal(SIGINT, onquit);
}

metri(b, x, y, d)
	char *b;
{
	char *bp = b;
	char *pp = om_buf;

	signal(SIGINT, SIG_IGN);
	*pp++ = 'R'; pp = setxy(pp, x, y);
	*pp++ = 'S'; pp = setxy(pp, x+d-1, y);
	*pp++ = 'n'; om_flush(pp);
	read(om_fd, bp, d);
	signal(SIGINT, onquit);
}
