#include "sam.h"

/*
 * Files are splayed out a factor of NDISC to reduce indirect block access
 */
Discdesc	*files[NDISC];
Discdesc	*transcripts[NDISC];
Buffer		*undobuf;
static String	*ftempstr();
int		fcount;

enum{
	SKIP=50,	/* max dist between file changes folded together */
	MAXCACHE=25000	/* max length of cache. must be < 32K-BLOCKSIZE */
};

Fstart()
{
	undobuf=Bopen(Dstart());
	snarfbuf=Bopen(Dstart());
	unixbuf=Bopen(Dstart());
}
File *
Fopen()
{
	register File *f;
	f=new(File, 1);
	if(files[fcount]==0){
		files[fcount]=Dstart();
		transcripts[fcount]=Dstart();
	}
	f->buf=Bopen(files[fcount]);
	f->transcript=Bopen(transcripts[fcount]);
	if(++fcount==NDISC)
		fcount=0;
	f->nbytes=0;
	f->markp=0;
	f->mod=0;
	f->dot.f=f;
	strinit(&f->name);
	straddc(&f->name, '\0');
	strinit(&f->cache);
	f->state=Unread;
	Fmark(f, (Mod)0);
	return f;
}
Fclose(f)
	File *f;
{
	Bclose(f->buf);
	Bclose(f->transcript);
	strclose(&f->name);
	strclose(&f->cache);
	if(f->rasp)
		listfree(f->rasp);
	free((uchar *)f);
}
Fmark(f, m)
	register File *f;
	Mod m;
{
	register Buffer *t=f->transcript;
	Posn p;
	if(f->state==Unread)	/* this is implicit 'e' of a file */
		return;
	p=m==0? -1 : f->markp;
	f->markp=t->nbytes;
	puthdr_M(t, p, f->dot.r, f->mark, f->mod, f->state);
	f->marked=TRUE;
	f->mod=m;
	f->hiposn=-1;
	/* Safety first */
	f->cp1=f->cp2=0;
	strzero(&f->cache);
}
Finsert(f, str, p1)
	register File *f;
	String *str;
	Posn p1;
{
	register Buffer *t=f->transcript;
	if(str->n==0)
		return;
	if(str->n<0 || str->n>32767)
		panic("Finsert");
	if(f->mod<modnum)
		Fmark(f, modnum);
	if(p1<f->hiposn)
		error(Esequence);
	if(str->n>=BLOCKSIZE){	/* don't bother with the cache */
		Fflush(f);
		puthdr_csP(t, 'i', str->n, p1);
		Binsert(t, str, t->nbytes);
	}else{	/* insert into the cache instead of the transcript */
		if(f->cp2==0 && f->cp1==0 && f->cache.n==0)	/* empty cache */
			f->cp1=f->cp2=p1;
		if(p1-f->cp2>SKIP || (long)f->cache.n+(long)str->n>MAXCACHE){
			Fflush(f);
			f->cp1=f->cp2=p1;
		}
		if(f->cp2!=p1){	/* grab the piece in between */
			unsigned char buf[SKIP];
			String s;
			Fchars(f, buf, f->cp2, p1);
			s.s=buf;
			s.n=p1-f->cp2;
			strinsert(&f->cache, &s, (long)f->cache.n);
			f->cp2=p1;
		}
		strinsert(&f->cache, str, (long)f->cache.n);
	}
	if(f!=cmd)
		quitok=FALSE;
	f->closeok=FALSE;
	if(f->state==Clean)
		state(f, Dirty);
	f->hiposn=p1;
}
Fdelete(f, p1, p2)
	register File *f;
	Posn p1, p2;
{
	if(p1==p2)
		return;
	if(f->mod<modnum)
		Fmark(f, modnum);
	if(p1<f->hiposn)
		error(Esequence);
	if(p1-f->cp2>SKIP)
		Fflush(f);
	if(f->cp2==0 && f->cp1==0 && f->cache.n==0)	/* empty cache */
		f->cp1=f->cp2=p1;
	if(f->cp2!=p1){	/* grab the piece in between */
		if(f->cache.n+(p1-f->cp2)>MAXCACHE){
			Fflush(f);
			f->cp1=f->cp2=p1;
		}else{
			unsigned char buf[SKIP];
			String s;
			Fchars(f, buf, f->cp2, p1);
			s.s=buf;
			s.n=p1-f->cp2;
			strinsert(&f->cache, &s, (long)f->cache.n);
		}
	}
	f->cp2=p2;
	if(f!=cmd)
		quitok=FALSE;
	f->closeok=FALSE;
	if(f->state==Clean)
		state(f, Dirty);
	f->hiposn=p2;
}
Fflush(f)
	register File *f;
{
	register Buffer *t=f->transcript;
	register Posn p1=f->cp1, p2=f->cp2;
	if(p1!=p2)
		puthdr_cPP(t, 'd', p1, p2);
	if(f->cache.n){
		puthdr_csP(t, 'i', f->cache.n, p2);
		Binsert(t, &f->cache, t->nbytes);
		strzero(&f->cache);
	}
	f->cp1=f->cp2=0;
}
Fsetname(f)
	register File *f;
{
	register Buffer *t=f->transcript;
	if(f->state==Unread){	/* This is setting initial file name */
		strdupstr(&f->name, &genstr);
		sortname(f);
	}else{
		if(f->mod<modnum)
			Fmark(f, modnum);
		if(genstr.n>BLOCKSIZE)
			error(Elong);
		puthdr_cs(t, 'f', genstr.n);
		Binsert(t, &genstr, t->nbytes);
	}
}
/*
 * The heart of it all. Fupdate will run along the transcript list, executing
 * the commands and converting them into their inverses for a later undo pass.
 * The pass runs top to bottom, so addresses in the transcript are tracked
 * (by the var. delta) so they stay valid during the operation.  This causes
 * all operations to appear to happen simultaneously, which is why the addresses
 * passed to Fdelete and Finsert never take into account other changes occurring
 * in this command (and is why things are done this way).
 */
