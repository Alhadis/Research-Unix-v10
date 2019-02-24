#ifdef X11
#include "xjerq.h"

#else
#ifdef TTY630
#include <dmd.h>
#include <5620.h>
#define Texture Texture16

#else
#include <jerq.h>
#include <font.h>
#endif
#endif

#include "anim.h"

#ifndef X11
Texture deadmouse = {
		0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x000C, 0x0082, 0x0441,
		0xFFE1, 0x5FF1, 0x3FFE, 0x17F0, 0x03E0, 0x0000, 0x0000, 0x0000, };
Texture skull ={
		0x0000, 0x0000, 0x0000, 0xC003, 0xE7E7, 0x3FFC, 0x0FF0, 0x0DB0,
		0x07E0, 0x0660, 0x37EC, 0xE427, 0xC3C3, 0x0000, 0x0000, 0x0000, };
#else /* X11 */
short deadmouse_bits[] = {
		0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x000C, 0x0082, 0x0441,
		0xFFE1, 0x5FF1, 0x3FFE, 0x17F0, 0x03E0, 0x0000, 0x0000, 0x0000, };
short skull_bits[] ={
		0x0000, 0x0000, 0x0000, 0xC003, 0xE7E7, 0x3FFC, 0x0FF0, 0x0DB0,
		0x07E0, 0x0660, 0x37EC, 0xE427, 0xC3C3, 0x0000, 0x0000, 0x0000, };
Cursor deadmouse, skull;
initcursors()
{
	deadmouse = ToCursor(deadmouse_bits, deadmouse_bits, 7, 7);
	skull = ToCursor(skull_bits, deadmouse_bits, 7, 7);
}
#undef Texture
#define Texture Cursor
#define sleep(n) usleep(16666*n)
#define V9 1
	/* X emulator lib uses V9 kind of blit library */
#endif X11

#define	PTR	(Texture *)0

#define	Do	Drect.origin
#define	Dc	Drect.corner
#define	kbdrect	Rect(Do.x+4, Do.y+4, Dc.x+4, Do.y+16+4)

char	kbdline[100];	/* keyboard input collects here */
char	*pkbd = kbdline;

typedef unsigned char uchar;

int	errline;
#define	PUT	{ char __buf[100]; errline = 0; sprintf(__buf,
#define	END	); putstring(__buf); }
#define	ERRPUT	{ char __buf[100]; errline = 1; sprintf(__buf,
#define	NAP	sleep(100)
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

extern	uchar	*savechar(), *read_obj(), *draw_obj(), *click_obj(), *step_obj();
extern	uchar	*prev_obj(), *next_obj();

extern	Point readpt(), fetchpt(), scalept();
int	xmax, ymax;

#define	MAXVIEW	20
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

int	delay		= 1;	/* how long to delay between things */
int	singstep	= 0;	/* single step if 1 */
#define	Fwd	1
#define	Back	0
int	dir		= Fwd;	/* 1 = fwd, 0 = backward */
int	fatness		= 0;	/* n => draw with 2n+1 lines */
Code	xormode		= F_XOR;	/* otherwise OR/CLR */

char	*m3[]		= { "again", "faster", "slower", "1 step", "backward",
			    "fatter", "thinner", "or mode", "new file", "Quit?", 0 };
char	*stepmenu[]	= { "1 step", "run" };
char	*dirmenu[]	= { "forward", "backward" };
char	*modemenu[]	= { "or mode", "xor mode" };

char *m3gen(n)
{
	static char buf[50];
	extern int delay;

	if (n < 0 || n > Quit)
		return 0;
	else if (n == Faster) {
		sprintf(buf, "faster %d", delay);
		return buf;
	} else if (n == Slower) {
		sprintf(buf, "slower %d", delay);
		return buf;
	} else if (n == Step) {
		return stepmenu[singstep];
	} else if (n == Forward) {
		return dirmenu[dir];
	} else if (n == Fatter) {
		sprintf(buf, "fatter %d", fatness+1);
		return buf;
	} else if (n == Thinner) {
		sprintf(buf, "thinner %d", fatness+1);
		return buf;
	} else if (n == Xor) {
		return xormode == F_XOR ? modemenu[0] : modemenu[1];
	} else
		return m3[n];
}

char *m2gen(n)
{
	static char buf[50];

	if (n < 0 || n >= nview+nclick)
		return 0;
	else if (n < nview) {
		sprintf(buf, "view %s", viewname[n]);
		return buf;
	} else {
		sprintf(buf, "click %s%s",
			clickname[n-nview], clickval[n-nview] ? "*" : "");
		return buf;
	}
}

#ifdef V9
Menu	mbut3	= { (char **) 0, m3gen };
Menu	mbut2	= { (char **) 0, m2gen };
#else
Menu	mbut3	= { (char **) 0, 0,0,m3gen };
Menu	mbut2	= { (char **) 0, 0,0,m2gen };
#endif
int	last_hit;
int	last_but;

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

