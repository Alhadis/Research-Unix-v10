#include <jerq.h>
#include <layer.h>
#include <font.h>
#include <queue.h>
#include <jerqproc.h>
#include <setup.h>
#include "frame/frame.h"
#include "../msgs.h"

#define	ECHO	(tp->flags0&TECHO)
#define	RAW	(tp->flags0&TRAW)
#define	CBREAK	(tp->flags0&TCBREAK)
#define	CRMOD	(tp->flags0&TCRMOD)

/*
 * Keyboard-dependent things
 */
#ifdef	BLITKBD
#define	BREAKKEY	0xE0
#define	SCROLLKEY	0xB0	/* Why not NO SCRL? */
#undef	ringbell
#define ringbell()	DUART->b_data=0x02
#else
#define	BREAKKEY	0x80
#define	SCROLLKEY	0xB2	/* Why not NUM LOCK? */
#endif

#define	CTRLW		0x17
#define	NSCRL	2
int	HIWATER;	/* #chars max to save off top of screen */
int	LOWATER;
int doubleclickOK;

typedef struct Type{
	Frame	*frame;		/* Frame being typed at */
	short	start;		/* start pos. of text to be sent to host */
	short	nchars;		/* number of chars typed */
	String	old;		/* old text scrolled off top */
	char	scroll;
}Type;

short		newlnsz;
String		buf;
short		scrltimeout;
#define		M	3	/* margin around frame */

