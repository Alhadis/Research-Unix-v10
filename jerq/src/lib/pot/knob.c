#include	<CC/jerq.h>

#define		N 		24
#define		R		20
#define	ray(x,r)	Pt(muldiv(cos(360*x/N),r,1024), muldiv(sin(360*x/N),r,1024))
#define	disp(i,r)		disc(&display,add(p,ray(i,R-r/2+1)),r,F_OR)
main()
{
	Point p;
	register i;

	request(KBD);
	p = div(add(Drect.origin, Drect.corner), 2);
	circle(&display, p, R, F_OR);
	for(i = 0; i < N; i++)
		segment(&display, p, add(p, ray(i,R+1)), F_OR);
	disp(9,4);
	disc(&display, p, 10, F_CLR);
	circle(&display, p, 10, F_OR);
	circle(&display, p, 8, F_OR);
	wait(KBD);
}
