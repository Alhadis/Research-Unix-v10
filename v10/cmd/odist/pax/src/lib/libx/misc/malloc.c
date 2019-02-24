#if cray

/* Phong needs to figure this one out */

int	_lib_dummy;

#else /* cray */

/**********************************************************************
	Memory management: malloc(), realloc(), free(), M_brksize.
	M_brksize: if > 0 is the maximum amount that the bottom free
		block can grow to. If not SEGMENT, GETCORE() will
		be used to compact bottom space.

	The following #-parameters may be redefined:
	SEGMENT: if defined, memory requests are assumed to be
		non-contiguous across calls of GETCORE's. SEGMENT defines
		the size of each GETCORE request.
	CORESIZE: min number of bytes to used with GETCORE.
		On a SEGMENT machine, this should be defined
		as the size of a segment. Default is 4096.
	GETCORE: a function to get more core memory. If not SEGMENT,
		GETCORE(0) is assumed to return the next available
		address. Default is 'sbrk'.
	ERRCORE: the error code as returned by GETCORE.
		Default is ((char*)(-1)).
	MTRACE:	if #define-d, code is included to trace and certify that
		malloc-ed blocks are not corrupted. Available functions
		and variables:
		int (*Mt_corrupt)(addr,size,stamp)
			The function called when a corrupted block is detected.
				addr: address of the corrupted block
				size: size of the block.
				stamp: a user/program-defined stamp (below).
			If Mt_corrupt is NULL, a default function is used.
		Mt_certify: if not 0, indicates that the arena should
			be automatically certified on each call to malloc,
			realloc, or free. Default is 0.
		Mt_trace: if >= 0, is the file descriptor to write out
			a trace of all calls to malloc, free, realloc.
			Default is -1.
		mt_stamp(VOID *data, VOID *stamp)
			Set a stamp for a malloc-ed block. This stamp is used
			in (*corrupt)() calls. For example, a stamp may be
			an indicator of the place where malloc was called.
		mt_certify()
			Check all blocks to see if they are ok. If a block is
			corrupted, (*corrupt)() is called as above.
		mt_stat(int fd)
			Print arena statistics.

	With minor variations, the basic allocation strategy is best-fit.
	Lists of free blocks of the same size are kept in a splay tree.
	For results on splay trees, see:
		Self-Adjusting Binary Trees,
		DD Sleator & RE Tarjan, JACM 1985.

	The header of a block contains the size of the data part in bytes.
	Since the size of a block is 0%4, the low two bits of the header
	are free and used as follows:
		BIT0:	1 for busy (block is in use), 0 for free.
		BIT1:	if the block is busy, this bit is 1 if the
			preceding block in contiguous memory is free.
			Otherwise, it is always 0.

	Kiem-Phong Vo, AT&T Bell Laboratories
**********************************************************************/

int	M_brksize;	/* the max size for Bottom */

/* debugging macros */
#ifdef	DEBUG
#define VOID		unsigned char
#define	ASSERT(p)	((void) ((p) ? 0 : (abort(),0)))
#define COUNT(n)	((void) ((n) += 1))
#define DISCOUNT(n)	((void) ((n) -= 1))
static int		N_malloc, N_realloc, N_free, N_block, N_list;
#else
#define VOID		char
#define	ASSERT(p)
#define COUNT(n)
#define DISCOUNT(n)
#endif /*DEBUG*/

/* system call to get more core */
#ifdef SEGMENT
#define CORESIZE	SEGMENT
#else
#define CORESIZE	(4096)
#endif
#define GETCORE		sbrk
#define ERRCORE		((VOID*)(-1))
extern VOID	*GETCORE(), *malloc(), *realloc();
extern void	free();

/* function to copy data from one area to another */
#include "FEATURE/bcopy"
#ifdef	_lib_bcopy
#undef	memcpy
#define memcpy(to,fr,n)		bcopy(fr,to,n)
#endif

/* for conveniences */
#define reg		register
#define uchar		unsigned char
#define uint		unsigned int
#define NULL		(0L)
#define NIL(p)		((p)(NULL))
#define MINSIZE		((int)(sizeof(TREE)-sizeof(WORD)))
#define ROUND(x,y)	((((x)+((y)-1))/(y))*(y))

