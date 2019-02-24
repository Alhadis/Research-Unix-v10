#include	"update.h"
#include	"suftree.h"

/*
**	Compute delta commands to transform the source string 'src'
**	to the target string 'tar'. Small blockmoves are transformed
**	to blockadds for space efficiency.
**	Return -1 in case of error.
**
**	For details on computing blockmoves, see:
**	"The String-to-String Correction Problem with Block Moves"
**	W. Tichy, ACM TOCS, v.2, No.4, 1984, pp.309-321.
**
**	Written by Kiem-Phong Vo, 5/18/88
*/

extern char	*malloc();

#define M_MAX	9	/* max size of a block move instruction */
#define A_MAX	5	/* max size of the header of an add instruction */

/* structure of a delta instruction */
typedef struct _m_
{
	int		type;	/* DELTA_MOVE or DELTA_ADD */
	long		size;	/* size of the block being moved or added */
	long		addr;	/* offset where the block starts */
	struct _m_	*last;	/* doubly linked list for easy insert/delete */
	struct _m_	*next;
} Move;

/* bases of the source and target strings */
static char	*Bsrc, *Btar;

/* Data buffer area */
static char	*Ddata, *Dend, *Dnext;
static int	Dfd;

#define delinit(buf,fd)	(Ddata=Dnext=buf, Dend=buf+BUFSIZE, Dfd=fd)
#define delflush()	(write(Dfd,Ddata,Dnext-Ddata) >= 0 ? (Dnext=Ddata,0) : -1)

static int delputc(byte)
char	byte;
{
	if(Dnext == Dend)
		if(delflush() < 0)
			return -1;
	*Dnext++ = byte;
	return 0;
}

static int delputl(n,v)
register int	n;
register long	v;
{
	register int	i;
	unsigned char	c[4];

	for(i = 0; i < n; ++i)
	{
		c[i] = (unsigned char)(v%BASE);
		v /= BASE;
	}
	for(i = n-1; i >= 0; --i)
		if(delputc((char)c[i]) < 0)
			return -1;
	return 0;
}

static int delputs(n,addr)
register long	n;
register long	addr;
{
	if(n < (Dend-Dnext))
	{
		memcopy(Dnext,Btar+addr,n);
		Dnext += n;
	}
	else
	{
		if(delflush() < 0)
			return -1;
		if(write(Dfd,Btar+addr,n) != n)
			return -1;
	}
	return 0;
}

/* write an instruction */
static int putMove(this)
Move	*this;
{
	register char	inst;

	inst = this->type;
	inst |= (NBYTE(this->size)&07) << 3;
	if(this->type == DELTA_MOVE)
	{
		inst |= NBYTE(this->addr)&07;
		if(delputc(inst) < 0 ||
		   delputl(NBYTE(this->size),this->size) < 0 ||
		   delputl(NBYTE(this->addr),this->addr) < 0)
			return -1;
	}
	else
	{
		if(delputc(inst) < 0 ||
		   delputl(NBYTE(this->size),this->size) < 0 ||
		   delputs(this->size,this->addr) < 0)
			return -1;
	}
	return 0;
}

/* constructor for Move */
static Move *newMove(type,size,addr,last)
int	type;
long	size;
long	addr;
Move	*last;
{
	register Move *this = (Move*) malloc(sizeof(Move));
	if(this == NIL(Move))
		return NIL(Move);
	this->type = type;
	this->size = size;
	this->addr = addr;
	if(last)
	{
		last->next = this;
		this->last = last;
	}
	else	this->last = NIL(Move);
	this->next = NIL(Move);
	return this;
}

/* destructor for Move, return the elt after move */
static Move *delMove(this)
Move	*this;
{
	register Move *next = this->next;
	register Move *last = this->last;
	if(last)
		last->next = next;
	if(next)
		next->last = last;
	free((char*)this); 
	return next ? next : last;
}

