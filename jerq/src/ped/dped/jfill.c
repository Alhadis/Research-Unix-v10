/*
*	Polygon Filling Algorithm, plus PED calls
*/
#include "jped.h"

#define HUGE 2048
typedef short number;
typedef long acnumber;
typedef Texture * Paint;
#define paint(X1,X2,Y,C) texture(&display,Rect(X1,Y,X2,Y+1),C,dr_mode);
#define CV(A) (short)A

typedef struct seg {
	number x, y, X, Y;
	char stat;
	}  segS, * segP;

segP sinp;

short bottom;
/*	menu handling and texture definitions of ped	*/

char *menuSH[2];

short shades(a)
	objP a;
{
	short jseat;

	if(subsel<0) return(0);
	/* see if coloring mode */
	if(a->type=='t' || a->type=='f') return(-1);
	if(fullob(a) && subsel != m_size && ('E' + subsel)==a->color ){
		jtag(0,"border");
		a->size = a->size? 0: 1;
		if(a->type=='Q' || a->type=='S') pl_poly(a,a->p0,a->p0 + a->n-1);
		return(0);
	}
	plot_one(a);	/*erase*/
	if(subsel==m_size){
		a->size = 0;
		if((a->type=='c' || a->type=='S') && fullob(a))
			a->color = SOLID;
		else
		if(fullob(a)) {
			a->type = 'P';	/*unfill*/
			a->color = SOLID;
		}
	}
	else	{
		if(a->type == 'c') {
			if( (jseat = obseat(a))<0) return(0);
			host_one(jseat,"k");
			a = obj[jseat];
		}
		if(a->type == 'p' || a->type == 's') a->type += ('P'-'p');
		if(a->type == 'P') {
			a->size = 0;
			a->type = 'Q';
		}
		a->color = 'E' + subsel;	/*will map into E to N */
	}
	plot_one(a);	/*plot new version*/
	return(0);
}

extern Texture nrdish, nldish, ncrcross, dotsh, pdotsh, grish;

Texture * T_tab[] = {
		&blksh, &hrzsh, &vrtsh, &rdish, &ldish,
		&nrdish, &nldish, &crcross, &ncrcross, &dotsh,
		&pdotsh, &grish,		
		(Texture *)0, &clock
	};

short ppfill(a)
	objP a;
{
	Texture * shade;
	if(a->color>='E' && a->color<='P') shade = T_tab[a->color-'E'];
	else shade = &skull;
	if(a->size && nosk) {
		if(a->type=='Q' || a->type=='S') pl_poly(a,a->p0,a->p0 + a->n-1);
	}
	pfill(a->p0,a->n,shade);
}
/*	end of ped stuff	*/

short pfill(pol, n, color)
	Point pol[]; short n; Paint color;
{
	short  so;
	if( (
	sinp = (segP)alloc((unsigned)((n+8)*sizeof(segS)))
	) == (segP)0) return(-1);
	so = sides(pol,n,sinp);
	if(so) realfill(so,color);
	free(sinp);
	return(1);
}

#define isbreak(A) (A->x < SILLY)

short sides(pol,n,array)
	Point pol[];
	segP array;
{
	register i, j, k;
	register Point *pp, *opp;
	register segP cur_side;

	bottom = HUGE;	/* init. to too big a value for fill */
	cur_side = array;
	for(i=0,opp=pp=pol,k=0; i<n; i++,pp++) {
		if(isbreak(pp)){
			j = mk_sides(opp,pp,cur_side);
			cur_side += j;
			k += j;
			opp = pp + 1;
		}
	}
	k += mk_sides(opp,pp,cur_side);
	return(k);
}

short mk_sides(p1,p2,sides)
	Point *p1, *p2;
	segP sides;
{
	register pointP qq, pp;
	register segP sp;
	number x, y, X, Y;
	short skount;	/* how many sides found */

	if(p2<=p1) return(0);
	qq = p2-1;	/* last point */
	x = qq->x;	y = qq->y;
	for(pp=p1,sp=sides,skount=0; pp<p2; pp++){
		X = pp->x;	Y = pp->y;
		if(Y<bottom) bottom = Y;
		sp->x = x;	sp->y = y;
		sp->X = X;	sp->Y = Y;
		sp->stat = 1;
		if(sp->Y > sp->y) sgswitch(sp);
		else if(sp->Y==sp->y && sp->X>sp->x) sgswitch(sp);
		x = X;	y = Y;
		sp++; skount++;
	}
	ed_sides(sides,skount);
	return(skount);
}

