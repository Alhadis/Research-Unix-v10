#include	"/usr/jerq/include/jerq.h"
#include	"instruments.h"
#include	"rebecca.h"

#define Wantright	want(-10, -20,   0,   0)
#define Wantleft	want( 20,  10,   0,   0)
#define Wantdown	want(  0,   0, -10, -20)
#define Wantup		want(  0,   0,  20,  10)
#define Wantreset	want(  0,   0,   0,   0)
#define Wantany		want(-10,  10, -10,  10)

extern Rectangle MMove, Move, dr;
extern int xoff, yoff, magn, res, minres, Sem4;

/*** Move Box ***/

makemove(m)
	Point m;
{
	Move.origin = m;
	Move.corner = add(m, Pt(20, 20));
	MMove.origin = sub(m, Pt(12, 12));
	MMove.corner = add(m, Pt(35, 35));

	drawmove();
}

drawmove()
{	Rectangle box;
	Point h1,h2,h3,h4;

	box = realbox(Move);
	outline(box, F_XOR);

	h1 = PT(box.origin.x - 10, box.origin.y + 10);
	h2 = PT(box.origin.x + 10, box.origin.y - 10);
	h3 = PT(box.corner.x + 10, box.origin.y + 10);
	h4 = PT(box.origin.x + 10, box.corner.y + 10);
	segment(&display, h1, h2, F_XOR);
	segment(&display, h2, h3, F_XOR);
	segment(&display, h3, h4, F_XOR);
	segment(&display, h4, h1, F_XOR);
	Tag("M", Pt(box.origin.x+10, box.origin.y+ 10));
}

clearmove() { drawmove(); }

flashmove()
{
	rectf(&display, realbox(Move), F_XOR);
}

want(a,b,c,d)
{	Point pt;
	pt = sub(mouse.xy, dr.origin);
	return (pt.x >= (Move.origin.x + a) && pt.x <= (Move.corner.x + b)
	&&  pt.y >= (Move.origin.y + c) && pt.y <= (Move.corner.y + d));
}

actmove()
{	int v=0, ox=xoff, oy=yoff, ores=res;
	extern int Sem7, Sem9;

	flashmove();
	while (button1() && Wantany)
		wait(CPU);
	if (!Wantany)
	{	flashmove();
		return;
	}
	if (Wantleft)
	{	ox += (magn)?max(1,SZ/(4*res)): max(1,SZ/4);
		ox  = (ox>SZ)?SZ:ox;
		v=1;
	} else if (Wantright)
	{	ox -= (magn)?max(1,SZ/(4*res)): max(1, SZ/4);
		ox  = (ox<0)?0:ox;
		v=1;
	} else if (Wantup)
	{	oy += max(1,SZ/(4*res));
		oy  = (oy>SZ)?SZ:oy;
		v=1;
	} else if (Wantdown)
	{	oy -= max(1,SZ/(4*res));
		oy  = (oy<0)?0:oy;
		v=1;
	} else if (Wantreset)
	{	ox = oy = magn = 0;
		ores = minres;
		Sem7 = 0;
		v=1;
	}
	flashmove();

	xoff = ox; yoff = oy;
	Psem(&Sem9);
	setreso(ores);

	if (v) Sem4 = 1;
}
