#include <suntool/sunview.h>
#include <suntool/canvas.h>
#include <fcntl.h>


/* from jerq.h... */

typedef struct Point {
	short	x;
	short	y;
} Point;

int muldiv(x,y,d) { return (x * y) / d; }

Point pt(x, y) { Point p; p.x = x; p.y = y; return p; }
Point Pt(x, y) { Point p; p.x = x; p.y = y; return p; }
Point add(p1, p2) Point p1, p2; { p1.x += p2.x; p1.y += p2.y; return p1; }
Point sub(p1, p2) Point p1, p2; { p1.x -= p2.x; p1.y -= p2.y; return p1; }

typedef struct Rectangle {
	Point origin;
	Point corner;
} Rectangle;

Rectangle raddp(r, p) Rectangle r; Point p;
{
	r.origin = add(r.origin, p);
	r.corner = add(r.corner, p);
	return r;
}

Rectangle Rpt(p1,p2) Point p1, p2;
{
	Rectangle r;
	r.origin.x = p1.x; r.origin.y = p1.y;
	r.corner.x = p2.x; r.corner.y = p2.y;
	return r;
}

Rectangle Rect4(x1,y1,x2,y2)
{
	Rectangle r;
	r.origin.x = x1; r.origin.y = y1;
	r.corner.x = x2; r.corner.y = y2;
	return r;
}

Rectangle inset(r, n) Rectangle r;
{
	r.origin.x += n; r.origin.y += n;
	r.corner.x -= n; r.corner.y -= n;
	return r;
}

int eqpt(p1, p2) Point p1, p2; { return p1.x == p2.x && p1.y == p2.y; }

Rectangle	Drect;
#define	Do	Drect.origin
#define	Dc	Drect.corner

Rectangle	getrect23();
Rectangle	display;	/* not used */
int	defont;

/* bitblt function codes */
#define	F_STORE	(PIX_SRC)	/* target = source */
#define	F_OR	(PIX_SRC|PIX_DST)	/* target |= source */
#define	F_CLR	(PIX_NOT(PIX_SRC)&(PIX_DST))	/* target &= ~source */
#define	F_XOR	(PIX_SRC^PIX_DST)	/* target ^= source */

Cursor *cursor,normalcursor,bullseye,coffeecup,sweep,deadmouse,lockarrow;

#include "anim.h"

typedef unsigned char uchar;

#define	PUT	{ char buf[100]; sprintf(buf,
#define	END	); putstring(buf); }
#define	readpoint(p)	{ p.x = readint(); p.y = readint(); }
#define	readpair(p1,p2)	{ readpoint(p1); readpoint(p2); }

/* holds data for all input objects */

unsigned memsize;		/* bytes */
uchar	*inbuf;			/* input collected here */
uchar	*input;			/* leave a null at front */
uchar	*inp;			/* next free slot in input */
int	nobj	= 0;		/* number of objects in input */
int	overflow = 0;		/* 1 => too much input */

long	slot[2000];		/* slots */
int	slotnum;

extern	uchar	*savechar(), *draw_obj(), *click_obj(), *step_obj();
extern	uchar	*prev_obj(), *next_obj();

extern	Point readpt(), fetchpt(), scalept();
int	xmax, ymax;

#define	MAXVIEW	10
char	*viewname[MAXVIEW];
typedef struct {
	Rectangle vr;
	int	xmax;
	int	ymax;
} View;
View	viewpt[MAXVIEW];
int	curview	= 0;
int	nview	= 0;
#define	INSET	4	/* picture inset from frame */

#define	AW	8	/* arrowhead width and height */
#define	AH	10

#define	MARGINPCT	6
int	margin	= MARGINPCT;	/* percent margin around edges */

#define	MAXCLICK	20
char	*clickname[MAXCLICK];		/* click names */
int	clickval[MAXCLICK];		/* 1 => click on this */
int	clicking = 0;			/* number of active clicks */
int	nclick	= 0;

char	buf[200];
char	*pbuf;

#define	Again	0		/* Menu items -- must be 0.. */
#define	Faster	(Again+1)
#define	Slower	(Faster+1)
#define	Step	(Slower+1)
#define	Forward	(Step+1)
#define	Fatter	(Forward+1)
#define	Thinner	(Fatter+1)
#define	Xor	(Thinner+1)
#define	File	(Xor+1)
#define	Quit	(File+1)		/* ... down this far */
#define	Backward (Quit+1)	/* subsequent names are arbitrary */
#define	Proceed	(Backward+1)
#define	Hit	(Proceed+1)
#define FreeRun	(Hit+1)

int	delay		= 1;	/* how long to delay between things */
int	singstep	= 0;	/* single step if 1 */
#define	Fwd	1
#define	Back	0
int	dir		= Fwd;	/* 1 = fwd, 0 = backward */
int	fatness		= 0;	/* n => draw with 2n+1 lines */
int	xormode		= F_XOR;	/* otherwise OR/CLR */

char	*m3[]		= { "again", "faster", "slower", "1 step", "backward",
			    "fatter", "thinner", "or mode", "new file", "Quit?", 0 };
char	*stepmenu[]	= { "1 step", "run" };
char	*dirmenu[]	= { "forward", "backward" };
char	*modemenu[]	= { "or mode", "xor mode" };

