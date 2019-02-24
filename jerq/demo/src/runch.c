/*% 3cc -go # %
 */
#define	SIZE	256
#include <jerq.h>
Point rescale(p)
Point p;
{
	return(add(sub(Pt(p.x&(SIZE-1), p.y&(SIZE-1)), Pt(SIZE/2, SIZE/2)),
		div(add(Drect.origin, Drect.corner), 2)));
}
Rectangle canon(r)
Rectangle r;
{
	register t;
	if(r.corner.x<r.origin.x){
		t=r.corner.x;
		r.corner.x=r.origin.x;
		r.origin.x=t;
	}
	if(r.corner.y<r.origin.y){
		t=r.corner.y;
		r.corner.y=r.origin.y;
		r.origin.y=t;
	}
	return(r);
}
main(argc, argv)
char *argv[];
{
	Point p, q;
	int dp=argc>1?atoi(argv[1]):1;
	int dpq=argc>2?atoi(argv[2]):33;
	int dq=argc>3?atoi(argv[3]):129;
	int sp=1, spq=1, sq=1;
	request(KBD|CPU);
	for(;;){
		for(p.x=0;p.x!=SIZE;p.x++){
			if(kbdchar()!=-1)
				exit();
			p.y=p.x^sp;
			q.x=p.y^spq;
			q.y=q.x^sq;
			rectf(&display, canon(Rpt(rescale(p), rescale(q))), F_XOR);
			wait(CPU);
		}
		sp+=dp;
		spq+=dpq;
		sq+=dq;
	}
}
