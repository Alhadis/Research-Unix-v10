#ifdef debug
#define ASSERT(p) if(!(p))botch("p");else
botch(s)
char *s;
{
	printf("assertion botched: %s\n",s);
	abort();
}
#else
#define ASSERT(p)
#endif

/*	C storage allocator
 *	circular first-fit strategy
 *	works with noncontiguous, but monotonically linked, arena
 *	each block is preceded by a ptr to the (pointer of) 
 *	the next following block
 *	blocks are exact number of words long 
 *	aligned to the data type requirements of ALIGN
 *	pointers to blocks must have BUSY bit 0
 *	bit in ptr is 1 for busy, 0 for idle
 *	gaps in arena are merely noted as busy blocks
 *	last block of arena is empty and
 *	has a pointer to first
 *	idle blocks are coalesced during space search
 *
 *	a different implementation may need to redefine
 *	ALIGN, NALIGN, BLOCK, BUSY, INT
 *	where INT is integer type to which a pointer can be cast
*/
#define INT int
#define ALIGN int
#define NALIGN 1
#define WORD sizeof(union store)
#define BLOCK 1024
#define BUSY 1
#define NULL 0
#define testbusy(p) ((INT)(p)&BUSY)
#define setbusy(p) (union store *)((INT)(p)|BUSY)
#define clearbusy(p) (union store *)((INT)(p)&~BUSY)

union store {
	      union store *ptr;
	      ALIGN dummy[NALIGN];
	      int calloc;	/*calloc clears an array of integers*/
};

static	union store alloca;	/* initial arena */
static	union store *allocb = &alloca;	/*arena base*/
static	union store *allocp = &alloca;	/*search ptr*/
static	union store *alloct = &alloca;	/*arena top; for speedier ialloc*/
static	union store allocx, allocy;	/*for benefit of realloc*/
extern	char *sbrk();

/* a cache list of frequently-used sizes is maintained. From each
 * cache entry hangs a chain of available blocks (marked busy
 * to keep out of the first fit search)  
*/
#define CACHEMAX 100	/* largest block to be cached (in words) */
#ifndef pdp11
#define CACHESIZ 13	/* number of entries (prime) */
#else
#define CACHESIZ 0
#endif
static struct cache {
	int size;
	union store *chain;
} cache[CACHESIZ];

char *
malloc(nbytes)
unsigned nbytes;
{
	register union store *p, *q;
	register nw;
	register temp;
	register struct cache *cp;

	ASSERT(allock(allocp)&1);
	nw = (nbytes+WORD+WORD-1)/WORD;
	if(CACHESIZ && nw<CACHEMAX && nw>2) {	/* see note + below */
		cp = cache + nw%CACHESIZ;
		p = cp->chain;
		if(p && nw==cp->size) {
			cp->chain = p[1].ptr;
			ASSERT(testbusy(p->ptr));
			ASSERT(clearbusy(p->ptr)-p==nw);
			return (char*)(p+1);
		}
	}
	ASSERT(allock(allocp)&3);
	for(; ; ) {	/* done at most twice */
		p = allocp;
		if(alloca.ptr!=0)		/*C can't initialize union*/
		for(temp=0; ; ) {
			if(!testbusy(p->ptr)) {
				while(!testbusy((q=p->ptr)->ptr)) {
					ASSERT(q>p);
					p->ptr = q->ptr;
				}
				allocp = p;
				if(q>=p+nw && p+nw>=p)	/*room; no wrap*/
					goto found;
			}
			q = p;
			p = clearbusy(p->ptr);
			if(p <= q) {
				ASSERT(p==allocb);
				if(++temp>1)
					break;
			}
		}
		temp = nw;
		p = (union store *)sbrk(0);
		q = allocp->ptr;
		if(!testbusy(q) && q+1 == p)
			temp -= p - allocp;
		temp = ((temp+BLOCK/WORD)/(BLOCK/WORD))*(BLOCK/WORD);
		if(p+temp <= p)
			return(NULL);
		for( ; ; ) {
			register s;
			q = (union store *)sbrk(temp*WORD);
			if((INT)q != -1)
				break;
			for(cp=cache; cp<cache+CACHESIZ; cp++) {
				for(p=cp->chain; p; p=p[1].ptr) {
					ASSERT(testbusy(p->ptr));
					p->ptr = clearbusy(p->ptr);
					ASSERT(p->ptr-p==cp->size);
				}
				cp->chain = 0;
			}
			s = (temp-nw)/2;
			if(s <= 0)
				return(NULL);
			temp -= s;
		}
		ialloc((char *)q, (unsigned)temp*WORD);
	}
found:
	q = p + nw;
	if(p->ptr > q) {
		allocx = *q;
		q->ptr = p->ptr;
	}
	p->ptr = setbusy(q);
	ASSERT(allock(allocp)&7);
	return((char *)(p+1));
}
/* + note CACHESIZ is tested for conditional compilation; nw>2 assumes there's
 * no point in caching smaller sizes (they are rarely used and always
 * can be allocated) and protects against the old storage compaction trick:
 * free(p); free(dummy);dummy=malloc(1);realloc(p)
 * (otherwise free(dummy) would destroy allocy, which pertains to p)
*/

