#include "frame.h"

int eightspaces=72;	/* sleazy */
Point endpoint;
nexttab(t, x)
	Frame *t;
{
	register xx=x-t->rect.origin.x;
	return(xx-(xx%eightspaces)+eightspaces+t->rect.origin.x);
}
void opshow();
int complete;
frameop(t, op, pt, cp, n)
	register Frame *t;
	register void (*op)();
	Point pt;
	register char *cp;
	register n;
{
	register i, j;
	Point startpt;
	register char *startcp;
	complete=0;
	endpoint=pt;
	i=0;	/* Not in for(;;) because of \t's */
    Top_of_loop:
	endpoint=pt;
	for(j=0,startpt=pt,startcp=cp; ; i++,j++,cp++){
    Continue:
		if(i>=n)
			break;
		if(*cp=='\n'){
			i++, j++, cp++;
	    Emit_newline:
			pt.x=t->rect.corner.x;
			endpoint=pt;
			/* must do this even if j==0 */
			(*op)(t, startpt, pt, startcp, j);
			j=0;
			startpt.x=t->rect.origin.x;
			startpt.y+=newlnsz;
			if(startpt.y >= t->rect.corner.y)	/* off screen */
				return;
			pt=startpt;
			startcp=cp;
			goto Continue;
		}
		if(*cp=='\t'){
			if(j>0){
				/* Emit what's saved up */
				(*op)(t, startpt, pt, startcp, j);
				goto Top_of_loop;
			}
			if((pt.x=nexttab(t, pt.x))>t->rect.corner.x)
				goto Emit_newline;
			(*op)(t, startpt, pt, cp++, 1);
			i++;
			goto Top_of_loop;
		}
		if(*cp>=' ' && *cp<='\177')
			if((pt.x+=cwidth(*cp)) > t->rect.corner.x)
				goto Emit_newline;
	}
	if(startpt.y<t->rect.corner.y){
		endpoint=pt;
		(*op)(t, startpt, pt, startcp, j);
		complete=1;
	}
}
char genbuf[100];
/*ARGSUSED*/
void
opdraw(t, p, q, cp, n)
	Frame *t;
	Point p, q;
	register char *cp;
	register n;
{
	register char *gp=genbuf;
	while(n--){
		if(*cp>=' ' && *cp<='\177')	/* in case the font has ctrl chars in it */
			*gp++= *cp;
		cp++;
	}
	*gp=0;
	p.x++;
	string(&defont, genbuf, D, p, F_XOR);
}
draw(t, p, s, n)
	Frame *t;
	Point p;
	char *s;
	int n;
{
	frameop(t, opdraw, p, s, n);
}

int	F_rectf;
/*ARGSUSED*/
void
oprectf(t, p, q, str, n)
	Frame *t;
	Point p, q;
	char *str;
	short n;
{
	rectf(D, Rpt(p, Pt(q.x, q.y+newlnsz)), F_rectf);
}

selectf(t, f)
	register Frame *t;
{
	F_rectf=f;
	frameop(t, oprectf, ptofchar(t, t->s1), t->str.s+t->s1, t->s2-t->s1);
}
void
opnull()
{}
rXOR(r)	/* a space-saving routine */
	Rectangle r;
{
	rectf(D, r, F_XOR);
}
Rectf(r, f)
	Rectangle r;
	Code f;
{
	rectf(D, r, f);
}
