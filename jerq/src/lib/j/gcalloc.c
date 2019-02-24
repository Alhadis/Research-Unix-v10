#include <jerq.h>
#include <setup.h>
/*
 * Garbage-collecting allocator
 *
 * called as:	gcalloc(nbytes, where)
 *	nbytes is unsigned long
 *	where is a pointer to the location which will point to the area
 *	(e.g. base in a Bitmap), to be updated when necessary
 */

/*
 * The arena is allocated in longs, to speed up compaction
 * Garbage is compacted towards the top (high address end) of the arena,
 * i.e. away from the regular, non-compacting, allocator.
 * A block has a struct header as its top HEADERSIZE longs, and
 * a pointer to the struct header as its bottom long, for use by gcfree.
 * header.caller points to the allocating process so things can be cleaned
 * up when it exits.
 * header.pval points to the USER location where the pointer to the block
 * is stored; this cell must be updated after a compaction.
 * Deallocated blocks have header.pval odd.
 */
extern char *allocendp;	/* defined in alloc.c */
#define	STARTAREA	(long *)(allocendp)
long	*ENDAREA;
static long *nextlong;
struct header{
	union uuu{
		long	Uival;		/* integer value of where */
		long	**Upval;	/* where */
	}u;
	long	*caller;		/* which process allocated */
	unsigned long nlongs;		/* # of longs in the user's data */
};
#define	ival	u.Uival
#define	pval	u.Upval
#define	HEADERSIZE	sizeof(struct header)/sizeof(long)
#define	hp	((struct header *)p)

gcinit()
{
	ENDAREA=(long *)(maxaddr[VALMAXADDR]-4);
	nextlong=ENDAREA;	/* init; this runs in rom, so .data no good */
}

char *
realgcalloc(nbytes, where, caller)
	register unsigned long nbytes;
	long **where;
	char *caller;	/* really a Proc * */
{
	register retry = 0, i;
	register long *p, *q;
	if(nextlong==0)
		nextlong=ENDAREA;	/* initialization; this runs in rom */
	if((long)caller&1)	/* head off a possible disaster */
		return 0;
	nbytes+=sizeof(long)-1;
	nbytes>>=2;	/* convert bytes to longs */
#define	Nlongs	nbytes
	Nlongs+=HEADERSIZE+1;
	while(nextlong-STARTAREA < Nlongs){
		if(retry++)
			return 0;
		compact();
	}
	p=nextlong-HEADERSIZE;
	hp->caller=(long *)caller;
	hp->pval=where;
	hp->nlongs=Nlongs-HEADERSIZE-1;
	nextlong-=Nlongs;
	*nextlong=(long)p;	/* so we can find the header when we free */
	*hp->pval=nextlong+1;
	for(q=*hp->pval, i=0; i<hp->nlongs; i++)
		*q++=0;
	return (char *)(*hp->pval);
}
char *
gcalloc(n, w)
	unsigned long n;
	long **w;
{
	return realgcalloc(n, w, (char *)0);
}
gcfree(cp)
	register char *cp;
{
#define	p	((long *)cp)
	/* nice and safe */
	if(p>nextlong && p<ENDAREA){
		cp=(char *)(p[-1]);	/* pointer to the header */
		if(p>nextlong && p<ENDAREA)
			hp->ival|=1;
	}
#undef	p
}
gcfreeall(whichproc)
	char *whichproc;
{
	register long *p;
	for(p=ENDAREA; p>nextlong; p-=hp->nlongs+1){
		p-=HEADERSIZE;
		if(hp->caller==(long *)whichproc)
			hp->ival|=1;
	}
}
static
compact()
{
	register long *w, *p;
	register struct header *header;
	register unsigned long n;
	static Rectangle r={750, 0, 800, 50};

	rectf(&display, r, F_XOR);
	w=ENDAREA;
	p=w;
	while(p>nextlong){
		header=(struct header *)(p-HEADERSIZE);
		n = header->nlongs;
		if(header->ival&1){
			p -= n+HEADERSIZE+1;
			continue;
		}
		if(w==p){
			p -= n+HEADERSIZE+1;
			w -= n+HEADERSIZE+1;
			continue;
		}
		*--w = n;
		*--w = (long)header->caller;
		*--w = header->ival;
		p-=HEADERSIZE;
		header = (struct header *)w;
		*(((struct header *)w)->pval)=w-n; /* update *where */
		if(n>0) do
			*--w = *--p;
		while(--n);
		--p;
		*--w = (long)header;	/* back pointer to header */
	}
	nextlong=w;
	rectf(&display, r, F_XOR);
}
