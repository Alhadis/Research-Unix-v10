#include <jerq.h>
#include <font.h>
#include <layer.h>
#include <queue.h>
#include <jerqproc.h>
#include <setup.h>
#include "../msgs.h"
#include "pconfig.h"
#include "proto.h"
#include "packets.h"

struct Pchannel pconvs[4];	/* Should be NPROC!!! */
extern struct Proc *debugger;
extern int recvchars(), sendpkt();
extern	boot();
short	usermouse=0;	/* kbdproc (a USER proc) has the mouse under its paw */
extern short second;
#define	crecvchars	recvchars

struct Pconfig pconfig={
	sendpkt,
	recvchars,
	(void(*)())crecvchars
};

Texture cup = {
	0x0100, 0x00E0, 0x0010, 0x03E0, 0x0400, 0x0FE0, 0x123C, 0x1FE2,
	0x101A, 0x101A, 0x1002, 0x103C, 0x1810, 0x6FEC, 0x4004, 0x3FF8,
};
Texture deadmouse = {
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x000C, 0x0082, 0x0441,
	0xFFE1, 0x5FF1, 0x3FFE, 0x17F0, 0x03E0, 0x0000, 0x0000, 0x0000,
};
Texture bullseye = {
	0x07E0, 0x1FF8, 0x399C, 0x63C6, 0x6FF6, 0xCDB3, 0xD99B, 0xFFFF,
	0xFFFF, 0xD99B, 0xCDB3, 0x6FF6, 0x63C6, 0x399C, 0x1FF8, 0x07E0,
};
Texture boxcurs = {
	0x43FF, 0xE001, 0x7001, 0x3801, 0x1D01, 0x0F01, 0x8701, 0x8F01,
	0x8001, 0x8001, 0x8001, 0x8001, 0x8001, 0x8001, 0x8001, 0xFFFF,
};
Texture eightball = {
	0x07E0, 0x1FF8, 0x21FC, 0x4CFE,	0x52FE, 0xCCFF, 0xD2FF, 0xCCFF,
	0xE1FF, 0xFFFF, 0xFFFF, 0x7FFE,	0x7FFE, 0x3FFC, 0x1FF8, 0x07E0,
};

