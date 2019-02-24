/**********************************************************************
	Memory management: malloc(), realloc(), free().

	Kiem-Phong Vo, ulysses!kpv,
	AT&T Bell Laboratories, MH5D102, 201-582-4869.

	The following #-parameters may be redefined:
	SEGMENTED: if defined, memory requests are assumed to be
		non-contiguous across calls of GETCORE's.
	GETCORE: a function to get more core memory. If not SEGMENTED,
		GETCORE(0) is assumed to return the next available
		address. Default is 'sbrk'.
	ERRCORE: the error code as returned by GETCORE.
		Default is ((char *)(-1)).
	CORESIZE: a desired unit (measured in bytes) to be used
		with GETCORE. Default is (1024*ALIGN).

	This algorithm is based on a  best fit strategy with lists of
	free elts maintained in a self-adjusting binary tree. Each list
	contains all elts of the same size. The tree is ordered by size.
	For results on self-adjusting trees, see the paper:
		Self-Adjusting Binary Trees,
		DD Sleator & RE Tarjan, JACM 1985.

	The header of a block contains the size of the data part in bytes.
	Since the size of a block is 0%4, the low two bits of the header
	are free and used as follows:

		BIT0:	1 for busy (block is in use), 0 for free.
		BIT1:	if the block is busy, this bit is 1 if the
			preceding block in contiguous memory is free.
			Otherwise, it is always 0.
**********************************************************************/

/* debugging macros */
#ifdef	DEBUG
#define	ASSERT(p)	if(!(p))abort()
#define COUNT(n)	n++
static int		nmalloc, nrealloc, nfree;
#else
#define	ASSERT(p)
#define COUNT(n)
#endif /*DEBUG*/

/* function to copy data from one area to another */
#ifdef	SYSV
#define memcopy(to,fr,n)	memcpy(to,fr,n)
#endif

#ifdef BSD
#define memcopy(to,fr,n)	bcopy(fr,to,n)
#endif

#ifndef memcopy
static	memcopy(to,fr,n)
register char	*to, *fr;
register int	n;
{
	for(; n > 0; --n)
		*to++ = *fr++;
}
#endif

/* for conveniences */
#define reg		register
#define NULL		(0)
#define WORDSIZE	((int)(sizeof(WORD)))
#define MINSIZE		((int)(sizeof(TREE)-sizeof(WORD)))
#define ROUND(s)	if(s%WORDSIZE) s += (WORDSIZE - (s%WORDSIZE))

#ifdef	DEBUG32
/* The following definitions ease debugging
** on a machine in which sizeof(pointer) == sizeof(int) == 4.
** These definitions are not portable.
*/
#define	ALIGN	4
typedef int	WORD;
typedef struct _t_
{
	unsigned int	t_s;
	struct _t_	*t_p;
	struct _t_	*t_l;
	struct _t_	*t_r;
	struct _t_	*t_n;
	struct _t_	*t_d;
}	TREE;
#define SIZE(b)		((b)->t_s)
#define AFTER(b)	((b)->t_p)
#define PARENT(b)	((b)->t_p)
#define LEFT(b)		((b)->t_l)
#define RIGHT(b)	((b)->t_r)
#define LINKFOR(b)	((b)->t_n)
#define LINKBAK(b)	((b)->t_p)

#else	/* !DEBUG32 */
/* The following definitions assume that "char *" has the largest size
** among all pointer types. If this is not true, PTRSIZE should
** be redefined to be the size of the largest pointer type (void * ?).
** All of our allocations will be aligned on the least multiple of 4
** that is >= PTRSIZE.
*/
#define PTRSIZE		((int)(sizeof(char *)))
#define ALIGN		(PTRSIZE%4 ? PTRSIZE+(4-(PTRSIZE%4)) : PTRSIZE)

/* the proto-word */
typedef union _w_
{
	unsigned int	w_i;		/* an int */
	struct _t_	*w_p;		/* a pointer */
	char		w_a[ALIGN];	/* to force alignment */
} WORD;

/* structure of a node in the free tree */
typedef struct _t_
{
	WORD	t_s;	/* size of this element */
	WORD	t_p;	/* parent node */
	WORD	t_l;	/* left child */
	WORD	t_r;	/* right child */
	WORD	t_n;	/* next in link list */
	WORD	t_d;	/* dummy to reserve space for self-pointer */
} TREE;

/* usable # of bytes in the block */
#define SIZE(b)		(((b)->t_s).w_i)