char *m3gen(n)
{
	static char buf[20][20];
	extern int delay;

	if (n < 0 || n > Quit)
		return 0;
	else if (n == Faster) {
		sprintf(buf[n], "faster %d", delay);
		return buf[n];
	} else if (n == Slower) {
		sprintf(buf[n], "slower %d", delay);
		return buf[n];
	} else if (n == Step) {
		return stepmenu[singstep];
	} else if (n == Forward) {
		return dirmenu[dir];
	} else if (n == Fatter) {
		sprintf(buf[n], "fatter %d", fatness+1);
		return buf[n];
	} else if (n == Thinner) {
		sprintf(buf[n], "thinner %d", fatness+1);
		return buf[n];
	} else if (n == Xor) {
		return xormode == F_XOR ? modemenu[0] : modemenu[1];
	} else
		return m3[n];
}

char *m2gen(n)
{
	static char buf[20][50];

	if (n < 0 || n >= nview+nclick)
		return 0;
	else if (n < nview) {
		sprintf(buf[n], "view %s", viewname[n]);
		return buf[n];
	} else {
		sprintf(buf[n], "click %s%s",
			clickname[n-nview], clickval[n-nview] ? "*" : "");
		return buf[n];
	}
}

Menu	mbut3;
Menu	mbut2;
int	last_hit;
int	last_but;

void	inevent();

Canvas	canvas;
Pixwin	*pw;
int	canvasfd;
int	height, width;	/* screen window */

init_params()
{
	int i;

	for (i = 0; i < MAXCLICK; i++)
		if (clickname[i]) {
			free(clickname[i]);
			clickname[i] = 0;
			clickval[i] = 0;
		}
	for (i = 0; i < MAXVIEW; i++)
		if (viewname[i]) {
			free(viewname[i]);
			viewname[i] = 0;
		}
	nclick = nview = curview = nobj = clicking = overflow = slotnum = 0;
}

main()
{
	int i, n;
	Frame baseframe;

	baseframe = window_create(NULL, FRAME, FRAME_LABEL, "Anim",
		WIN_ERROR_MSG, "Can't create Anim base window", 0);
	canvas = window_create(baseframe, CANVAS, 0);
	pw = canvas_pixwin(canvas);
	canvasfd = (int)window_get(canvas, WIN_FD);
	fcntl(canvasfd, F_SETFL, FNDELAY); /* allow nonblocking reads */
	width = (int)window_get(canvas, CANVAS_WIDTH);
	height = (int)window_get(canvas, CANVAS_HEIGHT);
	Drect.origin = pt(0,0);
	Drect.corner = add(Drect.origin, pt(width,height));

	do_rcv();

	initmenus();
	initcursors();
	/* get these events by default: LOC_WINENTER, LOC_WINEXIT, LOC_MOVE,
		 MS_LEFT, MS_MIDDLE, MS_RIGHT		*/
	window_set(canvas,
		WIN_CONSUME_KBD_EVENT, WIN_ASCII_EVENTS,
		WIN_EVENT_PROC, inevent, 0);
	window_main_loop(baseframe);
	send1char(P_QUIT); /* tell host part we're done */
	flushproto();
}

initmenus()
{
	int i;
	char *s;

	mbut2 = menu_create(MENU_DEFAULT, 1, 0);
	for (i = 0; s = m2gen(i); i++)
		menu_set(mbut2, MENU_STRING_ITEM, s, i+1, 0);
	mbut3 = menu_create(MENU_DEFAULT, 1, 0);
	for (i = 0; m3[i]; i++)
		menu_set(mbut3, MENU_STRING_ITEM, m3[i], i+1, 0);
}

char kbdline[100];	/* collect keyboard input here */
int kbdi = 0;		/* kbdline index */

void inevent(c, e)
	Canvas c;
	Event *e;
{
	int n, i, eid, ch;
	char *s;

	eid = event_id(e);
	switch (eid) {
	default:
		if (!event_is_ascii(e))
			return;
		if (eid != '\n' && eid != '\r') {	/* which do we get? */
			if (eid == '\b' && kbdi > 0)
				kbdi--;
			else
				kbdline[kbdi++] = eid;
			kbdline[kbdi] = '\0';
			putstring(kbdline);
			return;
		}
		kbdline[kbdi] = '\0';
		kbdi = 0;
		/* now we have a filename */
		send1char(P_FILE);
		sendstring(kbdline);
		if ((ch = readchar()) == P_FILE) {
			do_rcv();
		} else {
			PUT "can't open file %s", kbdline END;
		}
		return;
	case WIN_RESIZE:
		/* doesn't do much right now */
		width=(int)window_get(canvas, CANVAS_WIDTH);
		height=(int)window_get(canvas, CANVAS_HEIGHT);
		Drect.origin = pt(0,0);
		Drect.corner = add(Drect.origin, pt(width,height));
		view_setup(nview);
		return;
	case MS_LEFT:	/* button 1 */
		if (event_is_up(e))
			return;
		last_but = 1;
		last_hit = 0;
		n = domouse();
		break;
	case MS_MIDDLE:
		if (event_is_up(e))
			return;
		last_but = 2;
		last_hit = (int) menu_show(mbut2, canvas, e, 0) - 1;
		n = domouse();
		for (i = 0; s = m2gen(i); i++)
			menu_set(menu_get(mbut2, MENU_NTH_ITEM, i+1),
				MENU_STRING, s, 0);
		if (last_hit >= 0) /* start with this selected next time */
			menu_set(mbut2, MENU_DEFAULT, last_hit+1, 0);
		break;
	case MS_RIGHT:
		if (event_is_up(e))
			return;
		last_but = 3;
		last_hit = (int) menu_show(mbut3, canvas, e, 0) - 1;
		n = domouse();
		for (i = 0; s = m3gen(i); i++)
			menu_set(menu_get(mbut3, MENU_NTH_ITEM, i+1),
				MENU_STRING, s, 0);
		if (last_hit >= 0) /* start with this selected next time */
			menu_set(mbut3, MENU_DEFAULT, last_hit+1, 0);
		break;
	}
	run_one(n);
}

