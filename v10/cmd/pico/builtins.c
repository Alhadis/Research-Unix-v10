#include <stdio.h>
#include <math.h>
#include <ctype.h>
#include "pico.h"

extern struct	SRC src[MANY];
extern short	CURSCRATCH, CUROLD;
extern char	frameb, metheus;
extern int	DEF_LL, DEF_NL;
extern float	fsqrt();

println(s, aa)
	char *s;
	long aa;
{
	char c, *t = s;
	int argc=1;
	long *aaa = &aa;

	for (;;)
	switch (c = *t++) {
	case '\0': return argc;
	case  '%': switch (c = *t++) {
		   case '\0': putc('%', stderr); return argc;
		   case  'd': fprintf(stderr, "%d", (int)*aaa);
			      aaa++; argc++; break;
		   case  's': fprintf(stderr, "%s", (char *)*aaa);
			      aaa++; argc++; break;
		   case  '%': putc('%', stderr); break;
		   default  : putc('%', stderr); putc(c, stderr); break;
		   }
		   break;
	default  : putc(c, stderr); break;
	}
}

Clamp(a){ return ((a < 0)?0:((a>=DEF_LL)?(DEF_LL-1):a)); }
Abs(a)	{ return ((a<0)?(-a):a); }

int Midx = 256, Midy = 256;

setmid()
{
	Midx = (Old->sx + (Old->ex - Old->sx)/2);
	Midy = (Old->sy + (Old->ey - Old->sy)/2);
}

r_polar(x, y)
{	register float dx = (float) (x - Midx);
	register float dy = (float) (y - Midy);

	return (int) fsqrt(dx*dx + dy*dy);
}

a_polar(x, y)
{
	return patan2(x-Midx, y-Midy);
}
A_polar(x, y)
{
	return Patan2(x-Midx, y-Midy);
}

Y_cart(r, a)
{
	return Midy+(r*Psin(a)/10000);
}
y_cart(r, a)
{	int b = Midy+(r*psin(a)/10000);
	return (b >= 0 && b < DEF_NL)?b:0;
}

X_cart(r, a)
{
	return Midx+(r*Pcos(a)/10000);
}
x_cart(r, a)
{	int b = Midx+(r*pcos(a)/10000);
	return (b >= 0 && b < DEF_LL)?b:0;
}

setzmap(i, z)
{
	redcmap(i, z);
	grncmap(i, z);
	blucmap(i, z);
	return z;
}

setcmap(i, r,g,b)
{
	redcmap(i, r);
	grncmap(i, g);
	blucmap(i, b);
	return r;
}

putframe(n)
{	char nr[128];
	int i;

	sprintf(nr, "frame.%6d", n);
	for (i = 11; i > 5; i--)
		if (nr[i] == ' ') nr[i] = '0';
	i = CUROLD; CUROLD = CURSCRATCH; CURSCRATCH = i;
	putdpix(nr, 0);
	i = CUROLD; CUROLD = CURSCRATCH; CURSCRATCH = i;
	return 1;
}

swap()
{
	setscratch(Scratch, Old);
	return 1;
}

getframe(n)
{	char nr[128];
	int i;

	sprintf(nr, "frame.%6d", n);
	for (i = 11; i > 5; i--)
		if (nr[i] == ' ') nr[i] = '0';
	return newscreen(nr);
}

yank(x, y)
{	if (frameb) fbyank(x, y);
	else if (metheus) metyank(x,y);
}

intexp(a) int a; {	return (int) exp((double) a); }
intlog(a) int a; {	return (int) 1024*log((double) a); }
intlog10(a) int a; {	return (int) 1024*log10((double) a); }
intsqrt(a) int a; {	return (int) sqrt((double) a); }
intpow(a, b) int a,b; {	return (int) pow((double) a, (double) b); }
