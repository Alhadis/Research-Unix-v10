#include	<CC/jerq.h>
#include	"frame.pri"
#include	"camera.pri"
#include	"clock.pub"

static Rectangle part(Rectangle, int, int);
int sprintf(...);
int latitude(int);
long vscale;
extern long vscale;

Texture32 blup = {
#include	"lup.icon"
}, bldown = {
#include	"ldown.icon"
}, brup = {
#include	"rup.icon"
}, brdown = {
#include	"rdown.icon"
}, a1 = {
#include	"b1.icon"
}, a2 = {
#include	"b2.icon"
}, a3 = {
#include	"b3.icon"
}, a4 = {
#include	"b4.icon"
};

Camera.Camera(Rectangle arena, Rectangle *leftover)
{
	Rectangle r;
	Point o;

	o = arena.o;
	obs = new Globepot(raddp(Rect(10, 10, 130, 130), o), 0,
		45, 45, Pt(160, 0));
	obs->help = "angle of camera from center of data";
	dist = new Rulepot(raddp(Rect(200, 70, 400, 90), o), P_HORIZ|P_TICKS, 300,
		Point(130, -25), Point(50, -25), "distance", 250);
	dist->help = "distance of camera from center of data";
	ospin = new Bmappot(raddp(Rect(530, 54, 562, 86), o), P_CIRC,
		Point(-25, -35), "SPIN TYPE");
	dist->help = "how the camera position changes over time";
	ospin->item(Point(21, -25), "none", &a1);
	ospin->item(Point(21, 11), "lat=c", &a2);
	ospin->item(Point(-75, 11), "long=c", &a3);
	ospin->item(Point(-75, -25), "circle", &a4);
	ospinr = new Rulepot(raddp(Rect(650, 10, 670, 130), o), P_VERT|P_TICKS, 60,
		Point(72, 60), Point(35, 40), "Spin Rate");
	ospinr->help = "degrees of spin per time interval";
	r = arena;
	r.o.y = o.y+142; r.c.y = o.y+150;
	rectf(&display, r, F_OR);
	r.o = add(o, Pt(324, 104));
	r.c = add(o, Pt(491, 150));
	string(&defont, "CAMERA POSITION", &display, add(o, Pt(340, 120)), F_OR);
	bord(&display, r, 8, F_OR);

	o.y += 150;
	cam = new Globepot(raddp(Rect(10, 10, 130, 130), o), 0,
		-45, 225, Pt(160, 0));
	cam->help = "direction of camera at center to the spot";
	aim = new Bmappot(raddp(Rect(190, 65, 220, 95), o), P_VERT,
		Point(-34, 40), "AUTO-CENTER", 1);
	aim->help = "keep camera aimed at the origin";
	aim->item(Point(2, -20), "OFF", &blup);
	aim->item(Point(0, 0), "", &bldown);
	perspective = new Bmappot(raddp(Rect(320, 65, 350, 95), o), P_VERT,
		Point(-35, 40), "PERSPECTIVE", 0);
	perspective->help = "render scaled or perspective";
	perspective->item(Point(2, -20), "OFF", &brup);
	perspective->item(Point(0, 0), "", &brdown);
	cspin = new Bmappot(raddp(Rect(530, 54, 562, 86), o), P_CIRC,
		Point(-25, -35), "SPIN TYPE");
	cspin->help = "motion of the camera aim";
	cspin->item(Point(21, -25), "none", &a1);
	cspin->item(Point(21, 11), "lat=c", &a2);
	cspin->item(Point(-75, 11), "long=c", &a3);
	cspin->item(Point(-75, -25), "circle", &a4);
	cspinr = new Rulepot(raddp(Rect(650, 10, 670, 130), o), P_VERT|P_TICKS, 60,
		Point(72, 60), Point(35, 40), "Spin Rate");
	cspinr->help = "degrees of spin per time interval";
	r = arena;
	r.o.y = o.y+142; r.c.y = o.y+150;
	rectf(&display, r, F_OR);
	r.o = add(o, Pt(404, 104));
	r.c = add(o, Pt(526, 150));
	string(&defont, "CAMERA AIM", &display, add(o, Pt(420, 120)), F_OR);
	bord(&display, r, 8, F_OR);

	o.y += 150;
	m->adv = new Bmappot(raddp(Rect(60, 40, 90, 70), o), P_VERT,
		Point(-43, 40), "FRAME ADVANCE", 1);
	m->adv->help = "advance frames while spinning";
	m->adv->item(Point(2, -20), "OFF", &brup);
	m->adv->item(Point(0, 0), "", &brdown);
	m->run = new Bmappot(raddp(Rect(160, 40, 190, 70), o), P_VERT,
		Point(2, 40), "RUN", 1);
	m->run->help = "run movie";
	m->run->item(Point(2, -20), "OFF", &blup);
	m->run->item(Point(0, 0), "", &bldown);
	clock->speed = new Rulepot(raddp(Rect(250, 50, 450, 70), o),
		P_HORIZ|P_TICKS, SPEED, Point(86, 35), Point(1, -25),
		"delay between frames", 5);
	clock->speed->help = "delay (in ticks) between frames";
	m->hide = new Bmappot(raddp(Rect(520, 40, 550, 70), o), P_VERT,
		Point(-52, 40), "HIDDEN SURFACES", 0);
	m->hide->help = "eliminate hidden lines";
	m->hide->item(Point(2, -20), "OFF", &blup);
	m->hide->item(Point(0, 0), "", &bldown);
	vertscale = new Rulepot(raddp(Rect(650, 10, 670, 90), o),
		P_VERT|P_TICKS, 100, Point(56, 60), Point(35, 40), "V Scale", 100);
	vertscale->help = "vertical scale";
	r = arena;
	r.o.y = o.y+112; r.c.y = o.y+120;
	rectf(&display, r, F_OR);
	drawpot();

	*leftover = arena;
	leftover->o.y = o.y+150;
}