/* the following structures are used to compute a suitable alignment for all types */
typedef union _u_
{
	int	i;
	VOID*	s;
	double	d;
	VOID	big[1024];
} _ua_;
typedef struct _s_
{
	VOID	c;
	_ua_	u;
} _sa_;
#define ALIGN		((int)(sizeof(_sa_) - sizeof(_ua_)))
#define WORDSIZE        ROUND((int)ALIGN,4)

typedef union _w_
{	/* the proto-word */
	uint		w_i;		/* an int */
	struct _t_	*w_p;		/* a pointer */
	VOID		w_a[ALIGN];	/* to force alignment */
} WORD;

typedef struct _t_
{	/* structure of a node in the free tree */
	WORD	t_s;	/* size of this element */
	WORD	t_n;	/* next in link list */
	WORD	t_l;	/* left child */
	WORD	t_r;	/* right child */
	WORD	t_d;	/* dummy to reserve space for self-pointer */
} TREE;

/* usable # of bytes in the block */
#define SIZE(b)		(((b)->t_s).w_i)

/* free tree pointers */
#define LEFT(b)		(((b)->t_l).w_p)
#define RIGHT(b)	(((b)->t_r).w_p)

/* links in linked lists */
#define LINK(b)		(((b)->t_n).w_p)
#define BACK(b)		(((b)->t_l).w_p)

/* set/test indicator to see if a block is in the tree or in a list */
#define SETNOTREE(b)	(RIGHT(b) = (TREE*)(~0))
#define ISNOTREE(b)	(RIGHT(b) == (TREE*)(~0))

/* functions to get information on a block */
#define DATA(b)		(((VOID*) (b)) + WORDSIZE)
#define BLOCK(d)	((TREE*) ((d) - WORDSIZE))
#define SELFP(b)	((TREE**) (((VOID*) (b)) + SIZE(b)))
#define LAST(b)		(*((TREE**) (((VOID*) (b)) - WORDSIZE)))
#define NEXT(b)		((TREE*) (((VOID*) (b)) + SIZE(b) + WORDSIZE))
#define BOTTOM(b)	((DATA(b)+SIZE(b)+WORDSIZE) == Baddr)

/* functions to set and test the lowest two bits of a word */
#define	BIT0		(01)	/* ....01 */
#define BIT1		(02)	/* ...010 */
#define BITS01		(03)	/* ...011 */
#define ISBIT0(w)	((w) & BIT0)
#define ISBIT1(w)	((w) & BIT1)
#define	SETBIT0(w)	((w) |= BIT0)
#define SETBIT1(w)	((w) |= BIT1)
#define CLRBIT0(w)	((w) &= ~BIT0)
#define CLRBIT1(w)	((w) &= ~BIT1)
#define ISBITS01(w)	((w) & BITS01)
#define SETBITS01(w)	((w) |= BITS01)
#define CLRBITS01(w)	((w) &= ~BITS01)
#define CPYBITS01(w,f)	((w) |= (f)&BITS01)

static TREE	*Root,		/* root of the free tree */
		*Bottom,	/* the last free chunk in the arena */
		*morecore(),	/* function to get more core */
		*t_search();	/* to look for a free block */

static VOID	*Baddr,		/* current high address of the arena */
		*Lfree,		/* last free block with data intact */
		*Nofree;	/* this block is not to be freed */

/* lists of small blocks */
#define LGET	16
static TREE	*List[(MINSIZE-WORDSIZE)/WORDSIZE];

/* circular queue of delayed free blocks */
#define QSIZE	(1<<8)
#define QMASK	(QSIZE-1)
static VOID	**Qfree;
static int	Qhead = -1, Qtail = 0;
#define CYCLE(n)	(n = (n+1)&QMASK)
#define REWIND(n)	(n = (n+QMASK)&QMASK)
#define QVOID()		(Qhead < 0)
#define QFULL()		(Qtail == Qhead)
#define QENDADD()	(Qhead < 0 ? (Qhead = 0) : 0)
#define QENDDEL()	(Qtail == Qhead ? (Qhead = -1, Qtail = 0) : 0)
#define ENQUEUE(x)	((Qfree[Qtail] = (x)), CYCLE(Qtail), QENDADD())
#define DEQUEUE(x)	(((x) = Qfree[Qhead]), CYCLE(Qhead), QENDDEL())
#define DESTACK(x)	(REWIND(Qtail), ((x) = Qfree[Qtail]), QENDDEL())

