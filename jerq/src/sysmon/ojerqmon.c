#define MUXTERM
#define MPXTERM

#include <jerq.h>
#include <font.h>
#include <menu.h>
#include "defs.h"

#undef getname	/* strange hack to avoid name clash */

/* user nice sys queue idle */
Texture black={
	0xFFFF,	0xFFFF,	0xFFFF,	0xFFFF,	0xFFFF,	0xFFFF,	0xFFFF,	0xFFFF,	
	0xFFFF,	0xFFFF,	0xFFFF,	0xFFFF,	0xFFFF,	0xFFFF,	0xFFFF,	0xFFFF,	
};
Texture white={
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
};
Texture darkgrey = {
	0xDDDD, 0x7777, 0xDDDD, 0x7777, 0xDDDD, 0x7777, 0xDDDD, 0x7777,
	0xDDDD, 0x7777, 0xDDDD, 0x7777, 0xDDDD, 0x7777, 0xDDDD, 0x7777,
};
Texture lightgrey = {
	0x2222, 0x8888, 0x2222, 0x8888, 0x2222, 0x8888, 0x2222, 0x8888,
	0x2222, 0x8888, 0x2222, 0x8888, 0x2222, 0x8888, 0x2222, 0x8888,
};
Texture grey = {
	0xAAAA, 0x5555, 0xAAAA, 0x5555, 0xAAAA, 0x5555, 0xAAAA, 0x5555,
	0xAAAA, 0x5555, 0xAAAA, 0x5555, 0xAAAA, 0x5555, 0xAAAA, 0x5555,
};
Texture *txt[]={
	&black, &lightgrey, &darkgrey, &grey, &white
};

#	define TIMELEN 6
char time[TIMELEN];
#	define NAMELEN 9
#	define LOADLEN 13
#	define MAILLEN 18

struct nodedef {
    Rectangle bar;		/* the bar graph of system use */
    char name[NAMELEN];		/* node name */
    char load[LOADLEN];		/* load numbers */
    int vec[5];			/* current use vectors */
    int oldvec[5];		/* old use vectors */
    char mail[MAILLEN];		/* last mail from this node */
};
struct nodedef node0;		/* preallocate node 0 */
int nodes = 0;			/* number of nodes */
struct nodedef *node[MAXMACHS] = {
	&node0
};

/* size contraints */
#define ICONSEP 2
#define BARHEIGHT 18
#define MINBARHEIGHT 4
#define MAXBARHEIGHT 24
#define TIMEORIGIN 2
int CHARHEIGHT;
int CHARWIDTH;
int TIMECORNER;
int LOADORIGIN;
int LOADCORNER;
int MAILORIGIN;
int MAILCORNER;

int size;
int barheight;
int iconorigin;

/* the communications process */
struct Proc *cwp;

/* icon info */
#define ICONSIZE 48
#define ICONWORDS (ICONSIZE*(ICONSIZE/16))
Bitmap *icon, *ticon;
int icons = 0;
int maxicons;
int haveicons = 0;

/* dynamically add/remove monitored systems */

static NMitem astroitems[] = {
	{"alice"},
	{"arend"},
	{"atomic"},
	{"bowell"},
	{"dutoit"},
	{"encke"},
	{"forbes"},
	{"grigg"},
	{"ikeya"},
	{"kwee"},
	{"mrkos"},
	{"panther"},
	{"piggot"},
	{"polya"},
	{"research"},
	{"seki"},
	{"snb"},
	{"spit"},
	{"swift"},
	{"west"},
	{"wild"},
	{0}
};

static NMenu astromenu = {astroitems};

static NMitem phoneitems[] = {
	{"shannon"},
	{"shewhart"},
	{"sid"},
	{"slepian"},
	{"sweet"},
	{"tukey"},
	{0}
};

static NMenu phonemenu = {phoneitems};

static NMitem mercuryitems[] = {
	{"arachne"},
	{"balin"},
	{"bashful"},
	{"bifur"},
	{"capek"},
	{"doc"},
	{"eeyore"},
	{"gimli"},
	{"gunn"},
	{"happy"},
	{"hunny"},
	{"icarus"},
	{"indra"},
	{"iota"},
	{"jones"},
	{"kanga"},
	{"sapir"},
	{"siriusb"},
	{"sola"},
	{"sweet"},
	{"thorin"},
	{0}
};

