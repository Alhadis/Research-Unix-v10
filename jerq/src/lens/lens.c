#include <jerq.h>

/* Purpose: to be an interactive magnification program.  To achieve
 * this, the program has three principal states: going, peeking, and
 * stopped.  When going, a crawling rectangle is drawn around the
 * source image.  This rectangle can be moved by using the mouse.
 * The enlargement appears inside the window of lens.  When peeking,
 * the same rectangle last used in going is the source image, and
 * every few sixtieths of a second, the enlarged image is recomputed
 * and redisplayed, even if the lens window is not the current window.
 * This allows the user to continuously view magnified images of a
 * changing window.  When stopped, lens continues to display whatever
 * it was displaying when last given a command.
 */

/* fac is the desired magnification factor; ofac is the previous smaller
   Fibonacci number, for convenience */
Point ofac={1, 1},fac={2, 2};

/* get a pointer to the real bitmap, and to real bitblt */
#ifdef JERQ
Bitmap disp={(Word *)(156*1024L), 50, 0, 0, XMAX, YMAX};
#define rawblt(s, r, d, p, c)	(*((void(*)())0x430d6))(s, r, d, p, c)
#else
Bitmap disp={(Word *)(0x700000L), 25, 0, 0, XMAX, YMAX};
#define rawblt(s, r, d, p, c)	(*((void(*)())0x7a24))(s, r, d, p, c)
#endif

Bitmap *save,*zap,*zapsave;
Point boompt, zappt;
Rectangle zapdisp;		/* place to show the magnified image */
Point diag={50, 50};		/* size of lens */
int fakebutton1 = 0;		/* make button2 act like button1 */
int saving;			/* are we incremental? */
int zapping;			/* should we peek? */
Rectangle osrect, os;		/* destination and source rectangles */

/* The following siz textures enable us to make a crawling border */
Texture border0 = {
	 0xF0F0, 0xE1E1, 0xC3C3, 0x8787,
	 0x0F0F, 0x1E1E, 0x3C3C, 0x7878,
	 0xF0F0, 0xE1E1, 0xC3C3, 0x8787,
	 0x0F0F, 0x1E1E, 0x3C3C, 0x7878,
};

Texture border1 = {
	 0x3C3C, 0x7878, 0xF0F0, 0xE1E1,
	 0xC3C3, 0x8787, 0x0F0F, 0x1E1E,
	 0x3C3C, 0x7878, 0xF0F0, 0xE1E1,
	 0xC3C3, 0x8787, 0x0F0F, 0x1E1E,
};

Texture border2 = {
	 0x0F0F, 0x1E1E, 0x3C3C, 0x7878,
	 0xF0F0, 0xE1E1, 0xC3C3, 0x8787,
	 0x0F0F, 0x1E1E, 0x3C3C, 0x7878,
	 0xF0F0, 0xE1E1, 0xC3C3, 0x8787,
};

Texture border3 = {
	 0xC3C3, 0x8787, 0x0F0F, 0x1E1E,
	 0x3C3C, 0x7878, 0xF0F0, 0xE1E1,
	 0xC3C3, 0x8787, 0x0F0F, 0x1E1E,
	 0x3C3C, 0x7878, 0xF0F0, 0xE1E1,
};

Texture mask0 = {
	 0xCCCC, 0x9999, 0x3333, 0x6666,
	 0xCCCC, 0x9999, 0x3333, 0x6666,
	 0xCCCC, 0x9999, 0x3333, 0x6666,
	 0xCCCC, 0x9999, 0x3333, 0x6666,
};

Texture mask1 = {
	 0x3333, 0x6666, 0xCCCC, 0x9999,
	 0x3333, 0x6666, 0xCCCC, 0x9999,
	 0x3333, 0x6666, 0xCCCC, 0x9999,
	 0x3333, 0x6666, 0xCCCC, 0x9999,
};

Texture *(borders[4]) = {&border0, &border1, &border2, &border3};
Texture *(mask[2])= {&mask0, &mask1};
int bstate=0;			/* which border are we showing? */

/* this is the signature icon of the lens program */
Texture glass = {
	 0x7FFE, 0x8001, 0x91C1, 0xB365,
	 0xB261, 0xB0C5, 0xA181, 0xA32D,
	 0xB7E5, 0x8001, 0x7FFE, 0x001C,
	 0x66EE, 0xCA67, 0x4423, 0xEEC1,
};

