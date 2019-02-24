#include "xjerq.h"
#include <errno.h>
#ifdef BSD
#include <sys/ioctl.h>
#else
#include <sys/filio.h>
#endif

Rectangle Drect;
Bitmap display;
struct Mouse mouse;
static struct JProc sP;
struct JProc *P;
GC gcs[4];
Display *dpy;
unsigned long fgpix;
unsigned long bgpix;
Colormap colormap;
XColor fgcolor, bgcolor;
Font defont;

static short arrow_bits[]={
	0x0004,	0x000E,	0x001F,	0x003E,
	0x007C,	0x00F8,	0x01F0,	0x83E0,
	0x87C0,	0xCF80,	0xDF00,	0xFE00,
	0xFC00,	0xF800,	0xFE00,	0xFF80,
};
Cursor normalcursor;

/*
 * Buffer for keyboard input
 */
#define KBDBUFSIZE	128
static unsigned char kbdbuffer[KBDBUFSIZE];

static struct {
	unsigned char *buf;
	unsigned char *in;
	unsigned char *out;
	int cnt;
	int size;
} kbdbuf = {kbdbuffer, kbdbuffer, kbdbuffer, 0, KBDBUFSIZE};

initdisplay(argc, argv)
int argc;
char *argv[];
{
	int i;
	Window win;
	XSetWindowAttributes xswa;
	XSizeHints sizehints;
	Font *df;
	char *font;
	char *geom = 0;
	int flags;
	int width, height, x, y;
	char **ap;

	if (!(dpy= XOpenDisplay(NULL))) {
		perror("Cannot open display\n");
		exit(-1);
	}

	if ((font = XGetDefault(dpy, argv[0], "JerqFont")) == NULL)
		font = "fixed";
	bzero(&sizehints, sizeof(sizehints));
	ap = argv; i = argc;
	while (i-- > 0) {
		if (!strcmp("-fn", ap[0])) {
			font = ap[1];
			i--; ap++;
		}
		else if (ap[0][0] == '=') {
			geom = ap[0];	
			flags = XParseGeometry(ap[0], &x, &y, &width, &height);
			if(WidthValue & flags) {
				sizehints.flags |= USSize;
				sizehints.width = width;
			}
			if(HeightValue & flags) {
	    			sizehints.flags |= USSize;
				sizehints.height = height;
			}
			if(XValue & flags) {
				if(XNegative & flags)
				  x = DisplayWidth(dpy, DefaultScreen(dpy)) + x 
					- sizehints.width;
				sizehints.flags |= USPosition;
				sizehints.x = x;
			}
			if(YValue & flags) {
				if(YNegative & flags)
				  y = DisplayHeight(dpy, DefaultScreen(dpy)) + y
					-sizehints.height;
				sizehints.flags |= USPosition;
				sizehints.y = y;
			}
		}
		ap++;
	}

	df = XLoadQueryFont(dpy, font);
	defont = *df;
	P = &sP;

	sizehints.width_inc = sizehints.height_inc = 1;
	sizehints.min_width = sizehints.min_height = 20;
	sizehints.flags |= PResizeInc|PMinSize;
	if (!geom) {
		sizehints.width = defont.max_bounds.width * 80;
		sizehints.height = (defont.max_bounds.ascent +
				 defont.max_bounds.descent) * 24;
		sizehints.flags |= PSize;
	}

	xswa.event_mask = 0;
	bgpix = xswa.background_pixel = WhitePixel(dpy, 0);
	fgpix = xswa.border_pixel = BlackPixel(dpy, 0);
	win = XCreateWindow(dpy, RootWindow(dpy, DefaultScreen(dpy)),
		sizehints.x, sizehints.y, sizehints.width, sizehints.height,
		2, 0,
		InputOutput, DefaultVisual(dpy, DefaultScreen(dpy)),
		CWEventMask | CWBackPixel | CWBorderPixel, &xswa);

	XSetStandardProperties(dpy, win, argv[0], argv[0],
				None, argv, argc, &sizehints);

	XSelectInput(dpy, win,
		ButtonPressMask|ButtonReleaseMask|ButtonMotionMask|
		StructureNotifyMask|ExposureMask|KeyPressMask);
	XMapWindow(dpy, win);

	colormap = XDefaultColormap(dpy, 0);
	fgcolor.pixel = fgpix;
	bgcolor.pixel = bgpix;
	XQueryColor(dpy, colormap, &fgcolor);
	XQueryColor(dpy, colormap, &bgcolor);

	gcs[F_STORE] = XCreateGC(dpy, win, 0, NULL);
	XSetForeground(dpy, gcs[F_STORE], fgpix);
	XSetBackground(dpy, gcs[F_STORE], bgpix);
	XSetFont(dpy, gcs[F_STORE], defont.fid);
	gcs[F_OR] = XCreateGC(dpy, win, 0, NULL);
	XCopyGC(dpy, gcs[F_STORE], GCForeground|GCBackground|GCFont, gcs[F_OR]);
	gcs[F_CLR] = XCreateGC(dpy, win, 0, NULL);
	XCopyGC(dpy, gcs[F_STORE], GCBackground|GCFont, gcs[F_CLR]);
	XSetForeground(dpy, gcs[F_CLR], bgpix);
	gcs[F_XOR] = XCreateGC(dpy, win, 0, NULL);
	XCopyGC(dpy, gcs[F_STORE], GCBackground|GCFont, gcs[F_XOR]);
	XSetForeground(dpy, gcs[F_XOR], AllPlanes);
	XSetFunction(dpy, gcs[F_XOR], GXxor);

	display.dr = win;
	Drect.origin.x = 0;
	Drect.origin.y = 0;
	Drect.corner.x = sizehints.width;
	Drect.corner.y = sizehints.height;
	display.rect = Drect;
	while (! P->state & RESHAPED)
		handleinput();	/* wait for exposure */
	P->state &= ~RESHAPED;
	normalcursor = ToCursor(arrow_bits, arrow_bits, 0, 15);
	cursswitch(&normalcursor);


}

