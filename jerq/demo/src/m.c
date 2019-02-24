#include <jerq.h>
int Xmin, Ymin, Xmax, Ymax;
main(){
	request(KBD|CPU);
	grandmal();
	exit();
}
combox(x, y, xs, ys, f)
	int x, y, xs, ys;
	Code f;
{

	if (x<Xmin) { 
		xs+=Xmin-x;
		x=Xmin; 
	}
	if (y<Ymin) { 
		ys+=Ymin-y;
		y=Ymin; 
	}
	if ((x<Xmax) && (y<Ymax)) {
		if (x+xs>(Xmax-1)) xs=(Xmax-1)-x;
		if (y+ys>(Ymax-1)) ys=(Ymax-1)-y;
		rectf(&display, Rect(x, y, x+xs, y+ys), f);
	}
}
sort(p, q, u, v)
	register int *p, *q, *u, *v;
{
	register t;
	if(*p>*q){
		t= *p;
		*p= *q;
		*q= t;
		t= *u;
		*u= *v;
		*v= t;
	}
}
lim5(v)
	register v;
{
	if(v<-5)
		return -5;
	if(v>5)
		return 5;
}

grandmal() {
	int x0, y0, x1, y1, t, vx0, vy0, vx1, vy1, snooze;
	Code f;
	int c;

loop:
	Xmin=Drect.origin.x;
	Ymin=Drect.origin.y;
	Xmax=Drect.corner.x;
	Ymax=Drect.corner.y;
	rectf(&display, display.rect, F_OR);
	x1=x0=(Xmax+Xmin)/2;
	y1=y0=(Ymax+Ymin)/2;
	vx0=0;
	vy0=0;
	vx1=0;
	vy1=0;
	f=F_XOR;
	while ((c=kbdchar())==-1) {
		if ((x0!=x1) && (y0!=y1))
			combox (x0, y0, (x1-x0), (y1-y0), f);
		if (r(1, 10)!=1) f=F_XOR; 
		else f=F_OR;
		vx0=lim5(vx0+r(-1, 1));
		x0+=vx0;
		if (x0<Xmin) {
			x0=2*Xmin-x0;
			vx0= -vx0;
		} else if ((Xmax-1)<x0) {
			x0=2*(Xmax-1)-x0;
			vx0= -vx0;
		}
		vx1=lim5(vx1+r(-1, 1));
		x1+=vx1;
		if (x1<Xmin) {
			x1=2*Xmin-x1;
			vx1= -vx1;
		} else if ((Xmax-1)<x1) {
			x1=2*(Xmax-1)-x1;
			vx1= -vx1;
		}
		sort(&x0, &x1, &vx0, &vx1);
		vy0+=r(-1, 1);
		if (vy0<-5) vy0= -5; 
		else if (5<vy0) vy0=5;
		y0+=vy0;
		if (y0<Ymin) {
			y0=2*Ymin-y0;
			vy0= -vy0;
		} else if ((Ymax-1)<y0) {
			y0=2*(Ymax-1)-y0;
			vy0= -vy0;
		}
		vy1+=r(-1, 1);
		if (vy1<-5) vy1= -5; 
		else if (5<vy1) vy1=5;
		y1+=vy1;
		if (y1<Ymin) {
			y1=2*Ymin-y1;
			vy1= -vy1;
		} else if ((Ymax-1)<y1) {
			y1=2*(Ymax-1)-y1;
			vy1= -vy1;
		}
		sort(&y0, &y1, &vy0, &vy1);
		wait(CPU);
	}
	if(c==' ')
		goto loop;
}
#define PP       98
#define QQ       27
#define Pm1     (PP-1)
	
int I=Pm1, J=(Pm1+QQ)%PP;
int Table[PP]={
	0020651, 0147643, 0164707, 0125262, 0104256, 0074760, 0114470,
	0052607, 0045551, 0134031, 0024107, 0030766, 0154073, 0114777,
	0024540, 0111012, 0011042, 0104067, 0056332, 0142244, 0131107,
	0034074, 0052641, 0163046, 0026303, 0131352, 0077724, 0002462,
	0110775, 0127346, 0020100, 0137011, 0136163, 0145552, 0144223,
	0134111, 0075001, 0075221, 0176705, 0000210, 0103625, 0120246,
	0062614, 0016147, 0054723, 0151200, 0105223, 0021001, 0016224,
	0073377, 0150716, 0014557, 0112613, 0037466, 0002677, 0052542,
	0063572, 0105462, 0106436, 0063302, 0053171, 0133243, 0113130,
	0123222, 0072371, 0041043, 0163614, 0037432, 0147330, 0153403,
	0130306, 0056455, 0175640, 0120567, 0100601, 0042371, 0154635,
	0051133, 0074252, 0174525, 0163223, 0052022, 0022564, 0135512,
	0021760, 0006743, 0006451, 0067445, 0106210, 0025417, 0066566,
	0062723, 0124224, 0144643, 0164502, 0025342, 0003521, 0024050,
};

int random() {
	if (I == Pm1) I = 0; 
	else I++;
	if (J == Pm1) J = 0; 
	else J++;
	return (Table[I]^=Table[J]);
}
int r (i, j)
int i, j;
{
	int k;

	k = random() % (j-i+1);
	if (k<0) return(k+j+1);
	else return(k+i);
}