/* show a sunset to confirm exit */
Texture sunset = {
	 0x5006, 0xA819, 0x00A0, 0x04A0,
	 0x049F, 0x12A4, 0x0808, 0x03E0,
	 0x2412, 0x0808, 0x0808, 0x3FFF,
	 0x3C1F, 0x7E7E, 0x783E, 0xFCFC,
};

unsigned short delay = 64;	/* 60's of a second between peeks */

int fullsize = 0;		/* are we using the full window to enlarge? */

main()
{
	Bitmap **z0,**z1,**t;
	int got;
	request(MOUSE);
	cursswitch(&glass);
	P->state |= RESHAPED;		/* force first showoff */
	got = ALARM;
	for(;;){
		if (got&ALARM)
			alarm(delay);
		got=wait(MOUSE|ALARM);
		if(P->state&RESHAPED) {	/* redisplay jumbo lens when reshaped */
			bfree(save);
			bfree(zap);
			bfree(zapsave);
			freemag();
			save = zap = zapsave = 0;
			showoff(&glass);
		}
		if ((got&MOUSE) && (fakebutton1 || button123())) {
			if (button3()) {	/* put up system menu */
				request(0);
				sleep(1);
				request(MOUSE);
				alarm(delay);
				cursswitch(&glass);
			} else if (button2())
				runmenu(0);	/* put up lens menu */
			else if (button1() || fakebutton1) {
				bfree(zap);	/* start lensing */	
				bfree(zapsave);
				zap = zapsave = 0;
				track();
			}
		} else {
			if (zapping  && !zap) {
				prepkaboom();
				z0 = &zap;
				z1 = &zapsave;
				zapping = (zap != 0);
			}
			if (zap && (got&ALARM)) {
				rawblt(&disp,os,*z0,Pt(0,0),F_STORE);
				bitblt(*z0,zap->rect,*z1,Pt(0,0),F_XOR);
				magn(*z1,zap->rect,save,&display,boompt,fac,F_XOR);
				t = z1; z1 = z0; z0 = t;
			}
		}
	}
}

Point corr(q, a, b, fac)
Point q,a,b,fac;
{
	q = sub(q, a);
	q.x *= fac.x;
	q.y *= fac.y;
	return add(q, b);
}

Point corres(q, s, r, fac)
/* find the corresponding point to q when mapping from s to r, blowing up by fac */
Point q,fac;
Rectangle s,r;
{
	return corr(q, s.origin, r.origin, fac);
}

Rectangle correct(q, s, r, fac)
Rectangle q,s,r;
Point fac;
{
	q.origin = corr(q.origin, s.origin, r.origin, fac);
	q.corner = corr(q.corner, s.origin, r.origin, fac);
	return q;
}

Point delta(r)
Rectangle r;
{
	return sub(r.corner,r.origin);
}

Rectangle center(r,s)
/* center the rectangle r in s */
Rectangle r,s;
{
	return raddp(r,div(sub(add(s.origin,s.corner),add(r.origin,r.corner)),2));
}

Point showfac, showbase;

showoff(text)
Texture *text;
{
	Bitmap *b,*c;
	static Rectangle i = {0,0,16,16};
	Rectangle blow;
	c = balloc(i);
	P->state &= ~RESHAPED;
	if (c == (Bitmap *)0) {
		showfac.x = showfac.y = 0;
		return;
	}
	rectf(&display,Drect,F_CLR);
	texture(c, c->rect, text, F_STORE);
	showfac=div(delta(inset(Drect,10)),16);
	for(;;) {
		showbase = (center(blow = correct(i,i,i,showfac),Drect)).origin;
		if (b = balloc(blow)) {
			magn(c, i, b, &display, showbase, showfac, F_STORE);
			bfree(b);
			break;
		}
		if ((showfac.x == 1) && (showfac.y ==1)) {
			bitblt(c, i, &display, showbase, F_STORE);
			break;
		}
		if (showfac.x < showfac.y)
			showfac.y--;
		else
			showfac.x--;	
	}
	bfree(c);
}

#define RUNSTOP	0
#define SMALLER	1
#define BIGGER	2
#define FASTER	3
#define SLOWER	4
#define WHOLE	5
#define EXIT	6

char *lenstext[] = {"","     1x","     3x","0123456789AB","CDEFGHIJKLMN","","exit",NULL};

Menu lensmenu = { lenstext };