/*
**	Coalesce adjacent blocks and free left-over memory.
*/
#define UNLINK(u)	{ if(LINK(u)) \
				BACK(LINK(u)) = BACK(u); \
			  LINK(BACK(u)) = LINK(u);	/**/ DISCOUNT(N_block); \
			}
#define FMERGE(t,n)	{ n = NEXT(t); \
			  if(!ISBIT0(SIZE(n))) \
			  {	if(n != Bottom) \
				{	if(!ISNOTREE(n)) \
						t_search(SIZE(n)); \
					else	UNLINK(n); \
				} \
				else	Bottom = NIL(TREE*); \
				SIZE(t) += SIZE(n)+WORDSIZE; \
				CLRBIT1(SIZE(NEXT(t))); \
				/**/ ASSERT(ISBIT0(SIZE(NEXT(t)))); \
			  } \
			}
#define BMERGE(t,n)	{ n = LAST(t); \
			  if(!ISNOTREE(n)) \
				t_search(SIZE(n)); \
			  else	UNLINK(n); \
			  SIZE(n) += SIZE(t)+WORDSIZE; \
			  t = n; \
			}
#define FREE(f)		{ if(BOTTOM(f)) \
				Bottom = f; \
			  else \
			  {	SETBIT0(SIZE(f)); /**/ ASSERT(!Lfree); \
				Lfree = DATA(f); \
			  } \
			}
/*
** Trace malloc/free patterns.
*/
#ifdef MTRACE
#define MTNONE		0
#define MTFREE		1
#define MTMALLOC	2
#define MTREALLOC	3
#define MTSPACE		(3*WORDSIZE)
#define MTSIZE(b)	(SIZE(b)&(~BITS01))
#define MTNEXT(b)	((TREE*)(DATA(b) + MTSIZE(b)))
#define USIZE(b)	(*((uint*)(DATA(b)+MTSIZE(b)-(2*WORDSIZE))))
#define USTAMP(b)	(*((VOID**)(DATA(b)+MTSIZE(b)-WORDSIZE)))
#define MTSETFREE(b)	(USTAMP(b) = (VOID*)(~0))
#define MTISFREE(b)	(!ISBITS01(SIZE(b)) || USTAMP(b) == (VOID*)(~0))
#define SMAGIC(b)	((uchar*)(DATA(b)+USIZE(b)))
#define EMAGIC(b)	((uchar*)(DATA(b)+MTSIZE(b)-(2*WORDSIZE)))
#define MAGIC		0253	/* 10101011 pattern */

static VOID	*Laddr;		/* low address of the arena */
int	(*Mt_corrupt)();	/* function to process corrupted blocks */
int	Mt_certify;		/* automatically certify the arena */
int	Mt_trace = -1;		/* print trace of mallocs and frees */

/* Convert an int to a string */
static itoa(i,buf)
reg uint	i;
reg char	*buf;
{
	reg int	k, c;

	k = 0;
	do
	{
		buf[k++] = '0' + i%10;
		i /= 10;
	}	while(i != 0);
	buf[k] = '\0';
	for(i = 0; i < k/2; ++i)
	{
		c = buf[i];
		buf[i] = buf[(k-i)-1];
		buf[(k-i)-1] = c;
	}
}

/* internal function for warning on corruption */
static int mt_corrupt(addr,usize,stamp)
reg VOID	*addr;
reg uint	usize;
reg VOID	*stamp;
{
	char	buf[64], *mesg;

	mesg = "corrupt:addr=";
	write(2,mesg,strlen(mesg));
	itoa((uint)addr,buf);
	write(2,buf,strlen(buf));

	mesg = ":size=";
	write(2,mesg,strlen(mesg));
	itoa(usize,buf);
	write(2,buf,strlen(buf));

	mesg = ":stamp=";
	write(2,mesg,strlen(mesg));
	itoa((uint)stamp,buf);
	write(2,buf,strlen(buf));

	write(2,"\n",1);
}