run_one(n)
{
	static uchar *ip;
	int i;
	char *s;
	Event ev;
	Event *e= &ev;

again:
	if (ip == 0) {
		ip = dir == Fwd ? input : inp;
		clear();
	}
	if (n == Quit) {
		window_done(canvas);
		return;
	}
	if (n == Hit)		/* wait for another mouse hit */
		return;
	if (n == Forward) {	/* change from fwd to back -- fiddle ip */
		ip = next_obj(ip);
		return;
	}
	if (n == Backward) {
		ip = prev_obj(ip);
		return;
	}
	if (n == File) {
		putstring("filename? ");
		return;
	}
	if (n == Again) {
		clear();
		dir = Fwd;
		ip = input;
	}
	if (singstep) {
		if (clicking > 0)
			ip = click_obj(ip, xormode, dir);
		else
			ip = step_obj(ip, xormode, dir);
	} else {		/* free running */
		while (ip) {
			if (clicking > 0)
				ip = click_obj(ip, xormode, dir);
			else
				ip = step_obj(ip, xormode, dir);
			if (ip && window_read_event(canvas,e)==0) {
				/* this just about duplicates inevent */
				switch (event_id(e)) {
				case WIN_RESIZE:
					width=(int)window_get(canvas,
						CANVAS_WIDTH);
					height=(int)window_get(canvas,
						CANVAS_HEIGHT);
					Drect.origin = pt(0,0);
					Drect.corner = add(Drect.origin,
						pt(width,height));
					view_setup(nview);
					break;
				case WIN_REPAINT:
					pw_repairretained(pw);
					break;
				case MS_LEFT:	/* button 1 */
					if (event_is_up(e))
						break;
					last_but = 1;
					last_hit = 0;
					return; /* interrupt run */
				case MS_MIDDLE:
					if (event_is_up(e))
						break;
					last_but = 2;
					last_hit = (int) menu_show(mbut2,
						canvas, e, 0) - 1;
					n = domouse();
					for (i = 0; s = m2gen(i); i++)
						menu_set(menu_get(mbut2,
						  MENU_NTH_ITEM, i+1),
							MENU_STRING, s, 0);
					if (last_hit >= 0)
						menu_set(mbut2, MENU_DEFAULT
							, last_hit+1, 0);
					goto again;
				case MS_RIGHT:
					if (event_is_up(e))
						break;
					last_but = 3;
					last_hit = (int) menu_show(mbut3,
						canvas, e, 0) - 1;
					n = domouse();
					for (i = 0; s = m3gen(i); i++)
						menu_set(menu_get(mbut3,
							MENU_NTH_ITEM, i+1),
							MENU_STRING, s, 0);
					if (last_hit >= 0)
						menu_set(mbut3, MENU_DEFAULT
							, last_hit+1, 0);
					goto again;
				}
			}
			sleephz(delay-1);
		}
	}
}

domouse()
{
	int n;

	if (last_but == 1)
		return Proceed;
	if (last_but == 3) {
		switch (last_hit) {
		case Again:
			return Again;
		case Faster:
			if (delay > 1)
				delay /= 2;
			return Hit;
		case Slower:
			delay *= 2;
			return Hit;
		case Step:
			singstep = 1 - singstep;
			n = Step;
			return Hit;
		case Forward:
			dir = 1 - dir;
			if (xormode == F_OR)
				xormode = F_CLR;
			else if (xormode == F_CLR)
				xormode = F_OR;
			return dir == Fwd ? Forward : Backward;
		case Fatter:
			fatness++;
			return Hit;
		case Thinner:
			if (fatness > 0)
				fatness--;
			return Hit;
		case Xor:
			if (xormode == F_OR || xormode == F_CLR)
				xormode = F_XOR;
			else if (dir == Fwd)
				xormode = F_OR;
			else
				xormode = F_CLR;
			return Hit;
		case File:
			return File;
		case Quit:
			return Quit;
		default:
			return Hit;
		}
	} else if (last_but == 2) {
		Rectangle r, shrink();
		if (last_hit == -1)
			return Hit;
		else if (last_hit < nview) {
			r = getrect23();
			if (r.origin.x == 0 && r.corner.x == 0)	/* bailed out */
				return Hit;
			if (eqpt(r.origin, r.corner))
				r = Drect;
			drawrect(inset(viewpt[last_hit].vr, -(INSET+fatness)), F_CLR);
			drawrect(r, F_OR);
			viewpt[last_hit].vr = r = inset(r, INSET+fatness);
			viewpt[last_hit].xmax = r.corner.x - r.origin.x;
			viewpt[last_hit].ymax = r.corner.y - r.origin.y;
			return Hit;

		} else {	/* a click */
			if (clickval[last_hit-nview]) {	/* was on, so turn off */
				clickval[last_hit-nview] = 0;
				clicking--;
			} else {
				clickval[last_hit-nview] = 1;
				clicking++;
			}
			return Hit;
		}
	}
}