windowproc(){
	register Frame *f;
	register got;
	String rcvstr;
	static String zerostr;
	Type current;
	if((current.frame=f=fralloc(P->layer->rect, M))==0){
		mesg("can't allocate frame");
		for(;;)
			sleep(1000);
	}
	frinit(f);
	setborder(P);	/* frames always draw them black */
	muxmesg((int)(P-proctab), C_POPLD);
	current.start=0;
	current.nchars=0;
	current.scroll=0;
	current.old=zerostr;
	insure(&current.old, 128, P);
	rcvstr=zerostr;
	insure(&rcvstr, 128, P);
	Urequest(RCV|KBD|MOUSE);
	Ucursswitch((Texture *)0);
	setscroll(&current);
	for(;;){
		linecurse(f);	/* cursor is off, so turn it on */
		do{
			if(P->state&RESHAPED){
				frsetrects(f, P->layer->rect);
				drawframe(f);	/* sets complete */
				setborder(P);
				if(!complete && current.scroll)
					advance(&current);
				P->state&=~(RESHAPED|MOVED|BLOCKED);
				linecurse(f);	/* put it back */
			}
		}while((got=Uwait((P->state&BLOCKED)?
			KBD|MOUSE|ALARM : KBD|MOUSE|RCV|ALARM))==MOUSE
			&& button123()==0);
		linecurse(f);	/* turn it off */
		if(got&ALARM){
			setscroll(&current);
			P->state&=~ALARMREQD;
		}
		if(got&MOUSE && button123())
			wbuttons(&current);
		else if(got&KBD)
			kbd(&current, Ukbdchar());
		else if(got&RCV)
			rcv(&current, &rcvstr);
	}
}
bufinit(){
	newlnsz=defont.height;
	strzero(&buf);
	if(VALMAXADDR)
		HIWATER=10000, LOWATER=8000;
	else
		HIWATER=2500, LOWATER=1500;
}
getmuxbuf(s)
	register String *s;
{
	insure(s, buf.n, P);
	movstring(buf.n, buf.s, s->s);
	s->n=buf.n;
}
setmuxbuf(s)
	String *s;
{
	snarf(s, 0, s->n);
}
rcv(t, rcvstr)
	Type *t;
	register String *rcvstr;
{
	register char *p;
	register Frame *f=t->frame;
	register i, posn, c;
	int compl;
	/* Read the string */
	c=Urcvchar();
loop:
	while(c=='\b'){
		if(t->start > 0)
			deltype(t, t->start-1, t->start);
		c=Urcvchar();
	}
	p=rcvstr->s;
	for(i=0; c!=-1 && (c&=0x7F)!='\b'; c=Urcvchar()){	/*aplterm1*/
		if(c=='\7')
			ringbell();	/* smashes keyclick? */
		if(c && c<defont.n){
			*p++=c;
			if(++i==rcvstr->size)
				break;
		}
	}
	rcvstr->n=i;
	posn=t->start;
	if(f->s1<posn && posn<f->s2){
		t->start=posn=f->s1;	/* before selected text; avoids problems */
		t->nchars=0;
	}
	/* Undraw selection if necessary */
	if(posn<f->s2 && f->s2>f->s1)
		selectf(f, F_XOR);
	/* Find where it goes */
	instext(f, rcvstr, posn);
	compl=inscomplete;
	/* Adjust the selection and typing location */
	if(posn<f->s2 || (f->s1==f->s2 && posn==f->s2)){
		f->s1+=i;
		f->s2+=i;
	}
	t->start+=i;
	if(posn+i<f->s2 && f->s2>f->s1)
		selectf(f, F_XOR);
	if(!compl){
		if(t->scroll)
			advance(t);
		else
			P->state|=BLOCKED;	/* so cat hugefile is safe */
	}
	if(c=='\b')
		goto loop;
	alarm(scrltimeout);
}
linecurse(t)
	register Frame *t;
{
	Point p;
	if(t && t->str.s && t->s1==t->s2){
		p=ptofchar(t, t->s1);
		Rectf(Rpt(p, Pt(p.x+1, p.y+newlnsz)), F_XOR);
	}
}
mesg(s)
	char *s;
{
	string(&defont, s, P->layer, add(P->rect.origin, Pt(2, 2)), F_STORE);
}
kbd(t, ac)
	register Type *t;
	char ac;
{
	static int raw, echo, cbreak, crmod; /* can be static; we don't sleep here */
	register Frame *f=t->frame;
	register struct ttychars *tp=&P->ttychars;
	int tounix;
	unsigned char c;
	c=ac;	/* aplterm3 */
	if(c==SCROLLKEY){
		scrlf(t, charofpt(f,
		      Pt(f->rect.origin.x, f->rect.origin.y+newlnsz*f->nlines/2)));
		return;
	}
	if(f->s2>f->s1)
		cut(t, 1);
	if(f->s2>=t->start){
		tounix=TRUE;
		raw=RAW;
		cbreak=CBREAK;
		crmod=CRMOD;
		echo=ECHO;
	}else{
		tounix=FALSE;
		raw=FALSE;
		cbreak=FALSE;
		crmod=TRUE;
		echo=TRUE;
	}
	if(raw && c!=BREAKKEY){	/* break goes, even in raw mode */
		/* flush input */
		t->start+=t->nchars;
		t->nchars=0;
    Noecho:
		Usendchar(c);
		return;		/* no echo in raw mode */
	}
	if(c==BREAKKEY)
		c=tp->intrc;
	if(c==tp->intrc || c==tp->quitc){	/* always kill */
		/* flush input */
		t->start=f->str.n;
		t->nchars=0;
		muxkill(c==tp->intrc? 2 : 3, P);
		return;
	}
	/* never raw mode */
	if(!echo && t->nchars>0){	/* send off what's already there */
		sendsubstr(f->str.s, t->start, t->nchars-t->start);
		t->nchars=0;
		t->start=f->str.n;
	}
	c&=0x7F;	/* strip off sign bit from keypad */	/*aplterm2*/
	if(c==tp->eofc && !cbreak){
		if(tounix)
			goto Send;	/* throw away eofc */
		return;
	}
	if(c=='\r' && crmod)
		c='\n';
	if(!echo)
		goto Noecho;
	if(f->s2>0 && f->str.s[f->s2-1]=='\\' &&
	  (c==tp->erase || (c==tp->kill&&tounix) || c==CTRLW ||
	   c==tp->stopc || c==tp->startc)){
		deltype(t, f->s2-1, f->s2);	/* throw away \ */
		goto Ordinary;
	}
	if(c==tp->erase || c==CTRLW){
		if(f->s1 > (tounix? t->start : 0))
			deltype(t, f->s1-nback(f, c, tounix? t->start : 0), f->s1);
		return;
	}
	if(c==tp->kill && tounix){
		f->s2=f->str.n;
		inschar(t, '@');
		inschar(t, '\n');
		t->start=f->s2;
		t->nchars=0;
		return;
	}
	if(c==tp->stopc){
		P->state|=BLOCKED;
		return;
	}
	if(c==tp->startc){
		P->state&=~BLOCKED;
		return;
	}
    Ordinary:
	inschar(t, c);
	if(tounix && (P->state&BLOCKED))
		P->state&=~BLOCKED;
	if(tounix && f->s2>t->start && (c=='\n' || (cbreak && c=='\r'))){
    Send:
		sendsubstr(f->str.s, t->start, f->s2);
		t->nchars-=f->s2-t->start;
		t->start+=f->s2-t->start;
	}
}
nback(f, c, lim)
	Frame *f;
{
	register n=0, s1=f->s1;
	register char *s=f->str.s+s1;
	static char alphanl[]=
	    "\n0123456789_abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
#define	alphanum	alphanl+1
	if(s1 <= 0)
		return 0;
	if(c!=CTRLW || *--s=='\n')
		return 1;
	/* else it's ^W. first, get to an alphanumeric (or newline) */
	while(n<s1-lim && notin(*s, alphanl))
		--s, ++n;
	/* *s is alphanumeric or space; now back up to non-alphanumeric */
	while(n<s1 && !notin(*s, alphanum))
		--s, ++n;
	return n;
}
notin(c, s)
	register c;
	register char *s;
{
	c&=0xFF;
	while(*s)
		if(c == *s++)
			return FALSE;
	return TRUE;
}
inschar(t, ac)
	register Type *t;
{
	static char c;
	static String kbdstr={
		&c,
		1,
		1
	};
	register Frame *f=t->frame;
	c=ac;
	instype(t, &kbdstr, f->s2);
	if(!inscomplete)
		scrlf(t, getto(f, f->s2));
	setsel(f, f->s2+1);
}
getto(f, end)
	register Frame *f;
{
	register nl, nc, i;
	nl=max(NSCRL, f->nlines/5);
	nc=charofpt(f, f->rect.corner);
	while(nc<end)
		if(f->str.s[nc++]=='\n')
			nl++;
	/* nl is now #lines; find how many chars from start of frame */
	for(i=nc=0; i<end; )
		if(f->str.s[i++]=='\n'){
			nc=i;
			if(--nl<=0)
				return nc;
		}
	return end;
}
advance(t)
	Type *t;
{
	/* should work first time unless lines are folded */
	do; while(!scrlf(t, getto(t->frame, t->frame->str.n)));
}
insertstring(p, i, s, n)	/* warning: must call insure() before this! */
	String *p;
	char *s;
{
	String str;
	str.s=s, str.n=n;
	insstring(p, i, &str);
}
inserttype(t, s, n)		/* warning: must call insure() before this! */
	Type *t;
	char *s;
{
	register Frame *f=t->frame;
	String str;
	str.s=s, str.n=n;
	selectf(f, F_XOR);
	instext(f, &str, 0);
	f->s1+=n;
	f->s2+=n;
	t->start+=n;
	selectf(f, F_XOR);
}
scrlf(t, n)
	register Type *t;
	register n;
{
	register Frame *f=t->frame;
	if(n>0){
		if(n>f->str.n)
			n=f->str.n;
		sendoldtext(t, n);
		/* quick hack; don't worry about order of allocation */
		insure(&t->old, t->old.n+n, P);
		insertstring(&t->old, t->old.n, f->str.s, n);
		if(t->old.n>HIWATER)
			delstring(&t->old, 0, t->old.n-LOWATER);
		if(n=deltype(t, 0, n))	/* all text in frame now visible */
			P->state&=~BLOCKED;
	}
	setscroll(t);
	return n;
}
scrlb(t, n)
	register Type *t;
	register n;
{
	register Frame *f=t->frame;
	register i;
	if(n>0){
		if(n>t->old.n)
			n=t->old.n;
		i=t->old.n-n;
		while(i>0 && t->old.s[i-1]!='\n')
			i--, n++;
		insure(&t->old, t->old.n+n, P);
		inserttype(t, t->old.s+i, n);
		delstring(&t->old, i, t->old.n);
	}
	setscroll(t);
}
deltype(t, s1, s2)
	register Type *t;
	register s1, s2;
{
	register Frame *f=t->frame;
	int compl;
	if(s2<=t->start)
		t->start-=s2-s1;
	else if(s1>=t->start)
		t->nchars-=s2-s1;
	else if(s2>t->start){	/* deletion overlaps start */
		t->nchars-=s2-t->start;
		t->start=s1;
	}
	selectf(f, F_XOR);
	deltext(f, s1, s2);
	compl=complete;
	f->s1-=max(0, min(f->s1, s2)-s1);
	f->s2-=max(0, min(f->s2, s2)-s1);
	selectf(f, F_XOR);
	return compl;
}
instype(t, s, s1)
	register Type *t;
	String *s;
	register s1;
{
	if(s->n>0){
		if(s1<t->start)
			t->start+=s->n;
		else if(s1>=t->start)
			t->nchars+=s->n;
		instext(t->frame, s, s1);
	}
}

