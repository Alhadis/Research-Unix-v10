
#include "hped.h"
#include "device.h"

extern float FAC0;
extern int grade, box, fsize;
extern int HOR_OFF, VER_OFF;
extern int XBASE, YBASE;
extern int compact;

extern float FAC;

#define PUTFONT(A) printf("\n.ft %s\n.sp -1\n",A); \
	pmove(copxx,copyy);
#define PUTSIZE(A) printf("\n.ps %d\n.sp -1\n",A); \
	pmove(copxx,copyy);

extern char vout;
#include <math.h>


#define SMALL 5

#define F_WIDTH 10

short defsize = 10;
char deffont[8] = {"R"};

double copxx, copyy;
int CH = 0;

pvecr(x,y) double x,y;
{
	int kx, ky;
	kx = FAC*x + 0.5;	ky = FAC*y + 0.5;
	if(abs(kx)<SMALL && abs(ky)<SMALL) {
#ifdef OLDSTY
		if(vout=='a') printf("."); else printf("\\(l.");
#else
		printf(".");
#endif
	}
	else {
		if(grade || (kx&&ky) ) printf("\\D'l %du %du'",kx,ky);
		else { if(!ky) printf("\\l'%du'",kx);
			else printf("\\v'-.25m'\\L'%du\\(br'\\v'.25m'",ky);
		}
	}
	copxx += x;	copyy += y;
	if(CH++>8) { CH=0; pmove(copxx,copyy); }
}

#ifdef SROFF
avecr(x,y) double x,y;
{
	int kx, ky;
	kx = x + 0.5;	ky = y + 0.5;
	if(abs(kx)<SMALL && abs(ky)<SMALL) {
		if(vout=='a') printf("."); else printf("\\(l.");
	}
	else {
		if(grade || (kx&&ky) ) printf("\\D'l %du %du'",kx,ky);
		else { if(!ky) printf("\\l'%du'",kx);
			else printf("\\v'-.25m'\\L'%du\\(br'\\v'.25m'",ky);
		}
	}
	copxx += x;	copyy += y;
	if(CH++>8) { CH=0; pmove(copxx,copyy); }
}

amove(x,y)
	double x,y;
{
	printf("\n.sp -1\n\\h'%du'\\v'%du'",
	(int)(x+0.5),(int)(y+0.5));
	copxx = x;	copyy = y;
}
#endif
/*	only place where absolute motion is made	*/

pmove(x,y) 
	double x,y;
{
	printf("\n.sp -1\n\\h'%du'\\v'%du'",
	(int)(FAC*(x-XBASE)+0.5),(int)(FAC*(y-YBASE)+0.5));
	copxx = x;	copyy = y;
}

pline (Pa,Pb)
	Point Pa, Pb;
{
	double xa, ya, xb, yb;

	xa =  (scale*Pa.x + xshft);
	ya =  (scale*Pa.y + yshft);
	xb =  (scale*Pb.x + xshft);
	yb =  (scale*Pb.y + yshft);

	pmove(xa,ya);
	pvecr(xb-xa,yb-ya);
}

uline(Pa,Pb)
	Point Pa, Pb;
{
	pmove(Pa.x,Pa.y);
	pvecr(Pb.x-Pa.x,Pb.y-Pa.y);
}

pdot (Pa,Pb)
	Point Pa, Pb;
{
	double xa, ya;

	xa =  scale*(Pa.x + Pb.x)*0.5 + xshft;
	ya =  scale*(Pa.y + Pb.y)*0.5 + yshft;

	pmove(xa,ya);
	pvecr(0.,0.);
}

pcirc(cp,r)
	Point cp;
	double r;
{
	double xc, yc;


	if(r<0) r = - r;
	r *= scale;
	xc =  (scale*cp.x + xshft);
	yc =  (scale*cp.y + yshft);
	pmove(xc-r,yc);
	printf("\\D'c %du'",2*(int)(FAC*r));
}

paint(x1,x2,y,c)
	double x1, x2;
	char c;
{
	double yy;
	short kk;
	yy = y;
	if('E'<c && c<'J') {
		if(y%8) return(0);
		
	}
	else if('I'<c && c<'L') {
		if(y%4) return(0);
	}
	switch(c){
	case 'K':
	case 'I':
		unrot(x1,x2,yy,0.707,-0.707);
		break;
	case 'J':		
	case 'H':
		unrot(x1,x2,yy,0.707,0.707);
		break;		
	case 'G':
		unrot(x1,x2,yy,0.,1.);
		break;
	case 'F':
	case 'E':
	default:
#ifdef SROFF
		amove(x1,yy);
		avecr(x2-x1,(double)0);
#else
		pmove(x1,yy);
		pvecr(x2-x1,(double)0);
#endif
		break;
	}
	return(1);
}

