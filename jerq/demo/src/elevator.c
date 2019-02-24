#include <jerq.h>
main()
{
	register x, y, dx, dy;
	register long i;
	jinit();
	sysinit();
	WonB();
	dx=20;
	dy=20;
	for(y=0; y<YMAX; y+=dy){
		jmoveto(Pt(0, y));
		jlineto(Pt(XMAX, y), F_STORE);
	}
	for(x=0; x<XMAX; x+=dx){
		jmoveto(Pt(x, 0));
		jlineto(Pt(x, YMAX), F_STORE);
	}
	for(x=0; x<99; x++){
		bitblt(&display, Rect(99, 99, 300, 300),
			 &display, Pt(100,100), F_STORE);
		bitblt(&display, Rect(99, 399, 300, 600),
			 &display, Pt(100,400), F_STORE);
		bitblt(&display, Rect(99, 699, 300, 900),
			 &display, Pt(100,700), F_STORE);
		bitblt(&display, Rect(499, 99, 700, 300),
			 &display, Pt(500,100), F_STORE);
		bitblt(&display, Rect(499, 399, 700, 600),
			 &display, Pt(500,400), F_STORE);
		bitblt(&display, Rect(499, 699, 700, 900),
			 &display, Pt(500,700), F_STORE);
	}
	eor();
	for(i=200000; --i;) ;
	spl0();
	exit();
}
eor(){
	register long i, *p;
	p=(long *)display.base;
	for(i=0; i<25*1024L; i++)
		*p++^=0xFFFFFFFF;
}
