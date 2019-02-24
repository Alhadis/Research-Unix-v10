#include	<jerq.h>
#include	<font.h>
#include	<msgs.h>
#include	<signal.h>
#include	"instruments.h"
#include	"rebecca.h"

extern Rectangle dr;
extern int *ALIVE, QUIT, Sem2;
extern char anychange; char haschange=0;

struct Proc *PP[4];
struct Proc *startproc();

Point stayclear();
Rectangle Image, Rul, But, Move, MMove, Bar;
Rectangle Writer, Runner, Zoom, Paint, Probe;

int doproc(), paintproc(),  auxproc();
int doit(),   checkpaint(), checkaux();

Iinit()
{
	Image.origin = PT(  0,     0);
	Rul.origin   = PT(  0, SZ+18);
	But.origin   = PT(SZ+25, 120);
	Move.origin  = PT(SZ+20, 150);
	Bar.origin   = PT(SZ+20,  10);
	Zoom.origin  = PT(SZ+10, 200);
	Paint.origin = PT(SZ+60, 130);
	Probe.origin = PT(SZ+60, 190);
	Writer.origin = PT(SZ+50, 10);
	Runner.origin = PT(SZ+50, 80);
	instruments();
}

instruments()
{
	makeimage(Image.origin);
	makeruler(Rul.origin);
	makebutton(stayclear(But.origin));
	makemove(stayclear(Move.origin));
	makebar(stayclear(Bar.origin));
	makezoom(stayclear(Zoom.origin));
	makepaint(stayclear(Paint.origin));
	makeprobe(stayclear(Probe.origin));
	makewriter(stayclear(Writer.origin));
	makerunner(stayclear(Runner.origin));
}

clearinstruments()
{	rectf(&display, dr, F_CLR);
}

starters()
{
	PP[0] = startproc(doproc,    "image");
	PP[1] = startproc(paintproc, "paint");
	PP[2] = startproc(auxproc,   "aux");
/*	upfront(P->layer);	*/
}

generic(gproc)
	int (*gproc)();
{
	while (OK)
	{	gproc();
		wait(CPU);
	}
	kill(SIGKILL); 
}

doproc()    { generic(doit);       }
paintproc() { generic(checkpaint); }
auxproc()   { generic(checkaux);   }

checkpaint()
{
	checkin(actpaint, clearpaint, makepaint, &Paint);
}

checkaux()
{
	if (anychange!=haschange)
	{	clearwrite();
		haschange=anychange;
		drawriter();
	}
	checkin(actwrite,  clearwrite,  makewriter, &Writer);
	checkin(actrun,    clearrun,    makerunner, &Runner);
	checkin(actbutton, clearbutton, makebutton, &But);
	checkin(actbar,    clearbar,    makebar,    &Bar);
	checkin(actprobe,  clearprobe,  makeprobe,  &Probe);
	checkin(actzoom,   clearzoom,   makezoom,   &Zoom);
	checkin(actruler,  nothing,     nothing,    &Rul);
	checkin(actmove,   clearmove,   makemove,   &Move);
}

checkin(act, clear, make, Ori)
	int	(*act)(), (*clear)(), (*make)();
	Rectangle *Ori;
{
	Rectangle Touch;
	int mess=0;

	Touch.origin = sub(Ori->origin, Pt(10, 10));
	Touch.corner = add(Ori->corner, Pt(10, 10));
	if (!ptinrect(sub(mouse.xy, dr.origin), Touch)) return 0;

	if (button1())
		return act();
	while (button2())
	{	if (ptinrect(mouse.xy, dr))
		{	clear();
			make(sub(mouse.xy, dr.origin));
			mess=1;
		}
		wait(CPU);
	}
	if (mess)
	{	clear();
		make(stayclear(Ori->origin));
	}
	return 0;
}

