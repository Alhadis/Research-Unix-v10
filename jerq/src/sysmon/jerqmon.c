#define MUXTERM
#define MPXTERM

#ifndef DMD630
#include <jerq.h>
#include <menu.h>
#else
#include <dmd.h>
#include <5620.h>
#include <research.h>
#include <object.h>
#undef mouse
#undef CURSOR
#include <ucode/setup.h>
#include <ucode/io.h>
#include "menu.h"
#endif
#include <font.h>
#include <string.h>
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
Bitmap *icon, *ticon;
int icons = 0;
int maxicons;
int haveicons = 0;

#ifdef DMD630
Font *monfont;
#else
#define	monfont	(&defont)
#endif

NMenu *nmenu2;
NMenu *newmenu();

main()
{
	register i, state, hadmouse;


#ifdef DMD630
	monfont=&defont;
	{
		register Font *f;

		if(f=fontrequest("defont"))
			monfont=f;
	}
	cache((char *)0, A_NO_SHOW);
#endif /*DMD630*/
	nmenu2 = newmenu();
	request(RCV|MOUSE);

	CHARHEIGHT = monfont->height - 2;
	CHARWIDTH = strwidth(monfont, "m");
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
			reshape((char *)0);
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
		string(monfont, msg, &display,
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
		size = (i > MAILCORNER) ? MAILCORNER
					: ((i > LOADCORNER) ? LOADCORNER
							    : TIMECORNER);
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
		string (monfont, n->name, &display, 
			Pt(TIMEORIGIN+Drect.origin.x, n->bar.origin.y+barheight-6),
			F_XOR);
};

domenu() {
	NMitem *mhit(), *sys;
	char name[48];

	sys = mhit(nmenu2, 2, 0);
	if (sys && (sys->next==(NMenu *)0)) {
		register int i;
		strcpy(name, "M");
		strcat(name, sys->text);
		strcat(name, "|");
		strcat(name, sys->help);
		strcat(name, "\n");
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
	if(a<255)
		return o+s;
	ln = (255L*lo + ln) / 256L;
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
	string(monfont, n->load, &display, 
		Pt (Drect.origin.x+LOADORIGIN, n->bar.origin.y+barheight-6), F_XOR);
}

drawtime()
{
	if (size < TIMECORNER)
		return;
	string(monfont, time, &display,
		Pt (Drect.origin.x+TIMEORIGIN, Drect.origin.y+barheight-2), F_XOR);
}

drawmail(n)
	struct nodedef *n;
{
	if (size < MAILCORNER)
		return;
	string(monfont, n->mail, &display,
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
	int bltwidth, i;

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
	rect.origin.x += ICONSIZE/2;
	rect.origin.y += ICONSIZE/2;
	rect.corner.x -= ICONSIZE/2;
	rect.corner.y -= ICONSIZE/2;
	to.x += ICONSIZE/2;
	to.y += ICONSIZE/2;
	for (i=0; i<ICONSIZE/2; i++){
		rect.origin.x--;
		rect.origin.y--;
		rect.corner.x++;
		rect.corner.y++;
		to.x--;
		to.y--;
		bitblt(icon, rect, &display, to, F_STORE);
		sleep(2);
	}

	/* add a time stamp */
	to.y += ICONSIZE + 4;
	string(monfont, time, &display, to, F_XOR);
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

NMenu *
newmenu()
{
	NMenu *m;

	m = (NMenu *)alloc(sizeof(NMenu));
	m->generator = 0;
	m->prevhit = -1;
	m->prevtop = -1;
	m->item = (NMitem *)alloc((MAXLIST+1)*sizeof(NMitem));
	m->item->text = (char *)0;
	return(m);
}

void addentry(m, l)
NMenu *m;
char *l;
{
	NMitem *ip=m->item;
	int n, found=0;
	char ch, *nextname, *fullname;

	for (; (ch=*l) && (ch==' ' || ch=='\t'); l++) {}
	if (!*l)
		return;
	if ((nextname=strchr(l, '\t')) != (char *)0)
		*nextname++ = '\0';
	for (n=MAXLIST; n>0 && ip->text && !(found=!strcmp(ip->text, l)); ip++, n--) {}

	if (!found && n<=0)		/* no room for a new entry */
		return;
	if (!found) {
		if((fullname=strchr(l, '|')) != (char *)0)
			*fullname++ = '\0';
		else
			fullname = l;
		ip->text = strcpy(alloc(strlen(l)+1), l);
		ip->help = strcpy(alloc(strlen(fullname)+1), fullname);
		ip->next = (NMenu *)0;
		ip->dfn  = ip->bfn = ip->hfn = 0;
		ip->data = 0;
		(ip+1)->text = (char *)0;
	}
	if(!nextname)
		return;
	if(!ip->next) 
		ip->next = newmenu();
	addentry(ip->next, nextname);
}


/* process a host message */
get(){
	register int ch, i;
	register struct nodedef *n;
	struct nodedef *nodeptr();
	static starting = 1;
	char buf[PATHLEN+1];
#	define HMAGIC '_'

	do {
		ch = hostchar();
	} while(starting && ch != HMAGIC);
	starting = 0;
	if (ch == HMAGIC) {

		/* this is probably a message from the host half of sysmon */
		ch = hostchar();
		switch(ch){
		case 'E':
			i=0;
			while ((ch = hostchar()) != -1)
				if (i<PATHLEN)
					buf[i++] = ch;
			buf[i] = '\0';
			addentry(nmenu2, buf);
			sendnchars(2, "A\n");
			break;
		case 'I':
			geticon();
			drawicon();
			break;
		case 'L':
			while ((ch = hostchar()) != -1) {
				n = nodeptr(ch);
				drawload(n);
				getload(n);
				drawload(n);
			}
			break;
		case 'M':
			n = nodeptr(hostchar());
			drawmail(n);
			getmail(n);
			drawmail(n);
			break;
		case 'N':
			getname();
			reshape((char *)0);
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
			reshape((char *)0);
			break;
		case 'R':
			remove((int)(hostchar()-'0'));
			;
			break;
		case 'T':
			drawtime();
			gettime();
			drawtime();
			break;
		case 'V':
			while ((ch = hostchar()) != -1) {
				n = nodeptr(ch);
				getvec(n);
			}
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
	*node[nodes]->mail = *node[nodes]->load = 0;
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
	reshape((char *)0);
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
		for (j = 0; j < ICONSIZE/(8*sizeof(short)); j++) {
			c = hostchar();
			*ip = (c<<8) | hostchar();
			if (VALSCREENCOLOR==0)
				*ip = ~*ip;
			ip++;
		}
#if WORDSIZE==32
		*ip++ = 0;		/* extra short in 48-wide bitmap */
#endif

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


/*
 *	The rest of jerqmon.c contains routines to create and update the
 *	communications window.
 */ 
#define	INSET	4
#define	C_NEW	2

#ifndef DMD630
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

	muxnewwind(cwp, C_NEW);

	/* wait for a prompt */
	pout = cwp->cbufpout;
	do sleep( 60 ); while( pout == cwp->cbufpout );
	ringbell();

	return 0;
}
#else /*DMD630*/
makecw()
{
	register Rectangle r;
	register unsigned char *pout;
	Proc *upproc();

	/* first see if the process already exists */
	if (cwp != 0 && cwp->state != 0)
		return 0;
	else
		cwp = 0;

	/* create process */
	r = Drect;
	if(!(cwp=upproc(P->host->defproc, r, P->host, STKSIZE)))
		return(-1);
	cwp->rect=inset(r, INSET);
	if( !rectclip(&cwp->rect,Jrect) /* || !(cwp->layer=newlayer(r))*/){
		cwp->state=0;
		return -1;
	}

	/* wait for a prompt */
	pout = cwp->rcvq.first;
	do sleep( 60 ); while( pout == cwp->rcvq.first );
	ringbell();

	return 0;
}
#endif /*DMD630*/
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
#ifndef DMD630
			while (cwp->nchars < CBSIZE*3) {
				*(cwp->cbufpin++) = c != -1 ? c : '\n';
				cwp->nchars++;
				if (cwp->cbufpin >= &(cwp->cbuf[CBSIZE*3]))
					cwp->cbufpin = cwp->cbuf;
#else
			while (cwp->rcvq.c_cc < cwp->rcvq.size) {
				*(cwp->rcvq.last++) = c != -1 ? c : '\n';
				cwp->rcvq.c_cc++;
				if (cwp->rcvq.last >= cwp->rcvq.array_last)
					cwp->rcvq.last = cwp->rcvq.array;
#endif
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