short sgswitch(p) segP p;
{
	number xyt;
	xyt = p->y; p->y = p->Y; p->Y = xyt;
	xyt = p->x; p->x = p->X; p->X = xyt;
}

short ed_sides(sides,n)
	segP sides;
{
	register segP sp;
	register segP sbf, snx;
	segP sfin;

	/* edit horizontals */
	sfin = sides + n;
	for(sp=sides;sp<sfin;sp++){
		if(!(sp->stat)) continue;
		if(sp->y != sp->Y) continue;
		if(sp==sides) sbf = sp + n - 1; 
		else sbf = sp-1;
		if(sp<sfin) snx = sp+1;
		else snx = sides;
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
}
/* Filling interior of arbitrary polygons */

#define COMP(A)  if(p1->A>p2->A) return(-1); else if(p1->A<p2->A) return(1);

short place(p1,p2) segP p1, p2;
{
	COMP(y) else { 
		COMP(x) else { 
			COMP(X) else { 
				COMP(Y) else return(0);
			}
		}
	}
}

/* Fill a contour defined by a set of line segments */

short realfill(n_sides,shade) short n_sides; Paint shade;
{
	register i;
	register segP  sp;
	segP s1, s2, sfin, st;
	short yb;

	qsort(sinp,n_sides,sizeof(struct seg),place);

	sfin = sinp + n_sides;
	sp = sinp; 
	i = 0;
	s1 = sp; 
	yb = s1->y;
	for(;yb<=sp->y;sp++,i++);
	s2 = sp - 1;
	for(;yb>=bottom;yb--){
		draw(yb,s1,s2,shade);
		for(st=s1;st<=s2;st++){
			if(!(st->stat)) continue;
			if(yb<=st->Y) st->stat=0;
		}
		if(sp >= sfin) continue; /*nomore*/
		if(i>=n_sides) continue;
		for(; yb<=(sp->y+1)&&i<n_sides; sp++,i++){
			/*we have gone below next element */
			for(st=s1;st<=s2;st++){
				if(sp->y==st->Y){
					if(sp->x==st->X) st->stat=0;
					else if(sp->Y==sp->y&&sp->X==st->X) st->stat=0;
				}
			}
		}
		/*cleanup*/
		s2 = sp - 1;
		while(s1<sfin && !(s1->stat)) s1++;
	}
}

#define NCUTS 32
#define NCUTS_1 31

short draw(y,pr1,pr2,shade) short y; Paint shade;  segP pr1, pr2;
{
	register segP pr;
	register i, j, k, n;
	short x[NCUTS];

	for(pr=pr1,n=0; pr<=pr2 && n<NCUTS_1 ; pr++)
		if(pr->stat && pr->y!=pr->Y) x[n++] = xcut(y,pr);

	if(n<2) return(0);
	do {
		k = 0;
		for(i=1;i<n;i++){
			if(x[i-1]>x[i]) {
				j=x[i-1]; 
				x[i-1]=x[i]; 
				x[i]=j; 
				k=1;
			}
		}
	} while(k);
	for(i=1;i<n;i +=2) paint(x[i-1],x[i],y,shade);
	return(1);
}

short xcut(y,sgp)
	segP sgp;
{
	acnumber dx, dy, xy;
	acnumber x, x1, y1, x2, y2;
	x1 = sgp->x;	y1 = sgp->y;
	x2 = sgp->X;	y2 = sgp->Y;

	if(y1==y2) {
		x = x1>x2 ? x1: x2;
		return(CV(x));
	}

	dy = y1-y2; 
	dx = x1-x2; 
	xy = y1*x2-x1*y2;
	x = (y*dx+xy)/dy;	/*dy non zero because of prev. check*/
	return(CV(x));
}