Rectangle shrink(r, pct)	/* shrink rectangle by 2*pct */
	Rectangle r;
{
	int dx = muldiv(r.corner.x-r.origin.x, pct, 100);
	int dy = muldiv(r.corner.y-r.origin.y, pct, 100);
	r.origin = add(r.origin, Pt(dx,dy));
	r.corner = sub(r.corner, Pt(dx,dy));
	return r;
}

view_setup(n)
	int n;
{
	int i, dx, dy;

	dx = (Dc.x - Do.x) / 2;
	dy = (Dc.y - Do.y) / 2;
	viewpt[0].vr = Drect;
	switch (n) {
	case 2:
		viewpt[1].vr = Drect;
		viewpt[0].vr.corner.y -= dy;
		viewpt[1].vr.origin.y += dy;
		break;
	case 3: case 4:
		viewpt[0].vr.corner = add(Drect.origin, Pt(dx,dy));
		viewpt[1].vr = raddp(viewpt[0].vr, Pt(0,dy));
		viewpt[2].vr = raddp(viewpt[0].vr, Pt(dx,0));
		viewpt[3].vr = raddp(viewpt[0].vr, Pt(dx,dy));
		break;
	}
	for (i = 0; i < n; i++) {
		viewpt[i].vr = shrink(viewpt[i].vr, MARGINPCT);
		viewpt[i].xmax = viewpt[i].vr.corner.x - viewpt[i].vr.origin.x;
		viewpt[i].ymax = viewpt[i].vr.corner.y - viewpt[i].vr.origin.y;
	}
}

drawrect(r, mode)
	Rectangle r;
{
	segment(&display, r.origin, Pt(r.origin.x,r.corner.y), mode);
	segment(&display, r.origin, Pt(r.corner.x,r.origin.y), mode);
	segment(&display, r.corner, Pt(r.origin.x,r.corner.y), mode);
	segment(&display, r.corner, Pt(r.corner.x,r.origin.y), mode);
}

segment(dp, p1, p2, mode)
	Rectangle *dp;	/* fake */
	Point p1, p2;
	int mode;
{
	pw_vector(pw, p1.x, p1.y, p2.x, p2.y, mode, 1);
}

point(dp, p, mode)
	Rectangle *dp;
	Point p;
	int mode;
{
	register int val;
	val = (mode==F_XOR)? pw_get(pw, p.x, p.y)^1 :
			((mode==F_OR)? 1 : 0);
	pw_put(pw, p.x, p.y, val);
}

/* return the raster op that f needs to be when SRC is inverted */
invsrc(f)
	register int f;
{
	if(f==F_XOR) f=PIX_NOT(PIX_SRC)^PIX_DST;
	else if(f==F_OR) f=PIX_NOT(PIX_SRC)|PIX_DST;
	else if(f==F_STORE) f=PIX_NOT(PIX_SRC);
	else if(f==F_CLR) f=PIX_SRC&PIX_DST;
	return f;
}

rectf(dp, r, mode)
	Rectangle *dp;
	Rectangle r;
	int mode;
{
	pw_writebackground(pw,r.origin.x, r.origin.y,
		r.corner.x-r.origin.x, r.corner.y-r.origin.y, invsrc(mode));
}
circle(dp, p, r, mode)
	Rectangle *dp;
	Point p;
	int r, mode;
{
	int x1=p.x;
	register y1=p.y;
	register eps = 0;	/* x^2 + y^2 - r^2 */
	register dxsq = 1;		/* (x+dx)^2-x^2*/
	register dysq = 1 - 2*r;
	register exy;
	int x0 = x1;
	register y0 = y1 - r;
	y1 += r;
	pw_batch_on(pw);
	if (mode == F_XOR) {	/* endpoints coincide */
		point(dp,Pt(x0,y0),mode);
		point(dp,Pt(x0,y1),mode);
	}
	while(y1 > y0) {
		point(dp,Pt(x0,y0),mode);
		point(dp,Pt(x0,y1),mode);
		point(dp,Pt(x1,y0),mode);
		point(dp,Pt(x1,y1),mode);
		exy = eps + dxsq + dysq;
		if(-exy <= eps+dxsq) {
			y1--;
			y0++;
			eps += dysq;
			dysq += 2;
		}
		if(exy <= -eps) {
			x1++;
			x0--;
			eps += dxsq;
			dxsq += 2;
		}
	}
	point(dp,Pt(x0,y0),mode);
	point(dp,Pt(x1,y0),mode);
	pw_batch_off(pw);
}

disc(dp, p, r, mode)
	Rectangle *dp;
	Point p;
	int r, mode;
{
	register x1=p.x, y1=p.y;
	register eps = 0;	/* x^2 + y^2 - r^2 */
	int dxsq = 1;		/* (x+dx)^2-x^2*/
	int dysq = 1 - 2*r;
	int exy;
	register x0 = x1;
	register y0 = y1 - r;
	x1++;		/* to offset jerq's half-open lines*/
	y1 += r;
	pw_batch_on(pw);
	while(y1 > y0) {
		exy = eps + dxsq + dysq;
		if(-exy <= eps+dxsq) {
			rectf(dp, Rect4(x0, y0, x1, y0+1), mode);
			rectf(dp, Rect4(x0, y1, x1, y1+1), mode);
			y1--;
			y0++;
			eps += dysq;
			dysq += 2;
		}
		if(exy <= -eps) {
			x1++;
			x0--;
			eps += dxsq;
			dxsq += 2;
		}
	}
	rectf(dp, Rect4(x0, y0, x1, y0+1), mode);
	pw_batch_off(pw);
}