Bitmap *
balloc(r)
	Rectangle r;
{
	Bitmap *b;
	Pixmap pm;
	b=(Bitmap *)malloc(sizeof (struct Bitmap));
	pm = XCreatePixmap(dpy, display.dr, r.corner.x-r.origin.x,
		r.corner.y-r.origin.y, DefaultDepth(dpy, 0));
	b->dr=pm;
	b->rect=r;
	b->flag = BI_PIXMAP;
	return b;
}

void
bfree(b)
	Bitmap *b;
{
	if(b){
		XFreePixmap(dpy, b->dr);
		free((char *)b);
	}
}

#define brx(b)	(b->rect.origin.x)
#define bry(b)	(b->rect.origin.y)

void
rectf(b,r,f)
	Bitmap *b;
	Rectangle r;
	Code f;
{
	register wd=r.corner.x-r.origin.x;
	register ht=r.corner.y-r.origin.y;

	if (b->flag & BI_PIXMAP)
		r.origin = sub(r.origin, b->rect.origin);
	XFillRectangle(dpy, b->dr, gcs[f], r.origin.x, r.origin.y, wd, ht);
}

void
bitblt(sb,r,db,p,f)
	Bitmap *sb, *db;
	Rectangle r; /* in source bitmap */
	Point p; /* in dest bitmap */
	Code f;
{
	int wd=r.corner.x-r.origin.x;
	int ht=r.corner.y-r.origin.y;

	if (sb->flag & BI_PIXMAP)
		r.origin = sub(r.origin, sb->rect.origin);
	if (db->flag & BI_PIXMAP)
		p = sub(p, db->rect.origin);
	XCopyArea(dpy, sb->dr, db->dr, gcs[f], r.origin.x, r.origin.y,
		wd, ht, p.x, p.y);
}

Point
string(ft, s, b, p, f)
	XFontStruct *ft;
	char *s;
	Bitmap *b;
	Point p;
	Code f;
{
	Point p1;
	int i;

	i = strlen(s);
	p1 = p;
	p.y += ft->max_bounds.ascent;
	if (b->flag & BI_PIXMAP)
		p = sub(p, b->rect.origin);
	XDrawString(dpy, b->dr, gcs[f], p.x, p.y, s, i);
	p1.x += XTextWidth(ft, s, i);
	return p1;
}

