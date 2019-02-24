#include <jerq.h>
main(){
	register i;
	Point p,q;
	request(KBD|CPU);
	p = div(add(Drect.origin,Drect.corner),2);
	q = sub(Drect.corner,Drect.origin);
	for(i=norm(q.x, q.y, 0)/2; i>0; i--) {
		disc(&display, p, i, F_XOR);
		wait(CPU);
	}
	wait(KBD);
}
