#include <jerq.h>
struct Mouse mouse;
void nap();
static short c_ul[]={
	0xFF80,	0xFE00,	0xF800,	0xFC00,
	0xFE00,	0xDF00,	0xCF80,	0x87C0,
	0x83E0,	0x01F0,	0x00F8,	0x007C,
	0x003E,	0x001F,	0x000E,	0x0004,
};
static short c_ll[]={
	0x0004,	0x000E,	0x001F,	0x003E,
	0x007C,	0x00F8,	0x01F0,	0x83E0,
	0x87C0,	0xCF80,	0xDF00,	0xFE00,
	0xFC00,	0xF800,	0xFE00,	0xFF80,
};
static short c_ur[]={
	0x01FF,	0x007F,	0x001F,	0x003F,
	0x007F,	0x00FB,	0x01F3,	0x03E1,
	0x07C1,	0x0F80,	0x1F00,	0x3E00,
	0x7C00,	0xF800,	0x7000,	0x2000,
};
static short c_lr[]={
	0x2000,	0x7000,	0xF800,	0x7C00,
	0x3E00,	0x1F00,	0x0F80,	0x07C1,
	0x03E1,	0x01F3,	0x00FB,	0x007F,
	0x003F,	0x001F,	0x007F,	0x01FF,
};
static setxy(),	flipcursor();
struct curtab{
	short	*map;
	short	dx;
	short	dy;
}stdcurtab[4]={		/* 4 quadrants */
	c_ur,	-16,	0,
	c_ul,	0,	0,
	c_lr,	-16,	-16,
	c_ll,	0,	-16,
}, usercurtab[4];
struct curtab *curtabp;
struct cursor{
	unsigned x, y;
	unsigned oldx, oldy;
	char	quad, oldquad;
	short	inhibit;
	short	up;
}cursor;
static int	ud, lr;
#define	QRL_MASK	01
#define	QUD_MASK	02
#define	QRIGHT		0
#define	QLEFT		01
#define	QUP		0
#define	QDOWN		02
#define	QLEFT_MARGIN	XMAX/3
#define	QRIGHT_MARGIN	XMAX*2/3
#define	QUP_MARGIN	YMAX/3
#define	QDOWN_MARGIN	YMAX*2/3

newquad(x, y, quad)
	register x, y, quad;
{
	ud=quad&QUD_MASK;
	lr=quad&QRL_MASK;
	if(x < QLEFT_MARGIN)
		lr=QLEFT;
	if(x > QRIGHT_MARGIN)
		lr=QRIGHT;
	if(y < QUP_MARGIN)
		ud=QUP;
	if(y > QDOWN_MARGIN)
		ud=QDOWN;
	return ud|lr;
}
cursinhibit(){
	register sr=spl1();
	if(cursor.inhibit++==0 && cursor.up)
		flipcursor(cursor.x, cursor.y, cursor.quad);
	splx(sr);
}
cursallow(){
	if(cursor.inhibit>0)
		cursor.inhibit--;
}
#define	MOUSEMAX	2048	/* 1/2 resolution of mouse registers */
/*static*/ struct data {
	int	last;
	int	zero;
}xdata,	ydata;
static int
update(var, datap, max)
	register var, max;
	register struct	data *datap;
{
	register d;
	d=var-datap->last;
	if(d>MOUSEMAX)
		datap->zero+=MOUSEMAX*2;
	else if(d<-MOUSEMAX)
		datap->zero-=MOUSEMAX*2;
	datap->last=var;
	d=var-datap->zero;	/* re-use of d */
	if(d<0){
		datap->zero=var;
		d=0;
	}else if(d>=max){
		datap->zero=var-max;
		d=max-1;
	}
	return d;
}
static
setxy(){
	register struct	curtab *qp;
	register x, y, quad;
	x=  *XMOUSE & 0xfff;	/* coordinates go the wrong way, so negate */
	y= -(*YMOUSE & 0xfff);
	cursor.oldx=cursor.x;
	cursor.oldy=cursor.y;
	cursor.oldquad=cursor.quad;
	x=update(x, &xdata, XMAX);
	y=update(y, &ydata, YMAX);
	quad=newquad(x,	y, cursor.quad);
	qp= &curtabp[quad];
	if(x< -qp->dx)
		x= -qp->dx;
	if(x+(16+qp->dx)>=XMAX)
		x=XMAX-(16+qp->dx)-1;
	if(y< -qp->dy)
		y= -qp->dy;
	if(y+(16+qp->dy)>=YMAX)
		y=YMAX-(16+qp->dy)-1;
	if(x==cursor.oldx && y==cursor.oldy && cursor.up)
		return(0);
	cursor.x=x;
	cursor.y=y;
	cursor.quad=quad;
	mouse.xy.x=x;
	mouse.xy.y=y;
	return(1);
}
cursset(p)
	Point p;
{
	xdata.zero=xdata.last-p.x;
	ydata.zero=ydata.last-p.y;
	nap(1);
}
Bitmap cursbm;
static
flipcursor(x, y, q)
{
	register struct	curtab *qp;
	register s;
	qp= &curtabp[q];
	cursbm.base=(Word *) qp->map;
	cursblt(&display, qp->map, Pt(x+qp->dx,	y+qp->dy));
		/* almost equal	to
		/* bitblt(&cursbm, cursbm.rect,	&display,
		/*		Pt(x+qp->dx, y+qp->dy),	F_XOR);	  */
	cursor.up=1-cursor.up;
}
short interrupt;
cursinit(){
	curtabp=stdcurtab;
	cursbm.width=1;
	cursbm.rect.corner.x=cursbm.rect.corner.y=16;
	cursor.inhibit = 0;
}
auto1(){
	if(setxy() && cursor.inhibit==0){
		if(cursor.up)
			flipcursor(cursor.oldx,	cursor.oldy, cursor.oldquad);
		flipcursor(cursor.x, cursor.y, cursor.quad);
	}
	interrupt++;
}
void nap(n){
	if(n<=0)
		return;
	interrupt=0;
	do; while(interrupt<n);
}

/* static*/ Texture *old;
Texture	*
cursswitch(p)
	register Texture *p;
{
	register struct	curtab *ctp;
	register Texture *t=old;
	old=p;
	cursinhibit();
	if(p==0)
		curtabp=stdcurtab;
	else{
		curtabp=usercurtab;
		for(ctp=usercurtab; ctp<&usercurtab[4];	ctp++){
			ctp->map=(short	*)p;
			ctp->dx=ctp->dy= -8;
		}
	}
	cursallow();
	return t;
}
