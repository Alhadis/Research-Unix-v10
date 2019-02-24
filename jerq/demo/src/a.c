#include <jerq.h>
main(){
	request(KBD|CPU);
	ripple();
	exit();
}
#define	avg(a,b) (((a)+(b))>>1)
Rectangle
canon(p1, p2)
	Point p1, p2;
{
	Rectangle r;
	r.origin.x = min(p1.x, p2.x);
	r.origin.y = min(p1.y, p2.y);
	r.corner.x = max(p1.x, p2.x);
	r.corner.y = max(p1.y, p2.y);
	return r;
}
ripple() {
	Point p1, p2;
	register xinc, yinc;
	p1.x=p2.x=avg(Drect.origin.x, Drect.corner.x);
	p1.y=p2.y=avg(Drect.origin.y, Drect.corner.y);
	xinc=1;
	yinc=1;
	rectf(&display, Drect, F_XOR);
	while (kbdchar()==-1) {
		rectf(&display, canon(p1, p2), F_XOR);
		if (p1.x==Drect.origin.x || p1.x==Drect.corner.x) xinc=-xinc;
		if (p1.y==Drect.origin.y || p1.y==Drect.corner.y) yinc=-yinc;
		p1.x-=xinc; p1.y-=yinc;
		p2.x+=xinc; p2.y+=yinc;
		wait(CPU);
	}
}
