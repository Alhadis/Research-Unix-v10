#include "samterm.h"

#define	HSIZE	3	/* Type + short count */
Header	h;
uchar	indata[DATASIZE];
uchar	outdata[DATASIZE];
short	outcount;
long	inlong();
Text	*sweeptext();
rcv(){
	register c;
	static state=0;
	static count=0;
	static i=0;
	while((c=rcvchar())!=-1)
		switch(state){
		case 0:
			h.type=c;
			state++;
			break;
		case 1:
			h.count0=c;
			state++;
			break;
		case 2:
			h.count1=c;
			count=h.count0|(h.count1<<8);
			i=0;
			if(count>DATASIZE)
				panic("count>DATASIZE");
			if(count==0)
				goto zerocount;
			state++;
			break;
		case 3:
			indata[i++]=c;
			if(i==count){
		zerocount:
				inmesg(h.type, count);
				state=count=0;
				continue;
			}
			break;
		}
}
Text *
whichtext(tg)
	register tg;
{
	register i;
	for(i=0; i<nname; i++)
		if(tag[i]==tg)
			return text[i];
	panic("whichtext");
	return 0;
}
inmesg(type, count)
	Hmesg type;
{
	register Text *t;
	register i, m;
	register long l;
	register Flayer *lp;
	m=inshort(0);
	l=inlong(2);
	switch(type){
	case -1:
		panic("rcv error");
	default:
		panic("rcv unknown");
	case Hnewname:
		menuins(0, (uchar *)"", (Text *)0, ' ', m);
		break;
	case Hdelname:
		if((m=whichmenu(m))>=0)
			menudel(m);
		break;
	case Hmovname:
		if((m=whichmenu(m))<0)
			break;
		t=text[m];
		l=tag[m];
		i=name[m][0];
		text[m]=0;	/* suppress panic in menudel */
		menudel(m);
		if(t==&cmd)
			m=0;
		else{
			for(m=nname>0 && text[0]==&cmd; m<nname; m++)
				if(strcmp(indata+2, name[m]+1)<0)
					break;
		}
		menuins(m, indata+2, t, i, (int)l);
		break;
	case Hbindname:
		if((i=whichmenu(m))<0)
			break;
		/* in case of a race, a bindname may already have occurred */
		if((t=whichtext(m))==0)
			t=(Text *)l;
		else	/* let the old one win; clean up the new one */
			while(((Text *)l)->nwin>0)
				close(&((Text *)l)->l[((Text *)l)->front]);
		text[i]=t;
		text[i]->tag=m;
		break;
	case Hcurrent:
		if(whichmenu(m)<0)
			break;
		t=whichtext(m);
		i=which && ((Text *)which->user1)==&cmd && m!=cmd.tag;
		if(t==0 && (t=sweeptext(0, m))==0)
			break;
		if(t->l[t->front].textfn==0)
			panic("Hcurrent");
		lp=&t->l[t->front];
		if(i){
			flupfront(lp);
			flborder(lp, 0);
			work=lp;
		}else
			current(lp);
		break;
	case Hgrow:
		if(whichmenu(m)>=0)
			hgrow(m, l, inlong(6), 1);
		break;
	case Hdata:
		if(whichmenu(m)>=0)
			hdata(m, l, indata+6, count-7);
	Checkscroll:
		if(m==cmd.tag)
			for(i=0; i<NL; i++){
				lp=&cmd.l[i];
				if(lp->textfn && lp->user0)
					center(lp, l+1);
			}
		break;
	case Hgrowdata:
		if(whichmenu(m)<0)
			break;
		hgrow(m, l, inlong(6), 0);
		whichtext(m)->lock++;	/* fake the request */
		hdata(m, l, indata+10, count-11);
		goto Checkscroll;
	case Hcut:
		if(whichmenu(m)>=0)
			hcut(m, l, inlong(6));
		break;
	case Hclean:
		if((m=whichmenu(m))>=0)
			name[m][0]=' ';
		break;
	case Hdirty:
		if((m=whichmenu(m))>=0)
			name[m][0]='\'';
		break;
	case Hcheck0:
		if(whichmenu(m)>=0)
			outTs(Tcheck, m);
		break;
	case Hcheck:
		if(whichmenu(m)>=0)
			hcheck(m);
		break;
	case Hmoveto:
		if(whichmenu(m)>=0)
			hmoveto(m, l);
		break;
	case Hsetdot:
		if(whichmenu(m)>=0)
			hsetdot(m, l, inlong(6));
		break;
	case Horigin:
		if(whichmenu(m)>=0)
			horigin(m, l);
		break;
	case Hclose:
		if(whichmenu(m)<0 || (t=whichtext(m))==0)
			break;
		for(i=0,lp=t->l; i<NL; i++,lp++)
			if(lp->textfn)
				close(lp);
		break;
	case Hunlock:
		clrlock();
		break;
	case Hsetpat:
		setpat((char *)indata);
		break;
	case Hunlockfile:
		if(whichmenu(m)>=0 && (t=whichtext(m))->lock)
			--t->lock;
		break;
	case Hsetsnarf:
		hsetsnarf(m);
		break;
	case Hsnarflen:
		snarflen=inlong(0);
		break;
	case Hexit:
		outT0(Texit);
		break;
	}
}
setlock(){
	lock++;
	cursswitch(cursor=&lockarrow);
}
clrlock(){
	lock=0;
	cursswitch(cursor=(Texture *)0);
}
startfile(t)
	Text *t;
{
	outTsl(Tstartfile, t->tag, (long)t);
	outTs(Tunlockfile, t->tag);
	t->lock++;
}
startnewfile(type, t)
	Text *t;
{
	t->tag=Untagged;
	outTl(type, (long)t);
}
inshort(n)
{
	return indata[n]|(indata[n+1]<<8);
}
long
inlong(n)
{
	return indata[n]|(indata[n+1]<<8)|
		((long)indata[n+2]<<16)|((long)indata[n+3]<<24);
}
outT0(type)
	Tmesg type;
{
	outstart(type);
	outsend();
}
outTl(type, l)
	Tmesg type;
	long l;
{
	outstart(type);
	outlong(l);
	outsend();
}
outTs(type, s)
	Tmesg type;
{
	outstart(type);
	outshort(s);
	outsend();
}
outTss(type, s1, s2)
	Tmesg type;
{
	outstart(type);
	outshort(s1);
	outshort(s2);
	outsend();
}
outTsll(type, s1, l1, l2)
	Tmesg type;
	long l1, l2;
{
	outstart(type);
	outshort(s1);
	outlong(l1);
	outlong(l2);
	outsend();
}
outTsl(type, s1, l1)
	Tmesg type;
	long l1;
{
	outstart(type);
	outshort(s1);
	outlong(l1);
	outsend();
}
outTslS(type, s1, l1, s)
	Tmesg type;
	long l1;
	uchar *s;
{
	outstart(type);
	outshort(s1);
	outlong(l1);
	outcopy(strlen(s)+1, s);
	outsend();
}
outTsls(type, s1, l1, s2)
	Tmesg type;
	long l1;
{
	outstart(type);
	outshort(s1);
	outlong(l1);
	outshort(s2);
	outsend();
}
outstart(type)
	Tmesg type;
{
	outdata[0]=type;
	outcount=0;
}
outcopy(count, data)
	register uchar *data;
{
	while(count--)
		outdata[HSIZE+outcount++]=*data++;	
}
outshort(s)
{
	uchar buf[2];
	buf[0]=s;
	buf[1]=s>>8;
	outcopy(2, buf);
}
outlong(l)
	long l;
{
	uchar buf[4];
	buf[0]=l;
	buf[1]=l>>8;
	buf[2]=l>>16;
	buf[3]=l>>24;
	outcopy(4, buf);
}
outsend()
{
	if(outcount>sizeof outdata-HSIZE)
		panic("outcount>sizeof outdata");
	outdata[1]=outcount;
	outdata[2]=outcount>>8;
	sendnchars(outcount+HSIZE, (char *)outdata);
}
hsetdot(m, p0, p1)
	register long p0, p1;
{
	register Text *t=whichtext(m);
	register Flayer *l=&t->l[t->front];
	flsetselect(l, p0, p1);
}
horigin(m, p0)
	register long p0;
{
	register Text *t=whichtext(m);
	register Flayer *l=&t->l[t->front];
	register long a;
	if(!flprepare(l)){
		l->origin=p0;
		return;
	}
	a=p0-l->origin;
	if(a>=0 && a<l->f.nchars)
		frdelete(&l->f, (Posn)0, (Posn)a);
	else if(a<0 && -a<l->f.nchars)
		frinsert(&l->f, rload(&t->rasp, p0, l->origin), 0);
	else
		frdelete(&l->f, (Posn)0, l->f.nchars);
	l->origin=p0;
	scrdraw(l, t->rasp.nbytes);
	if(l->visible==Some)
		flrefresh(l, l->entire, 0);
	hcheck(m);
}
hmoveto(m, p0)
	register long p0;
{
	register Text *t=whichtext(m);
	register Flayer *l=&t->l[t->front];
	if(p0<l->origin || p0-l->origin>muldiv(l->f.nchars, 9, 10))
		outTsll(Torigin, m, p0, 2L);
}
hcheck(m)
{
	register Flayer *l;
	register Text *t;
	register reqd=0, i;
	register long n, a;
	if(m==Untagged)
		return;
	t=whichtext(m);
	for(l=&t->l[0], i=0; i<NL; i++, l++){
		if(l->textfn==0 || !flprepare(l))	/* BUG: don't
							   need this if BUG below
							   is fixed */
			continue;
		a=t->l[i].origin;
		n=rcontig(&t->rasp, a, a+l->f.nchars, 1);
		if(n<l->f.nchars)	/* text missing in middle of screen */
			a+=n;
		else{			/* text missing at end of screen? */
        Again:
		 	if(l->f.nlines==l->f.maxlines && /* BUG: lastlinefull!! */
		   		ptofchar(&l->f, l->f.nchars).y>=
				  l->f.r.origin.y+l->f.maxlines*l->f.font->height)
				goto Checksel;	/* all's well */
			a=t->l[i].origin+l->f.nchars;
			n=t->rasp.nbytes-a;
			if(n==0)
				goto Checksel;
			if(n>TBLOCKSIZE)
				n=TBLOCKSIZE;
			n=rcontig(&t->rasp, a, a+n, 1);
			if(n>0){
				rload(&t->rasp, a, a+n);
				n=l->f.nchars;
				flinsert(l, &scratch, l->origin+n);
				if(n==l->f.nchars)	/* made no progress */
					goto Checksel;
				goto Again;
			}
		}
		if(!reqd){
			n=rcontig(&t->rasp, a, a+TBLOCKSIZE, 0);
			if(n<=0)
				panic("hcheck request==0");
			outTsls(Trequest, m, a, (int)n);
			outTs(Tcheck, m);
			t->lock++;
			reqd++;
		}
	    Checksel:
		flsetselect(l, l->p0, l->p1);
	}
}
flnewlyvisible(l)
	register Flayer *l;
{
	hcheck(((Text *)l->user1)->tag);
}
hsetsnarf(m){
	struct{
		uchar *s;
		short n;
		short size;
	}s1, s2;
	register c, i;
	cursswitch(&deadmouse);
	s1.s=0;
	s1.n=s1.size=0;
	getmuxbuf(&s1);
	s2.n=s2.size=m;
	if(m>0){
		GCALLOC(m, &s2.s);
		for(i=0; i<m; i++){
			while((c=rcvchar())<0)
				wait(RCV);
			s2.s[i]=c;
		}
		setmuxbuf(&s2);
		gcfree(s2.s);
	}
	outTs(Tsetsnarf, s1.n);
	if(s1.n>0){
		snarflen=s1.n;
		sendnchars(s1.n, s1.s);
		gcfree(s1.s);
	}
	cursswitch(cursor);
}
hgrow(m, a, new, req)
	register long a, new;
{
	register i;
	register Flayer *l;
	register Text *t=whichtext(m);
	register long o, b;
	if(new<=0)
		panic("hgrow");
	rresize(&t->rasp, a, 0L, new);
	for(l=&t->l[0], i=0; i<NL; i++, l++){
		if(l->textfn==0)
			continue;
		o=l->origin;
		b=a-o-rmissing(&t->rasp, o, a);
		if(a<o)
			l->origin+=new;
		if(a<l->p0)
			l->p0+=new;
		if(a<l->p1)
			l->p1+=new;
		/* must prevent b temporarily becoming unsigned */
		if(!req || a<o || (b>0 && b>l->f.nchars) ||
		    (l->f.nchars==0 && a-o>0))
			continue;
		if(new>TBLOCKSIZE)
			new=TBLOCKSIZE;
		outTsls(Trequest, m, a, (int)new);
		t->lock++;
		req=0;
	}
}
hdata(m, a, s, len)
	register long a;
	uchar *s;
{
	register i;
	register Flayer *l;
	register Text *t=whichtext(m);
	register long o, b;
	if(t->lock)
		--t->lock;
	if(len==0)
		return;
	for(l=&t->l[0], i=0; i<NL; i++, l++){
		if(l->textfn==0)
			continue;
		o=l->origin;
		b=a-o-rmissing(&t->rasp, o, a);
		/* must prevent b temporarily becoming unsigned */
		if(a<o || (b>0 && b>l->f.nchars))
			continue;
		flinsert(l, &s, o+b);
	}
	rdata(&t->rasp, a, a+len, s);
	rclean(&t->rasp);
}
#define	a	A	/* need this line or get 'redeclaration of a' error! */
hcut(m, a, old)
	register long a, old;
{
	register Flayer *l;
	register Text *t=whichtext(m);
	register i;
	register long o, b;
	if(t->lock)
		--t->lock;
	for(l=&t->l[0], i=0; i<NL; i++, l++){
		if(l->textfn==0)
			continue;
		o=l->origin;
		b=a-o-rmissing(&t->rasp, o, a);
		/* must prevent b temporarily becoming unsigned */
		if((b<0 || b<l->f.nchars) && a+old>=o)
			fldelete(l, b<0? o : o+b,
			    a+old-rmissing(&t->rasp, o, a+old));
		if(a+old<o)
			l->origin-=old;
		else if(a<=o)
			l->origin=a;
		if(a+old<l->p0)
			l->p0-=old;
		else if(a<=l->p0)
			l->p0=a;
		if(a+old<l->p1)
			l->p1-=old;
		else if(a<=l->p1)
			l->p1=a;
	}
	rresize(&t->rasp, a, old, 0L);
	rclean(&t->rasp);
}