string(fp, s, dp, p, mode)
	int *fp;	/* actually a font pointer */
	uchar *s;
	Rectangle *dp;
	Point p;
	int mode;
{
	pw_text(pw, p.x, p.y+16, mode, 0, s);
}

do_rcv()
{
	int c, n, b, m, i;
	uchar *ip;

	init_params();
	clear();
	memsize = readint();
	if ((inbuf = (uchar *) malloc(memsize)) == (uchar *) NULL) {
		PUT "can't allocate %d bytes", memsize END
		sleephz(60);
		termabort();
	}
	input = inbuf+1;	/* leave a null at front */
	inp = inbuf+1;		/* next free slot in input */
	dir = Fwd;
	PUT "%d bytes, limit %d", inp - input, memsize END

  top:
	switch (c = readchar()) {
	case P_INIT:	/* initialize */
		break;
	case P_ENDFILE:
		*inp = 0;
		return;
	case P_CLEAR:
		clear();
		break;
	case P_DEFINE:
		/* view, click, ... */
		c = readchar();
		assert(c == 'c' || c == 'v', "illegal define");
		i = readint();
		readstring(buf);
		if (c == 'c') {
			clickname[i] = (char *) malloc(strlen(buf)+1);
			strcpy(clickname[i], buf);
			nclick++;
		} else {	/* c == 'v' */
			viewname[i] = (char *) malloc(strlen(buf)+1);
			strcpy(viewname[i], buf);
			nview++;
		}
		break;
	case P_OBJECT:	/* read an object */
		ip = inp;
		if (nobj++ == 0)
			view_setup(nview);
		if (nobj % 100 == 0)
			PUT "%d", nobj END
		read_obj();
		draw_obj(ip, F_XOR, Fwd);
		break;
	case P_PRINT:	/* print a string */
		readstring(buf);
		putstring(buf);
		break;
	case P_ERROR:	/* host is going to exit */
		termabort();
	default:
		if (overflow)
			break;
		putstring("do_rcv error; unrecognized command: ");
		for (pbuf = buf; c != '\n'; c = readchar())
			*pbuf++ = c;
		*pbuf = 0;
		putstring(buf);
		break;
	}
	goto top;
}

clear()
{
	rectf(&display, Drect, F_CLR);	/* clear screen */
}

read_obj()	/* read an object, stick it in input[] */
{
	int c, n, m;
	uchar *p;

	if (inp > input+memsize-50) {	/* eat up at least one */
		PUT "overflow at %d bytes, limit %d", inp - input, memsize END
		overflow++;
		return;
	}
	c = readchar();
	switch (c) {
	case ' ':
	case '\n':
		break;
	case 'b':
	case 'l':
		slotnum = readint();
		slot[slotnum] = inp-input;
		p = savechar(c);
		savechar(curview = readint());
		savechar(readint());	/* options */
		savept(readpt());
		savept(readpt());
		savect(inp-p);
		break;
	case 'o':
		slotnum = readint();
		slot[slotnum] = inp-input;
		p = savechar(c);
		savechar(curview = readint());
		savechar(readint());	/* options */
		savept(readpt());
		saveint(readint());	/* radius */
		savect(inp-p);
		break;
	case 't':
		slotnum = readint();
		slot[slotnum] = inp-input;
		p = savechar(c);
		savechar(curview = readint());
		savechar(readint());	/* options */
		savept(readpt());
		n = readstring(inp+1);	/* +1 leaves a hole */
		*inp = n;		/* insert count before string */
		inp += n + 2;		/* +2 = count before and \0 on end */
		savect(inp-p);
		break;
	case 'e':
		p = savechar(c);
		savelong(slot[readint()]);
		savect(inp-p);
		break;
	case 'c':
		p = savechar(c);
		savechar(readint());
		savect(inp-p);
		break;
	}
}

Point readpt()	/* read a Point */
{
	Point p;

	p.x = readint();
	p.y = readint();
	return p;
}

uchar *savechar(c)
{
	*inp++ = c;
	return inp-1;
}

savect(n)
{
	if (n > 255)
		putstring("text string too long");
	*inp++ = n;
}

saveint(n)
{
	*inp++ = n >> 8;
	*inp++ = n & 0377;
}

savelong(n)
	long n;
{
	*inp++ = n >> 24;
	*inp++ = n >> 16;
	*inp++ = n >> 8;
	*inp++ = n;
}

savept(p)
	Point p;
{
	*inp++ = p.x >> 8;
	*inp++ = p.x & 0377;
	*inp++ = p.y >> 8;
	*inp++ = p.y & 0377;
}

getpoint(ip)
	uchar *ip;
{
	return *ip << 8 | *(ip+1);
}

long getlong(ip)
	uchar *ip;
{
	return *ip << 24 | *(ip+1) << 16 | *(ip+2) << 8 | *(ip+3);
}

Point scalept(v, p)
	Point p;
{
	p.x = p.x * viewpt[v].xmax / 10000;
	p.y = viewpt[v].ymax - p.y * viewpt[v].ymax / 10000;
	return p;
}

