#include <jerq.h>
box(r, f)
	Rectangle r;
{
	register dx=r.corner.x-r.origin.x-1, dy=r.corner.y-r.origin.y-1;
	if(f==0)
		jrectf(r, F_CLR);
	jmoveto(r.origin);
	jline(Pt(dx, 0), F_XOR);
	jline(Pt(0, dy), F_XOR);
	jline(Pt(-dx,0), F_XOR);
	jline(Pt(0,-dy), F_XOR);
}
Rectangle w1={50,75, 450, 400};
char *t1[]={
	"$ where",
	"ikeya!/usr/rob/jerq",
	"$ who",
	"rob      tty0    Jul  7 08:34",
	"sally    tty3    Jul  7 10:13",
	"amr      tty6    Jul  7 09:58",
	"dbm      tty7    Jul  7 09:31",
	"bart     dk/dk04 Jul  7 10:03",
	"bart     dk/dk06 Jul  7 10:50",
	"$ ",
	0
};
Rectangle w3={75, 425, 475, 950};
Rectangle w2={295, 275, 730, 825};
char *t2[]={
	"$ where",
	"alice!/usr/rob/jerq",
	"$ cat demo.c",
	"#include <jerq.h>",
	"box(r)",
	"    Rectangle r;",
	"{",
	"    register dx=r.corner.x-r.origin.x-1;",
	"    register dy=r.corner.y-r.origin.y-1;",
	"    jrectf(r, F_CLR);",
	"    jmoveto(r.origin);",
	"    jline(Pt(dx, 0), F_STORE);",
	"    jline(Pt(0, dy), F_STORE);",
	"    jline(Pt(-dx,0), F_STORE);",
	"    jline(Pt(0,-dy), F_STORE);",
	"}",
	"texture(){",
	"    register i, j;",
	"    register Word *p=display.base;",
	"    for(i=0; i<YMAX; i+=2){",
	"        for(j=0; j<XMAX/16; j++)",
	"            *p++=0xAAAA;",
	"        for(j=0; j<XMAX/16; j++)",
	"            *p++=0x5555;",
	"    }",
	"}",
	"$ ",
	0,
};
char *sometext[]={
	"Anything may occur",
	"inside a layer...",
	0
};
Point dy={0, 20};
text(p, t)
	Point p;
	char *t[];
{
	register i;
	Point endPt;
	p=add(p, Pt(3, 3));
	for(i=0; t[i]; i++){
		jmoveto(add(p, mul(dy, i)));
		endPt=jstring(t[i]);
	}
	/* Draw a cursor */
	jrectf(Rect(endPt.x, endPt.y, endPt.x+10, endPt.y+14), F_STORE);
	jrectf(Rect(endPt.x+1, endPt.y+1, endPt.x+9, endPt.y+13), F_CLR);
}
Rectangle canon();
main(){
	Rectangle r;
	Bitmap *bp;
	Point p1, p2;
	jinit();
	cursinhibit();
	jgrey(display.rect);
	box(w1, 0);
	text(w1.origin, t1);
	box(w3, 0);
	chart(w3);
	box(w2, 0);
	text(w2.origin, t2);
	cursallow();
   loop:
	do; while(button1()==0);
	p1=mouse.xy;
	p2=mouse.xy;
	r=canon(p1, p2);
	box(r, 1);
	for(; button1(); nap(2)){
		cursinhibit();
		box(r, 1);
		p2=mouse.xy;
		r=canon(p1, p2);
		box(r, 1);
		cursallow();
	}
	if(r.corner.x-r.origin.x<200 || r.corner.y-r.origin.y<50){
		box(r, 1);
		goto loop;	/* too small */
	}
	bp=balloc(r);
	if(bp==0){
		box(r, 1);
		goto loop;
	}
	if(bp!=0){
		cursinhibit();
		box(r, 1);
		bitblt(&display, r, bp, bp->rect.origin, F_STORE);
		box(r, 0);
		cursallow();
		text(r.origin, sometext);
		do;while(button2()==0);
		cursinhibit();
		bitblt(bp, bp->rect, &display, bp->rect.origin, F_STORE);
		bfree(bp);
		cursallow();
	}
	goto loop;
}
int heights[]={401, 351, 241, 375, 121, 61};
int stipples[6][2]={
	0xFFFF, 0xFFFF,
	0x0000, 0x0000,
	0xAAAA, 0x5555,
	0xFFFF, 0xFFFF,
	0x0000, 0x0000,
	0xAAAA, 0x5555,
};
chart(r)
	Rectangle r;
{
	register texty;
	Word *p;
	register i, j;
	Rectangle out;
	texty=r.corner.y-21;
	r.origin.x+=25; r.corner.x-=25; r.origin.y+=25; r.corner.y-=25;
	r.origin.x|=15; r.origin.x++;
	box(r, 0);
	for(i=0; i<(sizeof heights/sizeof (int)); i++){
		r.origin.x+=16;
		r.corner.x=r.origin.x+32;
		r.origin.y=r.corner.y-heights[i];
		out.origin=add(r.origin, Pt(-1, -1));
		out.corner=add(r.corner, Pt( 1,  0));
		box(out, 0);
		jmoveto(r.origin.x, texty);
		jstring("Tax");
		p=display.base+((unsigned)r.origin.y)*(XMAX/16)+(unsigned)(r.origin.x/16);
		for(j=0; j<heights[i]-2; j+=2){
			*p++=stipples[i][0];
			*p--=stipples[i][0];
			p+=XMAX/16;
			*p++=stipples[i][1];
			*p--=stipples[i][1];
			p+=XMAX/16;
		}
		r.origin.x+=32;
	}
}
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
min(a, b)
{
	return(a<b? a : b);
}
max(a, b)
{
	return(a>b? a : b);
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
	if(button3())
		exit(0);
}
