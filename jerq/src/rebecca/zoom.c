#include	"/usr/jerq/include/jerq.h"
#include	"instruments.h"
#include	"rebecca.h"

extern Rectangle Zoom, dr;
extern int xoff, yoff, magn, res, Sem4, Sem9;

/*** Zoom box ***/

clearzoom() { drawzoom(); }

makezoom(z)
	Point z;
{
	Zoom.origin = z;
	Zoom.corner = add(z, Pt(40, 20));
	drawzoom();
}

drawzoom()
{	Rectangle box;

	box = realbox(Zoom);
	outline(box, F_XOR);
	segment(&display,add(box.origin,Pt(20,0)),sub(box.corner,Pt(20,0)),F_XOR);
	Tag("Z", Pt(box.origin.x+10, box.origin.y+10));
	Tag("U", Pt(box.origin.x+30, box.origin.y+10));
}

flashzoom(how)
{	Rectangle box;

	box = realbox(Zoom);
	if (!how)
		box.origin.x += 20;
	else
		box.corner.x -= 20;
	rectf(&display, box, F_XOR);
}

actzoom()
{	Point pt;
	char which;
	int dist;

	while (button1())
	{	pt = sub(mouse.xy, dr.origin);
		dist = pt.y - Zoom.origin.y;
		if (dist < 0 || dist > 20) return;
		dist = pt.x - Zoom.origin.x;
		if (dist < 0 || dist > 40) return;
		which = (dist <= 20);
		flashzoom(which);
		sleep(3);
		flashzoom(which);
	}
	if ((which)?zoom():unzoom()) Sem4=1;
}

#define Draw	if (rectclip(&sr, ir)) rectf(&display, sr, F_XOR)
#define UnDraw	Draw

zoom()
{	Point p;
	int i, j, ores=res;
	int ox=SZ, oy=SZ;
	Rectangle sr, ir;

	ir.origin = dr.origin;
	ir.corner = add(dr.origin, Pt(SZ, SZ));
	setreso((magn)?SZ/res:res);
	while (1)
	{	i = (magn) ? ((SZ*ores)/res) : SZ/res;
		sr.origin = add(dr.origin, Pt(ox, oy));
		sr.corner = add(sr.origin, Pt(i, i));
		Draw;
		nap(3);
		if ((button1() && button23()) || (button2() && button13()))
		{	UnDraw;
			setreso(ores);
			while(button123()) ;
			return 0;		/* reject */
		}
		if (button3())			/* accept */
		{	UnDraw;
			if (magn)
			{	xoff += ox/ores;	/* (SZ/ores)*(ox/SZ) */
				yoff += oy/ores;
			} else
			{	xoff = ox;
				yoff = oy;
			}
			xoff = (xoff<0)?0:(xoff>SZ)?SZ:xoff;
			yoff = (yoff<0)?0:(yoff>SZ)?SZ:yoff;
			magn = 1;
			Psem(&Sem9);
			while(button123()) ;
			return 1;
		}
		if (button2()) setreso(res/2);
		if (button1()) setreso(res*2);
		while(button123()) ;
		p = sub(mouse.xy, dr.origin);
		ox = p.x;
		oy = p.y;
		UnDraw;
	}
}

unzoom()
{	int oldn, newn, delta;

	oldn  = SZ/res;
	setreso((3*res)/4);
	newn  = SZ/res;
	delta = (newn-oldn)/2;
		
	xoff -= delta; xoff = (xoff<0)?0:xoff;
	yoff -= delta; yoff = (yoff<0)?0:yoff;
	Psem(&Sem9);

	return 1;
}