/* Print trace information */
static mt_trace(addr,type)
reg VOID	*addr;
reg int		type;
{
	char	*mesg, buf[64];

	mesg =	type == MTMALLOC ? "malloc" : type == MTFREE ? "free" : "realloc";
	write(Mt_trace,mesg,strlen(mesg));

	mesg = ":addr=";
	write(Mt_trace,mesg,strlen(mesg));
	itoa((uint)addr,buf);
	write(Mt_trace,buf,strlen(buf));

	mesg = ":size=";
	write(Mt_trace,mesg,strlen(mesg));
	itoa(USIZE(BLOCK(addr)),buf);
	write(Mt_trace,buf,strlen(buf));

	write(Mt_trace,"\n",1);
}

/* Print a warning */
static mt_didfree(addr,type)
reg VOID	*addr;
reg int		type;
{
	char	*mesg, buf[64];

	mesg = type == MTFREE ? "free:addr=" : "realloc:addr=";
	write(2,mesg,strlen(mesg));
	itoa((uint)addr,buf);
	write(2,buf,strlen(buf));
	mesg = ":already freed\n";
	write(2,mesg,strlen(mesg));
}

/* Set trace info for a block */
static mt_setinfo(bp,usize,type)
reg TREE	*bp;
reg uint	usize;
int		type;
{
	reg uchar	*magic, *emagic;

	USIZE(bp) = usize;
	USTAMP(bp) = NIL(VOID*);
	for(magic = SMAGIC(bp), emagic = EMAGIC(bp); magic < emagic; ++magic)
		*magic = MAGIC;

	if(Mt_trace >= 0 && type != MTNONE)
		mt_trace(DATA(bp),type);
}

/* Set a stamp */
mt_stamp(addr,stamp)
reg VOID	*addr, *stamp;
{
	USTAMP(BLOCK(addr)) = stamp;
}

/* Certify that no data block has been corrupted */
mt_certify()
{
	reg TREE	*bp, *endb;
	reg uchar	*magic, *endm;

	if(!Mt_corrupt)
		Mt_corrupt = mt_corrupt;

	for(bp = (TREE*)Laddr, endb = (TREE*) Baddr; bp < endb; bp = MTNEXT(bp))
	{
		if(MTISFREE(bp) || MTSIZE(bp) == 0)
			continue;
		if(USIZE(bp) >= MTSIZE(bp))
			(*Mt_corrupt)(DATA(bp),USIZE(bp),USTAMP(bp));
		else for(magic = SMAGIC(bp), endm = EMAGIC(bp); magic < endm; ++magic)
			if(*magic != MAGIC)
			{
				(*Mt_corrupt)(DATA(bp),USIZE(bp),USTAMP(bp));
				break;
			}
	}
}

/* Print block statistics */
mt_stat(fd)
int	fd;
{
	reg TREE	*bp, *endb;
	reg uint	nbusy, sfree, sbusy, mbusy;
	char		buf[64], *mesg;

	nbusy = sfree = sbusy = mbusy = 0;
	for(bp = (TREE*)Laddr, endb = (TREE*) Baddr; bp < endb; bp = MTNEXT(bp))
	{
		if(MTISFREE(bp) || MTSIZE(bp) == 0)
			sfree += MTSIZE(bp);
		else
		{
			nbusy += 1;
			sbusy += USIZE(bp);
			mbusy += MTSIZE(bp);
		}
	}

	mesg="free_space=";
	write(fd,mesg,strlen(mesg));
	itoa(sfree,buf);
	write(fd,buf,strlen(buf));

	mesg=", busy_blocks=";
	write(fd,mesg,strlen(mesg));
	itoa(nbusy,buf);
	write(fd,buf,strlen(buf));

	mesg=", user_space=";
	write(fd,mesg,strlen(mesg));
	itoa(sbusy,buf);
	write(fd,buf,strlen(buf));

	mesg=", malloc_space=";
	write(fd,mesg,strlen(mesg));
	itoa(mbusy,buf);
	write(fd,buf,strlen(buf));

	write(fd,"\n",1);
}
#endif	/* MTRACE */