strwidth(ft,s)
	XFontStruct *ft;
	char *s;
{
	return XTextWidth(ft, s, strlen(s));
}

#ifdef safe
Point
Pt(x, y)
	short x, y;
{
	Point p;
	p.x = x;
	p.y = y;
	return p;
}
Rectangle
SRect(x1, y1, x2, y2)
	short x1, y1, x2, y2;
{
	Rectangle r;
	r.origin.x = x1;
	r.origin.y = y1;
	r.corner.x = x2;
	r.corner.y = y2;
	return r;
}
Rectangle
Rpt(p1, p2)
	Point p1, p2;
{
	Rectangle r;
	r.origin = p1;
	r.corner = p2;
	return r;
}
#endif safe

Point
add(a, b)
	Point a, b;
{
	register short *ap= &a.x, *bp= &b.x;
	*ap++ += *bp++;
	*ap += *bp;
	return a;
}
Point
sub(a, b)
	Point a, b;
{
	register short *ap= &a.x, *bp= &b.x;
	*ap++ -= *bp++;
	*ap -= *bp;
	return a;
}
Rectangle
inset(r,n)
	Rectangle r;
	register n;
{
	register short *rp= &r.origin.x;
	*rp++ += n;
	*rp++ += n;
	*rp++ -= n;
	*rp   -= n;
	return r;
}
Rectangle
raddp(r, p)
	Rectangle r;
	Point p;
{
	register short *rp= &r.origin.x, *pp= &p.x;
	*rp++ += *pp++;
	*rp++ += *pp--;
	*rp++ += *pp++;
	*rp   += *pp;
	return r;
}
eqpt(p, q)
	Point p, q;
{
	register long *pp=(long *)&p, *qq=(long *)&q;
	return *pp==*qq;
}
ptinrect(p, r)
	Point p;
	Rectangle r;
{
	return(p.x>=r.origin.x && p.x<r.corner.x
	    && p.y>=r.origin.y && p.y<r.corner.y);
}

/*
 * Convert a blit style texture to a pixmap which can be used in tiling
 * or cursor operations.
 */
Pixmap ToPixmap(bits)
short bits[];
{
	static XImage *im;
	Pixmap pm;

	if (!im)
		im = XCreateImage(dpy, XDefaultVisual(dpy, 0), 1,
				  XYBitmap, 0, (char *)bits, 16, 16, 8, 2);
	else
		im->data = (char *)bits;
	pm = XCreatePixmap(dpy, display.dr, 16, 16, 1);
	XPutImage(dpy, pm, gcs[F_STORE], im, 0, 0, 0, 0, 16, 16);
	return pm;
}

Cursor ToCursor(source, mask, hotx, hoty)
short source[], mask[];
{
	Pixmap sp, mp;
	Cursor c;

	sp = ToPixmap(source);
	mp = ToPixmap(mask);
	c = XCreatePixmapCursor(dpy, sp, mp, &fgcolor, &bgcolor, hotx, hoty);
	XFreePixmap(dpy, sp);
	XFreePixmap(dpy, mp);
	return(c);
}

cursset(p)
	Point p;
{
	XWarpPointer(dpy, display.dr, display.dr, mouse.xy.x, mouse.xy.y,
		display.rect.corner.x, display.rect.corner.y, p.x, p.y);
	mouse.xy.x = p.x;
	mouse.xy.y = p.y;
}

jnap(i)
{
	handleinput();
}

kbdchar()
{
	int i;

	if (!kbdbuf.cnt)
		return -1;
	i = *kbdbuf.out++;
	if (kbdbuf.out == &kbdbuf.buf[kbdbuf.size])
		kbdbuf.out = kbdbuf.buf;
	if (--kbdbuf.cnt == 0)
		P->state &= ~KBD;
	return(i);
}