char *recprt(s,n)
unsigned n;
char *s;
{
	unsigned m = n/10;
	if (m)
		s = recprt(s,m);
	*s++ = '0' + n % 10;
	return s;
}

sprintn(s,n,t)
int n;
char *s,*t;
{
	if (n<0) {
		*s++ = '-';
		n = -n;
	}
	s = recprt(s,n);
	while (*s++ = *t++)
		;
}

runmenu(tracking)
int tracking;
{
	Point ndiag,setmouse;
	Rectangle r;

	setmouse=mouse.xy;
	bfree(zap);
	bfree(zapsave);
	bfree(save);
	freemag();
	save = zap = zapsave = 0;
	if (tracking) {
		Jcursallow();
		lenstext[RUNSTOP] = "peek";
	} else
		lenstext[RUNSTOP] = zapping?"stop":"go";
	lenstext[WHOLE] = fullsize?"inset":"full size";
	sprintn(lenstext[BIGGER],fac.x+ofac.x,"x");
	sprintn(lenstext[SMALLER],ofac.x,"x");
	sprintn(lenstext[FASTER],max(delay>>1,1),(delay > 3)?" ticks":" tick");
	sprintn(lenstext[SLOWER],delay<<1," ticks");
	switch(menuhit(&lensmenu,2)) {
		case RUNSTOP:
			if (!tracking && zapping) {
				bfree(save);
				save = 0;
				zapping = 0;
			} else
				fakebutton1 = 1;
			break;
		case BIGGER:
			fac=add(fac, ofac);
			ofac=sub(fac, ofac);
			break;
		case SMALLER:
			ofac=sub(fac, ofac);
			fac=sub(fac, ofac);
			if(ofac.x < 1)
				ofac.x=ofac.y=1;
			break;
		case FASTER:
			delay >>= 1;
			if (!delay)
				delay = 1;
			break;
		case SLOWER:
			delay <<= 1;
			break;
		case WHOLE:
			P->state |= RESHAPED;
			if (tracking)
				fakebutton1 = 1;
			fullsize = !fullsize;
			break;
		case EXIT:
			if (tracking)
				undraw();
			cursswitch(&sunset);
			showoff(&sunset);
			while (!button123())
				wait(MOUSE);
			while (button3())
				;
			if (button12()) {
				while (button123())
					wait(MOUSE);
				cursswitch(&glass);
				showoff(&glass);
				break;
			}
			exit();
	}
	if (tracking) {
		undraw();
		Jcursinhibit();
		cursset(setmouse);
		new();
	}
}	

Point old={-1,-1};

track(){
	if (old.x < 0)
		old = mouse.xy;
	else
		cursset(old);
	fakebutton1 = 0;
	Jcursinhibit();
	if (!save)
		new();
	do; while(button1());
	while(!button1() && !fakebutton1){
		draw();
		if(button2()){
			runmenu(1);
		}
	}
	undraw();
	fakebutton1 = 0;
	do; while(button123());
	zapping = 1;
	cursswitch(&glass);
	Jcursallow();
}

texrect(r,t,text1,text2)
Rectangle r;
Texture *text1,*text2;
int t;
{
	r = inset(r,-t);
	texture(&disp, Rect(r.origin.x,r.origin.y,r.corner.x-t,r.origin.y+t),
		text1, F_XOR);
	texture(&disp, Rect(r.origin.x+t,r.corner.y-t,r.corner.x,r.corner.y),
		text2, F_XOR);
	texture(&disp, Rect(r.corner.x-t,r.origin.y,r.corner.x,r.corner.y-t),
		text1, F_XOR);
	texture(&disp, Rect(r.origin.x,r.origin.y+t,r.origin.x+t,r.corner.y),
		text2, F_XOR);
}

outline(r,t)
Rectangle r;
int t;
{
	texrect(r,t,borders[bstate],borders[3-bstate]);
}

crawl(r,t)
Rectangle r;
int t;
{
	Texture *tex = mask[bstate&0x1];
	texrect(r,t,tex,tex);
	bstate++;
	bstate &= 0x3;
}

#define MININT		-32000	
#define MAXINT		32000
	
Rectangle makerect(ox, oy, cx, cy) {
	Rectangle r;
	r.origin.x = ox;
	r.origin.y = oy;
	r.corner.x = cx;
	r.corner.y = cy;
	return r;
/*	return *(Rectangle *)&ox;	*/
}