/*
**	malloc().
*/
VOID *malloc(size)
reg uint	size;
{
	reg TREE	*tp, *np, *fp;
	reg int		n, i;
#ifdef MTRACE
	/* save true size and make size large enough to hold our data */
	reg uint	mtsize = size;
	size = size <= (MINSIZE-MTSPACE) ? MINSIZE : size + MTSPACE;
	if(Mt_certify)
		mt_certify();
#endif
	/**/ COUNT(N_malloc);

	size = size == 0 ? WORDSIZE : ROUND(size,WORDSIZE);
	tp = NIL(TREE*);
	if(Lfree)
	{	/* see if the last free block can be used */
		fp = BLOCK(Lfree);
		Lfree = NIL(VOID*);
		n = SIZE(fp);
		CLRBITS01(SIZE(fp));
		if(SIZE(fp) == size)
		{	/* exact match, use it as is */
			SIZE(fp) = n;
			if(!QVOID())
				DESTACK(Lfree);
#ifdef MTRACE
			mt_setinfo(fp,mtsize,MTMALLOC);
#endif
			return DATA(fp);
		}
		else if(n >= MINSIZE && size >= MINSIZE && !ISBIT1(n))
		{	/* see if good enough */
			FMERGE(fp,np);
			if(!BOTTOM(fp) && SIZE(fp) >= size)
				tp = fp;
		}
		else	SIZE(fp) = n;
		if(!tp)
			free(Lfree = DATA(fp));
	}

	if(size < MINSIZE)
	{	/**/ ASSERT(!Lfree && QVOID());
		n = size/WORDSIZE - 1;
		if(List[n] == NIL(TREE*))
		{	/* get a bunch of these small blocks */
			if(!(tp = (TREE*) malloc((size+WORDSIZE)*LGET)))
				return NIL(VOID*);
			List[n] = tp;
			for(i = LGET-1; i > 0; --i)
			{
				SIZE(tp) = size;
				tp = LINK(tp) = NEXT(tp);
			}
			SIZE(tp) = size;
			LINK(tp) = NIL(TREE*);
		}
		tp = List[n];
		List[n] = LINK(tp);
		return DATA(tp);
	}

	if(!tp)
	{	/* normal malloc requests */
		if(Root && (tp = t_search(size)) != NIL(TREE*))
			CLRBIT1(SIZE(NEXT(tp)));
		else if((tp = Bottom) != NIL(TREE*) && SIZE(tp) >= size)
			Bottom = NIL(TREE*);
		else if((tp = morecore(size)) == NIL(TREE*))
			return NIL(VOID*);
	}	/**/ ASSERT(tp && !ISBITS01(SIZE(tp)));

	if((n = SIZE(tp)-size) >= (MINSIZE+WORDSIZE))
	{	/* the leftover is enough for a new free piece */
		SIZE(tp) = size;
		np = NEXT(tp);
		SIZE(np) = n-WORDSIZE;
#ifdef MTRACE
		MTSETFREE(np);
#endif
		FREE(np);
	}

	/* peel out a pending free block */
	if(!Lfree && !QVOID())
		DESTACK(Lfree);

	/* return the allocated space */
#ifdef MTRACE
	mt_setinfo(tp,mtsize,MTMALLOC);
#endif
	SETBIT0(SIZE(tp));
	return DATA(tp);
}