/* make a new add command */
static Move *makeAdd(beg,end,last)
char	*beg, *end;
Move	*last;
{
	register Move	*this;

	this = newMove(DELTA_ADD,(long)(end-beg),(long)(beg-Btar),NIL(Move));
	if(!this)
		return NIL(Move);

	/* remove small previous adjacent moves */
	while(last)
	{
		register int a_size, cost_m, cost_a;

		if(last->type == DELTA_ADD)
			break;

		cost_m = NBYTE(last->size) + NBYTE(last->addr) +
			 NBYTE(this->size) + this->size;
		a_size = this->size + last->size;
		cost_a = NBYTE(a_size) + a_size;
		if(cost_m < cost_a)
			break;

		this->size  = a_size;
		this->addr -= last->size;
		last = delMove(last);
	}

	/* merge with adjacent adds */
	if(last && last->type == DELTA_ADD)
	{
		this->size += last->size;
		this->addr -= last->size;
		last = delMove(last);
	}

	if(last)
	{
		last->next = this;
		this->last = last;
	}
	return this;
}

/* check to see if a move is appropriate */
static int chkMove(m_size,m_addr,a_size)
long	m_size, m_addr, a_size;
{
	register int cost_m, cost_a;

	cost_m = NBYTE(m_size) + NBYTE(m_addr);
	cost_a = NBYTE(m_size) + m_size;
	if(cost_m >= cost_a)
		return 0;

	/* it's good but it may be better to merge it to an add */
	if(a_size > 0)
	{
		register int m_cost, a_cost;

		m_cost = cost_m + NBYTE(a_size) + a_size;
		a_size += m_size;
		a_cost = NBYTE(a_size) + a_size;

		/* it is better to merge! */
		if(m_cost >= a_cost)
			return 0;
	}

	return m_size;
}

/* optimize a sequence of moves */
static Move *optMove(s)
register Move *s;
{
	register long	add, m_cost, a_cost;
	register Move	*this, *last;

	add = (s->last && s->last->type == DELTA_ADD) ? s->last->size : 0;

	m_cost = 0;
	a_cost = 0;
	for(this = s; this != NIL(Move); this = this->next)
	{
		register long cost_m, cost_a;

		if(this->type == DELTA_ADD || this->size > (M_MAX+A_MAX))
			break;

		m_cost += 1+NBYTE(this->size)+NBYTE(this->addr);
		a_cost += this->size;

		/* costs of alternatives */
		cost_m = m_cost;
		cost_a = a_cost;
		if(add > 0)
		{
			cost_m += 1 + add + NBYTE(add);
			cost_a += add;
		}
		if(this->next && this->next->type == DELTA_ADD)
		{
			cost_m += 1 + this->next->size + NBYTE(this->next->size);
			cost_a += this->next->size;
		}
		cost_a += 1 + NBYTE(cost_a);

		/* conversion is bad */
		if(cost_m < cost_a)
			continue;

		/* convert the entire sequence to an add */
		s->type = DELTA_ADD;
		while(this != s)
		{
			last = this->last;
			s->size += this->size;
			(void)delMove(this);
			this = last;
		}

		/* merge adjacent adds */
		if((last = s->last) && last->type == DELTA_ADD)
		{
			last->size += s->size;
			(void)delMove(s);
			s = last;
		} 
		if(s->next && s->next->type == DELTA_ADD)
		{
			s->size += s->next->size;
			(void)delMove(s->next);
		}
		/* done */
		break;
	}
	return s;
}