static NMenu mercurymenu = {mercuryitems};

static NMitem machitems[] = {
	{"mh/vlsi/araki"},
	{"alc/ram/most"},
	{"mh/astro", 0, &astromenu},
	{"mh/mercury", 0, &mercurymenu},
	{"mh/phone", 0, &phonemenu},
	{0}
};

NMenu nmenu2 = {machitems};

static struct {
	char *name;
	NMitem *base, *lim;
} menulist[] = {
	{"mh/astro/", astroitems, astroitems+sizeof(astroitems)/(sizeof(NMitem))-1},
	{"mh/mercury/", mercuryitems, mercuryitems+sizeof(mercuryitems)/(sizeof(NMitem))-1},
	{"mh/phone/", phoneitems, phoneitems+sizeof(phoneitems)/(sizeof(NMitem))-1},
	{0}
};


main()
{
	register i, state, hadmouse;

#ifdef LUCA
	Rectangle tempr;

	/*
	 *	The following is a layer reshaping for Luca.  See sysmon -l.
	 */
	tempr = display.rect;
	dellayer(P->layer);
	tempr.corner.y = tempr.origin.y + 8+ICONSIZE;
	if (Drect.corner.y > YMAX-16) {
		tempr.corner.y = YMAX-8;
		tempr.origin.y = YMAX-8-(8+ICONSIZE);
	}
	Drect = inset(tempr,2);
	Jdisplayp = (Bitmap *) (P->layer = newlayer(tempr));
	if (!P->layer)
		exit();
#endif

	request(RCV|MOUSE|KBD);

	CHARHEIGHT = defont.height - 2;
	CHARWIDTH = strwidth(&defont, "m");
	TIMECORNER = NAMELEN*CHARWIDTH;
	LOADORIGIN = TIMECORNER;
	LOADCORNER = TIMECORNER + LOADLEN*CHARWIDTH;
	MAILORIGIN = LOADCORNER;
	MAILCORNER = LOADCORNER + MAILLEN*CHARWIDTH;
	barheight = MAXBARHEIGHT;
	iconorigin = MAILCORNER;
	time[0] =  0;
	hadmouse = 0;

	for(;;sleep(3)){
		if (cwp != 0 && cwp->state == 0)
			cwp = 0;
		state = own();
		if(state&RCV)
			get();
		if(state&MOUSE) {
			if(button2()) {
				domenu();
			} else if(button3()) {
				request(RCV|KBD);
				sleep(1);
				request(RCV|KBD|MOUSE);
			} else if (!hadmouse || button1()) {
				clearmail();
				clearicon();
			}
		}
		hadmouse = state&MOUSE;
		if(state&KBD) {
			while(kbdchar() != -1)	/* dump any keyboard input */
				;
		}
		for (i = 0; i < nodes; i++)
			drawbar (node[i]);
		if(P->state&RESHAPED)
			reshape(0);
	}
}

reshape(msg)
	char *msg;
{
	register int i;

	P->state&=~RESHAPED;
	rectf(&display, Drect, F_CLR);

	/* output an error message */
	if (msg != 0) {
		string(&defont, msg, &display,
		Pt (Drect.origin.x+2, Drect.origin.y + 2), F_XOR);
		sleep(200);
		rectf(&display, Drect, F_CLR);
	}

	/* resize objects */
	size = Drect.corner.x-Drect.origin.x;
	barheight = (Drect.corner.y-Drect.origin.y-2)/(nodes?nodes:1) - CHARHEIGHT;
	if (barheight < MINBARHEIGHT)
		barheight = MINBARHEIGHT;
	if (haveicons) {
		if (barheight > MAXBARHEIGHT)
			barheight = MAXBARHEIGHT;
		i = 3*size/4;
#ifndef LUCA
		size = (i > MAILCORNER) ? MAILCORNER
					: ((i > LOADCORNER) ? LOADCORNER
							    : TIMECORNER);
#else
		size = (i > LOADCORNER) ? LOADCORNER : TIMECORNER;
#endif LUCA
		iconorigin = size;
		maxicons = (Drect.corner.x-Drect.origin.x-iconorigin)
			   / (ICONSIZE+ICONSEP);
	}

	/* redraw */
	for (i = 0; i < nodes; i++) {
		nodeinit(i);
		drawload(node[i]);
		drawmail(node[i]);
	};
	drawtime();
	clearmail();
	clearicon();
}


