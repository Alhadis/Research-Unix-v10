#include <jerq.h>
Rectangle rect={400, 400, 501, 501};
main()
{
	register x, y;
	register i;
	int xmin,xmax,ymin,ymax;
	request(KBD);
	xmin = Drect.origin.x;
	xmax = Drect.corner.x;
	ymin = Drect.origin.y;
	ymax = Drect.corner.y;
	for(y=ymin; y<ymax; y+=20)
		segment(&display, Pt(xmin, y), Pt(xmax, y), F_XOR);
	for(x=xmin; x<xmax; x+=20)
		segment(&display, Pt(x, ymin), Pt(x, ymax), F_XOR);
	rect.origin = div(add(Drect.origin,Drect.corner),2);
	rect.corner = add(rect.origin,Pt(101,101));
	while(rect.origin.x>xmin && rect.origin.y>ymin){
		bitblt(&display, rect, &display, sub(rect.origin, Pt(1, 1)), F_STORE);
		rect=rsubp(rect, Pt(1, 1));
	}
	wait(KBD);
}
