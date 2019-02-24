#include <jerq.h>
#include <jerqio.h>
#include <font.h>
#include "twid.h"
Menu cmdmenu={ cmdlist };
Menu stylemenu={ stylelist };
Menu copymenu={ copylist };
Menu unixmenu={ unixlist };
Menu brushmenu={ brushlist };
Menu txtmenu={ txtlist };
Menu butmenu={ butlist };
Menu codemenu={ codelist };

Bitmap *brushes[NBRSH];
Bitmap *brush;
Bitmap *notbrush;
Bitmap *scratch;
int	curbrush=BPOINT;
int	mode=INKING;
extern Bitmap *getbitmap();

Texture black, grey, checks, stip, pointcur, smallcur, medcur;
Texture curvecur, disccur, typing, hourglass, menucurs, boxcurs, sunset;
Texture *txt[1+NTXT]={ 0, &black, &grey, &checks, &stip };
int ntxt=TSTIPPLE+1;
int nbrsh=BBIG+1;
Texture *curtxt;
Texture *cursor[]={ 0, &pointcur, &smallcur, &medcur, &black };
Texture *curcur;
Rectangle screenrect;	/* drawing area */
Rectangle msgrect;	/* typing and messages */
main()
{
	screenrect=Drect;
	if(screenrect.origin.x&WORDMASK){	/* textures must align */
		screenrect.origin.x|=WORDMASK;
		screenrect.origin.x++;
	}
	screenrect.corner.y-=defont.height;
	msgrect=Drect;
	msgrect.origin.y=screenrect.corner.y;
	request(MOUSE);
	initbrushes();
	settxt(TBLACK);
	select(stylelist, INK);
	butmf(0);	/* Start with OR, CLR */
	wait(MOUSE);
	Cursswitch(curcur= &black, curbrush=BBIG);	/* curbrush is never 0! */
	for(;;){
		do wait(MOUSE); while(!ptinrect(mouse.xy, display.rect));
		updatecurs();
		if(button1())
			draw(onecode);
		if(button2())
			draw(twocode);
		if(button3())
			menu();
	}
}
msg(s)
	char *s;
{
	rectf(&display, msgrect, F_CLR);
	string(&defont, s, &display, msgrect.origin, F_XOR);
}
char *
type(){
	static char typeline[64];
	register char *s=typeline;
	request(MOUSE|KBD);
	Cursswitch(&typing, 0);
	for(;;){
		wait(KBD);
		switch(*s++=kbdchar()){
		case '\r':
			--s;
			goto out;
		case '\b':
			s-=2;
			if(s<typeline)
				s=typeline;
			break;
		case '@':
			s=typeline;
		}
		*s=0;
		msg(typeline);
	}
out:	*s=0;
	request(MOUSE);
	return s>typeline? typeline : (char *)0;
}
char *
GCAlloc(n, p)
	int n;
	char **p;
{
	while(gcalloc((unsigned long)n, p)==0)
		timeout();
}
timeout(){
	msg("out of memory; clean up bitmaps and hit a button");
	for(;;){
		sleep(30);
		wait(MOUSE);
		if(button123())
			break;
	}
	msg("trying again");
	buttons(UP);
}
int stylemf(), txtmf(), brshmf(), butmf(), copymf(), unixmf();
struct menu{
	Menu	*menu;
	int	(*fnc)();
}menus[]={
	&stylemenu,	stylemf,
	&txtmenu,	txtmf,
	&brushmenu,	brshmf,
	&butmenu,	butmf,
	&copymenu,	copymf,
	&unixmenu,	unixmf,
};
menu(){
	int m, b;
	Cursswitch((Texture *)0, 0);
	m=menuhit(&cmdmenu, 3);
	if(m>=0){
		Cursswitch(&menucurs, 0);
		buttons(DOWN);
		if(button3()){
			Cursswitch((Texture *) 0, 0);
			b=menuhit(menus[m].menu, 3);
			if(b>=0)
				(*menus[m].fnc)(b);
		}else
			buttons(UP);
	}
	Cursswitch(curcur, (mode<=POINTING)? curbrush : 0);
}
stylemf(b){
	switch(b){
	case INK:
		mode=INKING;
		curcur=cursor[curbrush];
		goto sel;
	case POINT:
		mode=POINTING;
		curcur=cursor[curbrush];
		goto sel;
	case LINE:
		curcur=0;
		mode=LINING;
		goto sel;
	case CURVE:
		curcur= &curvecur;
		mode=CURVING;
		goto sel;
	case DISC:
		curcur= &disccur;
		mode=DISCING;
 sel:		select(stylelist, b);
		break;
	}
}
brshmf(b){
	if(b==NEW)
		b=newbrsh();
	if(b>NEW)
		selectbrush(b);
}
txtmf(t){
	if(t==NEW)
		t=newtxt();
	if(t>NEW)
		settxt(t);
}
butmf(c){
	onecode=codes[c][0];
	twocode=codes[c][1];
	select(butlist, c);
}
copymf(c){
	register m;
	Rectangle r;
	Point p, q;
	register Bitmap *b;
	register char *s;
	switch(c){
	case COPY:
		Cursswitch(&menucurs, 0);
		buttons(DOWN);
		Cursswitch((Texture *)0, 0);
		if(   button3() && (m = menuhit(&codemenu,3)) != -1
		   && userrect(&r)) {
			b = Balloc(r);
			Cursinhibit();
			bitblt(&display,r,b,r.origin,F_STORE);
			if (m == F_MOVE)
				rectf(&display,r,F_CLR);
			p = sub(r.origin,mouse.xy);
			while (!button3()) {
				q = add(p,mouse.xy);
				bitblt(b,r,&display,q,F_XOR);
				bitblt(b,r,&display,q,F_XOR);
			}
			bitblt(b,r,&display,q,((m == F_MOVE) ? F_STORE : m));
			bfree(b);
			Cursallow();
		}
		buttons(UP);
		break;
	case ROTATE:
		if(userrect(&r)){
rotagain:
			Cursinhibit();
			r = rotate(&display,r);
			Cursallow();
			msg("again?");
			if((s=type()) && *s=='y')
				goto rotagain;
		}
		break;
	}
}
unixmf(u){
	register Bitmap *b;
	register char *s;
	Rectangle r;
	Point p;
	register m;
	switch(u){
	case READ:
		s=type();
		if(s){
			Cursswitch(&hourglass, 0);
			if(b=getbitmap(s, 0)){	/* assignment = */
   again:
				Cursswitch((Texture *)0, 0);
				msg("absolute positioning (y/n)?");
				s=type();
				if(s==0 || *s=='y')
					p=add(b->rect.origin, Drect.origin);
				else{
					msg("point to origin (UL) and hit a button");
					Cursswitch((Texture *)0, 0);
					buttons(DOWN);
					p=mouse.xy;
					buttons(UP);
				} 
				msg("mode (=, ~, ^, |)?");
				m=F_STORE;
				if(s=type()) switch(*s){
				case '=':
				default:
					m=F_STORE;
					break;
				case '~':
					m=F_CLR;
					break;
				case '|':
					m=F_OR;
					break;
				case '^':
					m=F_XOR;
					break;
				}
				Cursinhibit();
				bitblt(b, b->rect, &display, p, m);
				Cursallow();
				msg("again?");
				if((s=type()) && *s=='y')
					goto again;
				bfree(b);
			}
			else
				msg("can't read file");
		}
		break;
	case WRITE:
		if((s=type()) && 
			(msg("sweep out which rectangle"), userrect(&r))){
			Cursinhibit();
			if(putbitmap(&display, r, s, 0))
				msg("write done");
			else
				msg("can't open file");
			Cursallow();
		}
		break;
	case EXIT:
		Cursswitch(&sunset, 0);
		buttons(DOWN);
		if(button3()){
			buttons(UP);
			Cursswitch((Texture *)0, 0);
			exit();
		}
		buttons(UP);
	}
}
newbrsh(){
	Rectangle r, newr;
	char *s;
	if(nbrsh>=NBRSH+1){
		msg("too many brushes defined");
		return 0;
	}
	if(userrect(&r)==0)
		return 0;
	s=type();
	if(s){
		GCAlloc(strlen(s)+1, &brushlist[nbrsh]);
		strcpy(brushlist[nbrsh], s);
		newr=r;
		newr=rsubp(newr, div(add(r.origin, r.corner), 2));
		brushes[nbrsh]=Balloc(newr);
		Cursinhibit();
		bitblt(&display, r, brushes[nbrsh], newr.origin, F_STORE);
		Cursallow();
		return nbrsh++;
	}
	return 0;
}
Word holdbits[16];
Bitmap hold={ holdbits, 1, 0, 0, 16, 16 };
newtxt(){
	Point p;
	char *s;
	register short *w;			/* formerly Word */
	register h=0;
	if(ntxt>=NTXT+1){
		msg("too many textures defined");
		return 0;
	}
	Cursswitch(&black, 0);
	buttons(DOWN);
	if(button3())
		return 0;
	p=mouse.xy;
	Cursinhibit();
	bitblt(&display, Rpt(sub(p, Pt(8, 8)), add(p, Pt(8, 8))),
		&hold, Pt(0, 0), F_STORE);
	if(button2())
		rectf(&hold, hold.rect, F_XOR);
	Cursallow();
	buttons(UP);
	s=type();
	if(s){
		GCAlloc(strlen(s)+1, &txtlist[ntxt]);
		strcpy(txtlist[ntxt], s);
		GCAlloc(32, &txt[ntxt]);
		w=txt[ntxt]->bits;
		for(h=0; h<16; h++)
			*w++=holdbits[h]>>16;
		return ntxt++;
	}
	return 0;
}
/* call getrect() from system to sweep out a rectangle.
 */