/*	plot spline on the d202	(open only)	*/
set_pen(cpen, border, psz)
	short cpen, border, psz;
{
	printf("\n.ps %d\n.sp -1\n",psz);
	defsize = psz;	/* unfortunately */
	if(fullob(cpen)){
		if(border) printf("\\X'Pgfull'");
		else printf("\\X'Pfull'");
		return;
	}
	switch(cpen){
	case DASH:
		printf("\\X'Pdash'");
		break;
	case SOLID:
		printf("\\X'Psolid'");
		break;
	}
}
c_points(pa,pb,closed,obtype)
	Point * pa, *pb;
	char obtype;
{
	register Point *pp, *qq;
	int kx, ky;
	char ispline = 0;

	pmove(pa->x,pa->y);
	if(obtype=='s' || obtype=='S') ispline = 1;
	if(ispline) {
		printf("\\D'q");	if(pa->t) printf(" 0u 0u");
	}
	else printf("\\D'p");
	for(pp=pa+1;pp<pb;pp++) {
		kx = FAC*(pp->x - (pp-1)->x) + 0.5;
		ky = FAC*(pp->y - (pp-1)->y) + 0.5;
		printf(" %du %du",kx,ky);
		if(ispline && pp->t) printf(" 0u 0u");
	}
	if(closed) {
		kx = FAC*(pa->x - (pb-1)->x) + 0.5;
		ky = FAC*(pa->y - (pb-1)->y) + 0.5;
		printf(" %du %du",kx,ky);
		if(ispline) {
			if(pa->t) printf(" 0u 0u");
			else {
				kx = FAC*((pa+1)->x - pa->x) + 0.5;
				ky = FAC*((pa+1)->y - pa->y) + 0.5;
				printf(" %du %du",kx,ky);
			}
		}
	}
	printf("'\n.sp -1\n");
}

fl_pen(cpen)
	short cpen;
{
	if(fullob(cpen)) printf("\\X'Pe%c'\n.sp -1\n",cpen);
}

#define MAGIC_VS 0.25
#define MAGIC_HS 0.15

pstring(p1,s,psize,pfont)
	Point p1;
	char s[];
	short psize;
	char * pfont;
{
	register i;
	register char *nlp, *npp;
	char adjc, *index();
	register char  *txtp;
	double dx, dy;

	if(!fsize) psize = (FAC/FAC0)*psize + 0.5;
	if(psize != defsize) PUTSIZE(psize)
	if(strcmp(pfont,deffont)) PUTFONT(pfont)
	p1.x = scale*p1.x + xshft;
	p1.y = scale*p1.y + yshft;

	for(i=0,nlp=s;*nlp;nlp++) if(*nlp=='\n') i++;
	/*number of lines*/
	dy = i;
	dx = 0;
	dy = 0.5*dy + MAGIC_VS;
	txtp = s;
	if(s[0]=='\\') {
		txtp += 2;
		adjc = s[1];
	}
	else {
		adjc = ' ';
	}
	pmove(p1.x,p1.y);
	printf("\\h'%.1fm'\\v'-%.1fm'", dx, dy); /*shift up and left */

	do{
		nlp = index(txtp,'\n');
		if(nlp) *nlp = 0;	/* replace nl by null */
		label(txtp,adjc,0);
		if(nlp) {
			*nlp = '\n';	/* restore */
			txtp = nlp + 1;	/* go past last spot */
		}
	} while(nlp);
	if(psize != defsize) PUTSIZE(defsize)
	if(strcmp(pfont,deffont)) PUTSIZE(deffont)
}


label(s, t, nh)	/* text s of type t nh half-lines up */
	char *s;
	char t;
	int nh;
{
	int q;
	char *p;

	printf("\\v'1.1m'");

	/* just in case the text contains a quote: */
	q = 0;
	for (p = s; *p; p++)
		if (*p == '\'') {
			q = 1;
			break;
		}
	switch (t) {
	case 'L':
	default:
		if(q)
			printf("%s\\h\\(ts-\\w\\(ts%s\\(tsu\\(ts", s, s);
		else
			printf("%s\\h'-\\w'%s'u'", s, s);
		break;
	case 'C':
		if (q)
			printf("\\h\\(ts-\\w\\(ts%s\\(tsu/2u\\(ts%s\\h\\(ts-\\w\\(ts%s\\(tsu/2u\\(ts", s, s, s);
		else
			printf("\\h'-\\w'%s'u/2u'%s\\h'-\\w'%s'u/2u'", s, s, s);
		break;
	case 'R':
		if (q)
			printf("\\h\\(ts-\\w\\(ts%s\\(tsu\\(ts%s", s, s);
		else
			printf("\\h'-\\w'%s'u'%s", s, s);
		break;
	}
}