#define	CUT		0
#define	PASTE		1
#define	SNARF		2
#define	SENDIT		3
#define	SCROLL		4

#define	UP		0
#define	DOWN		1

static char *editstrs[]={
	"cut",
	"paste",
	"snarf",
	"send",
	0,
	0,
};
static	Menu	editmenu={editstrs};

wbuttons(t)
	register Type *t;
{
	register Frame *f=t->frame;
	static char *scrollstrs[]={ "scroll", "noscroll" };
	if(!ptinrect(mouse.xy, f->totalrect)){	/* not for us anyway */
		doubleclickOK=0;
		return;
	}
	if(ptinrect(mouse.xy, f->scrollrect))
		scroll(t, mouse.buttons);
	else if(button1()){
		if(ptinrect(mouse.xy, f->rect))
			frselect(f, mouse.xy);
	}else if(button2()){
		doubleclickOK=0;
		editstrs[SCROLL]=scrollstrs[t->scroll];
		switch(menuhit(&editmenu, 2)){
		case CUT:
			cut(t, 1);
			break;
		case PASTE:
			paste(t, &buf);
			break;
		case SNARF:
			if(f->s2>f->s1)
				snarf(&f->str, f->s1, f->s2);
			break;
		case SENDIT:
			send(t);
			break;
		case SCROLL:
			if((t->scroll^=1) &&
			    (frameop(f, opnull, f->rect.origin,
			      f->str.s, f->str.n), !complete))
				advance(t);
			break;
		}
	}else{
		doubleclickOK=0;
		Urequest(RCV|KBD);
		sleep(2);
		Urequest(RCV|KBD|MOUSE);
	}
}
snarf(p, i, j)
	register String *p;
	register short i, j;
{
	register n = j-i;
	insure(&buf, n, (struct Proc *)0);
	movstring(n, p->s+i, buf.s);
	buf.n = n;
}
cut(t, save)
	register Type *t;
{
	register n;
	register Frame *f=t->frame;
	if((n=f->s1) != f->s2){
		if(save)
			snarf(&f->str, n, f->s2);
		deltype(t, f->s1, f->s2);
		f->s1=f->s2;
		setsel(f, n);
		P->state&=~BLOCKED;	/* in case we are suspended */
	}
}
paste(t, s)
	register Type *t;
	String *s;
{
	register Frame *f=t->frame;
	if(s->n==0)
		return;
	cut(t, 0);
	instype(t, s, f->s1);
	f->s2=f->s1+s->n;
	selectf(f, F_XOR);
}
send(t)
	register Type *t;
{
	register Frame *f=t->frame;
	if(f->s1!=f->s2)
		snarf(&f->str, f->s1, f->s2);
	if(buf.n==0)
		return;
	if(t->nchars)
		deltype(t, t->start, t->start+t->nchars);
	if(P->ttychars.flags0&TECHO){
		instext(f, &buf, f->str.n);
		if(!inscomplete)
			advance(t);
	}
	selectf(f, F_XOR);
	f->s1=f->s2=f->str.n;
	sendsubstr(buf.s, 0, buf.n);
	if(buf.s[buf.n-1]!='\n'){
		if((P->ttychars.flags0&(TRAW|TCBREAK|TECHO))==TECHO){
			sendnchars(1, "\n");
			inschar(t, '\n');
		}
		delim();
	}
	/* else the delim's already been sent */
	t->start=f->s2;
	t->nchars=0;
}
sendsubstr(s, beg, end)
	register char *s;
{
	register m, n;
	if(beg==end){
		delim();
		return;
	}
	for(m=beg; m<end; m=n+1){
		/* invariant: n is the last character we are going to send */
		for(n=m; n<end && s[n]!='\n'; n++)
			if(n==end-1)
				break;
		sendnchars(n-m+1, s+m);
		if(n<end && s[n]=='\n')
			delim();
	}
}
sendoldtext(t, n)
	register Type *t;
{
	if(t->start<n && t->nchars>0){
		if(n>t->start+t->nchars)
			n=t->start+t->nchars;
		sendsubstr(t->frame->str.s, t->start, n);
	}
}
Texture grey = {
	0xAAAA, 0xAAAA, 0xAAAA, 0xAAAA, 0xAAAA, 0xAAAA, 0xAAAA, 0xAAAA,
	0xAAAA, 0xAAAA, 0xAAAA, 0xAAAA, 0xAAAA, 0xAAAA, 0xAAAA, 0xAAAA,
};
Point
scrollclip(p)
	Point p;
{
	if(p.x<0){
		p.y-=p.x;
		p.x=0;
	}
	if(p.y>SCROLLRANGE)
		p.y=SCROLLRANGE;
	return p;
}
Rectangle
scrollmark(f, p)
	register Frame *f;
	Point p;
{
	Rectangle r;
	p=scrollclip(p);
	r=f->scrollrect;
	r.origin.y=clixtopix(f, p.x);
	r.corner.y=clixtopix(f, p.y);
	return r;
}
clixtopix(f, y)
	register Frame *f;
{
	return f->scrollrect.origin.y+muldiv(f->nlines*newlnsz, y, SCROLLRANGE);
}
pixtoclix(f, y)
	register Frame *f;
{
	return muldiv(y-f->rect.origin.y, SCROLLRANGE, f->nlines*newlnsz);
}
drawscrollbar(f)
	register Frame *f;
{
	Rectangle r;
	register Bitmap *b;	/* offscreen to avoid flicker */
	r=inset(f->scrollrect, -1);
	b=balloc(r);
	if(b==0)
		b=(Bitmap *)D;
	Urectf(b, r, F_OR);
	lrectf(b, scrollmark(f, f->scroll), F_XOR);
	Ubitblt(b, r, D, r.origin, F_STORE);
	if(b!=(Bitmap *)D)
		bfree(b);
}
setscroll(t)
	register Type *t;
{
	register Frame *f=t->frame;
	Point new;
	register x, y;
	x=f->rect.origin.y+1;	/* a little margin at the top */
	y=f->rect.corner.y-1;	/* and bottom */
	if(t->old.n+f->str.n>0)
		x+=muldiv(f->nlines*newlnsz, t->old.n, t->old.n+f->str.n);
	if(f->cpl[f->nlines-1]>0){
		int n;
		n=charofpt(f, Pt(XMAX, YMAX));
		y-=muldiv(f->nlines*newlnsz, f->str.n-n, t->old.n+f->str.n);
	}
	if(x>y-(f->rect.corner.y-f->rect.origin.y)/30)
		x=y-(f->rect.corner.y-f->rect.origin.y)/30;
	if(y<x+(f->rect.corner.y-f->rect.origin.y)/30)
		y=x+(f->rect.corner.y-f->rect.origin.y)/30;
	new.x=pixtoclix(f, x);
	new.y=pixtoclix(f, y);
	if(abs(f->scroll.x-new.x)>20 || abs(f->scroll.y-new.y)>20){
		f->scroll=new;
		drawscrollbar(f);
	}
}
Point
checkmouse(f, mousep, p)
	Frame *f;
	Point mousep, p;
{
	if(!ptinrect(mousep, f->scrollrect)){
		extern Rectangle Null;
		return Null.origin;
	}
	return p;
}
Point
but1func(f, p)
	register Frame *f;
	Point p;
{
	register delta=muldiv(p.y-f->rect.origin.y, f->scroll.y-f->scroll.x, f->nlines*newlnsz);
	return checkmouse(f, p, sub(f->scroll, Pt(delta, delta)));
}
Point
but2func(f, p)
	register Frame *f;
	Point p;
{
	Point scroll;
	register size=(f->scroll.y-f->scroll.x)/2;
	scroll.x=pixtoclix(f, p.y)-size;
	scroll.y=pixtoclix(f, p.y)+size;
	return checkmouse(f, p, scroll);
}
Point
but3func(f, p)
	register Frame *f;
	Point p;
{
	register delta=muldiv(p.y-f->rect.origin.y, f->scroll.y-f->scroll.x, f->nlines*newlnsz);
	return checkmouse(f, p, add(f->scroll, Pt(delta, delta)));
}
typedef Point (*ptrfpoint)();
ptrfpoint butfunc[]={
	0,
	but1func,
	but2func,
	but3func,
};
Point
guide(m, p)
	Point m, p;
{
	if(abs(m.x-p.x)<3)
		m.x=p.x;
	return m;
}
scrollbar(f, but)
	register Frame *f;
{
	ptrfpoint fp;
	Point pt;
	fp=butfunc[but];
	pt=mouse.xy;
	while(button(but)){
		ltexture(D, scrollmark(f, (*fp)(f, pt)), &grey, F_XOR);
		do nap(3); while(eqpt(mouse.xy, pt) && button(but));
		ltexture(D, scrollmark(f, (*fp)(f, pt)), &grey, F_XOR);
		cursset(pt=guide(mouse.xy, pt));
	}
	if(ptinrect(pt, f->scrollrect)){
		f->scroll=(*fp)(f, pt);
		if(f->scroll.x<0){
			f->scroll.y-=f->scroll.x;
			f->scroll.x=0;
		}
		if(f->scroll.x>SCROLLRANGE){
			f->scroll.y-=f->scroll.x-SCROLLRANGE;
			f->scroll.x=SCROLLRANGE;
		}
		return 1;
	}
	return 0;
}
scroll(t, but)
	register Type *t;
{
	register Frame *f=t->frame;
	Point old, new;
	register y, b;
	old=f->scroll;
	if(scrollbar(f, b=whichbutton())){
		new=f->scroll;
		f->scroll=old;	/* ugh */
		y=mouse.xy.y;
		if(b==2){
			y=muldiv(f->str.n+t->old.n, new.x, SCROLLRANGE);
			if(y>=t->old.n){
				y-=t->old.n;
				do; while(y<f->str.n && f->str.s[y++]!='\n');
				scrlf(t, y);
			}else
				scrlb(t, t->old.n-y);
		}else if(new.x>=old.x)
				scrlf(t, charofpt(f, Pt(f->rect.origin.x, y)));
		else
			scrlb(t, oldlinepos(t, (y-f->scrollrect.origin.y)/newlnsz));
	}
}
oldlinepos(t, n)
	register Type *t;
	register n;
{
	register i=t->old.n;
	while(n>0 && i>0)
		if(t->old.s[--i]=='\n')
			--n;
	return t->old.n-i;
}