#undef button
handleinput()
{
	XEvent ev;
	unsigned char s[16], *cp;
	int n;

	XNextEvent(dpy, &ev);
	switch (ev.type) {
		case ButtonPress:
			mouse.buttons |= (8 >> ev.xbutton.button);
			mouse.xy.x = ev.xbutton.x;
			mouse.xy.y = ev.xbutton.y;
			mouse.time = ev.xbutton.time;
			break;
		case ButtonRelease:
			mouse.buttons &= ~(8 >> ev.xbutton.button);
			mouse.xy.x = ev.xbutton.x;
			mouse.xy.y = ev.xbutton.y;
			mouse.time = ev.xbutton.time;
			break;
		case MotionNotify:
			mouse.xy.x = ev.xmotion.x;
			mouse.xy.y = ev.xmotion.y;
			break;
		case MapNotify:
		case NoExpose:
			break;
		case ConfigureNotify:
			if (display.rect.corner.x != ev.xconfigure.width ||
			    display.rect.corner.y != ev.xconfigure.height) {
				display.rect.corner.x = ev.xconfigure.width;
				display.rect.corner.y = ev.xconfigure.height;
				Drect = display.rect;
			}
			break;
		case Expose:
			if (ev.xexpose.count == 0) {
				rectf(&display, Drect, F_CLR);
				P->state |= RESHAPED;
			}
			break;
		case KeyPress:
			mouse.xy.x = ev.xkey.x;
			mouse.xy.y = ev.xkey.y;
			mouse.time = ev.xkey.time;
			n = XLookupString(&ev, s, sizeof(s), NULL, NULL);
			if (n > 0) {
				cp = s;
				P->state |= KBD;
				do {
					if (kbdbuf.cnt == kbdbuf.size)
						break;
					*kbdbuf.in++ = *cp++;
					kbdbuf.cnt++;
					if (kbdbuf.in == &kbdbuf.buf[kbdbuf.size])
						kbdbuf.in = kbdbuf.buf;
				} while (--n);
			}
			break;
		default:
			break;
	}
}
#define button(i)		(mouse.buttons&(8>>i))

char *
gcalloc(nbytes, where)
	unsigned long nbytes;
	char **where;
{
	*where=(char *)alloc(nbytes);
	return *where;
}
void
gcfree(s)
	char *s;
{
	free(s);
}

min(a,b)
{
	return (a<b? a: b);
}
max(a,b)
{
	return (a>b? a: b);
}

/*	Form a circle of radius r centered at x1,y1
 */
circle(b,p,r,f)
	Bitmap *b;
	Point p;
{
	unsigned int diam = 2*r;
	if (b->flag & BI_PIXMAP)
		p = sub(p, b->rect.origin);
	p = sub(p, Pt(r,r));
	XDrawArc(dpy, b->dr, gcs[f], p.x, p.y, diam, diam, 0, 23040/* 360 deg */);
}

Cursor *
cursswitch(cp)
Cursor *cp;
{
	static Cursor *prev = &normalcursor;
	Cursor *ret = prev;
	if (!cp)
		cp = &normalcursor;
	XDefineCursor(dpy, display.dr, *cp);
	prev = cp;
	return ret;
}
/*	Fill a disc of radius r centered at x1,y1
 */
disc(b, p, r, f)
	Bitmap *b;
	Point p;
	int r;
	Code f;
{
	unsigned int diam = 2*r;
	if (b->flag & BI_PIXMAP)
		p = sub(p, b->rect.origin);
	p = sub(p, Pt(r,r));
	XFillArc(dpy, b->dr, gcs[f], p.x, p.y, diam, diam, 0, 23040/* 360 deg */);
}

typedef struct String{
	char *s;	/* pointer to string */
	short n;	/* number used, no terminal null */
	short size;	/* size of allocated area */
} String;

getmuxbuf(pmb)
	String *pmb;
{
	char *ans;
	int n;
	ans=XFetchBytes(dpy, &n);
	pmb->size=pmb->n=n;
	gcalloc(pmb->size, &(pmb->s));
	strncpy(pmb->s, ans, n);
	free(ans);
}

#define	UP	0
#define	DOWN	1

static short boxcurs_bits[] = {
	0x43FF, 0xE001, 0x7001, 0x3801, 0x1D01, 0x0F01, 0x8701, 0x8F01,
	0x8001, 0x8001, 0x8001, 0x8001, 0x8001, 0x8001, 0x8001, 0xFFFF,
};
static Cursor boxcurs;

buttons(updown)
{
	while((button123()!=0) != updown)
		jnap(2);
}

