#include <jerq.h>
#define	SIZE	33
main(){
	request(KBD|CPU);
	bounce1();
	exit();
}
bounce1() {
	int XSIZE=Drect.corner.x-SIZE, YSIZE=Drect.corner.y-SIZE;
	int x, y, xinc, yinc, i;
	xinc=1;
	yinc=1;
	rectf(&display, Drect, F_XOR);
	x=Drect.origin.x; y=Drect.origin.y;
	while (kbdchar()==-1){
		rectf(&display, Rect(x, y, x+SIZE, y+SIZE), F_XOR);
		x=x+xinc;
		if ((x==XSIZE) || (x==Drect.origin.x)) xinc= -xinc;
		y=y+yinc;
		if ((y==YSIZE) || (y==Drect.origin.y)) yinc= -yinc;
		wait(CPU);
	}
}