scalex(v, n)
{
	return n * viewpt[v].xmax / 10000;
}

Point fetchpt(v, ip)
	int v;
	uchar *ip;
{
	Point pt;

	pt.x = *ip << 8 | *(ip+1);
	pt.y = *(ip+2) << 8 | *(ip+3);
	pt = scalept(v, pt);
	return add(pt, viewpt[v].vr.origin);
}

/*
  Encoding:  type, view#, opts, coords, chars, etc., # = length of group
	bvoxxyyxxyy#
	lvoxxyyxxyy#
	ovoxxyyrr#
	tvoxxyynccc0#
	ennnn#
	cn#

*/

uchar *prev_obj(ip)
	uchar *ip;
{
	if (ip <= input)
		return 0;
	return ip - ip[-1] - 1;
}

uchar *next_obj(ip)
	uchar *ip;
{
	if (ip < input || ip >= inp)
		return 0;
	switch (*ip) {
	case 0:
		return 0;
	case 'b':
	case 'l':
		return ip + 12;
	case 'o':
		return ip + 10;
	case 't':
		return ip + ip[7] + 10;
	case 'c':
		return ip + 3;
	case 'e':
		return ip + 6;
	default:
		return 0;
	}
}

uchar *step_obj(ip, mode, dir)	/* draw objs until one that changes something */
	uchar *ip;
{
	int c;

	while (ip) {
		c = *ip;
		ip = draw_obj(ip, mode, dir);
		if (c == 'b' || c == 'l' || c == 't' || c == 'e' || c == 'o')
			return ip;
	}
	return ip;
}

uchar *click_obj(ip, mode, dir)	/* draw objs until matching click */
	uchar *ip;
{
	int c;
	uchar *oip;

	for (;;) {
		oip = ip;
		ip = draw_obj(ip, mode, dir);
		if (ip == 0 || (oip && *oip == 'c' && clickval[oip[1]]))
			return ip;
	}
}

uchar *draw_obj(ip, mode, dir)	/* draw obj from coords at ip */
	uchar *ip;
	int mode, dir;
{
	int c, r, thick, n, shift, head;
	Point p0, p1, p2;

	if (ip < input || ip >= inp)
		return 0;
	switch (c = *ip++) {
	case 'b':
		p0 = fetchpt(*ip, ip+2);
		p1 = fetchpt(*ip, ip+6);
		if (ip[1] == Bfill) {
			if (p0.y < p1.y)
				rectf(&display, Rpt(p0, p1), mode);
			else
				rectf(&display, Rect4(p0.x,p1.y,p1.x,p0.y), mode);
		} else {
			segment(&display, p0, Pt(p0.x,p1.y), mode);
			segment(&display, Pt(p0.x,p1.y), p1, mode);
			segment(&display, p1, Pt(p1.x,p0.y), mode);
			segment(&display, Pt(p1.x,p0.y), p0, mode);
		}
		if (dir == Fwd)
			ip += 1+9+1;
		else
			ip -= (*(ip-2) + 2);
		break;
	case 'l':
		p0 = fetchpt(*ip, ip+2);
		p1 = fetchpt(*ip, ip+6);
		thick = ip[1]/10;	/* ought to be a macro! */
		if (thick == Ldotted/10 || thick == Ldashed/10)
			thick = 1;
		thick = 2 * thick - 1;	/* 1,3,5 */
		fatline(p0, p1, mode, thick);
		head = ip[1]%10;	/* ditto */
		if (head == Larrow1 || head == Larrow3)
			arrow(p0, p1, AW, AH, mode);
		if (head == Larrow2 || head == Larrow3)
			arrow(p1, p0, AW, AH, mode);
		if (dir == Fwd)
			ip += 1+9+1;
		else
			ip -= (*(ip-2) + 2);
		break;
	case 'o':
		p0 = fetchpt(*ip, ip+2);
		r = scalex(*ip, getpoint(ip+6));
		if (ip[1] == Cnofill)
			circle(&display, p0, r, mode);
		else
			disc(&display, p0, r, mode);
		if (dir == Fwd)
			ip += 1+7+1;
		else
			ip -= (*(ip-2) + 2);
		break;
	case 't':
		p0 = fetchpt(*ip, ip+2);
		n = ip[6];
		shift = (ip[1]/10) * 10;	/* ought to be a macro! */
		if (shift == Tljust)
			shift = 0;
		else if (shift == Tcenter)
			shift = (9 * n) / 2;	/* 9 = char width */
		else
			shift = 9 * n;
		string(&defont, ip+7, &display, sub(p0, Pt(shift,8)), mode);
		if (dir == Fwd)
			ip += 1+5 + *(ip+6)+2 + 1;
		else
			ip -= (*(ip-2) + 2);
		break;
	case 'e':
		erase(ip-1);
		if (dir == Fwd)
			ip += 5;
		else
			ip -= (*(ip-2) + 2);
		break;
	case 'c':
		if (dir == Fwd)
			ip += 2;
		else
			ip -= (*(ip-2) + 2);
		break;
	default:
		ip = 0;
		break;
	}
	return ip;
}

erase(ip)
	uchar *ip;
{
	long target = getlong(ip+1);	/* target label index */
	int mode = F_XOR;

	if (xormode == F_OR || xormode == F_CLR)
		mode = dir == Fwd ? F_CLR : F_OR;
	draw_obj(input+target, mode, Fwd);
}