main(argc, argv)
	char *argv[];
{
	uchar *ip;
	int i, n, c;

#ifdef X11
	initdisplay(argc, argv);
	initcursors();
#endif X11

	request(KBD|MOUSE|SEND|RCV);
  top:
	clear();
	init_params();
	putstring("terminal alive");
	memsize = readint();
	if (inbuf == (uchar*)NULL && (inbuf = (uchar *) alloc(memsize)) == (uchar*)NULL) {
		ERRPUT "can't allocate %u bytes", memsize END; NAP;
		exit();
	}
	input = inbuf+1;	/* leave a null at front */
	inp = inbuf+1;		/* next free slot in input */

	cursswitch(&deadmouse);
	do_rcv();
	PUT "%d objects, %u/%u bytes", nobj, inp-input, memsize END
	cursswitch(PTR);

	dir = Fwd;
	ip = inp;	/* pointing at the end */
  again:
	for ( ; ip; wait(CPU)) {
#ifdef X11
		wait(MOUSE);
#endif
		while (checkmouse() == 0)
			wait(CPU);
		n = domouse();
		if (n == Quit) {
			send1char(P_QUIT);
			flushproto();
			exit();
		}
		if (n == Hit)		/* wait for another mouse hit */
			continue;
		if (n == Forward) {	/* change from fwd to back -- fiddle ip */
			ip = next_obj(ip);
			continue;
		}
		if (n == Backward) {
			ip = prev_obj(ip);
			continue;
		}
		if (n == File) {
			static char prevkbd[100];
			putstring("filename? ");
			if (do_kbd() == 0)
				continue;
			send1char(P_FILE);
			if (kbdline[0] == '\0')	/* RETURN only */
				strcpy(kbdline, prevkbd);
			strcpy(prevkbd, kbdline);
			sendstring(kbdline);
			if ((c = readchar()) == P_FILE)
				goto top;
			assert(c == P_ERROR, "char is not P_ERROR");
			ERRPUT "can't open file %s", kbdline END;
			continue;
		}
		if (n == Again) {
			if (P->state & RESHAPED) {
				view_setup(nview);
				P->state &= ~RESHAPED;
			}
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
				if (checkmouse())
					break;		/* back round main loop */
				sleep(delay-1);
				wait(CPU);
			}
		}
	}
	if (ip == 0)
		ip = dir == Fwd ? inp : input;
	goto again;
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
#ifdef V9
			r = getrect23();
#else
			r = getrect();
#endif
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

