#include	"/usr/jerq/include/jerq.h"
#include	"instruments.h"
#include	"rebecca.h"

/*** Greyscale (cannot be dragged) ***/

extern Rectangle Rul, dr;
extern int sos, eos, Sem4;
extern char buf1[128], buf2[128];

makeruler(b)
	Point b;
{	Rul.origin = b;
	Rul.corner = add(b, Pt(SZ, 27));
	drawruler();
}

actruler()
{	Point pt;
	int os=sos, oe=eos;

	while (button1())
	{	pt = sub(mouse.xy, dr.origin);
		if (!ptinrect(pt, Rul))
		{	wait(CPU);
			continue;
		}
		if (abs(pt.x/width - eos) < abs(pt.x/width - sos))
		{	eos = pt.x/width;
			eos = (eos < 15)?15:(eos>255)?255:eos;
			sos = (sos > eos-15)?eos-15:sos;
		} else
		{	sos = pt.x/width;
			sos = (sos < 0)?0:(sos>240)?240:sos;
			eos = (eos < sos+15)?sos+15:eos;
		}
		greyruler();
		sprintf(buf1, "%d<->%d", sos, eos);
		tagger(buf1, add(realpoint(Rul.origin), Pt(SZ-80, 10)), buf2);
	}
	tagger("", add(realpoint(Rul.origin), Pt(SZ-80, 10)), buf2);
	if (sos!=os || eos!=oe) Sem4=1;
}

drawruler()
{	Point pt, tp;
	Rectangle r;
	int j;

	r.origin = Rul.origin;
	r.corner = add(Rul.origin, Pt(SZ, 27));
	burp(0, r, BLIND);
	tp.x = width; tp.y = height;
	for (pt.x = 0, pt.y = Rul.origin.y, j = 0; j < 256; pt.x += width, j++)
	{	r.origin = pt;
		r.corner = add(pt, tp);
		burp(j, r, BLIND);
	}
	greyruler();
	Blast(Rul.origin.y, Rul.origin.y+height);
	greykader();
}

greykader()
{	Rectangle box;

	box.origin = sub(Rul.origin, Pt(0, 1));
	box.corner = add(Rul.origin, Pt(256*width+1, height+1));
	box = realbox(box);
	outline(box, F_STORE);
}

greyruler()
{	Rectangle box;

	box.origin = add(Rul.origin, Pt(   0, height+4));
	box.corner = add(Rul.origin, Pt(SZ+2, height+8));
	burp(0, box, SHOW);		/* clear old ruler */

	box.origin = add(Rul.origin, Pt(width*sos+1, height+5));
	box.corner = add(Rul.origin, Pt(width*eos+2, height+7));
	box = realbox(box);
	outline(box, F_STORE);		/* write new ruler */
}

nothing() { }
