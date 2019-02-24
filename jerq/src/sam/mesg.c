#include "sam.h"

Header	h;
uchar	indata[DATASIZE];
uchar	outdata[DATASIZE+3];
short	outcount;
long	inlong();
Posn	cmdpt;
Posn	cmdptadv;
Buffer	*snarfbuf;

log(out, s)
	char *s;
{
	static fd=0;
	if(fd<=0)
		fd=creat("/usr/rob/sam.out", 0666);
	fprint(fd, "%s%s\n", out? "out: " : "in:  ", s);
}
logn(out, n)
	int n;
{
	char buf[32];
	sprint(buf, "%d", n);
	log(out, buf);
}
#ifdef	DIST
#define	log(a, b)
#define logn(a, b)
#endif
rcvchar(){
	static uchar buf[64];
	static i, nleft=0;
	if(nleft<=0){
		nleft=read(0, (char *)buf, sizeof buf);
		if(nleft<=0)
			return -1;
		i=0;
	}
	--nleft;
	return buf[i++];
}
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
				state=count=0;
				return inmesg(h.type);
			}
			break;
		}
	return 0;
}
File *
whichfile(tag)
	register tag;
{
	register i;
	for(i=0; i<file.nused; i++)
		if(file.ptr[i]->tag==tag)
			return file.ptr[i];
	hiccough((char *)0);
	/*NOTREACHED*/
	return 0;
}
inmesg(type)
	Tmesg type;
{
	uchar buf[1025];
	register i, m;
	long l;
	register File *f;
	register Posn p0, p1;
	Range r;
	switch(type){
	case -1:
		panic("rcv error");
	default:
		panic("rcv unknown");
	case Tcut:
		log(0, "Tcut");
		f=whichfile(inshort(0));
		p0=inlong(2);
		p1=inlong(6);
		logn(0, (int)p0);
		logn(0, (int)p1);
		Fdelete(f, p0, p1);
		if(Fupdate(f, FALSE, FALSE))
			modnum++;
		f->dot.r.p1=f->dot.r.p2=p0;
		f->tdot=f->dot.r;   /* terminal knows the value of dot already */
		break;
	case Tpaste:
		log(0, "Tpaste");
		f=whichfile(inshort(0));
		p0=inlong(2);
		logn(0, (int)p0);
		for(l=0; l<snarfbuf->nbytes; l+=m){
			m=snarfbuf->nbytes-l;
			if(m>BLOCKSIZE)
				m=BLOCKSIZE;
			Bread(snarfbuf, genbuf, m, l);
			Finsert(f, tempstr(genbuf, m), p0);
		}
		if(Fupdate(f, FALSE, TRUE))
			modnum++;
		f->dot.r.p1=p0;
		f->dot.r.p2=p0+snarfbuf->nbytes;
		f->tdot.p1=-1;	/* force telldot to tell (arguably a BUG) */
		telldot(f);
		outTs(Hunlockfile, f->tag);
		break;
	case Tsnarf:
		log(0, "Tsnarf");
		snarf(whichfile(inshort(0)), inlong(2), inlong(6), snarfbuf, 0);
		break;
	case Ttype:
		log(0, "Ttype");
		f=whichfile(inshort(0));
		p0=inlong(2);
		logn(0, (int)p0);
		log(0, (char *)indata+6);
		i=strlen(indata+6);
		Finsert(f, tempstr(indata+6, i), p0);
		if(Fupdate(f, FALSE, FALSE))
			modnum++;
		if(f==cmd && p0==f->nbytes-i && i>0 && indata[i+6-1]=='\n')
			termcommand();
		f->dot.r.p1=f->dot.r.p2=p0+i; /* terminal knows this already */
		f->tdot=f->dot.r;
		break;
	case Tstartfile:
		log(0, "Tstartfile");
		f=whichfile(inshort(0));
		if(!f->rasp)	/* this might be a duplicate message */
			f->rasp=new(List, 1);
		current(f);
		outTsl(Hbindname, f->tag, inlong(2));
		outTs(Hcurrent, f->tag);
		logn(0, f->tag);
		if(f->state==Unread)
			load(f);
		else{
			if(f->nbytes>0){
				rgrow(f->rasp, 0L, f->nbytes);
				outTsll(Hgrow, f->tag, 0L, f->nbytes);
			}
			outTs(Hcheck0, f->tag);
			moveto(f, f->dot.r);
		}
		break;
	case Tstartcmdfile:
		log(0, "Tstartcmdfile");
		l=inlong(0);
		strdup(&genstr, (uchar *)"~~sam~~");
		cmd=newfile();
		outTsl(Hbindname, cmd->tag, l);
		outTs(Hcurrent, cmd->tag);
		Fsetname(cmd);
		cmd->rasp=new(List, 1);
		cmd->state=Clean;
		if(cmdstr.n){
			Finsert(cmd, &cmdstr, 0L);
			strdelete(&cmdstr, 0L, (Posn)cmdstr.n);
		}
		Fupdate(cmd, FALSE, TRUE);
		outT0(Hunlock);
		break;
	case Tstartnewfile:
		log(0, "Tstartnewfile");
		l=inlong(0);
		strdup(&genstr, (uchar *)"");
		f=newfile();
		f->rasp=new(List, 1);
		outTsl(Hbindname, f->tag, l);
		Fsetname(f);
		outTs(Hcurrent, f->tag);
		current(f);
		load(f);
		break;
	case Trequest:
		log(0, "Trequest");
		f=whichfile(inshort(0));
		p0=inlong(2);
		logn(0, (int)p0);
		logn(0, inshort(6));
		if(f->state==Unread)
			panic("Trequest: unread");
		p1=p0+inshort(6);
		if(p1>f->nbytes)
			p1=f->nbytes;
		if(p0>f->nbytes) /* can happen e.g. scrolling during command */
			p0=f->nbytes;
		if(p0==p1){
			buf[0]=0;
			r.p1=r.p2=p0;
		}else{
			r=rdata(f->rasp, p0, p1-p0);
			i=r.p2-r.p1;
			if(Fchars(f, buf, r.p1, r.p2)!=i)
				panic("Trequest 2");
			buf[i]=0;
		}
		outTslS(Hdata, f->tag, r.p1, buf);
		break;
	case Twrite:
		log(0, "Twrite");
		f=whichfile(inshort(0));
		addr.r.p1=0;
		addr.r.p2=f->nbytes;
		if(f->name.s[0]==0)
			error(Enoname);
		strdupstr(&genstr, &f->name);
		writef(f);
		break;
	case Tworkfile:
		log(0, "Tworkfile");
		logn(0, (int)inlong(2));
		logn(0, (int)inlong(6));
		f=whichfile(inshort(0));
		current(f);
		f->dot.r.p1=inlong(2);
		f->dot.r.p2=inlong(6);
		f->tdot=f->dot.r;
		break;
	case Tcheck:
		log(0, "Tcheck");
		/* go through whichfile to check the tag */
		outTs(Hcheck, whichfile(inshort(0))->tag);
		break;
	case Torigin:
		log(0, "Torigin");
		logn(0, (int)inlong(6));
		lookorigin(whichfile(inshort(0)), inlong(2), inlong(6));
		break;
	case Tclose:
		log(0, "Tclose");
		f=whichfile(inshort(0));
		current(f);
		trytoclose(f);
		break;
	case Tlook:
		log(0, "Tlook");
		f=whichfile(inshort(0));
		p0=inlong(2);
		p1=inlong(6);
		setgenstr(f, p0, p1);
		for(l=0; l<genstr.n; l++){
			i=genstr.s[l];
			if(i && !('a'<=i && i<='z') && !('A'<=i && i<='Z'))
				strinsert(&genstr, tempstr((uchar *)"\\", 1), l++);
		}
		straddc(&genstr, '\0');
{Regexp poot;
poot.text=genstr;
		nextmatch(f, &poot, p1, 1);	/* BUGGERED */
 }
		moveto(f, sel.p[0]);
		break;
	case Tsearch:
		log(0, "Tsearch");
		if(curfile==0)
			error(Enofile);
		if(lastpat.n==0)
			panic("Tsearch");
{Regexp poot;
poot.text=lastpat;
		nextmatch(curfile, &poot, curfile->dot.r.p2, 1);	/* BUGGERED */
 }
		moveto(curfile, sel.p[0]);
		break;
	case Tsend:
		log(0, "Tsend");
		p0=inlong(2);
		p1=inlong(6);
		setgenstr(cmd, p0, p1);
		Bdelete(snarfbuf, (Posn)0, snarfbuf->nbytes);
		Binsert(snarfbuf, &genstr, (Posn)0);
		outTl(Hsnarflen, (long)genstr.n);
		if(genstr.s[genstr.n-1]!='\n')
			straddc(&genstr, '\n');
		Fdelete(cmd, cmdpt, cmd->nbytes);
		Finsert(cmd, &genstr, cmd->nbytes);
		Fupdate(cmd, FALSE, TRUE);
		cmd->dot.r.p1=cmd->dot.r.p2=cmd->nbytes;
		telldot(cmd);
		termcommand();
		break;
	case Tunlockfile:
		log(0, "Tunlockfile");
		outTs(Hunlockfile, whichfile(inshort(0))->tag);
		break;
	case Tstartsnarf:
		log(0, "Tstartsnarf");
		if(snarfbuf->nbytes>BLOCKSIZE)
			error(Etoolong);
		outTs(Hsetsnarf, (int)snarfbuf->nbytes);
		if(snarfbuf->nbytes>0){
			Bread(snarfbuf, genbuf, (int)snarfbuf->nbytes, (Posn)0);
			Write(1, genbuf, (int)snarfbuf->nbytes);
		}
		break;
	case Tsetsnarf:
		log(0, "Tsetsnarf");
		m=inshort(0);
		if(m>BLOCKSIZE)
			error(Etoolong);
		if(m>0){
			for(i=0; i<m; i++)
				genbuf[i]=rcvchar();
			Bdelete(snarfbuf, (Posn)0, snarfbuf->nbytes);
			Binsert(snarfbuf, tempstr(genbuf, m), (Posn)0);
		}
		outT0(Hunlock);
		break;
	case Tdclick:
		log(0, "Tdclick");
		f=whichfile(inshort(0));
		p1=inlong(2);
		doubleclick(f, p1);
		f->tdot.p1=f->tdot.p2=p1;
		telldot(f);
		outTs(Hunlockfile, f->tag);
		break;
	case Texit:
		log(0, "Texit");
		return FALSE;
	}
	return TRUE;
}
snarf(f, p1, p2, buf, emptyok)
	register File *f;
	register Posn p1, p2;
	register Buffer *buf;
{
	register Posn l;
	register i;
	if(!emptyok && p1==p2)
		return;
	Bdelete(buf, (Posn)0, buf->nbytes);
	/* Stage through genbuf to avoid compaction problems */
	for(l=p1; l<p2; l+=i){
		i=p2-l>BLOCKSIZE? BLOCKSIZE : p2-l;
		Fchars(f, genbuf, l, l+i);
		Binsert(buf, tempstr(genbuf, i), buf->nbytes);
	}
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
setgenstr(f, p0, p1)
	register File *f;
	register Posn p0, p1;
{
	if(p0!=p1){
		if(p1-p0>=TBLOCKSIZE)
			error(Etoolong);
		strinsure(&genstr, p1-p0);
		Fchars(f, genbuf, p0, p1);
		bcopy(genbuf, genbuf+(p1-p0), genstr.s, 1);
		genstr.n=p1-p0;
	}else{
		if(snarfbuf->nbytes==0)
			error(Eempty);
		if(snarfbuf->nbytes>TBLOCKSIZE)
			error(Etoolong);
		Bread(snarfbuf, genbuf, (int)snarfbuf->nbytes, (Posn)0);
		strinsure(&genstr, snarfbuf->nbytes);
		bcopy(genbuf, genbuf+snarfbuf->nbytes, genstr.s, 1);
		genstr.n=snarfbuf->nbytes;
	}
}
outT0(type)
	Hmesg type;
{
	outstart(type);
	outsend();
}
outTl(type, l)
	Hmesg type;
	long l;
{
	outstart(type);
	outlong(l);
	outsend();
}
outTs(type, s)
	Hmesg type;
{
	outstart(type);
	logn(1, s);
	outshort(s);
	outsend();
}
outTsS(type, s1, s)
	Hmesg type;
	uchar *s;
{
	outstart(type);
	outshort(s1);
	outcopy(strlen(s)+1, s);
	log(1, (char *)s);
	outsend();
}
outTssl(type, s1, s2, l)
	Hmesg type;
	long l;
{
	outstart(type);
	outshort(s1);
	outshort(s2);
	outlong(l);
	outsend();
}
outTslS(type, s1, l1, s)
	Hmesg type;
	Posn l1;
	uchar *s;
{
	char buf[100];
	register i;
	outstart(type);
	outshort(s1);
	outlong(l1);
	logn(1, (int)l1);
	outcopy(strlen(s)+1, s);
	i=strlen(s);
	if(i>99)
		i=99;
	buf[i]=0;
	while(--i>=0)
		buf[i]=s[i];
	log(1, buf);
	outsend();
}
outTS(type, s)
	Hmesg type;
	uchar *s;
{
	char buf[100];
	register i;
	outstart(type);
	outcopy(strlen(s)+1, s);
	i=strlen(s);
	if(i>99)
		i=99;
	buf[i]=0;
	while(--i>=0)
		buf[i]=s[i];
	log(1, buf);
	outsend();
}
outTsllS(type, s1, l1, l2, s)
	Hmesg type;
	Posn l1, l2;
	uchar *s;
{
	char buf[100];
	register i;
	outstart(type);
	outshort(s1);
	outlong(l1);
	outlong(l2);
	logn(1, (int)l1);
	logn(1, (int)l2);
	outcopy(strlen(s)+1, s);
	i=strlen(s);
	if(i>99)
		i=99;
	buf[i]=0;
	while(--i>=0)
		buf[i]=s[i];
	log(1, buf);
	outsend();
}
outTsll(type, s, l1, l2)
	Hmesg type;
	Posn l1, l2;
{
	outstart(type);
	outshort(s);
	outlong(l1);
	outlong(l2);
	logn(1, (int)l1);
	logn(1, (int)l2);
	outsend();
}
outTsl(type, s, l)
	Hmesg type;
	Posn l;
{
	outstart(type);
	outshort(s);
	outlong(l);
	logn(1, (int)l);
	outsend();
}
outstart(type)
	Hmesg type;
{
	switch(type){
	case Hnewname:
		log(1, "Hnewname");
		break;
	case Hdelname:
		log(1, "Hdelname");
		break;
	case Hmovname:
		log(1, "Hmovname");
		break;
	case Hbindname:
		log(1, "Hbindname");
		break;
	case Hcurrent:
		log(1, "Hcurrent");
		break;
	case Hgrow:
		log(1, "Hgrow");
		break;
	case Hdata:
		log(1, "Hdata");
		break;
	case Hgrowdata:
		log(1, "Hgrowdata");
		break;
	case Hcut:
		log(1, "Hcut");
		break;
	case Hclean:
		log(1, "Hclean");
		break;
	case Hdirty:
		log(1, "Hdirty");
		break;
	case Hcheck0:
		log(1, "Hcheck0");
		break;
	case Hcheck:
		log(1, "Hcheck");
		break;
	case Hmoveto:
		log(1, "Hmoveto");
		break;
	case Hsetdot:
		log(1, "Hsetdot");
		break;
	case Horigin:
		log(1, "Horigin");
		break;
	case Hclose:
		log(1, "Hclose");
		break;
	case Hunlock:
		log(1, "Hunlock");
		break;
	case Hsetpat:
		log(1, "Hsetpat");
		break;
	case Hsetsnarf:
		log(1, "Hsetsnarf");
		break;
	case Hsnarflen:
		log(1, "Hsnarflen");
		break;
	case Hexit:
		log(1, "Hexit");
		break;
	}
	outdata[0]=type;
	outcount=0;
}
outcopy(count, data)
	register uchar *data;
{
	while(count--)
		outdata[3+outcount++]=*data++;	
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
	if(outcount>DATASIZE)
		panic("outcount>DATASIZE");
	outdata[1]=outcount;
	outdata[2]=outcount>>8;
	Write(1, outdata, outcount+3);
}
