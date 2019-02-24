/* garbage collecting storage allocator; definition of
 * cell is compatible with malloc, but slightly less portable
*/

typedef struct dummy { struct dummy *ptr;} cell;

#define GCMAX 5000
#define GCMIN 50
#define BPW (8*sizeof(*gcmap))	/*bits per word*/
#define QBPW >>LOGBPW	/* quotient /BPW if BPW not power of 2 */
#define RBPW &(BPW-1)	/* remainder %BPW if BPW not power of 2 */
#define MASK (sizeof(cell)-1)	/*assumes cell aligned to power of 2*/
#define testbit(b,n) ((b[(n) QBPW]>>((n) RBPW))&1)
#define setbit(b,n) (b[(n) QBPW]|=1<<((n) RBPW))
#define clrbit(b,n) (b[(n) QBPW]&=~(1<<((n) RBPW)))
#define testbusy(p) (*(int*)((p)-1)&1)
#define clearbusy(p) (*(int*)((p)-1)&=~1)
#define setbusy(p) (*(int*)((p)-1)|=1)
#define endptr(p) (cell*)(*(int*)((p)-1)&~1)

#ifdef pdp11
#define STACKTOP (cell*)(-2)
#define STATICBOT (cell*)2	/*appropriate for cc -i*/
#define LOGBPW 4
#endif
#ifdef vax
#define STACKTOP (cell*)0x7ffffffc
#define STATICBOT etext
#define LOGBPW 5
#endif
#define STATICTOP end

static unsigned *gcmap;	/*bitmap of busy blocks, grows by powers of 2*/
static unsigned gcsize;	/*number of bits in gcmap*/
long gcmax = GCMAX;	/*max number of allocations between collections*/
long gcmin = GCMIN;	/*min number*/
extern cell end[];
extern cell etext[];

char *
galloc(n)
unsigned n;
{
	static long count;	/*number of allocations since collection*/
	register cell* q;
	register unsigned d;
	unsigned qd;
	char *malloc();
	while(++count >= gcmax ||
	      (q=(cell*)malloc(n)) == 0) {	/* at most twice*/
		if(count <= gcmin)
			return(0);
		garbage();
		count = 0;
	}
	if(q < STATICTOP)	/* probably due to ialloc() */
		return((char*)q);
	qd = q - STATICTOP;
	if(qd >= gcsize) {
		unsigned *tgmap;
		register unsigned bsize;
		if(BPW != 1<<LOGBPW)
			abort();
		if(gcsize==0) {
			bsize = 16384 QBPW;
			tgmap = (unsigned*)malloc(bsize*sizeof(*gcmap));
		} else {
			bsize = (gcsize QBPW)*2;
			tgmap = (unsigned*)realloc((char*)gcmap,
				bsize*sizeof(*gcmap));
		}
		if(tgmap==0)
			return(0);
		gcmap = tgmap;
		for(d=gcsize QBPW; d<bsize; d++)
			gcmap[d] = 0;
		gcsize = bsize*BPW;
	}
	setbit(gcmap, qd);
	return((char *)q);
}

/* pointer reversal algorithm
 * gcmap bit is set for first word of each galloc'ed block
 * make all galloc'd blocks appear idle and mark them busy as
 * the garbage collector reaches them
*/
garbage()
{
	cell fence;	/* must really be on the stack*/
	register cell *p, *q;
	register unsigned d;
	register cell *s, *t, *u;	/* lots of regs to force full save */
	register unsigned asize;
	asize = (cell*)sbrk(0) - STATICTOP;
	if(asize > gcsize)
		asize = gcsize;
	for(d=0; d<asize; d++)
		if(testbit(gcmap,d))
			clearbusy(STATICTOP+d);
	q = 0;
	for(s=STATICBOT; s<STACKTOP; s=(s==STATICTOP-1?&fence:s+1)) {
		p = s->ptr;
		for(;;) {
			while(!((int)p&MASK) &&	/*is p a cell-aligned address?*/
			      p>=STATICTOP &&		/* pointing within */
			      (d=p-STATICTOP)<asize &&	/* bounds of arena? */
			      testbit(gcmap, d) &&	/*to galloc-ed block?*/
			      !testbusy(p)) {		/* not yet visited?*/
				setbusy(p);
				/* q,p,*p = p,*p,q */
				t = p;
				p = p->ptr;
				t->ptr = q;
				q = t;
			}
			while(q!=0) {
				for(d=q-STATICTOP; !testbit(gcmap,d); d--)
					continue;
				if(q<endptr(t=STATICTOP+d)-1)
					break;
				/* q,*q,p = *q,p,t */
				u = q;
				q = q->ptr;
				u->ptr = p;
				p = t;
			}
			if(q==0)
				break;
			/* *q,p,q[1] = p,q[1],*q; q++ */
			t = q->ptr;
			q->ptr = p;
			q++;
			p = q->ptr;
			q->ptr = t;
		}
	}
	for(d=0; d<asize; d++) {
		if(testbit(gcmap,d)&&!testbusy(STATICTOP+d))
			clrbit(gcmap,d);
	}
}

gfree(p)
cell *p;
{
	free((char*)p);
	clrbit(gcmap,p-STATICTOP);
}
