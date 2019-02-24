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
	Point p0, q0, p1, q1;
	int dp=argc>1?atoi(argv[1]):1;
	int dpq=argc>2?atoi(argv[2]):33;
	int dq=argc>3?atoi(argv[3]):129;
	int sp0=1, spq0=1, sq0=1, sp1, spq1, sq1, first=1;
	request(KBD|CPU);
	for(;;){
		for(p0.x=0;p0.x!=SIZE;p0.x++){
			if(kbdchar()!=-1)
				exit();
			p0.y=p0.x^sp0;
			q0.x=p0.y^spq0;
			q0.y=q0.x^sq0;
			segment(&display, rescale(p0), rescale(q0), F_XOR);
			if(!first){
				p1.x=p0.x;
				p1.y=p1.x^sp1;
				q1.x=p1.y^spq1;
				q1.y=q1.x^sq1;
				segment(&display, rescale(p1), rescale(q1), F_XOR);
			}
			wait(CPU);
		}
		sp1=sp0;
		spq1=spq0;
		sq1=sq0;
		sp0+=dp;
		spq0+=dpq;
		sq0+=dq;
		first=0;
	}
}