/* clear out the bar and write in the node name */
nodeinit (ind)
int ind;				/* index into node array */
{
	register struct nodedef *n;
	register int i;

	n = node[ind];

	/* make the bar */
	n->bar.origin.x = Drect.origin.x;
	n->bar.origin.y = Drect.origin.y + ind*(barheight+CHARHEIGHT);
	n->bar.corner.x = Drect.origin.x + size;
	n->bar.corner.y = n->bar.origin.y + barheight;
	n->bar = inset(n->bar, 4);
	rectf(&display, inset(n->bar, -2), F_OR);
	rectf(&display, n->bar, F_CLR);

	/* init the node */
	for(i = 0; i<4; i++)
		n->vec[i] = n->oldvec[i] = 0;
	n->vec[4]=n->oldvec[4]=size;

	/* write the name */
	if (ind != 0 && size >= TIMECORNER)
		string (&defont, n->name, &display, 
			Pt(TIMEORIGIN+Drect.origin.x, n->bar.origin.y+barheight-6),
			F_XOR);
};

domenu() {
	NMitem *mhit(), *sys;
	char name[48];

	sys = mhit(&nmenu2, 2, 0);
	if (sys) {
		register int i;
		strcpy(name, "M");
		for (i = 0; menulist[i].name; i++) {
			if (sys >= menulist[i].base && sys < menulist[i].lim) {
				strcat (name, menulist[i].name);
				break;
			}
		}
		strcat(name, sys->text);
		strcat(name, "\n");
		if (index (name, '/'))
			sendnchars(strlen(name), name);
	}
}


Point pt[4];
Rectangle new, old;

/* draw the bar graph for a system's CPU time */
drawbar (n)
struct nodedef *n;
{
	register i, now;

	pt[0].y = pt[2].y = n->bar.origin.y;
	pt[1].y = pt[3].y = n->bar.corner.y;
	new = old = n->bar;
	for(i=0; i<5; i++){
		now = relax(n->oldvec[i], n->vec[i]);
		old.corner.x = n->oldvec[i] + old.origin.x;
		new.corner.x = now+new.origin.x;
		sort(old.origin.x,old.corner.x, new.origin.x,new.corner.x);
		if (pt[3].x > n->bar.corner.x)
			pt[3].x = n->bar.corner.x;
		if (pt[1].x > n->bar.corner.x)
			pt[1].x = n->bar.corner.x;
		texture(&display, Rpt(pt[0], pt[1]), txt[i], F_XOR);
		texture(&display, Rpt(pt[2], pt[3]), txt[i], F_XOR);
		new.origin.x = new.corner.x;
		old.origin.x = old.corner.x;
		n->oldvec[i] = now;
	}
}

relax(o, n)
	int o, n;
{
	register a;
	register s=1;
	register long lo = o;
	register long ln = n;

	if(o>n)
		s= -1;
	a=abs(o-n);
	if(a<2)
		return n;
	if(a<127)
		return o+s;
	ln = (127L*lo + ln) / 128L;
	s = ln;

	return s;
}

sort(x)
	int x;
{
	register int *p= &x;
	register i,j;
	register t;
	for(i=0; i<3; i++){
		for(j=0; j<3; j++)
			if(p[j]>p[j+1]){
				t=p[j];
				p[j]=p[j+1];
				p[j+1]=t;
			}
	}
	for(i=0; i<4; i++)
		pt[i].x=p[i];
}

drawload(n)
	struct nodedef *n;
{
	if (size < LOADCORNER)
		return;
	string(&defont, n->load, &display, 
		Pt (Drect.origin.x+LOADORIGIN, n->bar.origin.y+barheight-6), F_XOR);
}

drawtime()
{
	if (size < TIMECORNER)
		return;
	string(&defont, time, &display,
		Pt (Drect.origin.x+TIMEORIGIN, Drect.origin.y+barheight-2), F_XOR);
}

drawmail(n)
	struct nodedef *n;
{
	if (size < MAILCORNER)
		return;
	string(&defont, n->mail, &display,
		Pt (Drect.origin.x+MAILORIGIN, n->bar.origin.y+barheight-6), F_XOR);
}

clearmail()
{
	register int i;

	for (i = 0; i < nodes; i++) {
		drawmail(node[i]);
		node[i]->mail[0] = '\0';
 	}
}

