#include "jim.h"
/*
 *	Allocator.  Manages arena between initial bss and garbage-compacted
 *	arena.  Uses shiftgcarena() when it needs to expand its dominion.
 */

#define	BRKINCR		4096
#define	LBRKINCR	(BRKINCR/sizeof(long))

typedef struct hdr{
	union{
		long	*Unext;	/* pointer to next object */
		long	Uinext;	/* integer next; odd if deallocated */
	}uu;
	long	nlongs;
}hdr;
#define	inext	uu.Uinext
#define	next	uu.Unext
#define	hp	((struct hdr *)p)

#define	HEADERSIZE	(sizeof(struct hdr)/sizeof(long))

static long	*basep;		/* beginning of arena; only set once */
static long	*endp;		/* end of arena */
static long	*nextp;		/* next one to be used */

allocinit(){
	basep=(long *)sbrk(0);
	if(brk((char *)(basep+LBRKINCR))!=0)
		allocerr();
	nextp=basep;
	endp=basep+LBRKINCR;
}
char *
alloc(nbytes)
	register ulong nbytes;
{
	register long *p, *q;
	register ulong nl;
	nbytes+=sizeof(long)-1;
	nbytes/=sizeof(long);	/* convert bytes to longs */
#define	Nlongs	nbytes
	Nlongs+=HEADERSIZE;
	/* look for exact match */
	for(p=basep; p<nextp; p=(long *)(hp->inext&~1))
		if((hp->inext&1) && hp->nlongs==Nlongs){
			hp->inext&=~1;
			return (char *)(p+HEADERSIZE);
		}
	/* try off the end */
	if(endp-nextp < Nlongs){
		nl=(nextp+Nlongs)-basep;		/* number we need */
		nl=((nl+LBRKINCR-1)/LBRKINCR)*LBRKINCR;	/* rounded up */
		nl-=(endp-basep);			/* minus number we have */
		if((int)sbrk((int)nl*sizeof(long))==-1)
			allocerr();
		shiftgcarena(nl);
		endp+=nl;
	}
	p=nextp;
	nextp+=Nlongs;
	hp->nlongs=Nlongs;
	hp->next=nextp;
	for(q=p+HEADERSIZE, Nlongs-=HEADERSIZE; Nlongs-->0; )
		*q++=0;
	return (char *)(p+HEADERSIZE);
}
free(cp)
	char *cp;
{
	register long *p=(long *)cp;
	if(p<=basep || nextp<=p)
		panic("free");
	p-=HEADERSIZE;
	hp->inext|=1;
}
allocerr()
{
	error("allocation failure", (char *)0);
}
char *
malloc(n)	/* for libraries */
	unsigned n;
{
	return alloc((ulong)n);
}
