#include	"/usr/jerq/include/jerq.h"
#include	"instruments.h"
#include	"rebecca.h"

/*** Grid Button ***/

extern Rectangle But, dr;
extern int gridon, magn, res;
extern char buf1[128], buf2[128];

makebutton(b)
	Point b;
{
	But.origin = b;
	But.corner = add(b, Pt(Button_r, Button_r));
	drawbutton();
	if (gridon) flashbutton();
}

flashbutton()
{	Point pt;

	pt = realpoint(add(But.origin, Pt(Button_r/4, Button_r/4)));
	disc(&display, pt, Button_r, F_XOR);
}

clearbutton()
{	if (gridon)
		flashbutton();
	drawbutton();
}

drawbutton()
{	Point pt;

	pt = realpoint(add(But.origin, Pt(Button_r/4, Button_r/4)));
	circle(&display, pt, Button_r,   F_XOR);
	disc  (&display, pt, Button_r/2, F_XOR);
}

actbutton()
{
	buf2[0] = '\0';
	gridon = 1 - gridon;
	flashbutton();
	if (magn)
		sprintf(buf1, "%d pel 1:1    ", SZ/res);
	else
		sprintf(buf1, "%d pel/square ", res);
	if (gridon)
	{	tagger(buf1, add(realpoint(But.origin), Pt(2*Button_r, 0)), buf2);
		grid(res);
	} else
		update();
	while (button1()) nap(3);
	buf1[0] = '\0';
	tagger(buf1,  add(realpoint(But.origin), Pt(2*Button_r, 0)), buf2);
}

grid(r)
{	Point p, q, z;
	extern int *ALIVE, QUIT;

	z = dr.origin;

	p.y = 0; q.y = SZ;
	for (p.x = q.x = 0; p.x < SZ && OK; p.x += r, q.x += r)
		segment(&display, add(p, z), add(q, z), F_XOR);

	p.x = 0; q.x = SZ;
	for (p.y = q.y = 0; p.y < SZ && OK; p.y += r, q.y += r)
		segment(&display, add(p, z), add(q, z), F_XOR);
}
