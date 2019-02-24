#include	"/usr/jerq/include/jerq.h"
#include	"instruments.h"
#include	"rebecca.h"

/*** Probe ***/

extern Rectangle Probe, dr, Image;
extern unsigned char *photo[SZ];

char buf1[128], buf2[128];

makeprobe(pt)
	Point pt;
{
	Probe.origin = pt;
	Probe.corner = add(pt, Pt(40, 30));
	drawprobe();
}

clearprobe()	{ drawprobe(); }
enlarge()
{
	clearprobe();
	Probe.corner = add(Probe.corner, Pt(85, 0));
	drawprobe();
}
reduce()
{
	clearprobe();
	Probe.corner = sub(Probe.corner, Pt(85, 0));
	drawprobe();
}

drawprobe()
{	Rectangle box;

	box = realbox(Probe);
	outline(box, F_XOR);
	box.origin = add(box.origin, Pt(3,3));
	box.corner = sub(box.corner, Pt(3,3));
	outline(box, F_XOR);
	Tag("P:", Pt(box.origin.x+5, box.origin.y+10));
}

flashprobe(how)
{	Rectangle box;

	box = realbox(Probe);
	rectf(&display, box, F_XOR);
}

Point lastp, destp;

actprobe()
{	Point pt, q;
	Rectangle box;
	Point putwhere;

	enlarge();
	box = realbox(Probe);
	putwhere = add(box.origin, Pt(25, 13));
	lastp = destp = realpoint(Probe.origin);
	flashprobe();
	while (button1()) ;
	pt = sub(mouse.xy, dr.origin);
	while (!button1() && ptinrect(mouse.xy, Drect))
	{	connect(mouse.xy);
		pt = sub(mouse.xy, dr.origin);
		if (ptinrect(pt, Image))
		{	pt = whereami(sub(mouse.xy, dr.origin));
			sprintf(buf1, "%d,%d=%d   ",
					pt.x, pt.y, 255-photo[pt.y][pt.x]);
			tagger(buf1, putwhere, buf2);
		}
		/* nap(3);	*/ wait(CPU);
	}
	buf1[0] = '\0';
	connect(destp); tagger(buf1, putwhere, buf2);
	flashprobe();
	reduce();
	return 0;
}

Point lastwhere;

tagger(thisstr, putwhere, buf)
	char *thisstr;
	Point putwhere;
{
	Tag(buf, lastwhere);
	Tag(thisstr, putwhere);
	strcpy(buf, thisstr);
	lastwhere = putwhere;
}

connect(pt)
	Point pt;
{
	segment(&display, lastp, destp, F_XOR);
	segment(&display, pt, destp, F_XOR);
	lastp = pt;
}
