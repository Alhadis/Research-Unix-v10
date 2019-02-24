#include	"/usr/jerq/include/jerq.h"
#include	"instruments.h"
#include	"rebecca.h"

extern Rectangle Bar, dr;
extern int res, Sem4;
extern char buf1[128], buf2[128];

/*** Resolver ***/

makebar(m)
	Point m;
{
	Bar.origin = m;
	Bar.corner = add(m, Pt(10, 90));
	drawbar();
}

clearbar() { drawbar(); }

actbar()
{	Point pt;
	int prevres = res;

	buf2[0] = '\0';
	while (button1())
	{	pt = sub(mouse.xy, dr.origin);
		setreso(blog(pt.y - Bar.origin.y));
		sprintf(buf1, "%d", res);
		tagger(buf1, add(realpoint(Bar.origin), Pt(17, alog(res))), buf2);
		wait(CPU);
	}
	buf1[0] = '\0';
	tagger(buf1, add(realpoint(Bar.origin), Pt(17, alog(res))), buf2);
	if (prevres != res) Sem4=1;
}

drawbar()
{	Rectangle box;

	box = realbox(Bar);
	outline(box, F_XOR);
	box.origin.x -= 2;
	box.corner.x += 2;
	box.origin.y += alog(res);
	box.corner.y  = box.origin.y + 2;
	outline(box, F_XOR);
}

setreso(n)
{
	clearbar();
	res = clog(n);
	drawbar();
}

alog(n)
	register int n;
{
	register int i, j;

	for (i = 512, j = 5; i > 1; i /= 2, j += 10)
		if (n >= i) return j;
	return 90;
}

blog(n)
	register int n;
{
	register int i, j;

	for (i = 10, j = 512; j > 2; i += 10, j /= 2)
		if (n < i) return j;
	return 2;
}

clog(n)
	register int n;
{
	register int i;

	for (i = 512; i > 1; i /= 2)
		if (n >= i) return i;
	return 2;
}
