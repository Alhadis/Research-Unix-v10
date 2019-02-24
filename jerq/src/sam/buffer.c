#include "sam.h"

static	flush();
static	setcache();

Buffer *
Bopen(dd)
	Discdesc *dd;
{
	register Buffer *b;
	b=new(Buffer, 1);
	b->disc=Dopen(dd);
	strinit(&b->cache);
	return b;
}
Bclose(b)
	register Buffer *b;
{
	Dclose(b->disc);
	strclose(&b->cache);
	free((uchar *)b);
}
int
Bread(b, addr, n, p0)
	register Buffer *b;
	uchar *addr;
	int n;
	Posn p0;
{
	register m;
	if(b->c2>b->disc->nbytes || b->c1>b->disc->nbytes)
		panic("bread cache");
	if(p0<0)
		panic("Bread p0<0");
	if(p0+n>b->nbytes){
		n=b->nbytes-p0;
		if(n<0)
			panic("Bread<0");
	}
	if(!incache(b, p0, p0+n)){
		flush(b);
		if(n>=BLOCKSIZE/2)
			return Dread(b->disc, addr, n, p0);
		else{
			Posn minp;
			if(b->nbytes-p0>BLOCKSIZE/2)
				m=BLOCKSIZE/2;
			else
				m=b->nbytes-p0;
			if(m<n)
				m=n;
			minp=p0-BLOCKSIZE/2;
			if(minp<0)
				minp=0;
			m+=p0-minp;
			strinsure(&b->cache, (ulong)m);
			if(Dread(b->disc, b->cache.s, m, minp)!=m)
				panic("Bread");
			b->cache.n=m;
			b->c1=minp;
			b->c2=minp+m;
			b->dirty=FALSE;
		}
	}
	bcopy(&b->cache.s[p0-b->c1], &b->cache.s[p0-b->c1+n], addr, 1);
	return n;
}
Binsert(b, s, p0)
	register Buffer *b;
	String *s;
	Posn p0;
{
	if(b->c2>b->disc->nbytes || b->c1>b->disc->nbytes)
		panic("binsert cache");
	if(p0<0)
		panic("Binsert p0<0");
	if(s->n==0)
		return;
	if(incache(b, p0, p0)){
		strinsert(&b->cache, s, p0-b->c1);
		b->dirty=TRUE;
		if(b->cache.n>BLOCKSIZE*2){
			b->nbytes+=s->n;
			flush(b);
			/* try to leave some cache around p0 */
			if(p0>=b->c1+BLOCKSIZE){
				/* first BLOCKSIZE can go */
				strdelete(&b->cache, 0, BLOCKSIZE);
				b->c1+=BLOCKSIZE;
			}else if(p0<=b->c2-BLOCKSIZE){
				/* last BLOCKSIZE can go */
				b->cache.n-=BLOCKSIZE;
				b->c2-=BLOCKSIZE;
			}else{
				/* too hard; negate the cache and pick up next time */
				b->cache.n=0;
				b->c1=b->c2=0;
			}
			return;
		}
	}else{
		flush(b);
		if(s->n>=BLOCKSIZE/2){
			b->cache.n=0;
			b->c1=b->c2=0;
			Dinsert(b->disc, s->s, s->n, p0);
		}else{
			register m;
			Posn minp;
			if(b->nbytes-p0>BLOCKSIZE/2)
				m=BLOCKSIZE/2;
			else
				m=b->nbytes-p0;
			minp=p0-BLOCKSIZE/2;
			if(minp<0)
				minp=0;
			m+=p0-minp;
			strinsure(&b->cache, (ulong)m);
			if(Dread(b->disc, b->cache.s, m, minp)!=m)
				panic("Bread");
			b->cache.n=m;
			b->c1=minp;
			b->c2=minp+m;
			strinsert(&b->cache, s, p0-b->c1);
			b->dirty=TRUE;
		}
	}
	b->nbytes+=s->n;
}
Bdelete(b, p1, p2)
	register Buffer *b;
	Posn p1, p2;
{
	if(p1<0 || p2<0)
		panic("Bdelete p<0");
	if(b->c2>b->disc->nbytes || b->c1>b->disc->nbytes)
		panic("bdelete cache");
	if(p1==p2)
		return;
	if(incache(b, p1, p2)){
		strdelete(&b->cache, p1-b->c1, p2-b->c1);
		b->dirty=TRUE;
	}else{
		flush(b);
		Ddelete(b->disc, p1, p2);
		b->cache.n=0;
		b->c1=b->c2=0;
	}
	b->nbytes-=(p2-p1);
}
static
flush(b)
	register Buffer *b;
{
	if(b->dirty){
		Dreplace(b->disc, b->c1, b->c2, b->cache.s, b->cache.n);
		b->c2=b->c1+b->cache.n;
		b->dirty=FALSE;
		if(b->nbytes!=b->disc->nbytes)
			panic("flush");
	}
}
int hits, misses;
incache(b, p1, p2)
	Buffer *b;
	Posn p1, p2;
{
if(b->c1<=p1 && p2<=b->c1+b->cache.n)hits++; else misses++;
	return b->c1<=p1 && p2<=b->c1+b->cache.n;
}
static
setcache(b, s, p0)
	register Buffer *b;
	String *s;
	Posn p0;
{
	strdelete(&b->cache, (long)0, (long)b->cache.n);
	strinsert(&b->cache, s, (long)0);
	b->c1=p0;
	b->c2=p0+s->n;
	b->dirty=FALSE;
}
