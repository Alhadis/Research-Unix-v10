#include "sam.h"
/*
 * Garbage-compacting allocator
 *
 * called as:	gcalloc(nbytes, where)
 *		gcrealloc(p, nbytes)
 *		gcfree(p)
 *	nbytes is unsigned long
 *	where is a pointer to the location which will point to the area
 *	(e.g. base in a Bitmap), to be updated when necessary.
 *	The return values  for the allocators are the new address,
 *	but since they update *where, the value isn't too useful.
 *	They panic if they fail.
 */

/*
 * The arena is allocated in longs, to speed up compaction
 * Garbage is compacted towards the bottom (low address end) of the arena.
 * A block has a struct header as its bottom HEADERSIZE longs for use by gcfree.
 * header.pval points to the USER location where the pointer to the block
 * is stored; this cell must be updated after a compaction.
 * Deallocated blocks have header.pval odd.
 */

#define	BRKINCR		(16*1024)	/* bytes to add to arena each time */
#define LBRKINCR	(BRKINCR/sizeof(long))
#define	FREE		1
#define	HEADERSIZE	sizeof(struct header)/sizeof(long)

static long	*nextlong;
static long	*startarena;
static long	*endarena;
static		compact();
int		dontcompact;

extern char	*brk();
extern char	*sbrk();

struct header{
	union uuu{
		long	Uival;		/* integer value of where */
		long	**Upval;	/* where */
	}u;
	ulong nlongs;			/* # of longs in the user's data */
};
#define	ival	u.Uival
#define	pval	u.Upval
#define	hp	((struct header *)p)

uchar *
gcalloc(nbytes, where)
	register ulong nbytes;
	long **where;
{
	register long *p;
	register ulong nl;
	if(nextlong>endarena)
		panic("nextlong>endarena");
	if((long)where&FREE)	/* head off a possible disaster */
		panic("where&FREE");
	if(startarena==0){
		if((int)(startarena=(long *)sbrk(BRKINCR))==-1)
			error(Ealloc);
		endarena=startarena+(BRKINCR/sizeof(long));
		nextlong=startarena;
	}
	nbytes+=sizeof(long)-1;
	nbytes/=sizeof(long);	/* convert bytes to longs */
#define	Nlongs	nbytes
	Nlongs+=HEADERSIZE;
	if(endarena-nextlong < Nlongs){
		if(!dontcompact)
			compact();
		if(endarena-nextlong < Nlongs){
			nl=(nextlong+Nlongs)-startarena;
			/* if !compacting, avoid greed */
			if(!dontcompact)
				nl=((nl+LBRKINCR-1)/LBRKINCR)*LBRKINCR;
			if(brk((char *)(startarena+nl))!=0)
				error(Ealloc);
			endarena=startarena+nl;
		}
	}			
	p=nextlong;
	hp->pval=where;
	hp->nlongs=Nlongs;
	nextlong+=Nlongs;
	return (uchar *)(*(hp->pval)=p+HEADERSIZE);
}
uchar *
gcrealloc(cp, nbytes)
	uchar *cp;
	register ulong nbytes;
{
	register long *p=(long *)cp, *q;
	register long *newp;
	register long **ptrold;
	register n;
	long *x;
	p-=HEADERSIZE;	/* the header */
	n=hp->nlongs;
	nbytes+=sizeof(long)-1;
	nbytes/=sizeof(long);	/* convert bytes to longs; nbytes is now Nlongs */
	ptrold=hp->pval; /* location that will be updated if compaction occurs */
	/* we give where==x to gcalloc to avoid collision with old header */
	newp=(long *)gcalloc((ulong)(Nlongs*sizeof(long)), &x);
	/* now it's safe to have both headers point to the same place */
	((struct header *)(newp-HEADERSIZE))->pval=ptrold;
	p=*ptrold;
	q=newp;
	if(n>Nlongs)
		n=Nlongs;
	if(n>0) do
		*q++= *p++;
	while(--n);
	(void)gcfree((uchar *)*ptrold);
	return (uchar *)(*ptrold=newp);
}
shiftgcarena(nl)
	ulong nl;
{
	register long *p;
	if(startarena==0 || dontcompact)
		return;
	bcopy((uchar *)startarena, (uchar *)nextlong, (uchar *)(startarena+nl), -1);
	nextlong+=nl;
	startarena+=nl;
	endarena+=nl;
	for(p=startarena; p<nextlong; p+=hp->nlongs){
		if((hp->ival&FREE)==0)
			*(hp->pval)+=nl;
	}
}
gcfree(cp)
	uchar *cp;
{
	register long *p=(long *)cp;
	if(p==0)
		return;
	p-=HEADERSIZE;
	if(p<startarena || nextlong<=p)
		panic("gcfree");
	hp->ival|=FREE;
}
static
compact()
{
	register long *w, *p;
	register ulong n;

	w=p=startarena;
	while(p<nextlong){
		if(hp->ival&FREE){
			p+=hp->nlongs;
			continue;
		}
		if(w==p){
			w+=hp->nlongs;
			p+=hp->nlongs;
			continue;
		}
		*(hp->pval)=w+HEADERSIZE; /* update *where */
		*w++=hp->ival;
		*w++=n=hp->nlongs;
		p+=HEADERSIZE;
		if((n-=HEADERSIZE)>0) do
			*w++ = *p++;
		while(--n);
	}
	nextlong=w;
}
gcchk()
{
	register long *p;
	if(startarena==0)
		return;
	for(p=startarena; p<nextlong; p+=hp->nlongs)
		if((hp->ival&FREE)==0){
			if(hp->pval==0 || ((long *)hp->pval>=startarena && ((int)(hp->pval)&0x70000000L)==0))
				panic("gcchk 1");
			if(p+hp->nlongs>nextlong)
				panic("gcchk 2");
		}
}