Rectangle
canon(p1, p2)
	Point p1, p2;
{
	Rectangle r;
	r.origin.x = min(p1.x, p2.x);
	r.origin.y = min(p1.y, p2.y);
	r.corner.x = max(p1.x, p2.x);
	r.corner.y = max(p1.y, p2.y);
	return(r);
}

static outline(r)
	Rectangle  r;
{
	XPoint p[5], *pp;
	
	pp = p;
	pp->x = r.corner.x; pp->y = r.origin.y; pp++;
	pp->x = r.corner.x; pp->y = r.corner.y; pp++;
	pp->x = r.origin.x; pp->y = r.corner.y; pp++;
	pp->x = r.origin.x; pp->y = r.origin.y; pp++;
	pp->x = r.corner.x; pp->y = r.origin.y; pp++;
	XDrawLines(dpy, display.dr, gcs[F_XOR], p, 5, CoordModeOrigin);
}

Rectangle
getrectb(n)
	int n;
{
	Rectangle r;
	Point p1, p2;
	
	if (!boxcurs)
		boxcurs = ToCursor(boxcurs_bits, boxcurs_bits, 8, 8);
	cursswitch(&boxcurs);
	buttons(UP);
	buttons(DOWN);
	if(!(mouse.buttons&n)){
		r.origin.x=r.origin.y=r.corner.x=r.corner.y=0;
		buttons(UP);
		goto Return;
	}
	p1=mouse.xy;
	p2=p1;
	r=canon(p1, p2);
	outline(r);
	for(; mouse.buttons&n; jnap(2)){
		outline(r);
		p2=mouse.xy;
		r=canon(p1, p2);
		outline(r);
	}
	outline(r);	/* undraw for the last time */
    Return:
	cursswitch(0);
	return r;
}

Rectangle
getrect(n)
{
	return getrectb(8>>n);
}

#define scale(x, inmin, inmax, outmin, outmax)\
	(outmin + muldiv(x-inmin,outmax-outmin,inmax-inmin))

#define bound(x, low, high) min(high, max( low, x ))

#define DISPLAY		16
#define DELTA		6
#define BARWIDTH	18

static char **table;
static char *
tablegen(i)
{
	return table[i];
}