void
Camera.adjust()
{
	if(aim->val)
		cam->setval(-obs->val, (180+obs->w)%360);
	clat = obs->val;
	clon = obs->w;
	cdist = dist->val;
	alat = cam->val;
	alon = cam->w;
	persp = perspective->val;
	vscale = muldiv(32768, vertscale->val, 100);
}

void
Camera.advance()
{
	int a, b;

	switch(ospin->val)
	{
	case 0:
	case 3:
		break;
	case 1:
		obs->setval(obs->val, (obs->w+ospinr->val)%360);
		break;
	case 2:
		b = obs->w;
		if(obs->front)
		{
			if((a = obs->val+ospinr->val) > 90)
			{
				a = 180-a;
				b = (b+180)%360;
			}
		}
		else
		{
			if((a = obs->val-ospinr->val) < -90)
			{
				a = -180-a;
				b = (b+180)%360;
			}
		}
		obs->setval(a, b);
		break;
	}
	switch(cspin->val)
	{
	case 0:
	case 3:
		break;
	case 1:
		cam->setval(cam->val, (cam->w+cspinr->val)%360);
		break;
	case 2:
		b = cam->w;
		if(cam->front)
		{
			if((a = cam->val+cspinr->val) > 90)
			{
				a = 180-a;
				b = (b+180)%360;
			}
		}
		else
		{
			if((a = cam->val-cspinr->val) < -90)
			{
				a = -180-a;
				b = (b+180)%360;
			}
		}
		cam->setval(a, b);
		break;
	}
	adjust();
}

static Rectangle
part(Rectangle r, int n, int i)
{
	int ox;

	ox = ((n-i)*r.o.x + i*r.c.x)/n;
	r.c.x = ((n-i-1)*r.o.x + (i+1)*r.c.x)/n;
	r.o.x = ox;
	return(r);
}

Hcoord
direct(short lat, short lon, short dist)
{
	Hcoord e(0, 0, muldiv(dist, isin(mkangle((lat+360)%360)), ONE), 1);
	register xx;
	
	xx = muldiv(dist, icos(mkangle((lat+360)%360)), ONE);
	e.x = muldiv(xx, icos(mkangle(lon)), ONE);
	e.y = muldiv(xx, isin(mkangle(lon)), ONE);
	return(e);
}

latitude(int lat)
{
	lat = (lat%360 + 360)%360;
	if(lat > 180)
		lat -= 360;
	return(lat);
}