userrect(rp)
	register Rectangle *rp;
{
	Cursswitch((Texture *)0, 0);	/* gotta synchronize */
	*rp=getrect3();
	Cursswitch(curcur, (mode<=POINTING)? curbrush : 0);
	return rectclip(rp, screenrect);
}
select(pp, n)	/* mark menuitem pp[n] with a '*' */
	register char **pp;
{
	register char **p;
	register char *s, *t;
	/* first deselect */
	for(p=pp; *p; p++)
		if(*p[0]=='*')
			for(s= *p, t=(*p)+1; *s++ = *t++; )
				;
	/* now select */
	s=pp[n];
	s+=strlen(s);	/* s now points at the null */
	t=s+1;
	while(s>=pp[n])
		*t-- = *s--;
	*t='*';
}		
Bitmap *
Balloc(r)
	Rectangle r;
{
	register Bitmap *b;
	while((b=balloc(r))==0)
		timeout();
	return b;
}
initbrushes()
{
	brushes[BPOINT]=Balloc(Rect(0, 0, 1, 1));
	rectf(brushes[BPOINT], brushes[BPOINT]->rect, F_STORE);
	brushes[BSMALL]=Balloc(Rect(-2, -2, 2, 2));
	rectf(brushes[BSMALL], brushes[BSMALL]->rect, F_STORE);
	brushes[BMED]=Balloc(Rect(-4, -4, 4, 4));
	rectf(brushes[BMED], brushes[BMED]->rect, F_STORE);
	brushes[BBIG]=Balloc(Rect(-8, -8, 8, 8));
	rectf(brushes[BBIG], brushes[BBIG]->rect, F_STORE);
	selectbrush(BBIG);
}
selectbrush(h)
{
	Rectangle r;
	bfree(notbrush);
	bfree(scratch);
	brush=brushes[h];
	r=brush->rect;
	notbrush=Balloc(r);
	bitblt(brush, r, notbrush, r.origin, F_STORE);
	rectf(notbrush, r, F_XOR);
	scratch=Balloc(Rect(0, 0, r.corner.x-r.origin.x+WORDSIZE,
		r.corner.y-r.origin.y+WORDSIZE));
	select(brushlist, h);
	curbrush=h;
	if(mode==POINTING)
		curcur=cursor[curbrush];
	else
		stylemf(INK);
}
settxt(t)
{
	curtxt=txt[t];
	select(txtlist, t);
}
dropbrush(p, c)
	Point p;
	Code c;
{
	Rectangle br;
	/* Center of brush is (0,0), but we must compensate for texture alignment */
	br.origin=add(p, brush->rect.origin);
	br.origin.x&=WORDMASK;
	br.origin.y&=WORDMASK;
	br.corner=add(br.origin, sub(brush->rect.corner, brush->rect.origin));
	/* Fill the scratch space with the current texture */
	texture(scratch, br, curtxt, F_STORE);
	/* Clear all bits in scratch which are not on in brush */
	bitblt(notbrush, notbrush->rect, scratch, br.origin, F_CLR);
	/* Copy scratch to screen */
	Cursinhibit();
	bitblt(scratch, br, &display, add(brush->rect.origin, p), c);
	Cursallow();
}
draw(c)
	Code c;
{
	int r;
	Point p, lastp;
	switch(mode){
	case INKING:
		do{
			dropbrush(mouse.xy, c);
			nap(2);
			updatecurs();
		}while(button12());
		break;
	case POINTING:
		dropbrush(mouse.xy, c);
		buttons(UP);
		break;
	case LINING:
		lastp=mouse.xy;
		for(; button3()==0; wait(MOUSE)){
			while(button12()){
				p=mouse.xy;
				segment(&display, lastp, p, F_XOR);
				nap(2);
				segment(&display, lastp, p, F_XOR);
			}
			Cursinhibit();
			segment(&display, lastp, p, c);
			Cursallow();
			lastp=p;
			do; while(button123()==0);
		}
		break;
	case CURVING:
		for(lastp=mouse.xy; button12(); nap(2)){
			p=mouse.xy;
			if(!eqpt(p, lastp)){
				Cursinhibit();
				segment(&display, lastp, p, c);
				Cursallow();
				lastp=p;
			}
		}
		break;
	case DISCING:
		p=mouse.xy;
		Cursswitch((Texture *)0, 0);
		while(button12()){
			r=norm(mouse.xy.x-p.x, mouse.xy.y-p.y, 0);
			discture(&display,p,r,curtxt,F_XOR);
			nap(2);
			discture(&display,p,r,curtxt,F_XOR);
			if(button3()){
				do; while(button3());
				Cursswitch(&disccur, 0);
				return;
			}
		}
		Cursinhibit();
		discture(&display,p,r,curtxt,c);
		Cursallow();
		Cursswitch(&disccur,0);
		break;
	}
}
buttons(updown)
{
	do wait(MOUSE); while((button123()!=0) != updown);
}