/* the real thing */
delta(src,n_src,tar,n_tar,delfd)
char	*src;
long	n_src;
char	*tar;
long	n_tar;
int	delfd;
{
	register char	*sp, *tp, *esrc, *etar;
	register long	size, addr;
	Suftree		*tree;
	Move		*moves, *last;
	char		inst, buf[BUFSIZE];
	long		mtchstring();

	/* initialize the output area */
	delinit(buf,delfd);

	/* output file sizes */
	inst = DELTA_TYPE | ((NBYTE(n_src)&07) << 3) | (NBYTE(n_tar)&07);
	if(delputc(inst) < 0)
		return -1;
	if(delputl(NBYTE(n_src),n_src) < 0 || delputl(NBYTE(n_tar),n_tar) < 0)
		return -1;

	/* bases */
	Bsrc = src;
	Btar = tar;
	esrc = src + n_src - 1;
	etar = tar + n_tar - 1;

	/* initialize list and last block */
	moves = NIL(Move);
	last = NIL(Move);

	/* try making suffix tree */
	if(!(tree = n_tar > 0 ? bldsuftree(src,n_src) : NIL(Suftree)))
	{
		/* not enough space for tree, remove matching prefix and suffix */
		for(; src <= esrc && tar <= etar; ++src, ++tar)
			if(*src != *tar)
				break;
		if((size = src-Bsrc) > 0)
		{
			register int cost_m, cost_a;

			cost_m = NBYTE(size) + NBYTE(0);
			cost_a = NBYTE(size) + size;
			if(cost_m < cost_a)
			{
				moves = newMove(DELTA_MOVE,size,0L,NIL(Move));
				if(!moves)
					return -1;
				n_src -= src-Bsrc;
				n_tar -= tar-Btar;
			}
			else
			{
				src = Bsrc;
				tar = Btar;
			}
		}

		for(sp = esrc, tp = etar; sp >= src && tp >= tar; --sp, --tp)
			if(*sp != *tp)
				break;
		if((size = esrc-sp) > 0)
		{
			addr = sp+1-Bsrc;
			if(chkMove(size,addr,0L) > 0)
			{
				last = newMove(DELTA_MOVE,size,addr,NIL(Move));
				if(!last)
					return -1;
				esrc = sp;
				etar = tp;
				n_tar = etar-tar+1;
				n_src = esrc-src+1;
			}
		}

		/* try making the tree again */
		tree = n_tar > 0 ? bldsuftree(src,n_src) : NIL(Suftree);
	}

	/* compute block moves */
	tp = NIL(char);
	while(n_tar > 0)
	{
		char	*match;

		if(tree)
			size = mtchsuftree(tree,tar,n_tar,&match);
		else	size = mtchstring(src,n_src,tar,n_tar,&match);
		if(size < 0)
			return -1;
		if(size > 0)
			size = chkMove(size,(long)(match-Bsrc),(long)(tp ? tp-tar : 0));

		/* output a block move */
		if(size > 0)
		{
			if(tp)
			{
				moves = makeAdd(tp,tar,moves);
				if(!moves)
					return -1;
				tp = NIL(char);
			}
			moves = newMove(DELTA_MOVE,size,(long)(match-Bsrc),moves);
			if(!moves)
				return -1;
			tar += size;
			n_tar -= size;
		}
		else
		{
			if(!tp)
				tp = tar;
			tar += 1;
			n_tar -= 1;
		}
	}

	/* add any remaining blocks */
	if(tp)
	{
		if(last && chkMove(last->size,last->addr,(long)(tar-tp)) <= 0)
		{
			tar += last->size;
			last = delMove(last);
		}
		moves = makeAdd(tp,tar,moves);
		if(!moves)
			return -1;
	}
	if(last)
	{
		moves->next = last;
		last->last = moves;
	}

	/* release space use for string matching */
	if(tree)
		delsuftree(tree);
	else	mtchstring(NIL(char),0L,NIL(char),0L,NIL(char*));

	/* optimize move instructions */
	if(moves)
	{
		register Move	*this;

		this = moves;
		while(this->last)
			this = this->last;
		for(; this != NIL(Move); this = this->next)
			if(this->type == DELTA_MOVE && this->size <= (M_MAX+A_MAX))
				moves = this = optMove(this);

		while(moves->last)
			moves = moves->last;
	}

	/* write out the move instructions */
	addr = 0L;
	while(moves)
	{
		if(moves->type == DELTA_ADD)
			moves->addr = addr;
		addr += moves->size;
		if(putMove(moves) < 0)
			return -1;
		moves = delMove(moves);
	}

	/* write ending token */
	delputc((char)DELTA_TYPE);

	/* flush buffer */
	return delflush();
}
