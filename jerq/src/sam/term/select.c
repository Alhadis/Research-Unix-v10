#include "frame.h"

select(f)	/* when called, button1 is down and we own mouse */
	register Frame *f;
{
	Posn p0, p1, q;
	Point pt0, pt1, qt;
	selectp(f, F_XOR);
	p0=p1=charofpt(f, mouse.xy);
	pt0=ptofchar(f, p0);
	pt1=ptofchar(f, p1);
	selectf(f, pt0, pt1, F_XOR);
	do{
		q=charofpt(f, mouse.xy);
		if(p1!=q){
			if(p0==p1)
				selectf(f, pt0, pt1, F_XOR);
			qt=ptofchar(f, q);
			if(p1<q)
				selectf(f, pt1, qt, F_XOR);
			else
				selectf(f, qt, pt1, F_XOR);
			p1=q, pt1=qt;
			if(p0==p1)
				selectf(f, pt0, pt1, F_XOR);
		}
	}while(button1());
	if(p0<p1)
		f->p0=p0, f->p1=p1;
	else
		f->p0=p1, f->p1=p0;
}
/* it is assumed p0<=p1 and both were generated by ptofchar() */
selectf(f, p0, p1, c)
	register Frame *f;
	Point p0, p1;
	Code c;
{
	int n;
	Point q0, q1;
	p0.x--, p1.x--;
	q0=p0, q1=p1;
	q0.y+=f->font->height, q1.y+=f->font->height;
	n=(p1.y-p0.y)/f->font->height;
	if(f->b==0)
		panic("selectf b==0");
	if(p0.y==f->r.corner.y)
		return;
	if(n==0){
		if(p0.x==p1.x)
			q1.x++;
		rectf(B, Rpt(p0, q1), c);
	}else{
		rectf(B, Rect(p0.x, p0.y, f->r.corner.x, q0.y), c);
		if(n>1)
			rectf(B, Rect(f->r.origin.x, q0.y, f->r.corner.x, p1.y), c);
		rectf(B, Rect(f->r.origin.x, p1.y, q1.x, q1.y), c);
	}
}
selectp(f, c)
	register Frame *f;
	Code c;
{
	Point pt0, pt1;
	pt0=ptofchar(f, f->p0);
	pt1=f->p0==f->p1? pt0 : ptofchar(f, f->p1);
	selectf(f, pt0, pt1, c);
}
