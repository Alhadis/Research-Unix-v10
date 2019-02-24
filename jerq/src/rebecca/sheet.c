#include	<jerq.h>
#include	"instruments.h"
#include	"rebecca.h"

Rectangle J;
Point A, B, C, D;

rubbersheet()
{	short miny, maxy, minx, maxx; int i;
	extern Rectangle dr;
	extern int Sem8, Sem4;
	extern char anychange;

	while (button123()) nap(1);

	if (Getquad(&J, &A, &B, &C, &D) == 0) return;
	A = whereami(sub(A, dr.origin));
	B = whereami(sub(B, dr.origin));
	C = whereami(sub(C, dr.origin));
	D = whereami(sub(D, dr.origin));
	J.origin = whereami(sub(J.origin, dr.origin));
	J.corner = whereami(sub(J.corner, dr.origin));

	quadhost(A.x, A.y, B.x, B.y, C.x, C.y, D.x, D.y,
		J.origin.x, J.origin.y, J.corner.x, J.corner.y);

	miny = maxy = A.y;
	if (B.y < miny) miny = B.y; else if (B.y > maxy) maxy = B.y;
	if (C.y < miny) miny = C.y; else if (C.y > maxy) maxy = C.y;
	if (D.y < miny) miny = D.y; else if (D.y > maxy) maxy = D.y;
	minx = maxx = A.x;
	if (B.x < minx) minx = B.x; else if (B.x > maxx) maxx = B.x;
	if (C.x < minx) minx = C.x; else if (C.x > maxx) maxx = C.x;
	if (D.x < minx) minx = D.x; else if (D.x > maxx) maxx = D.x;
	cutout(minx, maxx, miny, maxy);
	anychange = 1;
	Sem8 = Sem4 = 1;
}