/* free tree pointers */
#define PARENT(b)	(((b)->t_p).w_p)
#define LEFT(b)		(((b)->t_l).w_p)
#define RIGHT(b)	(((b)->t_r).w_p)

/* forward link in lists of small blocks */
#define AFTER(b)	(((b)->t_p).w_p)

/* forward and backward links for lists in the tree */
#define LINKFOR(b)	(((b)->t_n).w_p)
#define LINKBAK(b)	(((b)->t_p).w_p)

#endif	/* DEBUG32 */

/* set/test indicator if a block is in the tree or in a list */
#define SETNOTREE(b)	(LEFT(b) = (TREE *)(-1))
#define ISNOTREE(b)	(LEFT(b) == (TREE *)(-1))

/* functions to get information on a block */
#define DATA(b)		(((char *) (b)) + WORDSIZE)
#define BLOCK(d)	((TREE *) ((d) - WORDSIZE))
#define SELFP(b)	((TREE **) (((char *) (b)) + SIZE(b)))
#define LAST(b)		(*((TREE **) (((char *) (b)) - WORDSIZE)))
#define NEXT(b)		((TREE *) (((char *) (b)) + SIZE(b) + WORDSIZE))
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
#define SETBITS01(w)	((w) |= BITS01)
#define CLRBITS01(w)	((w) &= ~BITS01)

/* system call to get more core */
#define GETCORE		sbrk
#define ERRCORE		((char *)(-1))
#define CORESIZE	(1024*ALIGN)

static TREE	*Root,		/* root of the free tree */
		*Bottom,	/* the last free chunk in the arena */
		*_morecore();	/* function to get more core */

static char	*Baddr;		/* current high address of the arena */

char	*GETCORE(), *malloc(), *realloc();


/*
**	Allocation of small blocks
*/

static TREE	*List[MINSIZE/WORDSIZE]; /* lists of small blocks */

static char *	_smalloc(size)
unsigned int	size;
{
	reg TREE	*tp, *np;
	reg int		i, n;

	/* nothing to do */
	if(size == 0)
		return NULL;

	/* list to use */
	i = size/WORDSIZE - 1;

	if(List[i] == NULL)
	{
	/* number of blocks to get at one time */
#define NPS (WORDSIZE*8)
	/**/ASSERT((size+WORDSIZE)*NPS >= MINSIZE);

		/* get NPS of these block types */
		if((List[i] = (TREE *) malloc((size+WORDSIZE)*NPS)) == NULL)
			return NULL;

		/* make them into a link list */
		for(n = 0, np = List[i]; n < NPS; ++n)
		{
			tp = np;
			SIZE(tp) = size;
			np = NEXT(tp);
			AFTER(tp) = np;
		}
		AFTER(tp) = NULL;
	}

	/* allocate from the head of the queue */
	tp = List[i];
	List[i] = AFTER(tp);
	SETBIT0(SIZE(tp));
	return DATA(tp);
}



/*
**	malloc().
*/
char *	malloc(size)
unsigned int	size;
{
	reg int		n;
	reg TREE	*tp, *sp;

	/**/ COUNT(nmalloc);
	/**/ ASSERT(WORDSIZE == ALIGN);

	/* make sure that size is 0 mod ALIGN */
	ROUND(size);

	/* small blocks */
	if(size < MINSIZE)
		return _smalloc(size);

	/* search for an elt of the right size */
	sp = NULL;
	n  = -1;
	if(Root)
	{
		tp = Root;
		while(1)
		{
			/* branch left */
			if(SIZE(tp) >= size)
			{
				if(n < 0 || n >= SIZE(tp))
					sp = tp, n = SIZE(tp);
				if(LEFT(tp))
					tp = LEFT(tp);
				else	break;
			}
			else
			{ /* branch right */
				if(RIGHT(tp))
					tp = RIGHT(tp);
				else	break;
			}
		}

		if(sp)
		{
			t_delete(sp);
		}
		else if(tp != Root)
		{
			/* make the searched-to element the root */
			t_splay(tp);
			Root = tp;
		}
	}

	/* if found none fitted in the tree */
	if(!sp)
	{
		if(Bottom && size <= SIZE(Bottom))
			sp = Bottom;
		else	sp = _morecore(size);

		/* no more memory */
		if(!sp)
			return NULL;
	}

	/* tell the forward neighbor that we're busy */
	CLRBIT1(SIZE(NEXT(sp)));	/**/ ASSERT(ISBIT0(SIZE(NEXT(sp))));

	/* if the leftover is enough for a new free piece */
	if((n = (SIZE(sp) - size) - WORDSIZE) >= MINSIZE)
	{
		SIZE(sp) = size;
		tp = NEXT(sp);
		SIZE(tp) = n|BIT0;
		free(DATA(tp));
	}
	else if(BOTTOM(sp))
		Bottom = NULL;

	/* return the allocated space */
	SETBIT0(SIZE(sp));
	return DATA(sp);
}