#define	INSET	3
Layer *whichlayer();
struct Proc *kbdproc=0;
#define	DEMUX	0
#define	CONTROL 1
#define	UP	0
#define	DOWN	1
int control(), windowproc(), demux();
struct Mouse mouse;
main(){
	register struct Proc *p;
	*DADDR=0;
	BonW();
	qinit();
	aciainit(baud_speeds[VALBAUD]);
	binit();
	kbdinit();
	cursinit();
	if(VALMAXADDR){	/* 1024Kb */
		NPROC=4;
		p=proctab+NPROC;
		allocinit(((char *)p), ((char *)p)+512*1024L);
	}else{		/*  256Kb */
		NPROC=7;
		p=proctab+NPROC;
		allocinit(((char *)p), ((char *)p)+25*1024L);
	}
	gcinit();
	bufinit();
	if(pinit(NPROC)==-1)
		error("pinit", "-1");
	Lgrey(&display);
	spl0();
	swinit();
	P=newproc(demux);		/* process 0 */
	setrun(newproc(control));	/* process 1 */
	/*
	 * sw(0) will switch out of our local stack onto the control stack.
	 * we will never return to this stack
	 */
	sw(0);
	/*NOTREACHED*/
}
buttons(updown)
{
	do; while((button123()!=0) != updown);
}
Sw()
{
	if(second){
		second=0;
		if(Ptflag)
			ptimeout();
	}
	sw(P!=&proctab[CONTROL]);	/* if control, clock will restart us */
}
control(){
	register Layer *lp;
	register struct Proc *p, *pp;
	for(;;){
		pp=0;
		lp=whichlayer();
		for(p=&proctab[CONTROL+1]; p<&proctab[NPROC]; p++){
			if(p->state&WAKEUP){
				p->state&=~WAKEUP;
				setrun(p);
			}
			if((p->state&KBDLOCAL)==0 && p->kbdqueue.c_cc>0
			   && p->text!=(char *)boot){
				register c=qpeekc(&p->kbdqueue);
				if(c==0x80){	/* BREAKKEY */
					if(muxkill(2, p)!=-1)
						(void)qgetc(&p->kbdqueue);
				}else if(muxsendchar(c, p)!=-1)
					(void)qgetc(&p->kbdqueue);
			}
			if(lp && p->layer==lp)
				pp=p;		/* pp pointed at by mouse */
		}
		if(usermouse && (pp!=kbdproc || (pp->state&GOTMOUSE)==0)){
			usermouse=0;
			cursswitch((Texture *)0);
			cursallow();
		}else if(!usermouse && pp){
	Check_mouse:	if(pp==kbdproc && (pp->state&GOTMOUSE)){
				usermouse=1;
				cursswitch(pp->cursor);
				if(pp->inhibited)
					cursinhibit();
			}
		}
		if(button123()){
			if(lp==0 || (pp->state&GOTMOUSE)==0){
				dobutton(whichbutton());
				/* make sure kbdproc doesn't think
				   buttons are down */
				if(kbdproc){
					givemouse(kbdproc);
					goto Check_mouse;	/* usermouse==0 */
				}
			}
			if(pp && pp->state&GOTMOUSE)
				givemouse(pp);
		}
		if(RCVQUEUE.c_cc)
			setrun(&proctab[DEMUX]);
		Sw();
	}
}
New(){
	newwindow(windowproc);
}
Psend(a, b, c, d)
	char *b;
{
	while(psend(a, b, c, d)==-1)
		Sw();
}
Delete(){
	delete(whichlayer());
}
delete(l)
	register Layer *l;
{
	register struct Proc *p;
	register w;
	if(l){
		w=whichproc(l);
		p= &proctab[w];
		muxmesg(w, C_DELETE);
		delproc(p);
		dellayer(l);
		if(kbdproc==p)
			kbdproc=0;
	}
}
delproc(p)
	register struct Proc *p;
{
	p->state=0;		/* exit(w) */
	p->nticks=0;
	p->inhibited=0;
	qclear(&p->kbdqueue);
	freemem(p);
	p->layer=0;		/* sigh */
}
Top(){
	upfront(whichlayer());
}
Bottom(){
	downback(whichlayer());
}
Current(){
	register Layer *l;
	l=whichlayer();
	if(l)
		tolayer(l);
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
Rectangle
getrectb(n)
	int n;
{
	Rectangle r;
	Point p1, p2;
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
	for(; mouse.buttons&n; nap(2)){
		outline(r);
		p2=mouse.xy;
		r=canon(p1, p2);
		outline(r);
	}
	outline(r);	/* undraw for the last time */
    Return:
	cursswitch((P->state&USER)? P->cursor : (Texture *)0);
	return r;
}
Rectangle
getrect(n)
{
	return getrectb(8>>n);
}
Reshape(){
	register Layer *l;
	register struct Proc *p;
	Rectangle r;
	Point save;	/*SFBOTCH*/
	l=whichlayer();
	if(l==0)
		return;
	p= &proctab[whichproc(l)];
	r=getrect3();
	if(r.corner.x-r.origin.x>100 && r.corner.y-r.origin.y>40){
		/*SFBOTCHPoint save;*/
		save=l->rect.origin;
		dellayer(l);
		p->state&=~MOVED;
		p->state|=RESHAPED;
		l=newlayer(r);
		if(l==0){
			r.origin=save;
			r.corner=add(save, Pt(100, 50));
			l=newlayer(r);
			if(l==0){	/* oh shit */
				delproc(p);
				muxmesg((int)(p-proctab), C_DELETE);
				return;
			}
		}
		p->layer=l;
		p->rect=inset(r, INSET);
		setborder(p);
	}
	if(p->state&USER)
		setdata(p);
	muxnewwind(p, C_RESHAPE);
}
Move(){
	register Layer *l, *nl;
	register struct Proc *procp;
	Point p, op, dp;
	l=whichlayer();
	if(l==0)
		return;
	procp= &proctab[whichproc(l)];
	dp=sub(l->rect.corner, l->rect.origin);
	cursset(l->rect.origin);
	cursswitch(&boxcurs);
	p=l->rect.origin;
	while(button3()){
		if(button12())
			goto Return;
		outline(Rpt(p, add(p, dp)));
		nap(2);
		op=p;
		p=mouse.xy;
		/* using boxcurs, can't get off top or left! */
		if(p.x+dp.x >= XMAX-9)
			p.x=XMAX-9-dp.x;
		if(p.y+dp.y >= YMAX-9)
			p.y=YMAX-9-dp.y;
		outline(Rpt(op, add(op, dp)));
		cursset(p);
	}
	cursswitch(&deadmouse);
	nl=newlayer(Rpt(p, add(p, dp)));
	if(nl==0)
		goto Return;
	Ubitblt(l, l->rect, nl, p, F_STORE);
	procp->layer=nl;
	procp->rect=inset(nl->rect, INSET);
	dellayer(l);
	if(procp->state&USER)
		setdata(procp);
	if((procp->state&RESHAPED) == 0)
		procp->state|=MOVED|RESHAPED;	/* turn on RESHAPED for old progs */
	l=nl;
	setborder(procp);
   Return:
	cursset(div(add(l->rect.origin, l->rect.corner), 2));
	cursswitch((Texture *)0);
	/* No C_RESHAPE required */
}

/* button hit to indicate which process, invoked by debugger */
struct Proc *
debug(){
	debugger=P;
}
struct Proc *
getproc(){
	register Layer *l;
	struct Proc *z=0;
	cursswitch(&bullseye);
	buttons(DOWN);
	if(button3() && (l=whichlayer()))
		z=&proctab[whichproc(l)];
	buttons(UP);
	return z;
}
struct Proc *
getproctab(){
	return proctab;
}
char *menutext[]={
	"New", "Reshape", "Move", "Top", "Bottom", "Current",
	"Delete",  0
};
int (*menufn[])()={
	New,	Reshape,Move,	Top,	Bottom,	Current,
	Delete,	0,
};
Menu windowmenu={ menutext };
dobutton(b)
{
	register hit;
	register Layer *l;
	switch(b){
	case 1:
		if(l=whichlayer()){
			upfront(l);
			tolayer(l);
		}
		break;
	case 2:
		break;	/* dunno... */
	case 3:
		if((hit=menuhit(&windowmenu, 3))>=0){
			if(hit==0)	/* a little different because of getrect */
				New();
			else{
				cursswitch(&bullseye);
				buttons(DOWN);
				if(button3())
					(*menufn[hit])();
				cursswitch((Texture *)0);
			}
		}
		break;
	default:
		break;
	}
	buttons(UP);
}
whichproc(l)
	register Layer *l;
{
	register struct Proc *p;
	for(p=proctab+CONTROL+1; p<proctab+NPROC; p++)
		if(p->layer==l && (p->state&BUSY))
			return((int)(p-proctab));
	return(CONTROL+1);	/* HELP?? */
}
whichbutton()
{
	static int which[]={0, 3, 2, 2, 1, 1, 2, 2, };
	return which[mouse.buttons&7];
}
newwindow(fn)
	int (*fn)();
{
	register struct Proc *p;
	Rectangle r;

	r=getrect3();
	cursswitch(&deadmouse);
	if(r.corner.x-r.origin.x>100 && r.corner.y-r.origin.y>40){
		if(p=newproc(fn)){	/* Assignment = */
			p->rect=inset(r, INSET);
			if(p->layer=newlayer(r)){
				muxnewwind(p, C_NEW);
				tolayer(p->layer);
				setrun(p);
			}else
				p->state=0;
		}
	}
	cursswitch((Texture *)0);
}

void
sendnchars(n, p)
	int n; char * p;
{
	register int	cc;

	do{
		if((cc=n)>MAXPKTDSIZE-1)
			cc=MAXPKTDSIZE-1;
		Psend((int)(P-proctab), p, cc, C_SENDNCHARS);
	}while(p+=cc, (n-=cc)>0);
}
void
sendwithdelim(n, p)
	int n; char * p;
{
	register int	cc;

	for(;;){
		if((cc=n)>MAXPKTDSIZE-1)
			cc=MAXPKTDSIZE-1;
		if ((n-=cc) <= 0)
			break;
		Psend((int)(P-proctab), p, cc, C_SENDNCHARS);
		p+=cc;
	}
	Psend((int)(P-proctab), p, cc, C_SENDNCHARD);
}
/*
void
sendwithdelim(n, p)
	int n; char * p;
{
	register int	cc;
	sendnchars(n, p);
	delim();
}
*/
delim(){
	Psend((int)(P-proctab), (char *)0, 0, C_DELIM);
}
short	sendbusy;

int
sendpkt(p, n)
	register char *	p;
	register int	n;
{
	register int	sr;

	static Rectangle r={0, 0, 50, 50};

	while(sendbusy)
		sw(1);
	sendbusy=1;
	while(OUTQUEUE.c_cc>CBSIZE/2)
		sw(1);
	if (*p & P_CNTL)
		OUTQUEUE.state |= QPRIORITY;
	do
		if (!qputc(&OUTQUEUE, *(unsigned char *)p++)) {
			rectf(&display, r, F_XOR);
			if ((r.origin.y += 50) >= 1000)
				r.origin.y = 0;
			r.corner.y = r.origin.y + 50;
		}
	while(--n);
	OUTQUEUE.state &= ~QPRIORITY;
	sendbusy=0;
	aciatrint();
	return 0;
}

muxnewwind(p, c)
	register struct Proc *p;
	char c;
{
	char	mesg[6];
	register int	dx, dy;
	register char *	cp = mesg;

	dx=p->rect.corner.x-p->rect.origin.x;
	dy=p->rect.corner.y-p->rect.origin.y;
	*cp++=(dx-6)/p->defaultfont->info['0'].width;
	*cp++=(dy-6)/p->defaultfont->height;
	*cp++=dx;
	*cp++=(dx>>8);
	*cp++=dy;
	*cp++=(dy>>8);
	Psend((int)(p-proctab), mesg, sizeof mesg, c);
}
int
muxsendchar(c, p)
	char c;
	struct Proc *p;
{
	if(sendbusy || (OUTQUEUE.c_cc >= (CBSIZE/2)))
		return -1;	/* avoid "sw" in "sendpkt" */
	return psend((int)(p-proctab), &c, 1, C_SENDCHAR);
}
int
muxkill(s, p)
	char s;
	struct Proc *p;
{
	Psend((int)(p-proctab), &s, 1, C_KILL);
}
muxmesg(w, m){
	Psend(w, (char *)0, 0, m);
}
muxublk(p)
	register struct Proc *p;
{
	register int l = p-proctab;

	while(pconvs[l].user > 0){
		pconvs[l].user--;
		Psend(l, (char *)0, 0, C_UNBLK);
	}
}
Point
jline(p, dp)
	Point p, dp;
{
	dp.x+=p.x;
	dp.y+=p.y;
	segment(&display, p, dp, F_XOR);
	return dp;
}
outline(r)
	Rectangle  r;
{
	register dx=r.corner.x-r.origin.x-1, dy=r.corner.y-r.origin.y-1;
	Point p;
	p=jline(r.origin, Pt(dx, 0));
	p=jline(p, Pt(0, dy));
	p=jline(p, Pt(-dx,0));
	(void)jline(p, Pt(0,-dy));
}
min(a, b){
	return(a<b? a : b);
}
max(a, b){
	return(a>b? a : b);
}
Layer *
whichlayer()
{
	register Layer *lp;
	for(lp=lfront; lp; lp=lp->back)
		if(ptinrect(mouse.xy, lp->rect))
			return(lp);
	return(0);
}
tolayer(l)
	register Layer *l;
{
	register struct Proc *p, *okbdproc;
	for(p=proctab; p<&proctab[NPROC]; p++)
		if((p->state&BUSY) && l==p->layer){
			if(kbdproc!=p){
				okbdproc=kbdproc;
				kbdproc=p;
				if(okbdproc){
					setborder(okbdproc);
					okbdproc->state&=~GOTMOUSE;
				}
				setborder(p);
			}
			if(p->state&MOUSELOCAL){
				p->state|=GOTMOUSE;
				setrun(p);
			}
			break;
		}
}
clear(r, inh)
	Rectangle r;
{
	if(inh)
		cursinhibit();
	lrectf(P->layer, r, F_CLR);
	if(inh)
		cursallow();
}
border(l, r, i, c)	/* no flashing! */
	register Layer *l;
	Rectangle r;
	register i;
	Code c;
{
	Urectf(l, Rect(r.origin.x, r.origin.y, r.corner.x, r.origin.y+i), c);
	Urectf(l, Rect(r.origin.x, r.corner.y-i, r.corner.x, r.corner.y), c);
	Urectf(l, Rect(r.origin.x, r.origin.y+i, r.origin.x+i, r.corner.y-i), c);
	Urectf(l, Rect(r.corner.x-i, r.origin.y+i, r.corner.x, r.corner.y-i), c);
}
setborder(p)
	register struct Proc *p;
{
	border(p->layer, p->layer->rect, INSET, F_OR);
	if(p!=kbdproc)
		border(p->layer, inset(p->layer->rect, 1), INSET-1, F_XOR);
}
cansend(channel)
{
	register i;
	register Pch_p pcp=&pconvs[channel];
	register Pks_p psp;
	for(psp=pcp->nextpkt,i=NPCBUFS; i--;){
		if(psp->state!=PX_WAIT)
			return SEND;
		if(++psp>=&pcp->pkts[NPCBUFS])
			psp=pcp->pkts;
	}
	return 0;
}