#define abs(x) ((x) >= 0 ? (x) : -(x))

fatline(p0, p1, mode, thick)
	Point p0, p1;
{
	int i, fat, beg, nl;

	fat = thick * (2 * fatness + 1);
	beg = fat / 2;
	if (abs(p1.x-p0.x) >= abs(p1.y-p0.y)) {	/* horizontal */
		for (nl = 0, i = -beg; nl < fat; nl++, i++)
			segment(&display, add(p0, Pt(0,i)), add(p1, Pt(0,i)), mode);
	} else {
		for (nl = 0, i = -beg; nl < fat; nl++, i++)
			segment(&display, add(p0, Pt(i,0)), add(p1, Pt(i,0)), mode);
	} 
}

arrow(p1, p2, w, h, c)
	Point p1, p2;
	int w, h, c;
/*
** draw arrow of height,width (h,w) at p2 of segment p1,p2.
*/
{
	Point d;
	int norm, qx, qy, lx, ly;

	d = sub(p2, p1);
	/* norm = sqrt((long)d.x*d.x + (long)d.y*d.y); */
	norm = ((long)d.x*d.x + (long)d.y*d.y) / 2;
	if (norm == 0)	/* shouldn't happen, but ... */
		return;
	qx = p2.x - h*d.x/norm;
	qy = p2.y - h*d.y/norm;
	lx = w/2 * -d.y/norm;
	ly = w/2 * d.x/norm;
	/* segment(&display, p1, p2, c); */
	segment(&display, Pt(qx+lx, qy+ly), p2, c);
	segment(&display, Pt(qx-lx, qy-ly), p2, c);
}


putstring(a1, a2, a3, a4, a5)
	char *a1, *a2, *a3, *a4, *a5;
{
	char buf[100];

	sprintf(buf, "%50s", " ");	/* cheap clear */
	pw_text(pw, 20, 20, F_STORE, 0, buf);
	sprintf(buf, a1, a2, a3, a4, a5);
	pw_text(pw, 20, 20, F_STORE, 0, buf);
}

assert(c, s)	/* poor man's assertion */
	char *s;
{
	if (!c) {
		putstring("assertion %s failed\n", s);
		sleephz(60);
		termabort();
	}
}

termabort()
{
	send1char(P_QUIT);
	flushproto();
	window_done(canvas);
	exit(1);
}

readbyte()
{
	char c;

	read(0, &c, 1);
	return c;
}

sendbuf(buf, p)
	char *buf, *p;
{
	write(1, buf, p-buf);
}


short bullseye_bits[]={
	0x07E0, 0x1FF8, 0x399C, 0x63C6, 0x6FF6, 0xCDB3, 0xD99B, 0xFFFF,
	0xFFFF, 0xD99B, 0xCDB3, 0x6FF6, 0x63C6, 0x399C, 0x1FF8, 0x07E0,
};
short coffeecup_bits[]={
	0x0100, 0x00E0, 0x0010, 0x03E0, 0x0400, 0x0FE0, 0x123C, 0x1FE2,
	0x101A, 0x101A, 0x1002, 0x103C, 0x1810, 0x6FEC, 0x4004, 0x3FF8,
};
short sweep_bits[]={
	0x43FF, 0xE001, 0x7001, 0x3801, 0x1D01, 0x0F01, 0x8701, 0x8F01,
	0x8001, 0x8001, 0x8001, 0x8001, 0x8001, 0x8001, 0x8001, 0xFFFF,
};
short deadmouse_bits[]={
	0x0000, 0x0114, 0xA208, 0x4100, 0x0000, 0x0008, 0x0004, 0x0082,
	0x0441, 0xFFE1, 0x5FF1, 0x3FFE, 0x17F0, 0x03E0, 0x0000, 0x0000,
};
short darkgrey_bits[]={
	0xDDDD, 0x7777, 0xDDDD, 0x7777, 0xDDDD, 0x7777, 0xDDDD, 0x7777,
	0xDDDD, 0x7777, 0xDDDD, 0x7777, 0xDDDD, 0x7777, 0xDDDD, 0x7777,
};
short lockarrow_bits[]={
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0FC0, 0x0FC0,
	0x03C0, 0x07C0, 0x0EC0, 0x1CC0, 0x3800, 0x7000, 0xE0DB, 0xC0DB,
};
mpr_static(bullseye_pr, 16, 16, 1, bullseye_bits);
mpr_static(coffeecup_pr, 16, 16, 1, coffeecup_bits);
mpr_static(sweep_pr, 16, 16, 1, sweep_bits);
mpr_static(deadmouse_pr, 16, 16, 1, deadmouse_bits);
mpr_static(lockarrow_pr, 16, 16, 1, lockarrow_bits);
mpr_static(darkgrey, 16, 16, 1, darkgrey_bits);