/*
**	realloc().
*/
VOID *realloc(old,size)
VOID		*old;
reg uint	size;
{
	reg TREE	*tp, *np;
	reg int		n, ts;
	reg VOID	*new;
#ifdef MTRACE
	reg uint	mtsize = size;
	if(old)
	{
		size = size < (MINSIZE-MTSPACE) ? MINSIZE : size + MTSPACE;
		if(Mt_certify)
			mt_certify();
		if(MTISFREE(BLOCK(old)))
			mt_didfree(old,MTREALLOC);
		if(Mt_trace >= 0)
			mt_trace(old,MTREALLOC);
	}
#endif
	/**/ COUNT(N_realloc);

	if(!old)
		return malloc(size);

	if(Lfree)
	{	/* free everything except old */
		Nofree = old;
		free(Lfree);
		Nofree = NIL(VOID*);
	}

	size = size == 0 ? WORDSIZE : ROUND(size,WORDSIZE);
	tp = BLOCK(old);
	ts = SIZE(tp);
	if(size >= MINSIZE && ts >= MINSIZE)
	{
		CLRBITS01(SIZE(tp));
		if((n = SIZE(tp)-size) < 0)
		{	/* growing, try forward merging */
			FMERGE(tp,np);
			n = SIZE(tp) - size;
#ifndef SEGMENT
			if(n < 0 && BOTTOM(tp) && GETCORE(0) == Baddr)
			{	/* try extending core */
				Bottom = tp;
				if((tp = morecore(size)) != NIL(TREE*))
					n = SIZE(tp) - size;
			}
#endif /*!SEGMENT*/
		}
		if(n >= (MINSIZE+WORDSIZE))
		{	/* left over is enough for a new piece */
			SIZE(tp) = size;
			np = NEXT(tp);
			SIZE(np) = (n-WORDSIZE);
#ifdef MTRACE
			MTSETFREE(np);
#endif
			FREE(np);
		}
		CPYBITS01(SIZE(tp),ts);
		if(n >= 0) /* got it */
		{
#ifdef MTRACE
			mt_setinfo(tp,mtsize,MTMALLOC);
#endif
			return old;
		}
	}
#ifdef MTRACE
	MTSETFREE(BLOCK(old));
#endif
	/* call malloc to get a new block */
	CLRBITS01(ts);
	if((new = malloc(size)) != NIL(VOID*))
		memcpy(new,old,ts < size ? ts : size);

	/**/ ASSERT(!QFULL());
	if(!Lfree)
		Lfree = old;
	else	ENQUEUE(old);
#ifdef MTRACE
	mt_setinfo(BLOCK(new),mtsize,MTNONE);
#endif
	return new;
}

/*
**	free().
*/
void free(old)
reg VOID	*old;
{
	reg int		size;
	reg TREE	*tp, *np, *sp;
	reg VOID	*dequeue;
#ifdef MTRACE
	if(old != Lfree)
	{
		if(Mt_certify)
			mt_certify();
		if(MTISFREE(BLOCK(old)))
			mt_didfree(old,MTFREE);
		else	MTSETFREE(BLOCK(old));
		if(Mt_trace >= 0)
			mt_trace(old,MTFREE);
	}
#endif
	/**/ COUNT(N_free);

	if(!old)
		old = Lfree;

	dequeue = NIL(VOID*);
	if(Lfree != old)
	{	/* this is a normal free call */
		if(Lfree)
		{	/* make queue space for current Lfree */
			if(QFULL())
				DEQUEUE(dequeue);	/**/ ASSERT(!QFULL());
			ENQUEUE(Lfree);
		}
		Lfree = old;
		old = dequeue;
	}
	else	Lfree = NIL(VOID*);

	while(old)
	{
		if(old == Nofree)
			goto next;

		tp = BLOCK(old);
		if((size = SIZE(tp)) < MINSIZE)
		{	/* small block */
			size = size/WORDSIZE - 1;
			LINK(tp) = List[size];
			List[size] = tp;
			goto next;
		}

		/* merge adjacent free blocks */
		CLRBITS01(SIZE(tp));
		FMERGE(tp,np);
		if(ISBIT1(size))
			BMERGE(tp,np);

		if(BOTTOM(tp))
		{	/* bottom block */
			Bottom = tp;
#ifndef SEGMENT
			if(M_brksize > 0 && GETCORE(0) == Baddr)
			{
				M_brksize = ROUND(M_brksize,CORESIZE);
				if((size = SIZE(tp)-MINSIZE) >= M_brksize)
				{
					size = (size/CORESIZE)*CORESIZE;
					GETCORE(-size);
					if((old = GETCORE(0)) != Baddr)
					{
						Baddr = old;
						SIZE(tp) = (old-WORDSIZE) - DATA(tp);
						SIZE(NEXT(tp)) = BIT0;
					}
				}
			}
#endif
			goto next;
		}

		/* tell next block that this one is free */
		SETBIT1(SIZE(NEXT(tp)));	/**/ ASSERT(ISBIT0(SIZE(NEXT(tp))));

		/* leaf insert into the free tree */
		LEFT(tp) = RIGHT(tp) = LINK(tp) = NIL(TREE*);
		*(SELFP(tp)) = tp;
		/**/ COUNT(N_block);

		if(!Root)
		{	/**/ COUNT(N_list);
			Root = tp;
			goto next;
		}

		np = Root;
		size = SIZE(tp);
		while(1)
		{
			if(SIZE(np) > size)
			{
				if((sp = LEFT(np)) != NIL(TREE*))
					np = sp;
				else
				{	/**/ COUNT(N_list);
					LEFT(np) = tp;
					break;
				}
			}
			else if(SIZE(np) < size)
			{
				if((sp = RIGHT(np)) != NIL(TREE*))
					np = sp;
				else
				{	/**/ COUNT(N_list);
					RIGHT(np) = tp;
					break;
				}
			}
			else /* SIZE(np) == size */
			{
				if((sp = LINK(np)) != NIL(TREE*))
				{
					LINK(tp) = sp;
					BACK(sp) = tp;
				}
				LINK(np) = tp;
				BACK(tp) = np;
				SETNOTREE(tp);
				break;
			}
		}
	next:
		if(dequeue || QVOID())
			old = NIL(VOID*);
		else	DEQUEUE(old);
	}
}