struct Proc *
startproc(f, str)
	int (*f)();
	char *str;
{
	struct Proc *p;

	p = newproc(f);
	if (p == 0)
	{	disp("startproc failed", dr.origin);
		sleep(10);
		quit();
		return;
	}
	p->text = P->text;			/* keeps 3pi happy */
	p->data = P->data;
	p->layer = newlayer(Rect(0,0,0,0));
	if (p->layer == 0)
	{	p->state = 0; p = 0;
		disp("newlayer failed", dr.origin);
		sleep(10);
		quit();
		return;
	}
	muxnewwind(p, C_NEW);

	return p;
}

Point
PT(a, b)
{	Point t;
	t.x = a; t.y = b;
	return t;
}

Rectangle
realbox(r)
	Rectangle r;
{
	Rectangle box;

	box.origin = add(r.origin, dr.origin);
	box.corner = add(r.corner, dr.origin);

	return box;
}

Point
stayclear(pt)
	Point pt;
{
	Point npt;
	npt.x = pt.x; npt.y = pt.y;

	while ((npt.x <= Image.corner.x && npt.y <= Rul.corner.y)
		|| !ptinrect(add(npt, dr.origin), inset(dr, 10)))
	{	npt.x = rand()%(dr.corner.x-dr.origin.x);
		npt.y = rand()%(dr.corner.y-dr.origin.y);
		nap(2);
	}
	return npt;
}

Tag(s, pt)
	char *s;
	Point pt;
{
	pt.x -= 4;
	pt.y -= 8;
	string(&defont, s, &display, pt, F_XOR);
}

makewriter(m)
	Point m;
{	Writer.origin = m;
	Writer.corner = add(m, Pt(75, 50));
	drawriter();
}

flashwriter(how)
{	Rectangle box;

	box = realbox(Writer);
	if (how)
		box.origin.y += 25;
	else
		box.corner.y -= 25;
	rectf(&display, box, F_XOR);
}

flashbox(box)
	Rectangle box;
{
	rectf(&display, realbox(box), F_XOR);
}

clearwrite() { drawriter(); }

drawriter()
{	Rectangle box;
	char buf[128];

	box = realbox(Writer);
	outline(box, F_XOR);
	segment(&display,add(box.origin,Pt(0,25)),add(box.origin,Pt(75,25)),F_XOR);
	sprintf(buf, "write%s", (haschange)?"*":"");
	Tag(buf, add(realpoint(Writer.origin), Pt(10, 10)));
	sprintf(buf, "reopen%s", (haschange)?"*":"");
	Tag(buf, add(realpoint(Writer.origin), Pt(10, 35)));
}

actwrite()
{	Point pt; int how;
	extern int Sem5;

	pt = sub(mouse.xy, dr.origin);
	while (button1() && ptinrect(pt, Writer))
	{	flashwriter(how=(pt.y > 25));
		sleep(3);
		pt = sub(mouse.xy, dr.origin);
		flashwriter(how);
	}

	if (ptinrect(sub(mouse.xy, dr.origin), Writer))
	{	flashwriter(how);
		if (how)
			Psem(&Sem5);
		else
			Psem(&Sem2);
		flashwriter(how);
	}
}

makerunner(m)
	Point m;
{	extern int runlength;

	Runner.origin = m;
	Runner.corner = add(m, Pt((runlength)?130:95, 25));
	drawrunner();
}

actrun()
{	extern int runlength;

	while (button1()) ;
	if (ptinrect(sub(mouse.xy, dr.origin), Runner))
	{	clearrun();
		runlength = 1 - runlength;
		makerunner(Runner.origin);
	}
}

clearrun() { drawrunner(); }

drawrunner()
{	Rectangle box;
	char buf[128];
	extern int runlength;

	box = realbox(Runner);
	outline(box, F_XOR);
	sprintf(buf, "%srunlength", (runlength)?"non-":"");
	Tag(buf, add(realpoint(Runner.origin), Pt(10, 10)));
}
