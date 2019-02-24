#include "jim.h"
Point center();
curse(f)
	register Frame *f;
{
	Point p;
	if(f && f->str.s && f->s1==f->s2){
		p=ptofchar(f, f->s1);
		Rectf(Rpt(p, Pt(p.x+1, p.y+newlnsz)), F_XOR);
	}
}
cut(t, save, f_clr)
	register Text *t;
{
	register Frame *f=t->frame;
	register n;
	if((n=f->s1)!=f->s2){
		if(save){
			snarfhuge=t->selecthuge;
			snarf(&f->str, n, f->s2, &snarfbuf);
		}
		if(frdelete(f, f_clr))
			loadfile(t, f->str.n, INFINITY);
		setsel(t, n);
	}
}
txselect(t, pt)	/* don't use "but" */
	register Text *t;
	Point pt;
{
	register Frame *f=t->frame;
	frselect(f, pt);
	t->selecthuge=0;
	Send(O_SELECT, f->s1, 2, data2(f->s2-f->s1));
	while(button1())
		wait(MOUSE);
}
type(t)
	register Text *t;
{
	register Frame *f=t->frame;
	register short i;
	register char c;
	register startn;
	register s1=f->s1;
	register nsc=0;
	static char magic[]="\260\377\033\b\027";
	for(startn=f->s1; !button123(); wait(MOUSE)) {
		if(own()&KBD){
			curse(f);
			strzero(&typein);
			if(f->s2 > f->s1){
				if(t->selecthuge){
					mesg("sorry; first deselect that huge thing\n", 1);
					do; while(kbdchar()==-1);
					return;
				}
				Send(O_CUT, 0, 0, "");
				cut(t, TRUE, t==diag? diagclr : F_CLR);
			}
			if(t==diag && diagnewline){
				mesg("", FALSE);	/* clear the line */
				startn=0;
			}
			for(i=0; notin((c=kbdchar()), magic); i++){
				if((c&0x80) || c==0177 || (c<' ' && c!='\t' && c!='\r')){
					--i;	/* cause it didn't go */
					continue;
				}
				c&=0x7F;
				if(c=='\r'){
					if(t==diag){
						if(i>0){
							Send(O_INSERT, f->s1,
							   i, typein.s);
							frinsert(f, &typein, f->s1);
						}
						senddiag();
						goto Return;
					}
					c='\n';
				}
				straddc(&typein, c);
			}
			if(i>0){
				/*SFBOTCHregister s1=t->s1;
				register nsc=0;*/
				s1=f->s1;
				nsc=0;
				sendstr(t, O_INSERT, s1, i, typein.s);
				frinsert(f, &typein, s1);
				if(!inscomplete){
					nsc=scroll(t, newlines(&typein)+1);
					i-=nsc;
					startn=max(0, startn-nsc);
				}
				setsel(t, s1+i);
			}
			if(c==033){
				if(f->s2>=startn){
					f->s1=startn;
					selectf(f, F_XOR);
					Send(O_SELECT, f->s1, 2, data2(f->s2-f->s1));
					t->selecthuge=0;
				}
		Return:
				curse(f);
				return;
			}else if(c=='\b' || c==027){
				if(f->s1>0){
					f->s1 -= (i=nback(f, c));
					Send(O_BACKSPACE, f->s1, 2, data2(i));
					if(frdelete(f, t==diag? diagclr: F_CLR))
						loadfile(t, f->str.n, INFINITY);
				}
			}
			curse(f);
		}
	}
}
nback(f, c)
	Frame *f;
{
	register n=0, s1=f->s1;
	register char *s=f->str.s+s1;
	static char alphanl[]=
	    "\n0123456789_abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
#define	alphanum	alphanl+1
	if(s1 <= 0)
		return 0;
	if(c=='\b' || *--s=='\n')
		return 1;
	/* else it's ^W. first, get to an alphanumeric (or newline) */
	while(n<s1 && notin(*s, alphanl))
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
newlines(s)
	register String *s;
{
	register char *p=s->s;
	register n=s->n;
	register nl=0;
	while(n-->0)
		if(*p++ == '\n')
			nl++;
	return nl;
}
senddiag()
{
	diagnewline=TRUE;
	/*
	 * Send a 0-length guy to signify the end.
	 */
	send(worktext?worktext->file : 0, O_DIAGNOSTIC, 0, 0, (char *)0);
	waitunix(&diagdone);
}
setsel(t, n)
	register Text *t;
	register n;
{
	t->frame->s1=t->frame->s2=n;
	t->selecthuge=0;
}
/*
 * Change to work in named text, whether typing at diag or not.
 */
workintext(t)
	register Text *t;
{
	if(worktext != t){
		if(worktext){
			rXOR(worktext->frame->scrollrect);
			setchar(worktext->file, STARDOT, '*');
		}
		drawtext(t);
		worktext=t;
		setchar(t->file, STARDOT, '.');
	}
}
drawtext(t)
	register Text *t;
{
	if(t==worktext)
		return;
	if(worktext)
		setchar(worktext->file, STARDOT, '*');
	if(t->obscured)
		dodraw(t);
	else	/* scroll bar and cursor both visible */
		rXOR(t->frame->scrollrect);
	setchar(t->file, STARDOT, '.');
}
dodraw(t)
	register Text *t;
{
	register Frame *f=t->frame;
	rectf(D, f->totalrect, F_CLR);
	border(D, f->totalrect, M, F_XOR);
	ontop(t);
	if(f->str.s==0)	/* probably got closed */
		return;
	frameop(f, opdraw, f->rect.origin, f->str.s, f->str.n);
	if(complete)
		loadfile(t, f->str.n, 32767);
	selectf(f, F_XOR);
	tellseek(t, f->scrolly);
}
Text *
textofpoint(pt)
	Point pt;
{
	register Text *t;
	if(ptinrect(pt, diag->frame->totalrect))
		return diag;
	for(t=text; t; t=t->next)
		if(ptinrect(pt, t->frame->totalrect))
			return t;
	return 0;
}
ontop(t)
	register Text *t;
{
	register Text *a;
	if((a=text)==t)
		return;
	for(; a->next!=t; a=a->next)
		if(a->next==0)
			panic("ontop");
	a->next=t->next;	/* link it out */
	t->next=text;		/* link it in */
	text=t;
	obscured(t);
}
obscured(t)
	register Text *t;
{
	Rectangle r;
	r=t->frame->totalrect;
	for(t=t->next; t; t=t->next)
		if(rectXrect(r, t->frame->totalrect))
			t->obscured=TRUE;
}
buttons(updown)
{
	do wait(MOUSE); while((button123()!=0) != updown);
}
