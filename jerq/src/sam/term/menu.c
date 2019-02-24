#include "samterm.h"

uchar	*name[MAXFILES];	/* first byte is ' ' or '\'': modified state */
Text	*text[MAXFILES];	/* pointer to Text associated with file */
ushort	tag[MAXFILES];		/* text[i].tag, even if text[i] not defined */
int	nname;
char	*genmenu3();
char	*genmenu2();
char	*genmenu2c();
Text	*sweeptext();
enum Menu2{
	Cut,
	Paste,
	Snarf,
	Look,
	Exch,
	Search,
	NMENU2=Search,
	Send=Search,
	Scroll,
	NMENU2C
};
enum Menu3{
	New,
	Xerox,
	Reshape,
	Close,
	Write,
	NMENU3
};
char	*menu2str[]={
	"cut",
	"paste",
	"snarf",
	"look",
	"<mux>",
	0,		/* storage for last pattern */
};
char	*menu3str[]={
	"new",
	"xerox",
	"reshape",
	"close",
	"write",
};
Menu	menu2={0, genmenu2};
Menu	menu2c={0, genmenu2c};
Menu	menu3={0, genmenu3};
menu2hit()
{
	register Text *t=(Text *)which->user1;
	register w=which-t->l;
	switch(menuhit(t==&cmd? &menu2c : &menu2, 2)){
	case Cut:
		if(!lock && !t->lock)
			cut(t, w, 1);
		break;
	case Paste:
		if(!lock && !t->lock)
			paste(t, w);
		break;
	case Snarf:
		if(!lock && !t->lock)
			snarf(t, w);
		break;
	case Exch:
		outT0(Tstartsnarf);
		setlock();
		break;
	case Look:
		outTsll(Tlook, t->tag, which->p0, which->p1);
		break;
	case Search:
		outcmd();
		if(t==&cmd)
			outTsll(Tsend, 0 /*ignored*/, which->p0, which->p1);
		else
			outT0(Tsearch);
		break;
	case Scroll:
		if(cmd.l[w].user0^=1)
			center(&cmd.l[w], t->rasp.nbytes);
		break;
	}
}
menu3hit()
{
	Rectangle r;
	register Flayer *l;
	register m, i;
	register Text *t;
	switch(m=menuhit(&menu3, 3)){
	case -1:
		break;
	case New:
		sweeptext(1, 0);
		break;
	case Xerox:
	case Reshape:
		cursswitch(&bullseye);
		buttons(Down);
		if(button3() && (l=flwhich(mouse.xy)) && getr(&r))
			duplicate(l, r, l->f.font, m==Reshape);
		else
			cursswitch(cursor);
		buttons(Up);
		break;
	case Close:
		cursswitch(&bullseye);
		buttons(Down);
		if(button3() && (l=flwhich(mouse.xy)) && !lock){
			t=(Text *)l->user1;
			if(t!=&cmd || t->nwin>1){
				if(t->nwin>1)
					close(l);
				else
					outTs(Tclose, t->tag);
			}
		}
		cursswitch(cursor);
		buttons(Up);
		break;
	case Write:
		cursswitch(&bullseye);
		buttons(Down);
		if(button3() && (l=flwhich(mouse.xy)))
			outTs(Twrite, ((Text *)l->user1)->tag);
		cursswitch(cursor);
		buttons(Up);
		break;
	default:
		if(t=text[m-NMENU3]){
			i=t->front;
			if(t->nwin==0 || t->l[i].textfn==0)
				return;	/* not ready yet; try again later */
			if(t->nwin>1 && which==&t->l[i])
				do
					if(++i==NL)
						i=0;
				while(i!=t->front && t->l[i].textfn==0);
			current(&t->l[i]);
		}else
			sweeptext(0, tag[m-NMENU3]);
		break;
	}
}
Text *
sweeptext(new, tag)
{
	Rectangle r;
	register Text *t;
	if(!(own()&MOUSE))
		wait(MOUSE);
	if(getr(&r) && (t=(Text *)alloc(sizeof(Text)))){
		current((Flayer *)0);
		flnew(&t->l[0], gettext, 0, (char *)t);
		flinit(&t->l[0], r, &defont);
		t->nwin=1;
		rinit(&t->rasp);
		if(new)
			startnewfile(Tstartnewfile, t);
		else{
			rinit(&t->rasp);
			t->tag=tag;
			startfile(t);
		}
		return t;
	}
	return 0;
}
whichmenu(tg)
	register tg;
{
	register i;
	for(i=0; i<nname; i++)
		if(tag[i]==tg)
			return i;
	return -1;
}
menuins(n, s, t, m, tg)
	uchar *s;
	Text *t;
{
	register i;
	if(nname==MAXFILES)
		panic("menuins");
	for(i=nname; i>n; --i)
		name[i]=name[i-1], text[i]=text[i-1], tag[i]=tag[i-1];
	text[n]=t;
	tag[n]=tg;
	name[n]=ALLOC(strlen(s)+2);
	name[n][0]=m;
	copystr(s, name[n]+1);
	nname++;
	/* ehg's hack, implemented by rob: make this guy pop up under menu */
	/* non-jerq implementers: delete the rest of this procedure */
#define	MENULINES	16
	if((n+=NMENU3)<16)
		menu3.prevtop=0;
	else if(n>nname+NMENU3-MENULINES){
		menu3.prevtop=nname+NMENU3-MENULINES;
	}else
		menu3.prevtop=n-MENULINES/2;
	menu3.prevhit=n-menu3.prevtop;
}
menudel(n){
	register i;
	if(nname==0 || n>=nname || text[n])
		panic("menudel");
	free(name[n]);
	--nname;
	for(i=n; i<nname; i++)
		name[i]=name[i+1], text[i]=text[i+1], tag[i]=tag[i+1];
}
setpat(s)
	register char *s;
{
	static char pat[17];
	pat[0]='/';
	strcpy(pat+1, s);	/* host guarantees it fits */
	menu2str[Search]=pat;
}
static uchar buf[64]={' ', ' ', ' ', ' '};
char *
paren(s)
	register char *s;
{
	register uchar *t=buf;
	*t++='(';
	do; while(*t++=*s++);
	t[-1]=')';
	*t=0;
	return (char *)buf;
}
char *
genmenu2(n)
	register n;
{
	register Text *t=(Text *)which->user1;
	register char *p;
	if(n>=NMENU2+(menu2str[Search]!=0))
		return 0;
	p=menu2str[n];
	if(!lock && !t->lock || n==Search || n==Look)
		return p;
	return paren(p);
}
char *
genmenu2c(n)
	register n;
{
	register Text *t=(Text *)which->user1;
	register char *p;
	if(n>=NMENU2C)
		return 0;
	if(n==Scroll)
		p=which->user0? "noscroll" : "scroll";
	else if(n==Send)
		p="send";
	else
		p=menu2str[n];
	if(!lock && !t->lock)
		return p;
	return paren(p);
}
char *
genmenu3(n)
	register n;
{
	register Text *t;
	register i;
	if(n>=(int)NMENU3+nname)
		return 0;
	if(n<(int)NMENU3)
		return menu3str[n];
	n-=(int)NMENU3;
	if(n==0)	/* unless we've been fooled, this is cmd */
		return (char *)&name[n][1];
	t=text[n];
	buf[0]=name[n][0];
	buf[1]='-';
	buf[2]=' ';
	buf[3]=' ';
	if(t){
		if(t->nwin==1)
			buf[1]='+';
		else if(t->nwin>1)
			buf[1]='*';
		if(work && t==(Text *)work->user1)
			buf[2]='.';
	}
	if((i=strlen(name[n]+1))>sizeof buf-4-2){
		int c, k;
		k=sizeof buf/2;
		c=name[n][k];
		name[n][k]=0;
		copystr(name[n]+1, buf+4);
		name[n][k]=c;
		copystr((uchar *)"...", buf+4+k-1);
		copystr(name[n]+1+i-(sizeof buf-4-k-4), buf+4+k+2);
	}else
		copystr(name[n]+1, buf+4);
	i=strlen(buf);
	buf[i++]='\240';
	buf[i]=0;
	return (char *)buf;
}