menuhit(m, but)
register Menu *m;
{
	register int width, i, j, top, newtop, hit, newhit, items, lines, length;
	Point p, q, savep, baro, barc;
	Rectangle sr, tr, mr;	/* scroll, text, menu */
	register Bitmap *b;
	register char *s, *(*generator)(), *from, *to;
	char fill[64];
	Font *font = &defont;
	int spacing = font->max_bounds.ascent + font->max_bounds.descent;

#define sro sr.origin
#define src sr.corner
#define tro tr.origin
#define trc tr.corner
#define mro mr.origin
#define mrc mr.corner

	generator = (table=m->item) ? tablegen : m->generator;
	p = mouse.xy;
	length = width = items = 0;
	for( ; s=(*generator)(items); ++items) {
		length = max(length, strlen(s));
		width = max(width, strwidth(font,s));
	}
	if(items == 0){
		while(button(but));
		return -1;
	}
	width += 10;
	sro.x = sro.y = src.x = tro.x = mro.x = mro.y = 0;
	if(items <= DISPLAY)
		lines = items;
	else{
		lines = DISPLAY;
		tro.x = src.x = BARWIDTH;
		sro.x = sro.y = 1;
	}
	tro.y = 1;
	mrc = trc = add(Pt(tro.x, mro.y), Pt(width, min(items, lines)*spacing+2));
	trc.y = src.y = mrc.y-1;
	newtop = bound(m->prevtop, 0, items-lines);
	p.y -= bound(m->prevhit, 0, lines-1)*spacing+spacing/2;
	p.x = bound(p.x-(src.x+width/2), 0, display.rect.corner.x-mrc.x);
	p.y = bound(p.y, 0, display.rect.corner.y-mrc.y);
	sr = raddp(sr, p);
	tr = raddp(tr, p);
	mr = raddp(mr, p);
	b = balloc(mr);
	if(b)
		bitblt(&display, mr, b, mro, F_STORE);
	rectf(&display, mr, F_OR);
PaintMenu:
	rectf(&display, inset(mr, 1), F_CLR);
	top = newtop;
	if(items > DISPLAY){
		baro.y = scale(top, 0, items, sro.y, src.y);
		baro.x = sr.origin.x;
		barc.y = scale(top+DISPLAY, 0, items, sro.y, src.y);
		barc.x = sr.corner.x;
		rectf(&display, Rpt(baro,barc), F_XOR);
	}
	for(p=tro, i=top; i < min(top+lines, items); ++i){
		q = p;
		from = generator(i);
		for(to = &fill[0]; *from; ++from)
			if(*from & 0x80)
				for(j=length-(strlen(from+1)+(to-&fill[0])); j-->0;)
					*to++ = *from & 0x7F;
			else
				*to++ = *from;
		*to = '\0';
		q.x += (width-strwidth(font,fill))/2;
		string(font, fill, &display, q, F_XOR);
		p.y += spacing;
	}
	savep = mouse.xy;
	for(newhit = hit = -1; button(but); jnap(2)){
		if(ptinrect(p = mouse.xy, sr)){
			if(ptinrect(savep,tr)){
				p.y = (baro.y+barc.y)/2;
				cursset(p);
			}
			newtop = scale(p.y, sro.y, src.y, 0, items);
			newtop = bound(newtop-DISPLAY/2, 0, items-DISPLAY);
			if(newtop != top)
				goto PaintMenu;
		}else if(ptinrect(savep,sr)){
			register dx, dy;
			if(abs(dx = p.x-savep.x) < DELTA)
				dx = 0;
			if(abs(dy = p.y-savep.y) < DELTA)
				dy = 0;
			if(abs(dy) >= abs(dx))
				dx = 0;
			else
				dy = 0;
			cursset(p = add(savep, Pt(dx,dy)));
		}
		savep = p;
		newhit = -1;
		if(ptinrect(p, tr)){
			newhit = bound((p.y-tro.y)/spacing, 0, lines-1);
			if(newhit!=hit && hit>=0
			 && abs(tro.y+spacing*newhit+spacing/2-p.y) > spacing/3)
				newhit = hit;
		}
		if(newhit != hit){
			flip(tr, hit, spacing);
			flip(tr, hit = newhit, spacing);
		}
		if(newhit==0 && top>0){
			newtop = top-1;
			p.y += spacing;
			cursset(p);
			goto PaintMenu;
		}
		if(newhit==DISPLAY-1 && top<items-lines){
			newtop = top+1;
			p.y -= spacing;
			cursset(p);
			goto PaintMenu;
		}
	}
	if(b){
		bitblt(b, b->rect, &display, b->rect.origin, F_STORE);
		bfree(b);
	}
	if(hit>=0){
		m->prevhit = hit;
		m->prevtop = top;
		return hit+top;
	}else
		return -1;
}

static
flip(r,n,spacing)
	Rectangle r;
{
	if(n<0)
		return;
	++r.origin.x;
	r.corner.y = (r.origin.y += spacing*n) + spacing;
	--r.corner.x;
	rectf(&display, r, F_XOR);
}

void
point(b,p,f)
	Bitmap *b;
	Point p;
	Code f;
{
	if (b->flag & BI_PIXMAP)
		p = sub(p, b->rect.origin);
	XDrawPoint(dpy, b->dr, gcs[f], p.x, p.y);
}

#define PBSIZE	100

static XPoint xp[PBSIZE];
static xpcnt;
static Code fc;
static ispixmap;
static Bitmap *bitm;

#define flushpt()	if (xpcnt) flushpoints();

points(p)
	Point p;
{
	register XPoint *x;

	if (ispixmap)
		p = sub(p, bitm->rect.origin);
	x = &xp[xpcnt];
	x->x = p.x;
	x->y = p.y;
	if (++xpcnt == PBSIZE)
		flushpoints();
}

initpoints(b, f)
	Bitmap *b;
	Code f;
{
	if (b->flag & BI_PIXMAP)
		ispixmap = 1;
	else
		ispixmap = 0;
	bitm = b;
	fc = f;
}

endpoints()
{
	flushpt();
	XSync(dpy, 0);
}

flushpoints()
{
	if (xpcnt) {
		XDrawPoints(dpy, bitm->dr, gcs[fc], xp, xpcnt, CoordModeOrigin);
		xpcnt = 0;
	}
}

