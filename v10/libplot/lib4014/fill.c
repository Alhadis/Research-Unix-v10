#include <stdio.h>
#include <math.h>
#include "tek.h"
#define SEGS 256
#define ROUND(a) a>0?a+.5:a-.5
double	sinp, cosp;
int place();
fill(num,ff)
int num[];
double	*ff[];
{
	register struct seg *sp;
	register double x, y;
	int sgap;
	double slant;
	struct seg *in, *spend;
	struct seg ss[4];
	int	so;
	double	 X, Y, xyt, **fp;
	int ymin;
	int	M, *n;
	double	*xpp, *ypp;
	int fx, fy, flag;

	if(!internal){
		in = (struct seg *)malloc((unsigned)(SEGS*sizeof(struct seg)));
		if(in == 0){
			fprintf(stderr,"not enough space for fill segments\n");
			return(1);
		}
		spend = in + (SEGS-2);
	}
	else {
		in = ss;
		spend = &ss[3];
	}
	sinp = cos(e1->pslant);
	cosp = sin(e1->pslant);
	n = num;
	fp = ff;
	sp = in;
	ymin = 4000;
	while((M = *n++)){
		xpp = *fp++;
		ypp = xpp + 1;
		x = normx(*xpp);
		y = normy(*ypp);
		if (cosp)  {
			xyt = sinp * x - cosp * y;
			y = cosp * x + sinp * y;
			y = ROUND(y);
			x = ROUND(xyt);
		}
		fx = x;
		fy = y;
		flag = 0;
		while(--M){
			xpp += 2;
			ypp += 2;
			X = normx(*xpp);
			Y = normy(*ypp);
			if (cosp)  {
				xyt = sinp * X - cosp * Y;
				Y = cosp * X + sinp * Y;
				Y = ROUND(Y);
				X = ROUND(xyt);
			}
	check:
			if ( (Y > y) || (Y==y && X > x)){
				sp->x = X;
				sp->y = Y;
				sp->X = x;
				sp->Y = y;
			}
			else {
				sp->x = x;
				sp->y = y;
				sp->X = X;
				sp->Y = Y;
			}
			x=X;
			y=Y;
			sp->stat = 1;
			if(Y < ymin)
				ymin = Y;
			if ( sp->y == sp->Y) {
				if(sp > in){
				if ((sp-1)->y == (sp-1)->Y && (sp-1)->y == sp->y) {
					if (sp->x == (sp - 1)->X) {
						(sp - 1)->X = sp->X;
					sp--;
					} else if (sp->X == (sp - 1)->x) {
						(sp - 1)->x = sp->x;
					sp--;
					}
				}
				}
			}
			if(sp++ > spend){
				fprintf(stderr, "Too many sides\n");
				return(0);
			}
			if((M==1) && (flag == 0)){
				X = fx;
				Y = fy;
				flag = 1;
				goto check;
			}
		}
	}
	sgap = e0->pgap;
	slant = e0->pslant;
	e0->pgap = e1->pgap;
	e0->pslant = e1->pgap;
	esave = e1;
	putchar(ESC);
	putchar(e1->pbrush);
	e1 = e0;
	so = sp - in;
	realfill(so, in, ymin);
	e0->pgap = sgap;
	e0->pslant = slant;
	e1 = esave;
	putchar(ESC);
	putchar(e1->ppink);
	if(!internal)
		free(in);
	return(1);
}
double	xcut();
realfill(so, s, yymin)
int	so, yymin;
struct seg *s;
{
	register struct seg *sp, *s1, *s2, *st;
	register int	yb;
	struct seg *sfin;

	sfin = s1 = s + so - 1;
	for (sp = s; sp <= sfin;  sp++) {
		if (sp->stat){
			if (sp->y == sp->Y){
				if (sp < sfin) {
					s2 = sp+1;
					while(s2<=sfin && s2->y==s2->Y){
						if (sp->X == s2->x)
							sp->X = s2->X;
						else if (sp->x == s2->X)
							sp->x = s2->x;
						s2++->stat = 0;
					}
				}
				else s2 = sp;
				if (s1->y == sp->y &&s2->y == sp->y)
					sp->stat = 0;
				else if (s1->Y == sp->y && s2->Y == sp->y)
					sp->stat = 0;
			}
		}
		s1 = sp;
	}

	qsort(s, so, sizeof(struct seg), place);
	sfin++;
	s1 = s;
	yb = s1->y;
	for(sp=s; yb <= sp->y;sp++);
	s2 = sp - 1;
	for (; yb >= yymin; yb--) {
		if (!(yb % e1->pgap))
			draw(yb, s1, s2);
		for (st = s1; st <= s2; st++) {
			if (st->stat)
				if (yb <= st->Y)
					st->stat = 0;
		}
		if (sp >= sfin)
			continue;
		for (; yb <= (sp->y + 1) && sp < sfin; sp++) {
			for (st = s1; st <= s2; st++) {
				if (sp->y == st->Y) {
					if (sp->x == st->X)
						st->stat = 0;
					else if (sp->Y == sp->y && sp->X == st->X)
						st->stat = 0;
				}
			}
		}

		s2 = sp - 1;
		while (!(s1->stat))
			s1++;
	}
}
draw(y, pr1, pr2)
int	y;
struct seg *pr1, *pr2;
{
	register struct seg *pr;
	register double *xp;
	double	x[32], yy, yi, yii, xi, xii, xj;
	double *xpe;
	for(pr=pr1, xp=x; pr <= pr2;pr++){
		if (pr->stat)
			if (pr->y != pr->Y){
				*xp++ = xcut(y, pr->x, pr->y, pr->X, pr->Y);
				if(xp > &x[31])
					fprintf(stderr, "Too many intersections\n");
			}
	}
	xpe = xp;
	if(xpe < &x[1])
		return;
	do {
		xj = 0;
		for(xp=x+1;xp<xpe;xp++){
			if(*(xp-1) > *xp){
				xj = *(xp-1);
				*(xp-1) = *xp;
				*xp = xj;
			}
		}
	} while(xj);
	yy = y;
	for(xp=x; xp<xpe;xp++){
		xj = *xp++;
		if (!cosp)
			line(xj, unorm(yy), *xp, unorm(yy));
		else {
			xi = sinp * xj + cosp * yy;
			yi = -cosp * xj + sinp * yy;
			xii = sinp * *xp + cosp * yy;
			yii = -cosp * *xp + sinp * yy;
			line(xi, unorm(yi), xii, unorm(yii));
		}
	}

}


double	xcut(y, x1, y1, x2, y2)
{
	register double	dx, dy, xy, x;
	if (y1 == y2) {
		x = x1 > x2 ? x1 : x2;
		return(x);
	}
	dy = y1 - y2;
	dx = x1 - x2;
	xy = (float)y1 * x2 - x1 * (float)y2;
	x = (y * dx + xy) / dy;
	return(x);
}
place(p1, p2)
struct seg *p1, *p2;
{
	if(p1->y > p2->y)
		return(-1);
	else if(p1->y < p2->y)
		return(1);
	else if(p1->x > p2->x)
		return(-1);
	else if(p1->x < p2->x)
		return(1);
	else if(p1->X > p2->X)
		return(-1);
	else if(p1->X < p2->X)
		return(1);
	else if(p1->Y > p2->Y)
		return(-1);
	else if(p1->Y < p2->Y)
		return(1);
	else return(0);
}