Point pt(x, y) { Point p; p.x = x; p.y = y; return p; }

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
	int i, j, v, dx, dy, r, c;

	switch (n) {
	case 1: r = 1; c = 1; break;
	case 2: r = 2; c = 1; break;
	case 3: case 4: r = 2; c = 2; break;
	case 5: case 6: r = 3; c = 2; break;
	case 7: case 8: case 9: r = 3; c = 3; break;
	default: r = (n+2)/3; c = 3; break; /* finking out */
	}
	dx = (Dc.x - Do.x) / c;
	dy = (Dc.y - Do.y) / r;
	v = 0;
	for (i = 0; i < r && v < n; i++)
		for (j = 0; j < c && v < n; j++) {
			viewpt[v].vr = Drect;
			viewpt[v].vr.origin.x = Do.x + j * dx;
			viewpt[v].vr.corner.x = Do.x + (j+1) * dx;
			viewpt[v].vr.origin.y = Do.y + i * dy;
			viewpt[v].vr.corner.y = Do.y + (i+1) * dy;
			v++;
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

do_rcv()
{
	int c, n, b, m, i;
	uchar *ip;

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
			clickname[i] = alloc(strlen(buf)+1);
			strcpy(clickname[i], buf);
			nclick++;
		} else {	/* c == 'v' */
			viewname[i] = alloc(strlen(buf)+1);
			strcpy(viewname[i], buf);
			nview++;
		}
		break;
	case P_OBJECT:		/* read an object */
		if (nobj++ == 0)
			view_setup(nview);
		if (nobj % 100 == 0)
			PUT "%d objects, %u/%u bytes", nobj, inp-input, memsize END
		ip = read_obj();
		draw_obj(ip, F_XOR, Fwd);
		break;
	case P_PRINT:		/* print a string */
		readstring(buf);
		putstring(buf);
		break;
	case P_ERRPRINT:	/* print an error string */
		readstring(buf);
		ERRPUT buf END;
		break;
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


uchar *read_obj()	/* read an object, stick it in input[] */
{
	int c, n, m;
	uchar *p, *oinp = inp;

	if (inp > input+memsize-50) {	/* eat up at least one */
		ERRPUT "overflow at %u bytes, limit %u", inp - input, memsize END;
		/* overflow++; */
		/* return; */
		oinp = inp = input;	/* reset to see end of picture */
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
	return oinp;
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
	p.x = muldiv(p.x, viewpt[v].xmax, 10000);
	p.y = viewpt[v].ymax - muldiv(p.y, viewpt[v].ymax, 10000);
	return p;
}

scalex(v, n)
{
	return muldiv(n, viewpt[v].xmax, 10000);
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
				rectf(&display, Rect(p0.x,p1.y,p1.x,p0.y), mode);
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
			fatcircle(p0, r, mode, 1);
		else
			disc(&display, p0, r + fatness, mode);
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

fatcircle(p0, r, mode, thick)
	Point p0;
	int r, mode, thick;
{
	int i, fat, beg, nl;

	fat = thick * (2 * fatness + 1);
	beg = fat / 2;
	for (nl = 0, i = -beg; nl < fat; nl++, i++)
		circle(&display, p0, r+i, mode);
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
	norm = sqrt((long)d.x*d.x + (long)d.y*d.y);
	if (norm == 0)	/* shouldn't happen, but ... */
		return;
	qx = p2.x - muldiv(h, d.x, norm);
	qy = p2.y - muldiv(h, d.y, norm);
	lx = muldiv(w/2, -d.y, norm);
	ly = muldiv(w/2, d.x, norm);
	/* segment(&display, p1, p2, c); */
	segment(&display, Pt(qx+lx, qy+ly), p2, c);
	segment(&display, Pt(qx-lx, qy-ly), p2, c);
}

but(n)	/* parameterize button1(), etc. */
	int n;
{
	switch (n) {
	case 1: return button1();
	case 2:	return button2();
	case 3:	return button3();
	case 12: return button12();
	case 13: return button1()|button3();	/* not built in */
	case 23: return button23();
	case 123: return button123();
	default: return 0;
	}
}

checkmouse()	/* return button touched if any */
{
	int c, b;
	char *p = NULL;
	Point dp;
	Texture *tp;

	if (!(own()&MOUSE) || !button123())
		return 0;	/* no hit at all */
	tp = cursswitch(PTR);
	last_but = 0;
	last_hit = -1;
	c = 0;
	if (button3()) {
		last_hit = menuhit(&mbut3, 3);
		last_but = 3;
	} else if (button2()) {
		last_hit = menuhit(&mbut2, 2);
		last_but = 2;
	} else {		/* button1() */
		last_but = 1;
	}
	while (button123())
		wait(MOUSE);
	if (last_but == 3 && last_hit>=0) {
		p = m3[last_hit];
		c = p[strlen(p) - 1];
	}
	if (c == '?' && !confirm(last_but))
		last_hit = -1;
	cursswitch(tp);
	return last_but;
}

confirm(but)	/* ask for confirmation if menu item ends with '?' */
{
	int c;
	static int but_cvt[8] = { 0, 3, 2, 0, 1, 0, 0, 0 };
	Texture *tp;

	tp = cursswitch(&skull);
	while (!button123())
		wait(MOUSE);
	c = mouse.buttons & 7;
	while (button123())
		wait(MOUSE);
	cursswitch(tp);
	return but == but_cvt[c];
}

putstring(a1, a2, a3, a4, a5)
	char *a1, *a2, *a3, *a4, *a5;
{
	static Point p = {0, 0};
	static int jmax = 0, l;
	char buf[100], *s;

	p = Drect.origin;
	if (errline)
		p.y += 16;
	sprintf(buf, a1, a2, a3, a4, a5);
	rectf(&display, Rect(p.x,p.y,p.x+jmax,p.y+16), F_CLR);
	string(&defont, buf, &display, p, F_OR);
	if ((l = jstrwidth(buf)) > jmax)
		jmax = l;
	errline = 0;
}

assert(c, s)	/* poor man's assertion */
	char *s;
{
	if (!c) {
		putstring("assertion %s failed\n", s);
		sleep(60);
	}
}

readbyte()
{
	int c;

	if ((c = rcvchar()) == -1) {
		wait(RCV);
		c = rcvchar();
	}
	return c;
}

sendbuf(buf, p)
	char *buf, *p;
{
	sendnchars((int) (p-buf), buf);
}

do_kbd()
{
	int c;

	pkbd = kbdline;
	for (wait(KBD); (c = kbdchar()) != -1; wait(KBD)) {
		if (c == '\b' && pkbd > kbdline)
			--pkbd;
		else if (c == '@')
			pkbd = kbdline;
		else
			*pkbd++ = c;
		*pkbd = '\0';
		rectf(&display, Rect(Do.x,Do.y,Dc.x,Do.y+16), F_CLR);
		string(&defont, kbdline, &display, Do, F_STORE);
		if (c == '\n' || c == '\r') {
			pkbd[-1] = '\0';
			return '\n';
		}
	}
}