initcursors()
{
	normalcursor=cursor_copy(window_get(canvas,WIN_CURSOR));
	cursor_set(normalcursor,CURSOR_OP,F_XOR,0);
	bullseye=cursor_create(CURSOR_IMAGE,&bullseye_pr,
		CURSOR_XHOT,7,CURSOR_YHOT,7,CURSOR_OP,F_XOR,0);
	coffeecup=cursor_create(CURSOR_IMAGE,&coffeecup_pr,
		CURSOR_XHOT,7,CURSOR_YHOT,7,CURSOR_OP,F_XOR,0);
	sweep=cursor_create(CURSOR_IMAGE,&sweep_pr,
		CURSOR_XHOT,7,CURSOR_YHOT,7,CURSOR_OP,F_XOR,0);
	deadmouse=cursor_create(CURSOR_IMAGE,&deadmouse_pr,
		CURSOR_XHOT,7,CURSOR_YHOT,7,CURSOR_OP,F_XOR,0);
	lockarrow=cursor_create(CURSOR_IMAGE,&lockarrow_pr,
		CURSOR_XHOT,0,CURSOR_YHOT,0,CURSOR_OP,F_XOR,0);
	cursor= &normalcursor;
}


Rectangle getrect23()
{
	static Rectangle r;
	static Event ev;
	register Event *e= &ev;
	register int sweepstate=0;
	r.origin.x=r.origin.y=r.corner.x=r.corner.y=0;
	cursswitch(&sweep);
	window_set(canvas, WIN_CONSUME_PICK_EVENT, LOC_DRAG, 0, 0);
	window_release_event_lock(canvas);
	while(sweepstate<=1){
		if(window_read_event(canvas,e)!=0)
			; /* non-blocking read */
		switch (event_id(e)) {
		case MS_LEFT:
			if(sweepstate) showr(&r); /*erase box*/
			sweepstate=2;
			break;
		case MS_MIDDLE:
		case MS_RIGHT:
			if(!sweepstate && event_is_down(e)){
				r.origin.x=r.corner.x=event_x(e);
				r.origin.y=r.corner.y=event_y(e);
				sweepstate=1;
				showr(&r);
			}
			else if(sweepstate && event_is_up(e)){
				r.corner.x=event_x(e);
				r.corner.y=event_y(e);
				showr(&r);
				sweepstate=2;
			}
			break;
		case LOC_DRAG:
			showr(&r); /* erase old one */
			r.corner.x=event_x(e);
			r.corner.y=event_y(e);
			showr(&r);
			break;
		case LOC_WINEXIT:
			if(sweepstate){
				showr(&r);
				r.corner.x=event_x(e);
				r.corner.y=event_y(e);
				showr(&r);
				sweepstate=2;
			}
			break;
		}
	}
	window_set(canvas, WIN_IGNORE_PICK_EVENT, LOC_DRAG, 0);
	cursswitch(&normalcursor);
	return r;
}
showr(rp)
	register Rectangle *rp;
{
	register int x0=rp->origin.x;
	register int y0=rp->origin.y;
	register int x1=rp->corner.x;
	register int y1=rp->corner.y;
	pw_batch_on(pw);
	pw_vector(pw, x0, y0, x1, y0, PIX_SRC^PIX_DST, 1);
	if(y1>y0) y0++; /*avoid redoing corner*/
	else y0--;
	pw_vector(pw, x1, y0, x1, y1, PIX_SRC^PIX_DST, 1);
	if(x1>x0) x1--;
	else x1++;
	pw_vector(pw, x1, y1, x0, y1, PIX_SRC^PIX_DST, 1);
	pw_vector(pw, x0, y1, x0, y0, PIX_SRC^PIX_DST, 1);
	pw_batch_off(pw);
}

cursswitch(cp)
	Cursor *cp;
{
	window_set(canvas, WIN_CURSOR, cp? *cp : normalcursor, 0);
}


/* sleep, but in 1/60's of second */
/* #include <sys/time.h> */
#include <signal.h>

#define	mask(s)	(1<<((s)-1))
#define	setvec(vec, a) \
	vec.sv_handler = a; vec.sv_mask = vec.sv_onstack = 0

static int ringring;

sleephz(n)
	unsigned n;
{
	int sleepx(), omask;
	struct itimerval itv, oitv;
	register struct itimerval *itp = &itv;
	struct sigvec vec, ovec;

	if (n == 0)
		return;
	timerclear(&itp->it_interval);
	timerclear(&itp->it_value);
	if (setitimer(ITIMER_REAL, itp, &oitv) < 0)
		return;
	setvec(ovec, SIG_DFL);
	omask = sigblock(0);
	n *= 16666;
	itp->it_value.tv_sec = n / 1000000;
	itp->it_value.tv_usec = n % 1000000;
	if (timerisset(&oitv.it_value)) {
		if (timercmp(&oitv.it_value, &itp->it_value, >))
			oitv.it_value.tv_sec -= itp->it_value.tv_sec;
		else {
			itp->it_value = oitv.it_value;
			/*
			 * This is a hack, but we must have time to
			 * return from the setitimer after the alarm
			 * or else it'll be restarted.  And, anyway,
			 * sleep never did anything more than this before.
			 */
			oitv.it_value.tv_sec = 1;
			oitv.it_value.tv_usec = 0;
		}
	}
	setvec(vec, sleepx);
	(void) sigvec(SIGALRM, &vec, &ovec);
	ringring = 0;
	(void) setitimer(ITIMER_REAL, itp, (struct itimerval *)0);
	while (!ringring)
		sigpause(omask &~ mask(SIGALRM));
	(void) sigvec(SIGALRM, &ovec, (struct sigvec *)0);
	(void) setitimer(ITIMER_REAL, &oitv, (struct itimerval *)0);
}

static
sleepx()
{

	ringring = 1;
}
