#include "jim.h"
extern Message	m;
Rectangle	diagrect;
Rectangle	screenrect;
String		typein;
String		snarfbuf;
Text		*worktext;
Text		*text;
Text		*diag;
int		diagclr;
short		newlnsz;
Text		*current;
int		usualtest();
Point		loadpt;		/* location of next host text to be received */
int		ntoload;	/* number of characters to read from unix */
int		(*donetest)()=usualtest; /* test to decide when load is done */
int		snarfhuge;
Texture deadmouse = {
	 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x000C, 0x0082, 0x0441,
	 0xFFE1, 0x5FF1, 0x3FFE, 0x17F0, 0x03E0, 0x0000, 0x0000, 0x0000,
};
Texture bullseye = {
	 0x07E0, 0x1FF8, 0x399C, 0x63C6, 0x6FF6, 0xCDB3, 0xD99B, 0xFFFF,
	 0xFFFF, 0xD99B, 0xCDB3, 0x6FF6, 0x63C6, 0x399C, 0x1FF8, 0x07E0,
};
/*
 * Event flags to wait for in protocol
 */
int iodone, diagdone, scrolllines, filedone;
int reqlimit, nrequested, reqposn;
main()
{
	register got;
	request(KBD|MOUSE|RCV|SEND);
	newlnsz=defont.height;
	init();
	strzero(&snarfbuf);
	waitunix(&diagdone);	/* when menu is loaded */
	for(got=0; ; got=wait(MOUSE|KBD|RCV)){
		if(P->state&RESHAPED){
			rectf(&display, Drect, F_CLR);
			closeall();
			init();
			P->state&=~RESHAPED;
		}
		/* NOTE: cursor is OFF at all times... */
		if((got&RCV) && rcv()){
			curse(current->frame);
			(void)message();
			curse(current->frame);
		}
		if((got&MOUSE) && button123() && ptinrect(mouse.xy, display.rect)){
			curse(current->frame);
			buttonhit(mouse.xy, mouse.buttons);
			curse(current->frame);
		}
		if((got&KBD) && current)	/* ...except in type */
			type(current);	/* manages cursor itself */
	}
}
sendsnarf()
{
	register char *d=snarfbuf.s;
	register unsigned m;
	register unsigned n=snarfbuf.n;
	register posn=0;
	cursswitch(&deadmouse);
	send(0, O_SETSNARF, n, 0, (char *)0);
	do{
		if((m=n)>NDATA)
			m=NDATA;
		send(0, O_SETSNARF, posn, m, d);
		posn+=m;
		d+=m;
		n-=m;
	}while(n>0);
	if(snarfbuf.n)
		send(0, O_SETSNARF, 0, 0, (char *)0);
	cursswitch((Texture *)0);
}
panic(s)
	char *s;
{
	mesg(s, (char *)0);
	for(;;)
		sleep(10000);
}
closeall(){
	register Text *t;
	while(t=text){	/* assignment = */
		if(t->file)
			setchar(t->file, STARDOT, ' ');
		txfree(t);	/* resets text */
	}
	current=0;
}
Text *
txalloc(r)
	Rectangle r;
{
	register Text *t=(Text *)alloc(sizeof(Text));
	if(t==0 || (t->frame=fralloc(r, M))==0)
		panic("txalloc");
	t->next=text;
	text=t;
	txinit(t);
	return t;
}
txinit(t)
	register Text *t;
{
	frinit(t->frame);
	obscured(t);
	t->selecthuge=0;
	t->obscured=FALSE;
}
txfree(t)
	register Text *t;
{
	register Text *a;
	frfree(t->frame);
	t->file=0;
	t->obscured=TRUE;
	if((a=text)==t)
		text=t->next;
	else{
		for(; a->next!=t; a=a->next)
			if(a->next==0)
				panic("txfree");
		a->next=t->next;
	}
	free(t);
}
close(t)
	register Text *t;
{
	if(t==0)
		return;
	delmenu(t->file);
	obscured(t);
	rectf(D, t->frame->totalrect, F_CLR);
	txfree(t);
	if(t==worktext)
		worktext=0;
	if(t==current)
		current=0;
	if(worktext){
		if(worktext->obscured)
			dodraw(worktext);
	}else if((t=text) && t->obscured)
		workintext(t);
}
init(){
	diagrect=Drect;
	diagrect.origin.y=diagrect.corner.y-(defont.height+2*M);
	diagclr=F_CLR;
	diag=txalloc(diagrect);
	txinit(diag);
	/* a little adjustment ... */
	diag->frame->rect.origin.x-=3*SCROLLWIDTH/2;
	diag->frame->scrollrect.corner.x=diag->frame->scrollrect.origin.x;
	curse(diag->frame);
	screenrect=Drect;
	screenrect.corner.y=diagrect.origin.y;
	current=0;
	worktext=0;
}
/*ARGSUSED*/
seek(t, pt, but)
	register Text *t;
	Point pt;
{
	register Frame *f=t->frame;
	Rectangle r;
	register n;
	register y=pt.y;
	if(t==diag)
		return;
	txinit(t);
	ontop(t);
	r=f->scrollrect;
	n=muldiv(y-r.origin.y, YMAX, r.corner.y-r.origin.y);
	if(n<0)
		n=0;
	if(n>YMAX)
		n=YMAX;
	send(t->file, O_SEEK, n, 0, (char *)0);
	loadfile(t, 0, INFINITY);
	setsel(t, 0);
}
tellseek(t, y)
	Text *t;
{
	register Frame *f=t->frame;
	Rectangle r;
	f->scrolly=y;
	r=f->scrollrect;
	y=muldiv(y, r.corner.y-r.origin.y-2*M, YMAX)+r.origin.y+M;
	Rectf(r, F_OR);
	r.origin.y=y;
	r.corner.y=y+2;
	r.origin.x++;
	r.corner.x--;
	rXOR(r);
}
usualtest()
{
	return(!inscomplete);
}
loadfile(t, posn, n)
	register Text *t;
	register posn;
{
	loadpt=ptofchar(t->frame, posn);
	reqlimit=n;
	ntoload=n;
	nrequested=0;
	reqposn=posn;
	urequest(t->file);
	urequest(t->file);	/* double buffering */
	waitunix(&iodone);
	if(n==INFINITY){	/* the stuff's gotta die down! */
		reqlimit=0;
		while(wait(RCV)){
			if(rcv()){
				message();
				break;
			}
		}
	}
}
urequest(f)
	int f;
{
	register n;
	if(nrequested <= reqlimit){
		n=min(NDATA, reqlimit-nrequested);
		send((unsigned)f, O_REQUEST, reqposn, 2, (unsigned)data2(n));
		reqposn+=n;
		nrequested+=n;
	}
}
move(t, pt, but)
	register Text *t;
	Point pt;
{
	register Frame *f=t->frame;
	Rectangle r;
	register n;
	if(t==diag)
		return;
	selectf(f, F_XOR);
	r=f->scrollrect;
	n=muldiv(pt.y-r.origin.y-M, f->nlines, r.corner.y-r.origin.y-2*M);
	if(n<0)
		n=0;
	if(but==B1)	/* backwards */
		n= -n;
	pt=ptofchar(f, f->s1);
	(void)scroll(t, n);
	setsel(t, charofpt(f, pt));
	send(t->file, O_SELECT, f->s1, 2, data2(0));
}
waitunix(flag)
	register *flag;
{
	cursswitch(&deadmouse);
	*flag=FALSE;
	while(*flag==FALSE){
		wait(RCV);
		if(rcv())
			message();
	}
	cursswitch((Texture *) 0);
}
message()
{
	register n, op, posn;
	unsigned f;
	register char *data;
	static s1, s2;
	String rcvstr;
	register Text *t;
	register numdata;
	f=m.file;
	t=textoffile(f);
	n=m.nbytes;
	op=m.op;
	posn=m.posn;
	data=m.data;
	data[n]=0;
	numdata=((unsigned char)data[0])+(data[1]<<8);
	switch(op){
	case O_DIAGNOSTIC:
		mesg(data, FALSE);
		break;
	case O_INSERT:	/* only comes after a REQUEST */
		if(n==0 || t==0){	/* t==0 is rare, but can come from a race */
	    Done:
			iodone=TRUE;
			break;
		}
		rcvstr.n=n;
		rcvstr.s=data;
		frinsert(t->frame, &rcvstr, posn);
		ntoload-=n;
		if((*donetest)())
			goto Done;
		urequest(f);
		break;
	case O_SEARCH:
		keepsearch(posn);
		break;
	case O_SEEK:
		tellseek(t, posn);
		break;
	case O_RESET:
		seek(t, Pt(0, 0), 0);
		if(t!=current)
			curse(t->frame);
		break;
	case O_SELECT:
		curse(t->frame);
		s1=posn;
		s2=s1+numdata;
		break;
	case O_MOVE:
		if(posn==-1)	/* it's out in left field; reset */
			txinit(t);
		else
			selectf(t->frame, F_XOR);
		if(posn<=0)	/* 0==> just before top of screen */
			loadfile(t, 0, integer(data));
		/* otherwise it's on the screen now */
		t->frame->s1=s1;
		t->frame->s2=s2;
		t->selecthuge=0;
		if(s2>t->frame->str.n){
			s2=t->frame->str.n;
			t->selecthuge=1;
		}
		selectf(t->frame, F_XOR);
		curse(t->frame);
		break;
	case O_SCROLL:
		scrolllines=TRUE;
		break;
	case O_DONE:
		diagdone=TRUE;
		break;
	case O_FILENAME:
		if(f==0)
			filedone= -1;
		else{
			setname(f, data);
			filedone=f;
		}
		break;
	case O_CLOSE:
		close(t);
		break;
	case O_MODIFIED:
		modified(t, posn);
		break;
	case O_CHARSONSCREEN:
		send(0, O_CHARSONSCREEN, charofpt(t->frame, Pt(XMAX, YMAX)), 0, (char *)0);
		break;
	case O_SWEEP:
		wait(MOUSE);
		menughit(t, f, 1);
		break;
	default:
		mesg("unk\n", TRUE);
	}
}
integer(s)
	register char *s;
{
	return (unsigned char)s[0]+(s[1]<<8);
}
int diagnewline=TRUE;
mesg(s, sendit)
	register char *s;
{
	char buf[80+1];
	register char *p=buf, *q=s;
	String diagstr;
	register Text *t=diag;
	register Frame *f=t->frame;
	register i;
	if(diagnewline){	/* last thing caused a newline */
		Rectf(f->rect, current==t? diagclr: F_CLR);
		strzero(&f->str);
		setsel(t, 0);
	}
	diagnewline=FALSE;
	while(*p= *q++)
		p++;
	if(sendit)
		sendstr(diag, O_INSERT, f->s1, p-buf, s);
	if(p>&buf[0] && p[-1]=='\n'){
		diagnewline=TRUE;
		*--p=0;
	}
	diagstr.n=i=p-buf;
	diagstr.s=buf;
	frinsert(f, &diagstr, f->s1);
	setsel(t, f->s1+i);
	if(p>&buf[0] && sendit==FALSE){
		rXOR(f->rect);
		sleep(10);
		rXOR(f->rect);
	}
}
/*VARARGS1*/
sendstr(t, op, posn, n, d)
	Text *t;
	register n;
	register char *d;
{
	register unsigned m;
	do{
		if((m=n)>NDATA)
			m=NDATA;
		send(t->file, op, posn, m, d);
		posn+=m;
		d+=m;
		n-=m;
	}while(n > 0);
}
scrolltest()
{
	return(ntoload<=0);
}
scroll(t, nlines)
	register Text *t;
	register nlines;
{
	register nchars;
	register Frame *f=t->frame;
	if(nlines==0 || t==diag)
		return 0;
	send(t->file, O_SCROLL, nlines, 0, (char *)0);
	waitunix(&scrolllines);
	nchars=m.posn;
	if(nchars>0){	/* scroll forwards */
		f->s1=0;
		if(nchars>f->str.n)
			f->s2=f->str.n;
		else
			f->s2=nchars;
		t->selecthuge=0;
		if(frdelete(f, F_CLR))
			loadfile(t, f->str.n, INFINITY);
	}else{	/* scroll backwards */
		setsel(t, 0);
		donetest=scrolltest;
		loadfile(t, 0, -nchars);
		donetest=usualtest;
	}
	return nchars;
}
char *
data2(n){
	static char x[2];
	x[0]=n;
	x[1]=n>>8;
	return x;
}
Send(op, posn, n, s)
	register op;
	register posn;
	register n;
	register char *s;
{
	send(current->file, op, posn, n, s);
}