/*
**	Get more core. Gaps in memory are noted as busy blocks.
*/
static TREE *morecore(size)
reg uint	size;
{
	reg TREE	*tp, *bp;
	reg VOID	*addr;

	/* space for queue of delayed free blocks */
	if(!Qfree)
		size += QSIZE*sizeof(Qfree[0]);

	/* try to extend the Bottom block if possible */
	bp = Bottom;
	Bottom = NIL(TREE*);

	/* get memory */
	size += 2*WORDSIZE;
	size = ROUND(size,CORESIZE);
	if((addr = GETCORE(size)) == ERRCORE)
	{
		Bottom = bp;
		return NIL(TREE*);
	}

	if(addr == Baddr)
	{	/* contiguous memory, merge with previous bottom */
		if(bp)
		{
			addr = ((VOID*)bp);
			size += SIZE(bp) + 2*WORDSIZE;
		}
		else
		{
			addr = Baddr-WORDSIZE;
			size += WORDSIZE;
		}
	}
	else
	{
#ifndef SEGMENT
		if((((uint)addr)%ALIGN) != 0)
		{	/* make sure alignment is correct */
			reg uint n = ALIGN - ((uint)addr)%ALIGN;
			if(GETCORE(n) == ERRCORE)
			{
				Bottom = bp;
				return NIL(TREE*);
			}
			addr += n;
		}
#endif
		if(!Qfree)
		{	/* space for the free queue */
			Qfree = (VOID**) addr;
			addr += QSIZE*sizeof(Qfree[0]);
			size -= QSIZE*sizeof(Qfree[0]);
		}
#ifdef MTRACE
		if(!Laddr)
			Laddr = addr;
#endif
	}

	/* new bottom address */
	Baddr = addr + size;

	/* new bottom block */
	tp = ((TREE*) addr);
	SIZE(tp) = size - 2*WORDSIZE;	/**/ASSERT((SIZE(tp)%WORDSIZE) == 0);

	/* reserved the last word to head any noncontiguous memory */
	SETBIT0(SIZE(NEXT(tp)));

	if(bp && bp != tp)
	{	/* non-contiguous memory, free old bottom block */
		/**/ ASSERT(!Lfree && QVOID());
		SETBIT0(SIZE(bp));
		ENQUEUE(DATA(bp));
	}

	return tp;
}

