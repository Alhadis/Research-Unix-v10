/*% 3cc -o # %
 */
#include <jerq.h>
#include <font.h>
#define	CHARH	defont.height
#define	CHARW	defont.info['0'].width
Bitmap screen;
char *menutext[]={
	"stop",
	"chars",
	"pixels",
	"exit",
	0
};
Menu menu={menutext};
int hog;
Point myposn, cen;
Point mymouse(){
	Point p;
	wait(MOUSE);
	if(hog){
		p=add(sub(mouse.xy, cen), myposn);
		if(!eqpt(p, myposn)){
			mycursor();
			myposn=p;
			if(myposn.x<0) myposn.x=0;
			if(800<=myposn.x) myposn.x=799;
			if(myposn.y<0) myposn.y=0;
			if(1024<=myposn.y) myposn.y=1023;
			mycursor();
			cursset(cen);
		}
	}
	else
		myposn=mouse.xy;
	return(myposn);
}
track(me){
	if(me){
		if(!hog){
			hog++;
			menutext[0]="stop";
			cursinhibit();
			wait(MOUSE);
			myposn=mouse.xy;
			cursset(cen);
		}
	}
	else if(hog){
		menutext[0]="measure";
		hog=0;
		cursset(myposn);
		cursallow();
	}
}
int xres=1, yres=1;
Texture tracktex={
0x0000,0x0000,0x0000,0x0000,
0x0000,0x0000,0x0000,0x0000,
0x0080,0x01C0,0x03E0,0x07F0,
0x0FF8,0x1FFC,0x3FFE,0x7FFF,
};
Bitmap *tracker;
mycursor(){
	bitblt(tracker, tracker->rect, &screen, sub(myposn, Pt(8, 8)), F_XOR);
}
main(){
	Point p, q, r;
	request(CPU|MOUSE);
	tracker=balloc(Rect(0, 0, 16, 16));
	screen.base=addr(&display, Pt(0, 0));
	screen.width=XMAX/WORDSIZE;
	screen.rect=Jrect;
	texture(tracker, tracker->rect, &tracktex, F_STORE);
	cen=div(add(Drect.origin, Drect.corner), 2);
	track(1);
	mycursor();
	for(;;){
		p=mymouse();
		if(hog && button1()){
			q=p;
			mycursor();
			scroll();
			stats(p, q);
			mycursor();
			box(p, q);
			while(button1()){
				r=mymouse();
				r.x=(r.x-p.x)/xres*xres+p.x;
				r.y=(r.y-p.y)/yres*yres+p.y;
				if(r.x!=q.x || r.y!=q.y){
					box(p, q);
					q=r;
					stats(p, q);
					box(p, q);
				}
			}
			box(p, q);
		}
		if(button3()){
			if(hog){
				mycursor();
				cursallow();
			}
			switch(menuhit(&menu, 3)){
			case 0:	track(!hog);		break;
			case 1:	xres=CHARW; yres=CHARH;	break;
			case 2:	xres=1; yres=1;		break;
			case 3:	cursallow(); exit(0);
			}
			if(hog){
				cursinhibit();
				mycursor();
			}
		}
	}
}
scroll(){
	bitblt(&display, Rpt(add(Drect.origin, Pt(0, CHARH)), Drect.corner),
		&display, Drect.origin, F_STORE);
	rectf(&display, Rpt(Pt(Drect.origin.x, Drect.corner.y-CHARH), Drect.corner),
		F_CLR);
}
isqrt(x)
register x;
{
	register s, u;
	if(x<=0)
		return(0);
	for(s=2, u=4;u<x;s+=s, u*=4);
	while((u=((x+s*s)/s)>>1)<s)
		s=u;
	return(s);
}
stats(p, q)
Point p, q;
{
	char buf[100];
	register x=(abs(p.x-q.x)+xres-1)/xres, y=(abs(p.y-q.y)+yres-1)/yres;
	sprintf(buf, "down=%d %d, up=%d %d, size=%d %d, diag=%d",
		p.x, p.y, q.x, q.y, x, y, isqrt(x*x+y*y));
	rectf(&display,Rpt(string(&defont, buf, &display,
		Pt(Drect.origin.x+4, Drect.corner.y-CHARH), F_STORE),
			Drect.corner), F_CLR);
}
box(p, q)
Point p, q;
{
	segment(&screen, p, Pt(p.x, q.y), F_XOR);
	segment(&screen, p, Pt(q.x, p.y), F_XOR);
	segment(&screen, q, Pt(p.x, q.y), F_XOR);
	segment(&screen, q, Pt(q.x, p.y), F_XOR);
}