drawicon()
{
	Rectangle rect;
	Point to;
	int bltwidth;

	if (!haveicons || maxicons < 1)
		return;

	/* move icons right */
	bltwidth = (maxicons-1)*(ICONSIZE+ICONSEP) - ICONSEP;
	rect.origin.x = Drect.origin.x + iconorigin;
	rect.origin.y = Drect.origin.y + 2;
	rect.corner.x = rect.origin.x + bltwidth;
	rect.corner.y = rect.origin.y + ICONSIZE + 4 + CHARHEIGHT;
	to.x = rect.origin.x + ICONSIZE + ICONSEP;
	to.y = rect.origin.y;
	bitblt(&display, rect, &display, to, F_STORE);

	if (icons != maxicons)
		icons++;

	/* clear area */
	rect.corner.x = to.x;
	rectf(&display, rect, F_CLR);

	/* drop in new picture */
	to = rect.origin;
	rect = icon->rect;
	for (rect.origin.y += ICONSIZE-1;
	     rect.origin.y >= icon->rect.origin.y; 
	     rect.origin.y--) {
		bitblt(icon, rect, &display, to, F_STORE);
		sleep(2);
	}

	/* add a time stamp */
	to.y += ICONSIZE + 4;
	string(&defont, time, &display, to, F_XOR);
}

clearicon()
{
	Rectangle rect;

	if (!haveicons || maxicons < 1 || icons == 0)
		return;
	icons = 0;
	rect.origin.x = Drect.origin.x + iconorigin - 1;
	rect.origin.y = Drect.origin.y + 1;
	rect.corner.x = rect.origin.x + maxicons*(ICONSIZE+ICONSEP) - ICONSEP + 2;
	rect.corner.y = rect.origin.y + ICONSIZE + CHARHEIGHT + 6;
	rectf(&display, rect, F_CLR);
}

/* process a host message */
get(){
	register int ch;
	register struct nodedef *n;
	struct nodedef *nodeptr();
#	define MAGIC '_'

	ch = hostchar();
	if (ch == MAGIC) {

		/* this is probably a message from the host half of sysmon */
		ch = hostchar();
		switch(ch){
		case 'V':
			while ((ch = hostchar()) != -1) {
				n = nodeptr(ch);
				getvec(n);
			}
			break;
		case 'L':
			while ((ch = hostchar()) != -1) {
				n = nodeptr(ch);
				drawload(n);
				getload(n);
				drawload(n);
			}
			break;
		case 'T':
			drawtime();
			gettime();
			drawtime();
			break;
		case 'M':
			n = nodeptr(hostchar());
			drawmail(n);
			getmail(n);
			drawmail(n);
			break;
		case 'I':
			geticon();
			drawicon();
			break;
		case 'N':
			getname();
			reshape(0);
			break;
		case 'Q':
			icon = balloc(Rect(0, 0, ICONSIZE, ICONSIZE));
			ticon = balloc(Rect(0, 0, ICONSIZE, ICONSIZE));
			if (!icon || !ticon) {
				reshape("Not enough memory for faces.");
				sendnchars(2, "R\n");		/* refuse faces */
				break;
			}
			sendnchars(2, "A\n");		/* accept faces */
			haveicons = 1;
			reshape(0);
			break;
		case 'R':
			remove(hostchar()-'0');
			reshape(0);
			break;
		case -1:
			return;
		default:
			writecw(ch);
			break;
		}
	} else {
		writecw(ch);
	}
	ignorerest();
}

struct nodedef *
nodeptr(c)
	int c;
{
	register i;

	i = c - '0';
	if (i < 0 || i >= nodes)
		i = 0;
	return node[i];
}

getvec(n)
	struct nodedef *n;
{
	register int i, sum, nsum, vec[5];

	for(sum=0,i=0; i<5; i++)
		sum += (vec[i] = hostchar());
	sum = sum ? sum : 1;
	for(nsum=0,i=0; i<4; i++)
		nsum += n->vec[i] = ((long)(vec[i])*size) / sum;
	n->vec[4]=size-nsum;
}

gettime()
{
	register char *p=time;
	register int c, i=0;

	while((c = hostchar()) != -1 && ++i < TIMELEN) 
		*p++ = c;
	*p=0;
}

getload(n)
	struct nodedef *n;
{
	register char *p=n->load;
	register int i=0;

