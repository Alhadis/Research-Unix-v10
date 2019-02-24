#include "samterm.h"

Text	cmd;
uchar	*scratch;
long	nscralloc;
long	typeaddr=-1;
Texture	*cursor;
Flayer	*which=0;
Flayer	*work=0;
long	snarflen;
char	lock=1;		/* goes to 2 soon */
main()
{
	register got, scr;
	register Text *t;
	Rectangle r;
	register Flayer *nwhich;
	GCALLOC(100, &scratch);
	nscralloc=100;
	request(KBD|MOUSE|RCV);
	for(; wait(MOUSE) && !getr(&r); sleep(60))
		;
	setlock();
	flstart(Drect);
	rinit(&cmd.rasp);
	flnew(&cmd.l[0], gettext, 1, (char *)&cmd);
	flinit(&cmd.l[0], r, &defont);
	cmd.nwin=1;
	which=&cmd.l[0];
	cmd.tag=Untagged;
	startnewfile(Tstartcmdfile, &cmd);
	P->state&=~RESHAPED;
	for(;;got=wait(KBD|MOUSE|RCV)){
		if(lock<2 && (P->state&RESHAPED))
			reshape();
		if(got&RCV)
			rcv();
		if(which && (got&KBD))
			type(which);
		if((got&MOUSE) && lock<2){
			if(!ptinrect(mouse.xy, Drect))
				continue;
			nwhich=flwhich(mouse.xy);
			scr=which && ptinrect(mouse.xy, which->scroll);
			if(button1()){
				typeaddr=-1;
				if(nwhich){
					if(nwhich!=which)
						current(nwhich);
					else if(scr)
						scroll(which, 1);
					else{
						t=(Text *)which->user1;
						if(flselect(which)){
							outTsl(Tdclick, t->tag, which->p0);
							t->lock++;
						}else if(t!=&cmd)
							outcmd();
					}
				}
			}else if(button2() && which){
				typeaddr=-1;
				if(scr)
					scroll(which, 2);
				else
					menu2hit();
			}else if(button3()){
				typeaddr=-1;
				if(scr)
					scroll(which, 3);
				else
					menu3hit();
			}
		}
	}
}
reshape(){
	register i;
	flreshape(Drect);
	for(i=0; i<nname; i++)
		if(text[i])
			hcheck(text[i]->tag);
	P->state&=~(MOVED|RESHAPED);
}
current(nw)
	register Flayer *nw;
{
	register Text *t;
	if(which)
		flborder(which, 0);
	if(nw){
		typeaddr=-1;
		cursswitch(&deadmouse);
		flupfront(nw);
		flborder(nw, 1);
		buttons(Up);
		cursswitch(cursor);
		t=(Text *)nw->user1;
		t->front=nw-&t->l[0];
		if(t!=&cmd)
			work=nw;
	}
	which=nw;
}
close(l)
	register Flayer *l;
{
	register Text *t=(Text *)l->user1;
	register m;
	m=whichmenu(t->tag);
	if(m<0)
		return;
	cursswitch(&deadmouse);
	flclose(l);
	cursswitch(cursor);
	if(l==which){
		which=0;
		current(flwhich(Pt(0, 0)));
	}
	if(l==work)
		work=0;
	if(--t->nwin==0){
		rclear(&t->rasp);
		free((char *)t);
		text[m]=0;
	}else if(l==&t->l[t->front]){
		for(m=0; m<NL; m++)	/* find one; any one will do */
			if(t->l[m].textfn){
				t->front=m;
				return;
			}
		panic("close");
	}
}
Flayer *
findl(t)
	register Text *t;
{
	register i;
	for(i=0; i<NL; i++)
		if(t->l[i].textfn==0)
			return &t->l[i];
	return 0;
}
duplicate(l, r, f, close)
	register Flayer *l;
	Rectangle r;
	Font *f;
{
	register Text *t=(Text *)l->user1;
	register Flayer *nl=findl(t);
	if(nl){
		cursswitch(&deadmouse);
		flnew(nl, gettext, l->user0, (char *)t);
		flinit(nl, r, f);
		nl->origin=l->origin;
		flinsert(nl, (*l->textfn)(l, (long)l->f.nchars), l->origin);
		flsetselect(nl, l->p0, l->p1);
		if(close){
			flclose(l);
			if(l==which)
				which=0;
		}else
			t->nwin++;
		current(nl);
		hcheck(t->tag);
	}
	cursswitch(cursor);
}
buttons(updown)
{
	while((button123()!=0) != updown)
		wait(MOUSE);
}
getr(rp)
	Rectangle *rp;
{
	Point p;
	Rectangle r;
	*rp=getrect3();
	if(rp->corner.x && rp->corner.x-rp->origin.x<=5 && rp->corner.y-rp->origin.y<=5){
		p=rp->origin;
		r=cmd.l[cmd.front].entire;
		*rp=Drect;
		if(cmd.nwin==1){
			if (p.y <= r.origin.y)
				rp->corner.y = r.origin.y;
			else if (p.y >= r.corner.y)
				rp->origin.y = r.corner.y;
			if (p.x <= r.origin.x)
				rp->corner.x = r.origin.x;
			else if (p.x >= r.corner.x)
				rp->origin.x = r.corner.x;
		}
	}
	return rectclip(rp, Drect) &&
	   rp->corner.x-rp->origin.x>100 && rp->corner.y-rp->origin.y>40;
}
snarf(t, w)
	register Text *t;
{
	register Flayer *l=&t->l[w];
	if(l->p1>l->p0){
		snarflen=l->p1-l->p0;
		outTsll(Tsnarf, t->tag, l->p0, l->p1);
	}
}
cut(t, w, save)
	Text *t;
{
	register long p0, p1;
	if((p0=t->l[w].p0)==(p1=t->l[w].p1))
		return;
	if(p0<0)
		panic("cut");
	if(save)
		snarf(t, w);
	outTsll(Tcut, t->tag, p0, p1);
	t->lock++;
	flsetselect(&t->l[w], p0, p0);
	hcut(t->tag, p0, p1-p0);
	hcheck(t->tag);
}
paste(t, w)
	Text *t;
{
	long p0=t->l[w].p0;
	if(snarflen){
		cut(t, w, 0);
		t->lock++;
		outTsl(Tpaste, t->tag, p0);
	}
}
strlen(s)
	register uchar *s;
{
	register n=0;
	while(*s++)
		n++;
	return n;
}
alnum(c)
	register c;
{
	return ('0'<=c && c<='9') || (c=='_') ||
	       ('a'<=c && c<='z') || ('A'<=c && c<='Z')  ||
/*	       ('À'<=c && c<='ÿ');  many have trouble with latin-1*/
	       (0300<=c && c<=0377);
}
raspc(r, p)
	register Rasp *r;
	register long p;
{
	rload(r, p, p+1);
	return scratch[0];	/* will be 0 if p is in a hole */
}
long
ctlw(r, o, p)
	register Rasp *r;
	register long o, p;
{
	register c;
	if(--p<o)
		return o;
	if(raspc(r, p)=='\n')
		return p;
	for(; p>=o && !alnum(c=raspc(r, p)); --p)
		if(c=='\n')
			return p+1;
	for(; p>o && alnum(raspc(r, p-1)); --p)
		;
	return p>=o? p : o;
}
#define	SCROLLKEY	0x8F	/* B2 on 5620; 8F on latin1 5620 */
#define	ESC		0x1B
type(l)	/* what a bloody mess this is */
	register Flayer *l;
{
	Text *t=(Text *)l->user1;
	uchar buf[100];
	register uchar *p=buf;
	register c, backspacing=0;
	register long a;
	int scrollkey=qpeekc(&P->kbdqueue)==SCROLLKEY;	/* ICK */
	if(lock || t->lock)
		return;
	if((a=l->p0)!=l->p1 && !scrollkey){
		cut(t, t->front, 1);
		return;	/* it may now be locked */
	}
	while(!backspacing && (c=kbdchar())>0 && c!=SCROLLKEY && c!=ESC)
		switch(c/*&=0x7F*/){
		case '\027':
		case 0x7f:
		case '\b':
			backspacing=1;
			break;
		case '\r':
			c='\n';
		default:
			*p++=c;
			break;
		}
	if(p>buf){
		if(typeaddr<0)
			typeaddr=a;
		*p=0;
		hgrow(t->tag, a, (long)(p-buf), 0);
		t->lock++;	/* pretend we Trequest'ed */
		hdata(t->tag, a, buf, (int)(p-buf));
		if(t==&cmd && a+(p-buf)==t->rasp.nbytes && p[-1]=='\n'){
			setlock();
			outcmd();
			outTslS(Ttype, t->tag, a, buf);
				typeaddr=-1;
		}else
			outTslS(Ttype, t->tag, a, buf);
		l->p0=l->p1=(a+=p-buf);
		center(l, a);
	}
	if(c==SCROLLKEY)
		center(l, l->origin+l->f.nchars+1);
		/* backspacing immediately after outcmd(): sorry */
	else if(!lock && backspacing){
		if(l->f.p0>0 && a>0){
			l->p1=a;
			l->p0=(c=='\b'||c==0x7f)? a-1 : ctlw(&t->rasp, l->origin, a);
			cut(t, t->front, 0);
			if(typeaddr>=l->p0)
				typeaddr=l->p0;
		}
	}else{
		if(c==ESC && typeaddr>=0){
			l->p0=typeaddr, l->p1=a;
			typeaddr=-1;
		}
		for(l=t->l; l<&t->l[NL]; l++)
			if(l->textfn)
				flsetselect(l, l->p0, l->p1);
	}
}
center(l, a)
	register Flayer *l;
	register long a;
{
	register Text *t=(Text *)l->user1;
	if(!t->lock && (a<l->origin || l->origin+l->f.nchars<a)){
		if(a>t->rasp.nbytes)
			a=t->rasp.nbytes;
		outTsll(Torigin, t->tag, a, 2L);
		outTs(Tunlockfile, t->tag);
		t->lock++;
		return 1;
	}
	return 0;
}
outcmd(){
	if(work)
		outTsll(Tworkfile, ((Text *)work->user1)->tag, work->p0, work->p1);
}
panic(s)
	char *s;
{
	rectf(D, Drect, F_XOR);
	string(&defont, s, D, add(Drect.origin, Pt(4, 4)), F_STORE);
	P->state|=1024;	/* ZOMBIE */
	request(0);
}
strgrow(s, n, want)	/* can always toss the old data when called */
	uchar **s;
	long *n;
{
	if(*n>=want)
		return;
	gcfree(*s);
	GCALLOC(want, s);
	*n=want;
}
uchar **
gettext(l, n)
	register Flayer *l;
	long n;
{
	register Text *t=(Text *)l->user1;
	rload(&t->rasp, l->origin, l->origin+n);
	return &scratch;
}
long
scrtotal(l)
	Flayer *l;
{
	return ((Text *)l->user1)->rasp.nbytes;
}
uchar *
ALLOC(n)
{
	register uchar *p=(uchar *)alloc(n);
	if(p==0)
		panic("ALLOC");
	return p;
}
GCALLOC(n, w)
	uchar **w;
{
	register i, j;
	register Text *t;
	register Flayer *l;
	if(gcalloc((unsigned long)n, (char **)w)==0){
		for(i=0; i<nname; i++)
			if(t=text[i]){
				/*raspcleanup(t); - uncache worthless stuff*/
				/*rclean(&t->rasp); (Can we do this if !t->lock?) */
				for(j=0,l=&t->l[0]; j<NL; j++,l++)
					if(l->textfn && l->visible==Some && l->f.b){
						bfree(l->f.b);
						l->f.b=0;
					}
			}
		if(gcalloc((unsigned long)n, w)==0)
			panic("GCALLOC");
	}
}