/*	freeing strategy tuned for LIFO allocation
*/
free(ap)
char *ap;
{
	register union store *p = (union store *)ap, *q;
	register nw;
	register struct cache *cp;

	--p;
	ASSERT(allock(p));
	ASSERT(testbusy(p->ptr));
	nw = clearbusy(p->ptr) - p;
	ASSERT(nw>0);
	if(CACHESIZ && nw<CACHEMAX && nw>2) {
		cp = cache + nw%CACHESIZ;
		if(nw != cp->size) {
			q = cp->chain;
			if(q) {
				ASSERT(testbusy(q->ptr));
				q->ptr = clearbusy(q->ptr);
				ASSERT(q->ptr-q==cp->size);
				cp->chain = q[1].ptr;
			}
			if(!q)
				cp->size = nw;
			else
				goto nocache;
		}
		allocy = p[1];
		p[1].ptr = cp->chain;
		cp->chain = p;
		ASSERT(allock(allocp)&017);
		return;
	}
nocache:
	allocp = p;
	p->ptr = clearbusy(p->ptr);
	ASSERT(allock(allocp)&037);
	ASSERT(p->ptr<=alloct);
}

/* ialloc(q, nbytes) inserts a block that did not come
 * from malloc into the arena
 *
 * q points to new block
 * r points to last of new block
 * p points to last cell of arena before new block
 * s points to first cell of arena after new block
*/
ialloc(qq, nbytes)
char *qq;
unsigned nbytes;
{
	register union store *p, *q, *s;
	union store *r;

	q = (union store *)qq;
	r = q + (nbytes/WORD) - 1;
	q->ptr = r;
	if(alloca.ptr==0)		/*C can't initialize union*/
		alloca.ptr = &alloca;
	if(q > alloct) {
		p = alloct;
		alloct = r;
	} else
		allocp = p = allocb;
	for( ; ; p=s) {
		s = clearbusy(p->ptr);
		if(s==allocb)
			break;
		ASSERT(s>p);
		if(s>r) {
			if(p<q)
				break;
			else
				ASSERT(p>r);
		}
	}
	p->ptr = q==p+1? q: setbusy(q);
	r->ptr = s==r+1? s: setbusy(s);
	if(allocb > q)
		allocp = allocb = q;
}

/*	realloc(p, nbytes) reallocates a block obtained from malloc()
 *	and freed since last call of malloc()
 *	to have new size nbytes, and old content
 *	returns new location, or 0 on failure
*/

char *
realloc(pp, nbytes)
char *pp;
unsigned nbytes;
{
	register union store *q;
	register union store *p = (union store *)pp;
	register union store *s, *t;
	register unsigned nw;
	struct cache *cp;
	unsigned onw;

	q = p-1;
	ASSERT(allock(q));
	if(testbusy(q->ptr)) {
		allocp = q;
		q->ptr = clearbusy(q->ptr);
		if(CACHESIZ) {
			nw = q->ptr - q;
			cp = cache + nw%CACHESIZ;
			if(cp->chain==q) {
				ASSERT(cp->size==nw);
				cp->chain = p->ptr;	/*p->ptr==q[1].ptr*/
				*p = allocy;
			} else
				ASSERT(notonchain(q,cp->chain));
		}
	}
	onw = q->ptr - p;
	q = (union store *)malloc(nbytes);
	if(q==NULL || q==p)
		return((char *)q);
	ASSERT(q<p||q>p[-1].ptr);
	s = p;
	t = q;
	nw = (nbytes+WORD-1)/WORD;
	if(nw<onw)
		onw = nw;
	while(onw--!=0)
		*t++ = *s++;
	ASSERT(clearbusy(q[-1].ptr)-q==nw);
	if(q<p && q+nw>=p)
		q[q+nw-p] = allocx;
	ASSERT(allock(q-1));
	return((char *)q);
}

#ifdef debug

notonchain(p,q)
union store *p, *q;
{
	for(;q; q=clearbusy(q->ptr))
		if(q==p)
			return 0;
	return 1;
}

allock(q)
union store *q;
{
#ifdef longdebug
	register union store *p, *r;
	int x;
	for(x=0; x<CACHESIZ; x++) {
		p = cache[x].chain;
		if(p==0)
			continue;
		for( ; p; p=p[1].ptr) {
			ASSERT(testbusy(p->ptr));
			ASSERT(clearbusy(p->ptr)-p==cache[x].size);
		}
	}
	x = 0;
	p = allocb;
	if(alloca.ptr==0)
		return(1);
	for( ; (r=clearbusy(p->ptr)) > p; p=r) {
		if(p==q)
			x++;
	}
	return(r==allocb&(x==1|p==q));
#else
	return(q>=allocb);
#endif
}
#endif
