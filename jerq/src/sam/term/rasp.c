#include "samterm.h"

rinit(r)
	register Rasp *r;
{
	r->nbytes=0;
	r->sect=0;
}
rclear(r)
	register Rasp *r;
{
	register Section *s, *ns;
	for(s=r->sect; s; s=ns){
		ns=s->next;
		gcfree(s->text);
		free(s);
	}
	r->sect=0;
}
Section *
rsinsert(r, s)	/* insert before s */
	register Rasp *r;
	register Section *s;
{
	register Section *t=(Section *)ALLOC(sizeof(Section));
	register Section *u;
	if(r->sect==s){	/* includes empty list case: r->sect==s==0 */
		r->sect=t;
		t->next=s;
	}else{
		u=r->sect;
		if(u==0)
			panic("rsinsert 1");
		do{
			if(u->next==s){
				t->next=s;
				u->next=t;
				goto Return;
			}
			u=u->next;
		}while(u);
		panic("rsinsert 2");
	}
    Return:
	return t;	/* for cyntax */
}
rsdelete(r, s)
	register Rasp *r;
	register Section *s;
{
	register Section *t;
	if(s==0)
		panic("rsdelete");
	if(r->sect==s){
		r->sect=s->next;
		goto Free;
	}
	for(t=r->sect; t; t=t->next)
		if(t->next==s){
			t->next=s->next;
	Free:
			gcfree(s->text);	/* freeing 0 is ok */
			free(s);
			return;
		}
	panic("rsdelete 2");
}
splitsect(r, s, n0)
	register Rasp *r;
	register Section *s;
	register long n0;
{
	if(s==0)
		panic("splitsect");
	rsinsert(r, s->next);
	if(s->text==0)
		s->next->text=0;
	else{
		GCALLOC(TBLOCKSIZE+1, &s->next->text);
		copystr(s->text+n0, s->next->text);
		s->text[n0]=0;
	}
	s->next->nbytes=s->nbytes-n0;
	s->nbytes=n0;
}
Section *
findsect(r, s, p, q)	/* find sect containing q and put q on a sect boundary */
	register Rasp *r;
	register Section *s;
	register long p, q;
{
	if(s==0 && p!=q)
		panic("findsect");
	for(; s && p+s->nbytes<=q; s=s->next)
		p+=s->nbytes;
	if(p!=q){
		splitsect(r, s, q-p);
		s=s->next;
	}
	return s;
}
rresize(r, a, old, new)
	register Rasp *r;
	register long a;
	long old, new;
{
	register Section *s, *t, *ns;
	s=findsect(r, r->sect, 0L, a);
	t=findsect(r, s, a, a+old);
	for(; s!=t; s=ns){
		ns=s->next;
		rsdelete(r, s);
	}
	/* now insert the new piece before t */
	if(new>0){
		ns=rsinsert(r, t);
		ns->nbytes=new;
		ns->text=0;
	}
	r->nbytes+=new-old;
}
rdata(r, p0, p1, cp)
	register Rasp *r;
	register long p0, p1;
	uchar *cp;
{
	register Section *s, *t, *ns;
	s=findsect(r, r->sect, 0L, p0);
	t=findsect(r, s, p0, p1);
	for(; s!=t; s=ns){
		ns=s->next;
		if(s->text)
			panic("rdata");
		rsdelete(r, s);
	}
	p1-=p0;
	s=rsinsert(r, t);
	GCALLOC(TBLOCKSIZE+1, &s->text);
	copystr(cp, s->text);
	s->nbytes=p1;
}
rclean(r)
	register Rasp *r;
{
	register Section *s;
	for(s=r->sect; s; s=s->next)
		while(s->next && (s->text!=0)==(s->next->text!=0)){
			if(s->text){
				if(s->nbytes+s->next->nbytes>TBLOCKSIZE)
					break;
				copystr(s->next->text, s->text+s->nbytes);
			}
			s->nbytes+=s->next->nbytes;
			rsdelete(r, s->next);
		}
}
uchar **
rload(r, p0, p1)
	register Rasp *r;
	register unsigned long p0, p1;
{
	register Section *s;
	register long p;
	register n, nb=0;
	strgrow(&scratch, &nscralloc, (int)(p1-p0+1));
	scratch[0]=0;
	for(p=0,s=r->sect; s && p+s->nbytes<=p0; s=s->next)
		p+=s->nbytes;
	while(p<p1 && s){
		/*
		 * Subtle and important.  If we are preparing to handle an 'rdata'
		 * call, it's because we have an 'rresize' hole here, so the
		 * screen doesn't have data for that space anyway (it got cut
		 * first).  So pretend it isn't there.
		 */
		if(s->text){
			n=s->nbytes-(p0-p);
			if(n>p1-p0)	/* all in this section */
				n=p1-p0;
			copystrn(s->text+(p0-p), scratch+nb, n);
			nb+=n;
		}
		p+=s->nbytes;
		p0=p;
		s=s->next;
	}
	return &scratch;
}
rmissing(r, p0, p1)
	register Rasp *r;
	register unsigned long p0, p1;
{
	register Section *s;
	register long p;
	register n;
	register nm=0;
	for(p=0,s=r->sect; s && p+s->nbytes<=p0; s=s->next)
		p+=s->nbytes;
	while(p<p1 && s){
		if(s->text==0){
			n=s->nbytes-(p0-p);
			if(n>p1-p0)	/* all in this section */
				n=p1-p0;
			nm+=n;
		}
		p+=s->nbytes;
		p0=p;
		s=s->next;
	}
	return nm;
}
rcontig(r, p0, p1, text)
	register Rasp *r;
	register unsigned long p0, p1;
{
	register Section *s;
	register long p;
	register long n;
	register np=0;
	for(p=0,s=r->sect; s && p+s->nbytes<=p0; s=s->next)
		p+=s->nbytes;
	while(p<p1 && s && (text? (int)s->text : (s->text==0))){
		n=s->nbytes-(p0-p);
		if(n>p1-p0)	/* all in this section */
			n=p1-p0;
		np+=n;
		p+=s->nbytes;
		p0=p;
		s=s->next;
	}
	return np;
}
copystrn(from, to, n)
	register uchar *from, *to;
	register n;
{
	if(n) do
		*to++ = *from++;
	while(--n);
	*to=0;
}
