#include <jerq.h>
#include "layer.h"

extern Layer *newlayer();
Layer *lfront=0, *lback=0;

box(l)
	register Layer *l;
{
	cursinhibit();
	jrectf(l->rect, F_STORE);
	jrectf(inset(l->rect, 2), F_CLR);
	cursallow();
}


Layer *l1, *l2, *l3;
Point dy={0, 15};
Layer *
whichlayer(p)
	Point p;
{
	register Layer *lp;
	for(lp=lfront; lp; lp=lp->back)
		if(ptinrect(p, lp->rect))
			return(lp);
	return(0);
}

Rectangle
rectangle(a, b, c, d){
	Rectangle r;
	r.origin.x=a; r.origin.y=b; r.corner.x=c; r.corner.y=d;
	return(r);
}
char *alloc();
main(){
	Rectangle toscreen();
	register Layer *newl;
	register Bitmap *bp;
	register i;
	Point p;
	jinit();
	sysinit();
	jgrey(display.rect);
	spl0();
	l1=newlayer(Rect(100, 100, 300, 300));
	p=add(l1->rect.origin, Pt(3, 3));
	for(i=0; i<=12; i++){
		jmoveto(add(p, mul(dy, i)));
		jstring("1111111111111111111111");
	}
	l2=newlayer(Rect(225, 225, 425, 425));
	p=add(l2->rect.origin, Pt(3, 3));
	for(i=0; i<=12; i++){
		jmoveto(add(p, mul(dy, i)));
		jstring("22222222222222222222");
	}
	l3=newlayer(Rect(125, 250, 275, 350));
	p=add(l3->rect.origin, Pt(3, 3));
	for(i=0; i<=5; i++){
		jmoveto(add(p, mul(dy, i)));
		jstring("3333333333333333");
	}
	bp=balloc(Rect(224, 224, 224+32, 224+32));
	rectf(bp, bp->rect, F_OR);
	check(l1);check(l2);check(l3);
	newl=newlayer(Rect(250, 275, 500, 500));
	check(newl);check(l1);check(l2);check(l3);
	spl0();
	for(;;nap(1)){
		if(button1()){
			cursinhibit();
			upfront(whichlayer(mouse.xy));
			cursallow();
			check(l1);check(l2);check(l3);
			do; while(button1());
			checkl();
		}
		else if(button2()){
/*
			lblt(bp, Rect(224, 224, 224+32, 224+32), l1, F_STORE);
*/
			dellayer(whichlayer(mouse.xy));
			do; while(button2());
			checkl();
			check(l1);check(l2);check(l3);
		}else if(button3()){
			/*
			invertbox();
			 */
			newwindow();
			checkl();
			check(l1);check(l2);check(l3);
		}
	}
}
check(l)
	register Layer *l;
{
/*
	register Obscured *o;
	printf("Layer %d\n", l->z);
	for(o=l->obs; o; o=o->next){
		;
	}
	printf(".......other way.....\n");
	for(o=l->obs; o&&o->next; o=o->next);
	if(o=l->obs; o; o=o->prev){
		;
	}
*/
}
/*
write(a)
	char *a;
{
	while(*a)
		jputchar(*a++);
}
*/
/*
printf(a, b, c, d, e, f, g, h, i, j, k, l)
{
	char buf[256];
	sprintf(buf, a, b, c, d, e, f, g, h, i, j, k, l);
	write(buf);
}
*/
Rectangle
canon(p1, p2)
	Point p1, p2;
{
	Rectangle r;
	r.origin.x = min(p1.x, p2.x);
	r.origin.y = min(p1.y, p2.y);
	r.corner.x = max(p1.x, p2.x);
	r.corner.y = max(p1.y, p2.y);
	return(r);
}
invertbox(){
	Rectangle r;
	Point p1, p2;
	p1=mouse.xy;
	p2=p1;
	r=canon(p1, p2);
	outline(r);
	for(; button3(); nap(2)){
		cursinhibit();
		outline(r);
		p2=mouse.xy;
		r=canon(p1, p2);
		outline(r);
		cursallow();
	}
	outline(r);	/* undraw for the last time */
	lrectf(r, F_XOR, l1);
}
newwindow(){
	Rectangle r;
	Point p1, p2;
	p1=mouse.xy;
	p2=p1;
	r=canon(p1, p2);
	outline(r);
	for(; button3(); nap(2)){
		cursinhibit();
		outline(r);
		p2=mouse.xy;
		r=canon(p1, p2);
		outline(r);
		cursallow();
	}
	cursinhibit();
	outline(r);	/* undraw for the last time */
	(void)newlayer(r);
	cursallow();
}
outline(r)
	Rectangle  r;
{
	register dx=r.corner.x-r.origin.x-1, dy=r.corner.y-r.origin.y-1;
	jmoveto(r.origin);
	jline(Pt(dx, 0), F_XOR);
	jline(Pt(0, dy), F_XOR);
	jline(Pt(-dx,0), F_XOR);
	jline(Pt(0,-dy), F_XOR);
}
min(a, b){
	return(a<b? a : b);
}
max(a, b){
	return(a>b? a : b);
}
checkl(){
/*
	register Layer *l;
	printf("checkl: ");
	for(l=lback; l; l=l->front)
		printf("%d ", l->z);
	printf("\nfront to back: ");
	for(l=lfront; l; l=l->back)
		printf("%d ", l->z);
	printf("\n");
*/
}
jgrey(r)
	Rectangle r;
{
	static Word greymap[16]={
		0xAAAA,	0x5555,	0xAAAA,	0x5555,	0xAAAA,	0x5555,	0xAAAA,	0x5555,
		0xAAAA,	0x5555,	0xAAAA,	0x5555,	0xAAAA,	0x5555,	0xAAAA,	0x5555,
	};
	texture(&display, r, greymap, F_STORE);
}
introutine(){
	extern int end;
	if((mouse.buttons&7)==7)
		exit();
	if(button1()&&button2()){
		*DADDR= (long)&end>>2;
		while(button1()) ;
		*DADDR=156*(1024L/4);
	}
}