/*
 * Buffer for keyboard input
 */
#define RCVBUFSIZE	1024

static unsigned char rcvbuffer[RCVBUFSIZE];

static struct {
	unsigned char *buf;
	unsigned char *in;
	unsigned char *out;
	int cnt;
	int size;
}rcvbuf = { rcvbuffer, rcvbuffer, rcvbuffer, 0, RCVBUFSIZE };


rcvchar()
{
	int i;

	if (!rcvbuf.cnt)
		return -1;
	i = *rcvbuf.out++;
	if (rcvbuf.out == &rcvbuf.buf[rcvbuf.size])
		rcvbuf.out = rcvbuf.buf;
	if (--rcvbuf.cnt == 0)
		P->state &= ~RCV;
	return(i);
}

rcvfill()
{
	register i;

	if (rcvbuf.cnt == rcvbuf.size)
		return;
	if (rcvbuf.in < rcvbuf.out)
		i = rcvbuf.out - rcvbuf.in;
	else
		i = &rcvbuf.buf[rcvbuf.size] - rcvbuf.in;
	i = read(0, rcvbuf.in, i);
	if (i <= 0)
		return;
	P->state |= RCV;
	rcvbuf.cnt += i;
	rcvbuf.in += i;
	if (rcvbuf.in == &rcvbuf.buf[rcvbuf.size])
		rcvbuf.in = rcvbuf.buf;
}

void
request(r)
	int r;
{
	int i=1;
	if (r & RCV) {
		/* need non-blocking I/O on stdout */
#ifdef BSD
		ioctl(1, FIONBIO, &i);
#else
		ioctl(1, FIOWNBLK, 0);
#endif
	}
	/* for now, assume MOUSE|KBD requested */
}

void
segment(b,p,q,f)
	Bitmap *b;
	Point p, q;
	Code f;
{
	if (b->flag & BI_PIXMAP) {
		p = sub(p, b->rect.origin);
		q = sub(q, b->rect.origin);
	}
	XDrawLine(dpy, b->dr, gcs[f], p.x, p.y, q.x, q.y);
}

#ifndef BSD
#define EWOULDBLOCK EBUSY
#endif

sendnchars(n,p)
	char *p;
	int n;
{
	int i;
	int maxfd, rmask, wmask;

	while (n) {
		i = write(1, p, n);
		if (i > 0) {
			n -= i;
			p += i;
			continue;
		}
		if (i < 0 && errno == EWOULDBLOCK) {
			maxfd = dpy->fd + 1;
			do {
				while (XPending(dpy))
					handleinput();
				rmask = (1 << dpy->fd) | 1;
				wmask = 2;
#ifdef BSD
				select(maxfd, &rmask, &wmask, 0, 0);
#else
				select(maxfd, &rmask, &wmask, 0x6fffffff);
#endif
				if (rmask & 1)
					rcvfill();
				if (rmask & (1 << dpy->fd))
					handleinput();
			} while (!wmask);
		}
		else
			exit(1);
	}
}
#define MAXROOT 0xb504
sqrt(x)
	register long x;
{
	register long high=MAXROOT;
	register long low=0;
	register long current=MAXROOT/2;
	if(x<=0)
		return 0;
	if(x>=MAXROOT*MAXROOT)
		return(MAXROOT);
	while(high>low+1){
		if(current*current==x)
			return (current);
		if(current*current>x)
			high=current;
		else
			low=current;
		current=(high+low)>>1;
	}
	return(current);
}

wait(resource)
{
	int maxfd, smask, i;

	maxfd = dpy->fd + 1;
	for(;;) {
		if (P->state & resource)
			break;
		if (XPending(dpy))
			goto xin;
		if (resource & CPU)
			break;
		smask = (1 << dpy->fd) | 1;
#ifdef BSD
		select(maxfd, &smask, 0, 0, 0);
#else
		select(maxfd, &smask, 0, 0x6fffffff);
#endif
		if (smask & 1)
			rcvfill();
		if (smask & (1 << dpy->fd)) {
xin:
			handleinput();
			/* We always have the mouse and cpu */
			if (resource & (MOUSE|CPU))
				break;
		}
	}
	return P->state;
}

int
own()
{
	return P->state|MOUSE;
}
