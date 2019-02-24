#include	<CC/jerq.h>
#include	"pot.pub"

Globepot *g;
Bmappot *b;
Rulepot *hr, *vr;

main()
{
	Rectangle r = Drect;

	request(MOUSE);
	initdisplay();

	while(wait(MOUSE))
	{
		if(moved())
		{
			reshapepot(r, Drect);
			r = Drect;
		}
		if(button1())
		{
			hitpot(1);
			if(b->val) g->setval(hr->val, vr->val);
			/* hitpot has already redrawn */
		}
		else if(button3() && confirm(3))
			exit(0);
	}
}

Texture32 bup = {
#include	"up.icon"
}, bdown = {
#include	"down.icon"
}, a1 = {
#include	"b1.icon"
}, a2 = {
#include	"b2.icon"
}, a3 = {
#include	"b3.icon"
}, a4 = {
#include	"b4.icon"
};

Bitmap *
bt(Texture32 *t)
{
	Bitmap *b = balloc(Rect(0, 0, 32, 32));

	texture32(b, b->rect, t, F_OR);
	return(b);
}

initdisplay()
{
	Rectangle r;
	Bmappot *bb;

	wait(MOUSE);
	string(&defont, "h rule", &display, Drect.o, F_STORE);
	r = getrect(3);
	hr = new Rulepot(r, P_HORIZ|P_TICKS, 360, Point(130, -17),
		Point(50, -17), "Distance");
	string(&defont, "v rule", &display, Drect.o, F_STORE);
	r = getrect(3);
	vr = new Rulepot(r, P_TICKS, 360, Point(80, -17),
		Point(0, -17), "Distance");
	string(&defont, "button 1", &display, Drect.o, F_STORE);
	r = getrect(3);
	b = new Bmappot(r, 0, Point(-50, 8), "Gas");
	b->item(Point(40, 0), "ON", bt(&bup));
	b->item(Point(40, 0), "OFF", bt(&bdown));
	string(&defont, "button 2", &display, Drect.o, F_STORE);
	r = getrect(3);
	bb = new Bmappot(r, P_CIRC, Point(-50, 8), "Snork");
	bb->item(Point(20, -20), "UP", bt(&a1));
	bb->item(Point(20, 20), "DOWN", bt(&a2));
	bb->item(Point(-75, 20), "RIGHT", bt(&a3));
	bb->item(Point(-60, -20), "LEFT", bt(&a4));
	string(&defont, "globe     ", &display, Drect.o, F_STORE);
	r = getrect(3);
	g = new Globepot(r, 0, 45, 45, Pt(120, -20), Pt(0, -20), "where am i");
	drawpot();
}