/*
**	realloc().
**	If the block size is increasing, we try forward merging first.
**	This is not best-fit but it avoids some data recopying.
*/
char *	realloc(old,size)
char		*old;
unsigned int	size;
{
	reg TREE	*tp, *np;
	reg int		n, ts;
	reg char	*new;

	/**/ COUNT(nrealloc);

	/* make sure that size is 0 mod ALIGN */
	ROUND(size);

	/* pointer to the block */
	tp = BLOCK(old);
	ts = SIZE(tp);

	/* if the block was freed, data has been destroyed. */
	if(!ISBIT0(ts))
		return NULL;

	/* nothing to do */
	CLRBITS01(SIZE(tp));
	if(size == SIZE(tp))
	{
		SIZE(tp) = ts;
		return old;
	}

	/* special cases involving small blocks */
	if(size < MINSIZE || SIZE(tp) < MINSIZE)
		goto call_malloc;

	/* block is increasing in size, try merging the next block */
	if(size > SIZE(tp))
	{
		np = NEXT(tp);
		if(!ISBIT0(SIZE(np)))
		{
			/**/ ASSERT(SIZE(np) >= MINSIZE);
			/**/ ASSERT(!ISBIT1(SIZE(np)));
			SIZE(tp) += SIZE(np)+WORDSIZE;
			if(np != Bottom)
				t_delete(np);
			else	Bottom = NULL;
			CLRBIT1(SIZE(NEXT(np)));
		}

#ifndef SEGMENTED
		/* not enough & at TRUE end of memory, try extending core */
		if(size > SIZE(tp) && BOTTOM(tp) && GETCORE(0) == Baddr)
		{
			Bottom = tp;
			_morecore(size);
		}
#endif /*!SEGMENTED*/
	}

	/* got enough space to use */
	if(size <= SIZE(tp))
	{
		if((n = (SIZE(tp) - size) - WORDSIZE) >= MINSIZE)
		{
			SIZE(tp) = size;
			np = NEXT(tp);
			SIZE(np) = n|BIT0;
			free(DATA(np));
		}
		else if(BOTTOM(tp))
			Bottom = NULL;

		/* the previous block may be free */
		if(ISBIT1(ts))
			SETBIT1(SIZE(tp));
		SETBIT0(SIZE(tp));
		return old;
	}

	/* call malloc to get a new block */
call_malloc:
	SETBIT0(SIZE(tp));
	if(ISBIT1(ts))
		SETBIT1(SIZE(tp));
	if((new = malloc(size)) != NULL)
	{
		ts = SIZE(tp);
		CLRBITS01(ts);
		memcopy(new,old,ts < size ? ts : size);
	}
	free(old);
	return new;
}