	while((*p = hostchar()) != '\t' && ++i < LOADLEN)
		p++;
	*p=0;
}

getname()
{
	register char *p;
	register int c, i=0;

	if (nodes != 0) {
		node[nodes] = (struct nodedef *)alloc(sizeof (struct nodedef));
		if (!node[nodes]) {
			reshape("out of memory");
			sendnchars(2, "R\n");		/* refuse connection */
			return;
		}
	}
	sendnchars(2, "A\n");				/* accept connection */
	p = node[nodes]->name;
	while((c = hostchar()) != -1 && ++i < NAMELEN)
		*p++ = c;
	*p=0;
	nodes++;
}

remove(ind)
	register int ind;
{
	if (ind < 0 || ind >= nodes)
		return;
	free (node[ind]);
	for (; ind < nodes-1; ind++)
		node[ind] = node[ind+1];
	nodes--;
}

getmail(n)
	struct nodedef *n;
{
	register char *p=n->mail;
	register int c, i=0;

	while((c = hostchar()) != -1 && ++i < MAILLEN)
		*p++ = c;
	upfront(P->layer);
	ringbell ();
	*p=0;
}

geticon()
{
	register int c, i, j;
	register short *ip;

	/* once per scan line */
	ip = (short *)icon->base;
	for (i = 0; i < ICONSIZE; i++) {

		/* once per short */
		for (j = 0; j < ICONSIZE/16; j++) {
			c = hostchar();
			*ip = (c<<8) | hostchar();
#ifndef	mc68000
			if (VALSCREENCOLOR==0)
				*ip = ~*ip;
#endif mc68000
			ip++;
		}

#ifndef	mc68000
		/* blit and 5620 have different size bitmaps */
		ip++;
#endif mc68000
	}
}

int atend = 0;		/* non-zero if at end of message */

hostchar(){
	register int c;

	if (atend)
		return -1;

	if((c=rcvchar())==-1)
		wait(RCV), c=rcvchar();

	if (c == '\n') {

		/* newline's mark end of message */
		atend = 1;
		return -1;
	} else if (c == '\\') {

		/* backslashes are character escapes */
		if ((c=rcvchar())==-1)
			wait(RCV), c=rcvchar();
	}

	return c&0xff;
}

ignorerest()
{
	while (hostchar() != -1)
		;
	atend = 0;
}


#ifdef mc68000
ringbell ()
{
	*((char *)(384*1024L+062)) = 2;
}
#endif mc68000

/*
 *	The rest of jerqmon.c contains routines to create and update the
 *	communications window.
 */ 
#define	INSET	4
#define	C_NEW	2

makecw()
{
	register Rectangle r;
	register unsigned char *pout;

	/* first see if the process already exists */
	if (cwp != 0 && cwp->state != 0)
		return 0;
	else
		cwp = 0;

	/* create process */
	r = Drect;
	if(!(cwp=newproc((struct Proc *)0)))
		return -1;
	cwp->rect=inset(r, INSET);
	if( !rectclip(&cwp->rect,Jrect) || !(cwp->layer=newlayer(r))){
		cwp->state=0;
		return -1;
	}

	/* give it a window */
#ifndef mc68000
	muxnewwind(cwp, C_NEW);
#else
	mpxnewwind(cwp, C_NEW);
#endif mc68000

	/* wait for a prompt */
	pout = cwp->cbufpout;
	do sleep( 60 ); while( pout == cwp->cbufpout );
	ringbell();

	return 0;
}

/*
 *	Write a string to the communications window.  Create the
 *	window if it has disappeared.
 */
writecw(c)
	register int c;
{
	if (makecw() < 0) {
		drawmail(node[0]);
		getmail(node[0], "can't open comm window\n");
		drawmail(node[0]);
		return;
	}
	do {
		if (cwp->state) {
			upfront(cwp->layer);
			while (cwp->nchars < CBSIZE*3) {
				*(cwp->cbufpin++) = c != -1 ? c : '\n';
				cwp->nchars++;
				if (cwp->cbufpin >= &(cwp->cbuf[CBSIZE*3]))
					cwp->cbufpin = cwp->cbuf;
				if (c == -1)
					break;
				c = hostchar();
			}
			cwp->state |= WAKEUP;
			sleep (10);
		} else
			return;
	} while (c != -1);
}
