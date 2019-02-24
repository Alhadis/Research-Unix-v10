#include <jerq.h>
#include <layer.h>
#include "jerqproc.h"
#include <font.h>
static inhibited;
Texture *Ucursswitch();
#define	DB		((Word *)0x700000)
#define	ONSCREEN(b)	(b->base>=DB && b->base<DB+XMAX/WORDSIZE*YMAX)

char *
Ualloc(n)
	unsigned n;
{
	extern char *realalloc();
	return realalloc(n, (char *)P);
}
Bitmap *
Uballoc(r)
	Rectangle r;
{
	extern Bitmap *realballoc();
	return realballoc(r, (char *)P);
}
Uexit(){
	extern windowproc();
	shutdown(P);
	exec(windowproc);
}
Urequest(r)
{
	extern struct Proc *kbdproc;
	register struct Proc *p=P;
	if(r&KBD)
		p->state|=KBDLOCAL;
	else
		p->state&=~KBDLOCAL;
	if(r&MOUSE){
		p->state|=MOUSELOCAL;
		if(kbdproc==p){
			p->state|=GOTMOUSE;
			if(p->state&GOTMOUSE)	/* if not, it's windowproc */
				givemouse(p);
		}
		sleep(1);	/* Let control() update the mouse */
	}else{
		Ucursallow();
		Ucursswitch((Texture *) 0);
		p->state&=~(GOTMOUSE|MOUSELOCAL);
	}
	if(r&ALARM)
		p->state|=ALARMREQD;
	else
		p->state&=~ALARMREQD;
}
Urcvchar(){
	if(P->nchars==0)
		return -1;
	return(getchar());
}
Ukbdchar(){
	if((P->state&KBDLOCAL)==0)
		return -1;
	return(qgetc(&P->kbdqueue));
}
Uown()
{
	register got=CPU|SEND;
	if(P->state&GOTMOUSE)
		got|=MOUSE;
	if(P->kbdqueue.c_cc>0)
		got|=KBD;
	if(P->nchars>0)
		got|=RCV;
	if(P->state&ALARMREQD && P->nticks== 0)
		got|=ALARM;
	return got;
}
Uwait(r)
	register r;
{
	register u;

	if((r&RCV) && P->nchars==0)
		muxublk(P);
	sw(1);
	if(r==0)
		return;	/* dumb person */
	spl1();
	while((u=Uown()&r)==0 && (r&CPU)==0){
		spl0();
		sw(0);
		spl1();
	}
	spl0();
	return u;
}
Ucursallow(){
	if(P->state&MOUSELOCAL){
		if(ptinrect(mouse.xy, P->layer->rect) && (P->state&GOTMOUSE))
			cursallow();
		P->inhibited=0;
	}
}
Ucursset(p)
	Point p;
{
	if(P->state&GOTMOUSE){
		cursset(p);
		givemouse(P);
	}
}
Ucursinhibit(){
	if(P->state&MOUSELOCAL){
		if(ptinrect(mouse.xy, P->layer->rect) && (P->state&GOTMOUSE))
			cursinhibit();
		P->inhibited=1;
	}
}
Texture *
Ucursswitch(t)
	register Texture *t;
{
	Texture *ot;
	ot=0;
	if(P->state&MOUSELOCAL){
		ot=P->cursor;
		if(ptinrect(mouse.xy, P->layer->rect) && (P->state&GOTMOUSE))
			cursswitch(t);
		P->cursor=t;
	}
	sleep(1);
	return ot;
}
Point
string(f,s,b,p,fc)
	register Font *f;
	unsigned char *s;
	register Layer *b;
	Point p;
	int fc;
{
	register c;
	int full = (fc == F_STORE);
	Point q;
	Rectangle r;
	register Fontchar *i;
	inhibited=0;
	if(fc!=F_XOR && ONSCREEN(b)){
		cursinhibit();
		inhibited=1;
	}
	if (full) {
		r.origin.y = 0;
		r.corner.y = f->height;
	}
	for (; c = *s++; p.x += i->width) {
		i = f->info + c;
		if (!full) {
			r.origin.y = i->top;
			r.corner.y = i->bottom;
		}
		r.origin.x = i->x;
		r.corner.x = (i+1)->x;
		q.x = p.x+i->left;
		q.y = p.y+r.origin.y;
		if (b->obs == 0)
			bitblt(f->bits,r,b,q,fc);
		else
			lblt(f->bits,r,b,q,fc);
	}
	if(inhibited)
		cursallow();
	return(p);
}
Usendchar(c)
	char c;
{
	while(muxsendchar(c, P) == -1)
		sw(1);
}
Upoint(l, p, f)
	register Layer *l;
	Point p;
{
	inhibited=0;
	if(f!=F_XOR && ONSCREEN(l)){
		cursinhibit();
		inhibited=1;
	}
	lpoint(l, p, f);
	if(inhibited)
		cursallow();
}
Ubitblt(s, r, d, p, f)
	Layer *s, *d;
	Rectangle r;
	Point p;
{
	inhibited=0;
	if((f!=F_XOR && ONSCREEN(d)) || ONSCREEN(s)){
		cursinhibit();
		inhibited=1;
	}
	lbitblt(s, r, d, p, f);
	if(inhibited)
		cursallow();
}
Urectf(l, r, f)
	Layer *l;
	Rectangle r;
{
	inhibited=0;
	if(f!=F_XOR && ONSCREEN(l)){
		cursinhibit();
		inhibited=1;
	}
	/* speed hack: rectf clips */
	if(l->obs)
		lrectf(l, r, f);
	else
		rectf(l, r, f);
	if(inhibited)
		cursallow();
}
Usegment(l, p, q, f)
	Layer *l;
	Point p, q;
{
	inhibited=0;
	if(f!=F_XOR && ONSCREEN(l)){
		cursinhibit();
		inhibited=1;
	}
	lsegment(l, p, q, f);
	if(inhibited)
		cursallow();
}
Utexture(l, r, t, f)
	Layer *l;
	Rectangle r;
	Texture *t;
{
	inhibited=0;
	if(f!=F_XOR && ONSCREEN(l)){
		cursinhibit();
		inhibited=1;
	}
	/* speed hack; texture clips anyway */
	if(l->obs)
		ltexture(l, r, t, f);
	else
		texture(l, r, t, f);
	if(inhibited)
		cursallow();
}
Uscreenswap(b, r, s)
	Bitmap *b;
	Rectangle r, s;
{
	cursinhibit();
	screenswap(b, r, s);
	cursallow();
}
struct Proc *
Unewproc(f)
	int (*f)();
{
	extern windowproc();
	if(f==0)
		f=windowproc;
	return newproc(f);
}