/*
**	free().
**	Coalescing of adjacent free blocks is done first.
**	Then, the new free block is leaf-inserted into the free tree
**	without splaying. This strategy does not guarantee the amortized
**	O(nlogn) behaviour for the insert/delete/find set of operations
**	on the tree. In practice, however, free is much more infrequent
**	than malloc/realloc and the tree searches performed by these
**	functions adequately keep the tree in balance.
*/
free(old)
char	*old;
{
	reg TREE	*tp, *sp, *np;
	reg int		ts, size;

	/**/ COUNT(nfree);

	/* pointer to the block */
	tp = BLOCK(old);
	ts = SIZE(tp);
	if(!ISBIT0(ts))
		return -1;
	CLRBITS01(SIZE(tp));

	/* small block, put it in the right linked list */
	if(SIZE(tp) < MINSIZE)
	{
		ts = SIZE(tp)/WORDSIZE - 1;
		AFTER(tp) = List[ts];
		List[ts] = tp;
		return 0;
	}

	/* see if coalescing with next block is warranted */
	np = NEXT(tp);
	if(!ISBIT0(SIZE(np)))
	{
		if(np != Bottom)
			t_delete(np);
		SIZE(tp) += SIZE(np)+WORDSIZE;
	}

	/* the same with the preceding block */
	if(ISBIT1(ts))
	{
		np = LAST(tp);		/**/ ASSERT(!ISBIT0(SIZE(np)));
					/**/ ASSERT(np != Bottom);
		t_delete(np);
		SIZE(np) += SIZE(tp)+WORDSIZE;
		tp = np;
	}

	/* initialize tree info */
	PARENT(tp) = LEFT(tp) = RIGHT(tp) = LINKFOR(tp) = NULL;

	/* the last word of the block contains self's address */
	*(SELFP(tp)) = tp;

	/* set bottom block, or insert in the free tree */
	if(BOTTOM(tp))
		Bottom = tp;
	else
	{
		/* search for the place to insert */
		if(Root)
		{
			size = SIZE(tp);
			np = Root;
			while(1)
			{
				if(SIZE(np) > size)
				{
					if(LEFT(np))
						np = LEFT(np);
					else
					{
						LEFT(np) = tp;
						PARENT(tp) = np;
						break;
					}
				}
				else if(SIZE(np) < size)
				{
					if(RIGHT(np))
						np = RIGHT(np);
					else
					{
						RIGHT(np) = tp;
						PARENT(tp) = np;
						break;
					}
				}
				else
				{
					if(sp = PARENT(np))	/* assignment = */
					{
						if(np == LEFT(sp))
							LEFT(sp) = tp;
						else	RIGHT(sp) = tp;
						PARENT(tp) = sp;
					}
					else	Root = tp;

					/* insert to head of list */
					if(sp = LEFT(np))	/* assignment = */
						PARENT(sp) = tp;
					LEFT(tp) = sp;

					if(sp = RIGHT(np))	/* assignment = */
						PARENT(sp) = tp;
					RIGHT(tp) = sp;

					/* doubly link list */
					LINKFOR(tp) = np;
					LINKBAK(np) = tp;
					SETNOTREE(np);

					break;
				}
			}
		}
		else	Root = tp;
	}

	/* tell next block that this one is free */
	SETBIT1(SIZE(NEXT(tp)));	/**/ ASSERT(ISBIT0(SIZE(NEXT(tp))));

	return 0;
}



/*
**	Get more core. Gaps in memory are noted as busy blocks.
*/
static TREE *	_morecore(size)
unsigned int	size;
{
	reg TREE	*tp;
	reg int		n;
	reg char	*addr;

	/* compute new amount of memory to get */
	tp = Bottom;
	n = size + 2*WORDSIZE;

#ifndef SEGMENTED
	/* if not segmented memory, what we need may be smaller */
	if((addr = GETCORE(0)) == Baddr)
		n = (size - (tp ? SIZE(tp) : 0)) + WORDSIZE;
#endif /*!SEGMENTED*/

	/* get a multiple of CORESIZE */
	n = (n/CORESIZE + 1) * CORESIZE;
	if((addr = GETCORE(n)) == ERRCORE)
		return NULL;

	/* contiguous memory */
	if(addr == Baddr)
	{
		if(tp)
		{
			addr = ((char *)tp);
			n += SIZE(tp) + 2*WORDSIZE;
		}
		else
		{
			addr = Baddr-WORDSIZE;
			n += WORDSIZE;
		}
	}

	/* new bottom address */
	Baddr = addr + n;

	/* new bottom block */
	tp = ((TREE *) addr);
	SIZE(tp) = n - 2*WORDSIZE;	/**/ASSERT((SIZE(tp)%ALIGN) == 0);

	/* reserved the last word to head any noncontiguous memory */
	SETBIT0(SIZE(NEXT(tp)));

	/* non-contiguous memory, free old bottom block */
	if(Bottom && Bottom != tp)
	{
		SETBIT0(SIZE(Bottom));
		free(DATA(Bottom));
	}

	return tp;
}



/*
**	Tree rotation functions (BU: bottom-up, TD: top-down)
*/

#define LEFT1(x,y)	if((RIGHT(x) = LEFT(y))) PARENT(RIGHT(x)) = x;\
			if((PARENT(y) = PARENT(x)))\
				if(LEFT(PARENT(x)) == x) LEFT(PARENT(y)) = y;\
				else RIGHT(PARENT(y)) = y;\
			LEFT(y) = x; PARENT(x) = y

#define RIGHT1(x,y)	if((LEFT(x) = RIGHT(y))) PARENT(LEFT(x)) = x;\
			if((PARENT(y) = PARENT(x)))\
				if(LEFT(PARENT(x)) == x) LEFT(PARENT(y)) = y;\
				else RIGHT(PARENT(y)) = y;\
			RIGHT(y) = x; PARENT(x) = y