int partition(pr0, s, pr1, pr2)
Rectangle *pr0,s,*pr1,*pr2;
/* pr0 must be a rectangle which is smaller than s */
{
	Rectangle a1, a2;
	register Point so, sc;
	so=s.origin;
	sc=s.corner;
	*pr1 = *pr2 = *pr0;
	if (rectclip(pr0,s)) {
		a1 = makerect(so.x,MININT,sc.x,so.y);
		a2 = makerect(so.x,sc.y,sc.x,MAXINT);
		if (!rectclip(pr1,a1) && !rectclip(pr1,a2))
			pr1->corner = pr1->origin;
		a1 = makerect(MININT,MININT,so.x,MAXINT);
		a2 = makerect(sc.x,MININT,MAXINT,MAXINT);
		if (!rectclip(pr2,a1) && !rectclip(pr2,a2))
			pr2->corner = pr2->origin;
		return 1;
	} else
		return 0;
}

hardcase(r,s)
Rectangle r,s;
{
	Rectangle q, oq, q1, q2;
	Point qp;
	int fax,fay;
/* 1: Figure out what part of the old expansion is still worthwhile */
	oq = s;
	partition(&oq, os, &q1, &q2);
	q = correct(oq, s, r, fac);
	oq = correct(oq, os, osrect, fac);
/* 2: move the common piece to its new location, correcting outlines */
	bitblt(&display, oq, &display, q.origin, F_STORE);
/* 3: expand the residue */
	qp = corres(q1.origin,s,r,fac);
	magn(&disp, q1, save, &display, qp, fac, F_STORE);
	qp = corres(q2.origin,s,r,fac);
	magn(&disp, q2, save, &display, qp, fac, F_STORE);
}

draw(){
	Rectangle r, s;
	old = mouse.xy;
	s.origin.x=min(max(old.x-diag.x/2, 2), XMAX-diag.x-2);
	s.origin.y=min(max(old.y-diag.y/2, 2), YMAX-diag.y-2);
	s.corner=add(s.origin, diag);
	if (!save)
		new();
	r=osrect;
	if (saving)
		outline(os,2);
	if (!rectXrect(s,osrect) && saving && rectXrect(s,os))
		hardcase(r,s);
	else
		magn(&disp, s, save, &display, r.origin, fac, F_STORE);
	outline(s,2);
	saving = 1;
	os = s;
	while(!button12() && eqpt(mouse.xy, old)) {
		nap(2);
		crawl(os,2);
	}
}

undraw(){
	if (saving)
		outline(os,2);
	saving = 0;
}

new() {
	Rectangle r;
	Point d;
	bfree(save);
	freemag();
	if (fullsize)
		osrect = inset(Drect,4);
	else {
		osrect.origin = corr(Pt(1,1),Pt(0,0),showbase,showfac);	
		osrect.corner = corr(Pt(15,10),Pt(0,0),showbase,showfac);
		osrect = inset(osrect,min(8,min(showfac.x,showfac.y)));
	}
	rectf(&display,osrect,F_CLR);
	d = delta(osrect);
	while(((save=balloc(Rect(0, 0, (diag.x=d.x/fac.x)*fac.x, (diag.y=d.y/fac.y)*fac.y)))==0) ||
	      (!setmag(fac.x) && (((diag.x/8)*diag.y) > 64))){
		bfree(save);
		ofac=sub(fac, ofac);
		fac=sub(fac, ofac);
		if(ofac.x < 1){
			ofac.x=ofac.y=1;
			diag = d;
			break;
		}
	}
	osrect = center(Rect(0,0,diag.x*fac.x,diag.y*fac.y), osrect);
	os.origin.x=min(max(old.x-diag.x/2, 2), XMAX-diag.x-2);
	os.origin.y=min(max(old.y-diag.y/2, 2), YMAX-diag.y-2);
	os.corner=add(os.origin, diag);
}

prepkaboom() {
	Rectangle hole, boomto;
	Point p;
	if (!save)
		new();
	if (!setmag(fac.x))
		return;
	bfree(zap);
	bfree(zapsave);
	zap = zapsave = 0;
	zap = balloc(Rpt(Pt(0,0),diag));
	if (!zap)
		return;
	zapsave = balloc(zap->rect);
	if (!zapsave) {
		bfree(zap);
		zap = 0;
		return;
	}
	rectf(&display, osrect, F_CLR);
	rectf(zap,zap->rect,F_CLR);
	rectf(zapsave,zap->rect,F_CLR);

	boompt = osrect.origin;
}
		