Fupdate(f, isundo, toterm)
	register File *f;
{
	register Buffer *t=f->transcript;
	register Buffer *u=undobuf;
	register n, ni;
	register Posn p0, p1, p2, p, deltadot=0, deltamark=0, delta=0;
	int changes=FALSE;
	uchar buf[32];
	uchar tmp[BLOCKSIZE];
	Fflush(f);
	if(f->marked)
		p0=f->markp+sizeof(Mark);
	else
		p0=0;
	while((n=Bread(t, buf, sizeof buf, p0))>0){
		switch(buf[0]){
		default:
			panic("unknown in Fupdate");
		case 'd':
			GETPOSN(p1, buf+1);
			GETPOSN(p2, buf+1+sizeof(Posn));
			p0+=1+2*sizeof(Posn);
			if(p2<=f->dot.r.p1)
				deltadot-=p2-p1;
			if(p2<=f->mark.p1)
				deltamark-=p2-p1;
			p1+=delta, p2+=delta;
			delta-=p2-p1;
			if(!isundo)
				for(p=p1; p<p2; p+=ni){
					if(p2-p>BLOCKSIZE)
						ni=BLOCKSIZE;
					else
						ni=p2-p;
					puthdr_csP(u, 'i', ni, p1);
					Bread(f->buf, tmp, ni, p);
					Binsert(u, ftempstr(tmp, ni), u->nbytes);
				}
			f->nbytes-=p2-p1;
			Bdelete(f->buf, p1, p2);
			changes=TRUE;
			break;
		case 'f':
			n=buf[1]&0xFF;
			n|=buf[2]<<8;
			p0+=1+SS;
			strinsure(&genstr, (ulong)n);
			Bread(t, tmp, n, p0);
			p0+=n;
			strdup(&genstr, tmp);
			if(!isundo){
				puthdr_cs(u, 'f', f->name.n);
				Binsert(u, &f->name, u->nbytes);
			}
			strdupstr(&f->name, &genstr);
			sortname(f);
			changes=TRUE;
			break;
		case 'i':
			n=buf[1]&0xFF;
			n|=buf[2]<<8;
			GETPOSN(p1, (buf+1+SS));
			p0+=1+SS+sizeof(Posn);
			if(p1<f->dot.r.p1)
				deltadot+=n;
			if(p1<f->mark.p1)
				deltamark+=n;
			p1+=delta;
			delta+=n;
			if(!isundo)
				puthdr_cPP(u, 'd', p1, p1+n);
			changes=TRUE;
			f->nbytes+=n;
			while(n>0){
				if(n>BLOCKSIZE)
					ni=BLOCKSIZE;
				else
					ni=n;
				Bread(t, tmp, ni, p0);
				Binsert(f->buf, ftempstr(tmp, ni), p1);
				n-=ni;
				p1+=ni;
				p0+=ni;
			}
			break;
		}
	}
	toterminal(f, toterm);
	f->dot.r.p1+=deltadot;
	f->dot.r.p2+=deltadot;
	if(f->dot.r.p1>f->nbytes)
		f->dot.r.p1=f->nbytes;
	if(f->dot.r.p2>f->nbytes)
		f->dot.r.p2=f->nbytes;
	f->mark.p1+=deltamark;
	f->mark.p2+=deltamark;
	if(f->mark.p1>f->nbytes)
		f->mark.p1=f->nbytes;
	if(f->mark.p2>f->nbytes)
		f->mark.p2=f->nbytes;
	if(n<0)
		panic("Fupdate read");
	if(f==cmd)
		f->mod=0;	/* can't undo command file */
	if(p0>f->markp+sizeof(Posn)){	/* for undo, this throws away the undo transcript */
		if(f->mod>0){	/* can't undo the dawn of time */
			Bdelete(t, f->markp+sizeof(Mark), t->nbytes);
			/* copy the undo list back into the transcript */
			for(p=0; p<u->nbytes; p+=ni){
				if(u->nbytes-p>BLOCKSIZE)
					ni=BLOCKSIZE;
				else
					ni=u->nbytes-p;
				Bread(u, tmp, ni, p);
				Binsert(t, ftempstr(tmp, ni), t->nbytes);
			}
		}
		Bdelete(u, (Posn)0, u->nbytes);
	}
	return f==cmd? FALSE : changes;
}
puthdr_csP(b, c, s, p)
	Buffer *b;
	char c;
	short s;
	Posn p;
{
	uchar buf[1+SS+sizeof p];
	register uchar *a=buf;
	if(p<0)
		panic("puthdr_csP");
	*a++=c;
	*a++=s;
	*a++=s>>8;
	PUTPOSN(a, &p);
	Binsert(b, ftempstr(buf, sizeof buf), b->nbytes);
}
puthdr_cs(b, c, s)
	Buffer *b;
	char c;
	short s;
{
	uchar buf[1+SS];
	register uchar *a=buf;
	*a++=c;
	*a++=s;
	*a=s>>8;
	Binsert(b, ftempstr(buf, sizeof buf), b->nbytes);
}
puthdr_M(b, p, dot, mk, m, s1)
	Buffer *b;
	Posn p;
	Range dot;
	Range mk;
	Mod m;
	short s1;
{
	Mark mark;
	static first=1;
	if(!first && p<0)
		panic("puthdr_M");
	mark.p=p;
	mark.dot=dot;
	mark.mark=mk;
	mark.m=m;
	mark.s1=s1;
	Binsert(b, ftempstr((uchar *)&mark, sizeof mark), b->nbytes);
}
puthdr_cPP(b, c, p1, p2)
	Buffer *b;
	char c;
	Posn p1, p2;
{
	uchar buf[1+2*sizeof p1];
	register uchar *a=buf;
	if(p1<0 || p2<0)
		panic("puthdr_cPP");
	*a++=c;
	PUTPOSN(a, &p1);
	PUTPOSN(a, &p2);
	Binsert(b, ftempstr(buf, sizeof buf), b->nbytes);
}
Fchars(f, addr, p1, p2)
	register File *f;
	uchar *addr;
	Posn p1, p2;
{
	return Bread(f->buf, addr, (int)(p2-p1), p1);
}
Fgetcset(f, p)
	File *f;
	Posn p;
{
	if(p<0 || p>f->nbytes)
		panic("Fgetcset out of range");
	if((f->ngetc=Fchars(f, f->getcbuf, p, p+NGETC))<0)
		panic("Fgetcset Bread fail");
	f->getcp=p;
	f->getci=0;
	return f->ngetc;
}
Fbgetcset(f, p)
	File *f;
	Posn p;
{
	if(p<0 || p>f->nbytes)
		panic("Fbgetcset out of range");
	if((f->ngetc=Fchars(f, f->getcbuf, p<NGETC? (Posn)0 : p-NGETC, p))<0)
		panic("Fbgetcset Bread fail");
	f->getcp=p;
	f->getci=f->ngetc;
	return f->ngetc;
}
Fgetcload(f, p)
	File *f;
	Posn p;
{
	if(Fgetcset(f, p)){
		--f->ngetc;
		f->getcp++;
		return f->getcbuf[f->getci++];
	}
	return -1;
}
Fbgetcload(f, p)
	File *f;
	Posn p;
{
	if(Fbgetcset(f, p)){
		--f->getcp;
		return f->getcbuf[--f->getci];
	}
	return -1;
}
static String *
ftempstr(s, n)
	uchar *s;
{
	static String p;
	p.s=(uchar *)s;
	p.n=n;
	p.size=n;
	return &p;
}