#define BULEFT2(x,y,z)	if((RIGHT(x) = LEFT(y))) PARENT(RIGHT(x)) = x;\
			if((RIGHT(y) = LEFT(z))) PARENT(RIGHT(y)) = y;\
			if((PARENT(z) = PARENT(x)))\
				if(LEFT(PARENT(x)) == x) LEFT(PARENT(z)) = z;\
				else RIGHT(PARENT(z)) = z;\
			LEFT(z) = y; PARENT(y) = z; LEFT(y) = x; PARENT(x) = y

#define BURIGHT2(x,y,z)	if((LEFT(x) = RIGHT(y))) PARENT(LEFT(x)) = x;\
			if((LEFT(y) = RIGHT(z))) PARENT(LEFT(y)) = y;\
			if((PARENT(z) = PARENT(x)))\
				if(LEFT(PARENT(x)) == x) LEFT(PARENT(z)) = z;\
				else RIGHT(PARENT(z)) = z;\
			RIGHT(z) = y; PARENT(y) = z; RIGHT(y) = x; PARENT(x) = y

#define TDLEFT2(x,y,z)	if((RIGHT(y) = LEFT(z))) PARENT(RIGHT(y)) = y;\
			if((PARENT(z) = PARENT(x)))\
				if(LEFT(PARENT(x)) == x) LEFT(PARENT(z)) = z;\
				else RIGHT(PARENT(z)) = z;\
			PARENT(x) = z; LEFT(z) = x;

#define TDRIGHT2(x,y,z)	if((LEFT(y) = RIGHT(z))) PARENT(LEFT(y)) = y;\
			if((PARENT(z) = PARENT(x)))\
				if(LEFT(PARENT(x)) == x) LEFT(PARENT(z)) = z;\
				else RIGHT(PARENT(z)) = z;\
			PARENT(x) = z; RIGHT(z) = x;



/*
**	Delete a tree element
*/
static	t_delete(op)
TREE	*op;
{
	reg TREE	*tp, *sp, *gp;

	/* if this is a non-tree node */
	if(ISNOTREE(op))
	{
		tp = LINKBAK(op);
		if(sp = LINKFOR(op))	/* assignment = */
			LINKBAK(sp) = tp;
		LINKFOR(tp) = sp;
		return;
	}

	/* make op the root of the tree */
	if(PARENT(op))
		t_splay(op);

	/* if this is the start of a list */
	if(tp = LINKFOR(op))	/* assignment = */
	{
		PARENT(tp) = NULL;
		if(sp = LEFT(op))	/* assignment = */
			PARENT(sp) = tp;
		LEFT(tp) = sp;

		if(sp = RIGHT(op))	/* assignment = */
			PARENT(sp) = tp;
		RIGHT(tp) = sp;

		Root = tp;
		return;
	}

	/* if op has a non-null left subtree */
	if(tp = LEFT(op))	/* assignment = */
	{
		PARENT(tp) = NULL;

		if(RIGHT(op))
		{
			/* make the right-end of the left subtree its root */
			for(sp = RIGHT(tp); sp != NULL; sp = RIGHT(tp))
			{
				if(gp = RIGHT(sp))	/* assignment = */
				{
					TDLEFT2(tp,sp,gp);
					tp = gp;
				}
				else
				{
					LEFT1(tp,sp);
					tp = sp;
				}
			}

			/* hook the right subtree of op to the above elt */
			RIGHT(tp) = RIGHT(op);
			PARENT(RIGHT(tp)) = tp;
		}
	}

	/* no left subtree */
	else if(tp = RIGHT(op))		/* assignment = */
		PARENT(tp) = NULL;

	Root = tp;
}



/*
**	Bottom up splaying (simple version).
**	The basic idea is to roughly cut in half the
**	path from Root to tp and make tp the new root.
*/
static	t_splay(tp)
reg TREE	*tp;
{
	reg TREE	*pp, *gp;

	/* iterate until tp is the root */
	for(pp = PARENT(tp); pp != NULL; pp = PARENT(tp))
	{
		/* grandparent of tp */
		gp = PARENT(pp);

		/* x is a left child */
		if(LEFT(pp) == tp)	
		{
			if(gp && LEFT(gp) == pp)
			{
				BURIGHT2(gp,pp,tp);
			}
			else
			{
				RIGHT1(pp,tp);
			}
		}
		else
		{		/**/ ASSERT(RIGHT(pp) == tp);
			if(gp && RIGHT(gp) == pp)
			{
				BULEFT2(gp,pp,tp);
			}
			else
			{
				LEFT1(pp,tp);
			}
		}
	}
} 
