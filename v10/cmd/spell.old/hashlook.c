/*	@(#)hashlook.c	1.4	*/
#include <stdio.h>
#include "hash.h"
#include "huff.h"

unsigned *table, *ctable;
unsigned short index[NI];
unsigned short cindex[NI];

#define  B (BYTE*sizeof(unsigned))
#define L (BYTE*sizeof(long)-1)
#define MASK (~(1L<<L))

#ifdef pdp11	/*sizeof(unsigned)==sizeof(long)/2 */
this version of spell is too big to run on a pdp11
#define fetch(wp,bp)\
	(((((long)wp[0]<<Bleng)|wp[1])<<(Bleng-bp))|(wp[2]>>bp))
#else 		/*sizeof(unsigned)==sizeof(long)*/
#define fetch(wp,bp) (bp==Bleng?wp[0]:((wp[0]<<(Bleng-bp))|(wp[1]>>bp)))
#endif

unsigned long codetable[EXTRA];

unsigned
hashlook(s)
char *s;
{
	long h;
	long t;
	register bp,Bleng,rw;
	register unsigned *wp;
	unsigned *cwp;
	int i,count,cbp;
	long sum;
	long y;
	unsigned *tp;

	h = hash(s);
	t = h>>(HASHWIDTH-INDEXWIDTH);
	wp = &table[index[t]];
	tp = &table[index[t+1]];
	cwp = &ctable[cindex[t]];
	count = 0;
	bp = Bleng = B;
	sum = (long)t<<(HASHWIDTH-INDEXWIDTH);
	rw = w;
	for(;;) {
		{/*	this block is equivalent to
			 bp -= decode((fetch(wp,bp)>>1)&MASK, &t);*/
			long v;
			y = (fetch(wp,bp)>>1) & MASK;
			if(y < cs) {
				t = y >> (L+QW-rw);
				bp -= rw-QW;
			}
			else {
				for(bp-=rw,v=v0; y>=qcs; y=(y<<QW)&MASK,v+=n)
					bp -= QW;
				t = v + (y>>(L-rw));
			}
		}
		while(bp<=0) {
			bp += Bleng;
			wp++;
		}
		if(wp>=tp&&(wp>tp||bp<Bleng))
			return(0);
		sum += t;
		if(sum<h) {
			count++;
			continue;
		}
		if(sum==h ) {
			i = (count * CODEWIDTH);
			cwp += i/B;
			cbp = B - (i%B);
			y = (fetch(cwp,cbp)>>(BYTE*sizeof(long)-CODEWIDTH))&
			((1<<CODEWIDTH)-1);
			return codetable[y];
		}
		else
			return 0;
	}
}


prime(argc,argv)
char **argv;
{
	register fd;
	register int size,sizei,i;
	extern char *malloc();
	long seekpt;
	 
	if(argc <= 1)
		return(0);
	if(sizeof(long) > sizeof(unsigned))
		abort();	/*wrong fetch macro*/
	if((fd = open(argv[1], 0)) == NULL)
		return(0);
	sizei = sizeof(*index) * NI;
	if((i = read(fd,(char*)codetable, sizeof(codetable))) == sizeof(codetable)
	&& rhuff(fd)
	&& (i = read(fd,(char*)index,sizei)) == sizei) {
		read(fd,(char*)&seekpt,sizeof(long));
		size = sizeof(*table) * index[NI-1];
		read(fd,(char*)cindex,sizei);
		if(((table = (unsigned*)malloc(size)) == 0)
		|| ( i = read(fd,(char*)table,size))
	   	!= size)
			return(0);
		lseek(fd,seekpt,0);
		size = sizeof(*ctable) * cindex[NI-1];
		if(((ctable = (unsigned*)malloc(size)) == 0)
		||(i = read(fd,(char*)ctable,size)) != size)
			return(0);
	}
	else  {
			return(0);
		}
	close(fd);
	hashinit();
	return(1);
}
