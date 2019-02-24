#include <jerq.h>

#define SCALE (long) 1000
#define STEPS 10

void bspline(b, p, n, c)
Bitmap *b; register Point *p; int n; Code c;
{	register long w, t1, t2, t3; 
	register int i, j; 
	Point q;
	Point pcurrent;

	if (p == (Point *) NULL) return;
	p[0] = p[1];
	p[n+1] = p[n];
	cursinhibit();
	pcurrent = p[0];
	for (i = 0; i < n; i++) {
		for (j = 0; j < STEPS; j++) {
			w = SCALE * j / STEPS;
			t1 = w * w / (2 * SCALE);
			w = w - SCALE/2;
			t2 = 3*SCALE/4 - w * w / SCALE;
			w = w - SCALE/2;
			t3 = w * w / (2*SCALE);
			q.x=(t1*p[i+2].x+t2*p[i+1].x+t3*p[i].x+SCALE/2)/SCALE;
			q.y=(t1*p[i+2].y+t2*p[i+1].y+t3*p[i].y+SCALE/2)/SCALE;
			segment(b, pcurrent, q, c);
			pcurrent = q;
		}
	}
	cursallow();
}