/*
**	Tree rotation functions
*/
#define RROTATE(t,r)	(t = LEFT(r), LEFT(r) = RIGHT(t), RIGHT(t) = r, r = t)
#define LROTATE(t,r)	(t = RIGHT(r), RIGHT(r) = LEFT(t), LEFT(t) = r, r = t)
#define RLINK(r,s,x)	(r ? (s = LEFT(s) = x) : (r = s = x))
#define LLINK(r,s,x)	(r ? (s = RIGHT(s) = x) : (r = s = x))
#define RTWICE(t,r)	(LEFT(LEFT(r)) = RIGHT(t), RIGHT(t) = r, r = t)
#define LTWICE(t,r)	(RIGHT(RIGHT(r)) = LEFT(t), LEFT(t) = r, r = t)

/*
**	Look up a suitable element in the tree. If found, delete it from
**	the tree and return its address.
**	This uses the top-down splay strategy.
*/
static TREE *t_search(size)
reg int		size;
{
	reg int		cmp;
	reg TREE	*t, *del, *left, *right, *lroot, *rroot;

	/* find the right one to delete */
	del = Root;
	lroot = rroot = NIL(TREE*);
	while(del)
	{	/**/ ASSERT(!ISBITS01(size) && !ISBITS01(SIZE(del)));
		if((cmp = size - SIZE(del)) == 0)
			break;
		if(cmp < 0)
		{
			if((t = LEFT(del)) == NIL(TREE*))
			{
				RLINK(rroot,right,del);
				del = NIL(TREE*);
			}
			else if((cmp = size - SIZE(t)) <= 0)
			{	/* left,left case */
				RROTATE(t,del);
				if(cmp == 0)
					break;
				RLINK(rroot,right,del);
				del = LEFT(del);
			}
			else
			{	/* left, right case */
				RLINK(rroot,right,del);
				LLINK(lroot,left,t);
				del = RIGHT(t);
			}
		}
		else
		{
			if((t = RIGHT(del)) == NIL(TREE*))
			{
				LLINK(lroot,left,del);
				del = NIL(TREE*);
			}
			else if((cmp = size - SIZE(t)) >= 0)
			{	/* right, right case */
				LROTATE(t,del);
				if(cmp == 0)
					break;
				LLINK(lroot,left,t);
				del = RIGHT(del);
			}
			else
			{	/* right, left case */
				LLINK(lroot,left,del);
				RLINK(rroot,right,t);
				del = LEFT(t);
			}
		}
	}

	if(del)
	{
		if(lroot)
		{
			RIGHT(left) = LEFT(del);
			LEFT(del) = lroot;
		}
		if(rroot)
		{
			LEFT(right) = RIGHT(del);
			RIGHT(del) = rroot;
		}
	}
	else
	{
		if(lroot)
			RIGHT(left) = NIL(TREE*);
		if(rroot)
		{	/* get least one > size */
			LEFT(right) = NIL(TREE*);
			while(LEFT(rroot))
			{	/* left zig-zig case */
				if((t = LEFT(LEFT(rroot))) != NIL(TREE*))
					RTWICE(t,rroot);
				else	RROTATE(t,rroot);
			}
			LEFT(rroot) = lroot;
			del = rroot;
		}
		else
		{
			Root = lroot;
			return NIL(TREE*);
		}
	}

	if((t = LINK(del)) != NIL(TREE*))
	{	/* start of a non-singleton list */
		LEFT(t) = LEFT(del);
		RIGHT(t) = RIGHT(del);
		Root = t;	/**/ ASSERT(!ISNOTREE(t));
	}
	else
	{	/**/ DISCOUNT(N_list);
		if((right = RIGHT(del)) != NIL(TREE*))
		{	/* make least elt of right tree the root */
			while(LEFT(right))
			{	/* left zig-zig case */
				if((t = LEFT(LEFT(right))) != NIL(TREE*))
					RTWICE(t,right);
				else	RROTATE(t,right);
			}
			LEFT(right) = LEFT(del);
			Root = right;
		}
		else	Root = LEFT(del);
	}	/**/ DISCOUNT(N_block);

	return del;
}

#endif /* cray */
