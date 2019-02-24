#include	<jerq.h>
#define		SILLY		-20

/* Filling interior of arbitrary polygons on the Blit */
static draw(), xcut(), place(), realfill();

#ifdef	TEST
#include	<font.h>
Texture black =
{
	0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
	0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
};

main()
{
	Point pts[40], *p;

	request(MOUSE);
	string(&defont, "b1 vertex; b2 end; b3 exit", &display, Drect.origin, F_XOR);
	p = pts;
	for(;;)
	{
		wait(CPU);
		wait(MOUSE);
		if((own()&MOUSE) && button1())
		{
			*p++ = mouse.xy;
			if(p != pts) segment(&display, p[-2], p[-1], F_OR);
			while(button1()) nap(1);
		}
		if((own()&MOUSE) && button2())
		{
			*p++ = pts[0];
			while(button2()) nap(1);
			ppfill(pts, (int)(p-pts), &display, &black, F_OR);
			p = pts;
		}
		if((own()&MOUSE) && button3())
		{
			while(button3()) nap(1);
			exit(0);
		}
	}
}
#endif	TEST

struct seg { short x, y, X, Y; char stat;} ;
typedef struct seg segS;
typedef struct seg * segP;
typedef Point *pointP;
#ifdef BRAVE
segP sinp;
#else
segS sinp[128];
#endif

static short ymin;

#define COMP(A)  if(p1->A>p2->A) return(-1); else if(p1->A<p2->A) return(1);

static
place(p1,p2) segP p1, p2;
{
	COMP(y) else { 
		COMP(x) else { 
			COMP(X) else { 
				COMP(Y) else return(0);
			}
		}
	}
}

#define SWITCH { xyt=y; y=Y; Y=xyt; xyt=x; x=X; X=xyt; ren=0;}

ppfill(pts, npts, bm, text, mode)
	Point *pts;
	Bitmap *bm;
	Texture *text;
{
	short  so;
	short x, y, X, Y;
	short xyt, ren, newc;
	register pointP pp, qq, opp;
	register segP sp;
	register j;

#ifdef BRAVE
	sinp = (segP)alloc((unsigned)((npts+2)*sizeof(segS)));
	if(!sinp) return(-1);
#else
	if(npts>125) return(-1);
#endif
#ifdef EFFIC
	if(npts==4){
		rectf(bm,Rect(pts->x,pts->y,
			(pts[2]).x,(pts[2]).y),mode);
	}
#endif
	ymin = Drect.origin.y;
	qq = pts;
	x = qq->x;	y = qq->y;
	pp = qq + 1;
	for(j=1,sp=sinp,so=0;j<=npts;j++,pp++) {
		/* missing: we should take precaution abt HH */
		if(j==npts) pp = qq;
		if(pp->x < SILLY) {	/*break in contour*/
			opp = pp;
			pp = qq;
			newc = 1;
		}
		else newc = 0;
		X = pp->x;	Y = pp->y;
		if(Y<ymin) ymin = Y;
		if(Y>y) SWITCH
		else {if(Y==y && x<X) SWITCH	else ren=1;}
		sp->x=x; sp->y=y; sp->X=X; sp->Y=Y; sp->stat=1;
		if(ren) { x = X; y = Y;}
		sp++; so++;
		if(newc) {
			qq = opp + 1;
			if(!qq) break;
			x = qq->x;	y = qq->y;
			pp = qq;	/*will be incr. in loop*/
			j++;
		}
	}
	realfill(so,ymin,bm,text,mode);
#ifdef BRAVE
	free(sinp);
#endif
	return(1);
}

/* Fill a contour defined by a set of line segments */

static
realfill(so, yymin, bm, text, mode)
	int so, yymin;
	Bitmap *bm;
	Texture *text;
{
	int i, n;
	int yb;
	segP  sp, s1, s2, sfin, st, sbf, snx;
	for(i=0,sp=sinp;i<so;i++,sp++) sp->stat = 1;
	sfin = sp; 	/*sp was not incremented last time*/
	/* edit horizontal lines */
	for(i=0,sp=sinp;i<so;i++,sp++){
		if(!(sp->stat)) continue;
		if(sp->y != sp->Y) continue;
		if(sp==sinp) sbf = sp+so-1; 
		else sbf = sp-1;
		if(sp<sfin) snx = sp+1;
		else snx = sp;
		if(sbf->y==sp->y){	/* point before is below*/
			if(snx->y==sp->y){	/* both below */
				sp->stat = 0;
			}
			continue;
		}
		else if(sbf->Y==sp->y){	/*previous point above*/
			if(snx->Y==sp->y){	/* both above */
				sp->stat = 0;
			}
			continue;
		}
	}
	/* finished with horizontal editing */

	qsort(sinp,so,sizeof(struct seg),place);

	sp = sinp; 
	i = 0;
	n = so;
	s1 = sp; 
	yb = s1->y;
	for(;yb<=sp->y;sp++,i++);
	s2 = sp - 1;
	for(;yb>=yymin;yb--){
		draw(yb,s1,s2, bm, text, mode);
		for(st=s1;st<=s2;st++){
			if(!(st->stat)) continue;
			if(yb<=st->Y) st->stat=0;
		}
		if(sp >= sfin) continue; /*nomore*/
		if(i>=n) continue;
		for(;yb<=(sp->y+1)&&i<n;sp++,i++){ /*we have gone below next element */
			for(st=s1;st<=s2;st++){
				if(sp->y==st->Y){
					if(sp->x==st->X) st->stat=0;
					else if(sp->Y==sp->y&&sp->X==st->X) st->stat=0;
				}
			}
		}
		/*cleanup*/
		s2 = sp - 1;
		while(!(s1->stat)) s1++;
	}
}

#define NCUTS 32
#define NCUTS_1 31

static
draw(y,pr1,pr2, bm, text, mode)
	int y;  segP pr1, pr2;
	Bitmap *bm;
	Texture *text;
{
	register segP pr;
	register i, j, k, n;
	int x[NCUTS];
	for(pr=pr1,n=0;pr<=pr2;pr++){
		if(!(pr->stat)) continue;
		if(pr->y==pr->Y) continue; /* ignore horizontals */
		x[n++] = xcut(y,pr->x,pr->y,pr->X,pr->Y);
		if(n>NCUTS_1) /*jtag(1,"Too many intersections")*/;
	}
	if(n<=1) return(0);
	k=1;
	while(k){
		k = 0;
		for(i=1;i<n;i++){
			if(x[i-1]>x[i]) {
				j=x[i-1]; 
				x[i-1]=x[i]; 
				x[i]=j; 
				k=1;
			}
		}
	}
	for(i=1;i<n;i +=2) 
		texture(bm,Rect(x[i-1],y,x[i]+1,y+2),text, mode);

	return(1);
}

static
xcut(y,x1,y1,x2,y2){
	long dx, dy, xy;
	int x;
	if(y1==y2) {
		x = x1>x2? x1: x2; 
		return(x);
	}
	dy = y1-y2; 
	dx = x1-x2; 
	xy = (long)y1*x2-x1*(long)y2;
	x = (y*dx+xy)/dy;	/*dy non zero because of prev. check*/
	return(x);
}
