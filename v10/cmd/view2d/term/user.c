#include <CC/jerq.h>
#include <CC/layer.h>
#include "jerqproc.h"
#include <CC/font.h>
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
ostring(f,s,b,p,fc)
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
		q.x = p.x+((i->left&0x80)? (i->left|0xffffff00):i->left);
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
#define CWID	9	/* wired in for defont */
Point
string(f,s,b,p,fc)	/* highly 5620 dependent! */
	Font *f;
	unsigned char *s;
	register Layer *b;
	Point p;
	int fc;
{
	register Word *src,*dst;	/* %r7, %r6 */
	register m,shift,i;		/* %r5, %r4, %r3 */
	static Bitmap *mrfont;	/* local font, guaranteed to not straddle source */
	char *cp,mrbuf[2];
	int dw;
	Fontchar *info = f->info;
	if (b->obs != 0 || f != &defont || fc != F_XOR
		|| p.x < b->rect.origin.x || p.y < b->rect.origin.y
		|| p.y+f->height > b->rect.corner.y)
			return(ostring(f,s,b,p,fc));
	i = p.x;
	cp = s;
	do i += CWID; while (*cp++);	/* *s==0 doesn't matter */
	if (i > b->rect.corner.x)
		return(ostring(f,s,b,p,fc));
	if (mrfont == 0) {	/* create Mr Font */
		mrbuf[1] = 0;
		mrfont = balloc(Rect(0,0,32,f->height*128));
		rectf(mrfont,mrfont->rect,F_CLR);
		for (i = 0; i < 128; i++) {
			mrbuf[0] = i;
			ostring(f,mrbuf,mrfont,Pt(0,i*f->height),F_XOR);
		}
	}
	dw = 4*b->width;
	for (; m = *s++;) {
		if (m&0x80)
			continue;
		i = info[m].top;
		dst = addr(b,Pt(p.x,p.y+i));
		src = mrfont->base + f->height*m + i;
		shift = p.x & 31;
		i = info[m].bottom - i;
		if (shift + CWID <= 32) {
			do {
asm("				ROTW	%r4,(%r7),%r5	");
				src++;
				*dst ^= m;
				(char *) dst += dw;
			} while (--i > 0);
		}
		else {
			((short *) dst)++;
			do {
asm("				ROTW	%r4,(%r7),%r5	");
				src++;
asm("				XORH2	%r5,(%r6)	");
asm("				ROTW	&16,%r5,%r5	");
asm("				XORH2	%r5,2(%r6)	");
				(char *) dst += dw;
			} while (--i > 0);
		}
		p.x += CWID;
	}
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
