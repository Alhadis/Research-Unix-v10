
# line 1 "prefix.mt"

#include <stdio.h>
#include <ctype.h>
typedef	struct node *	NODEPTR;
#define	COST		int
#define	INFINITY	1000000
#define DEFAULT_COST	0
#define COSTLESS(x,y)	((x)<(y))

NODEPTR Root;
struct node {
	char op;		/* '\0' if node is a leaf */
	NODEPTR left, right;
	char id;
	int mark;
};

# line 1 "/usr/tjiang/twig/walker.c1"
/*
 * The machine is laid out as a sequence of integers in the walker.
 * The form described above is only used inside the preprocessor.
 * Each machine state is one of the following sequence:
 *
 * TABLE <value_1><index_1>...<value_n><index_n> -1 [FAIL <fail_index>] -1
 * or
 * TABLE2 <value_1><index_1>...<value_n><index_n> -1 [FAIL <fail_index>] -1
 * or
 * ACCEPT <accept table index> -1
 *
 * The accept table is in the form:
 *
 * <tree index_1> ...<tree_index_m> -1
 *
 */
/* Keutzer - all "short int"'s have been changed to "short" for
   portability to UTS. */
#include "symbols.h"
/* Keutzer  1/88- _mtG now uses varargs for portability to SUN4 etc.*/
#include <varargs.h>

#ifndef FILE
#include <stdio.h>
#endif
#include <assert.h>

/* These constants must be the same as the ones in machine.c */
#define FASTLIM	0
#define TABLE	1
#define FAIL	2
#define ACCEPT	3
#define TABLE2	4
#define EOT	-1

/* special machine state */
#define HANG	-1

/* used by the evaluator to interpret path table */
#define	eSTOP	0
#define	ePOP	-1
#define eEVAL	-2
#define eNEXT	-3
#define ePUSH	-4

/* Tags that indicate the type of a value */
#define M_BRANCH 010000
#define M_NODE	0
#define M_LABEL 01000
#define MAX_NODE_VALUE 00777
#define MTAG_SHIFT 9
#define M_DETAG(x)	((x)&~(M_BRANCH|M_LABEL|M_NODE))

/* predicates to tell whether a value x is of type NODE, BRANCH, or LABEL */
#define MI_NODE(x)	(((x)&(M_BRANCH|M_LABEL))==0)
#define MI_DEFINED(x)	((x)!=-1)
#define MI_LABEL(x)	(((x)&M_LABEL)!=0)
#define MI_BRANCH(x)	(((x)&M_BRANCH)!=0)

/* build a tagged value */
#define MV_NODE(x)	(x)
#define MV_BRANCH(x)	((x)|M_BRANCH)
#define MV_LABEL(x)	((x)|M_LABEL)

/* limits */
/*
 * The depth of a pattern must be 7 or less.  Otherwise the type of he
 * partial mask in skeleton must be changed
 */
#define MAXDEPTH	7

/* modes */
#define xTOPDOWN	3
#define xABORT		2
#define xREWRITE	1
#define xDEFER		0

/* macros */
#define tDO(m)	_do((m)->skel, (m), 1)
#define REWRITE	return(_m->cost = cost, xREWRITE)
#define TOPDOWN return(_m->cost = cost, xTOPDOWN)
#define ABORT return(xABORT)

/*
 * REORDER macro allows a knowledgeable user to change the order
 * of evaluation of the leaves.
 */
#ifndef REORDER
#define REORDER(list)	_evalleaves(list)
#endif
#define EVAL	REORDER(_ll)

#define mpush(s,m)	(m)->next = s, s = m

/* skeleton structure */
typedef struct skeleton skeleton;
typedef struct __match __match;
typedef struct __partial __partial;
typedef struct __hshcls	__hshcls;	/* a hashed closure entry */
typedef int labelset;			/* a bit vector of labels */

struct __partial {
	unsigned char treeno;
	char bits;
};

struct __hshcls {
	__hshcls *next;
	labelset labels;
	int treecnt;		/* number of partial matches */
	__partial partial[MAXTREES];
};

struct skeleton {
	skeleton *sibling;
	skeleton *leftchild;		/* leftmost child */
	skeleton *rightchild;		/* rightmost child */
	NODEPTR	root;
	NODEPTR parent;			/* our parent */
	int nson;
	int treecnt;
	__match *succ[MAXLABELS];
	__partial *partial;
	__match *winner;
	COST mincost;
};

struct __match {
	__match	**lleaves;	/* labelled leaves */
	skeleton *skel;		/* back pointer to associated skeleton node */
	COST cost;
	short tree;
	short mode;
};

struct freeb {
	struct freeb *next;
};

struct _mem {
        struct _mem *next;
        int size;       /* size of individual elements in bytes */
        int blkf;       /* blocking factor */
        int bsize;      /* block size */
        char *block;    /* block */
        int cnt;        /* count of free elem in block */
        char *freelist; /* free list */
        int totelem;    /* total number elements we have */
        int freecnt;    /* number of times mem_free was called */
};

/* turn this flag on if your machine has a fast byte string zero operation */
/*#define BZERO	1*/
int mtDebug = 0;
int treedebug = 0;
extern int _machstep();
extern short mtStart;
extern short mtTable[], mtAccept[], mtMap[], mtPaths[], mtPathStart[];
#ifdef LINE_XREF
	extern short mtLine[];
#endif

#ifndef MPBSIZE
#define MPBSIZE 8000
#endif

#ifdef ADDCOST
#define DEFAULT_COST sumleaves(_ll)
COST sumleaves(list) __match **list;
{COST cost;
 cost=ZEROCOST;
 for(; *list; list++)
   {ADDCOST((cost),(*list)->cost);
   }
 return cost;
}
#endif

extern NODEPTR mtGetNodes(), mtAction();
extern skeleton *_allocskel();
extern __match *_allocmatch();
extern __partial *_allocpartial();

__match *_mpblock[MPBSIZE], **_mpbtop;

__match **
_getleaves (mp, skp)
	register __match *mp; register skeleton *skp;
{
	skeleton *stack[MAXDEPTH];
	skeleton **stp = stack;
	register short *sip = &mtPaths[mtPathStart[mp->tree]];
	register __match **mmp = _mpbtop;
	__match **mmp2 = mmp;
	if((_mpbtop += *sip++ + 1) > &_mpblock[MPBSIZE]) {
		printf("Tree too large: make MPBSIZE larger.\n\
(i.e. cc -c -DMPBSIZE=%d walker.c)\n",MPBSIZE*2);
		assert(0);
	}

	for(;;)
		switch(*sip++) {
		case ePUSH:
			*stp++ = skp;
			skp = skp->leftchild;
			break;

		case eNEXT:
			skp = skp->sibling;
			break;

		case eEVAL:
			if ((mp = skp->succ[M_DETAG(*sip++)])==NULL) abort();
			*mmp++ = mp;
			break;

		case ePOP:
			skp = *--stp;
			break;

		case eSTOP:
			*mmp = NULL;
			return (mmp2);
		}
}

_evalleaves (mpp)
	register __match **mpp;
{
	for (; *mpp!=NULL; mpp++) {
		register __match *mp = *mpp;
		_do (mp->skel, mp, 1);
	}
}


_do(sp, winner, evalall)
	skeleton *sp; register __match *winner; int evalall;
{
	register __match **mpp;
	register skeleton *skel = winner->skel;
	if(winner==NULL) {
		_nowin(sp);
		return;
	}
	if(winner->mode==xDEFER || (evalall && winner->mode!=xTOPDOWN))
		REORDER(winner->lleaves);
	mtSetNodes (skel->parent, skel->nson,
		skel->root=mtAction (winner->tree, winner->lleaves, sp));
}

skeleton *
_walk(sp, ostate)
	register skeleton *sp;
	int ostate;
{
	int state, nstate, nson;
	register __partial *pp;
	register __match *mp, *mp2;
	register skeleton *nsp, *lastchild = NULL;
	NODEPTR son, root;

	root = sp->root;
	nson = 1; sp->mincost = INFINITY;
	state = _machstep (ostate, root, mtValue(root));

	while((son = mtGetNodes(root, nson))!=NULL) {
		nstate = _machstep (state, NULL, MV_BRANCH(nson));
		nsp = _allocskel();
		nsp->root = son;
		nsp->parent = root;
		nsp->nson = nson;
		_walk(nsp, nstate);
		if(COSTLESS(nsp->mincost,INFINITY)) {
			assert (nsp->winner->mode==xREWRITE);
			_do(nsp, nsp->winner, 0);
			_freeskel(nsp);
			continue;
		}
		_merge (sp, nsp);
		if (lastchild==NULL) sp->leftchild = nsp;
		else lastchild->sibling = nsp;
		lastchild = nsp;
		nson++;
	}

	for (pp = sp->partial; pp < &sp->partial[sp->treecnt]; pp++)
		if (pp->bits&01==1) {
			mp = _allocmatch();
			mp->tree = pp->treeno;
			_addmatches (ostate, sp, mp);
		}
	if(son==NULL && nson==1)
		_closure (state, ostate, sp);

	sp->rightchild = lastchild;
	if (root==NULL) {
		COST c; __match *win; int i; nsp = sp->leftchild;
		c = INFINITY;
		win = NULL;
		for (i = 0; i < MAXLABELS; i++) {
			mp = nsp->succ[i];
			if (mp!=NULL && COSTLESS (mp->cost, c))
				{ c = mp->cost; win = mp; }
		}
		_do (nsp, win, 0);
	}
	return(sp);
}

/*
 * Convert the start state which has a large branching factor into
 * a index table.  This must be called before the matcher is used.
 */
static short _nodetab[MAXNDVAL], _labeltab[MAXLABELS];
_matchinit()
{
	short *sp;
/* Keutzer - this syntax doesn't work on UTS
	struct pair { short val, where } *pp; 
*/
	struct pair { short val; short where; } *pp; 
	for (sp=_nodetab; sp < &_nodetab[MAXNDVAL]; sp++) *sp = HANG;
	for (sp=_labeltab; sp < &_labeltab[MAXLABELS]; sp++) *sp = HANG;
	sp = &mtTable[mtStart];
	assert (*sp==TABLE);
	for (pp = (struct pair *) ++sp; pp->val!=EOF; pp++) {
		if (MI_NODE(pp->val))
			_nodetab[M_DETAG(pp->val)] = pp->where;
		else if (MI_LABEL(pp->val))
			_labeltab[M_DETAG(pp->val)] = pp->where;
	}
}

int
_machstep(state, root, input)
	short state, input;
	NODEPTR root;
{
	register short *stp = &mtTable[state];
	int start = 0;
	if (state==HANG)
		return (input==(MV_BRANCH(1)) ? mtStart : HANG);
rescan:
	if (stp==&mtTable[mtStart]) {
		if (MI_NODE(input)) return(_nodetab[M_DETAG(input)]);
		if (MI_LABEL(input)) return(_labeltab[M_DETAG(input)]);
	}
	
	for(;;) {
		if(*stp==ACCEPT) stp += 2;

		if(*stp==TABLE) {
			stp++;
			while(*stp!=EOT) {
				if(input==*stp) {
					return(*++stp);
				}
				stp += 2;
			}
			stp++;
		}
		if(*stp!=FAIL) {
			if (start) return (HANG);
			else { stp = &mtTable[mtStart]; start = 1;  goto rescan;}
		} else {
			stp++;
			stp = &mtTable[*stp];
			goto rescan;
		}
	}
}

_addmatches(ostate, sp, np)
	int ostate;
	register skeleton *sp;
	register __match *np;
{
	int label;
	int state;
	register __match *mp;

        label = mtMap[np->tree];

	/*
	 * this is a very poor substitute for good design of the DFA.
	 * What we need is a special case that allows any label to be accepted
	 * by the start state but we don't want the start state to recognize
	 * them after a failure.
	 */
	state = _machstep(ostate, NULL, MV_LABEL(label));
	if (ostate!=mtStart  && state==HANG) {
		_freematch(np);
		return;
	}

	np->lleaves = _getleaves (np, sp);
	np->skel = sp;
        if((np->mode = mtEvalCost(np, np->lleaves, sp))==xABORT)
	{
		_freematch(np);
		return;
	}

/*
	if(np->mode==xREWRITE && COSTLESS(np->cost, sp->mincost)) {
		sp->mincost = np->cost;
		sp->winner = np;
	}
*/
	if ((mp = sp->succ[label])!=NULL) {
		if (!COSTLESS (np->cost, mp->cost))
			{ _freematch(np); return; }
		else _freematch(mp);
	}
	if(COSTLESS(np->cost,sp->mincost)) {
		if(np->mode==xREWRITE) {
			sp->mincost = np->cost; sp->winner = np; }
		else { sp->mincost = INFINITY; sp->winner = NULL; }
	}
	sp->succ[label] = np;
	_closure(state, ostate, sp);
}

_closure(state, ostate, skp)
	int state, ostate;
	skeleton *skp;
{
	register struct ap { short tree, depth; } *ap;
	short *sp = &mtTable[state];
	register __match *mp;

	if(state==HANG || *sp!=ACCEPT) return(NULL);

	for(ap = (struct ap *) &mtAccept[*++sp]; ap->tree!=-1; ap++)
		if (ap->depth==0) {
			mp = _allocmatch();	
			mp->tree = ap->tree;
			_addmatches (ostate, skp, mp);
		} else {
			register __partial *pp, *lim = &skp->partial[skp->treecnt];
			for(pp=skp->partial; pp < lim; pp++)
				if(pp->treeno==ap->tree)
					break;
			if(pp==lim) {
				skp->treecnt++;
				pp->treeno = ap->tree;
				pp->bits = (1<<(ap->depth));
			} else pp->bits |= (1<<(ap->depth));
		}
}

_merge (old, new)
	skeleton *old, *new;
{
	register __partial *op = old->partial, *np = new->partial;
	int nson = new->nson;
	register __partial *lim = np + new->treecnt;
	if (nson==1) {
		old->treecnt = new->treecnt;
		for(; np < lim; op++, np++) {
			op->treeno = np->treeno;
			op->bits = np->bits/2;
		}
	} else {
		__partial *newer = _allocpartial();
		register __partial *newerp = newer;
		register int cnt;
		lim = op+old->treecnt;
		for(cnt=new->treecnt; cnt-- ; np++) {
			for(op = old->partial; op < lim; op++)
				if (op->treeno == np->treeno) {
					newerp->treeno = op->treeno;
					newerp++->bits = op->bits & (np->bits/2);
					break;
				}
		}
		_freepartial(old->partial);
		old->partial = newer;
		old->treecnt = newerp-newer;
	}
}
 
/* Save and Allocate Matches */
#define BLKF	100
__match *freep = NULL;
static int _count = 0;
static __match *_blockp = NULL;
#ifdef CHECKMEM
int x_matches, f_matches;
#endif

__match *
_allocmatch()
{
	__match *mp;

	if(freep!=NULL) {
		mp = freep;
		freep = ((__match *)((struct freeb *) freep)->next);
#ifdef CHECKMEM
		f_matches--;
#endif
	} else {
		if(_count==0) {
			_blockp = (__match *) malloc (BLKF*sizeof (__match));
			_count = BLKF;
#ifdef CHECKMEM
			x_matches += BLKF;
#endif
		}
		mp = _blockp++;
		_count--;
	}
	return(mp);
}

_freematch(mp)
	__match *mp;
{
	((struct freeb *) mp)->next = (struct freeb *) freep;
	freep = mp;
#ifdef CHECKMEM
	f_matches++;
#endif
}

static __partial *pfreep = NULL;
static int pcount = 0;
static __partial *pblock = NULL;
#ifdef CHECKMEM
static int x_partials, f_partials;
#endif

__partial *
_allocpartial()
{
	__partial *pp;
	if (pfreep!=NULL) {
		pp = pfreep;
		pfreep = *(__partial **) pp;
#ifdef CHECKMEM
		f_partials--;
#endif
	} else {
		if (pcount==0) {
			pblock=(__partial *)malloc(BLKF*MAXTREES*sizeof(__partial));
			pcount = BLKF;
#ifdef CHECKMEM
			x_partials += BLKF;
#endif
		}
		pp = pblock;
		pblock += MAXTREES;
		pcount--;
	}
	return(pp);
}

_freepartial(pp)
	__partial *pp;
{
	* (__partial **)pp = pfreep;
	pfreep = pp;
#ifdef CHECKMEM
	f_partials++;
#endif
}


/* storage management */
static skeleton *sfreep = NULL;
static int scount = 0;
static skeleton * sblock = NULL;

skeleton *
_allocskel()
{
	skeleton *sp;
	int i;
	if(sfreep!=NULL) {
		sp = sfreep;
		sfreep = sp->sibling;
	} else {
		if(scount==0) {
			sblock = (skeleton *) malloc (BLKF * sizeof (skeleton));
			scount = BLKF;
		}
		sp = sblock++;
		scount--;
	}
	sp->sibling = NULL; sp->leftchild = NULL;
	for (i=0; i < MAXLABELS; sp->succ[i++] = NULL);
	sp->treecnt = 0;
	sp->partial = _allocpartial();
	return(sp);
}

_freeskel(sp)
	skeleton *sp;
{
	int i;
	__match *mp;
	if(sp==NULL) return;
	_freeskel (sp->leftchild);
	_freeskel (sp->sibling);
	_freepartial (sp->partial);
	for (i=0; i < MAXLABELS; i++)
		if ((mp = sp->succ[i])!=NULL) _freematch (mp);
	sp->sibling = sfreep;
	sfreep = sp;
}

_match()
{
	skeleton *sp;
	sp = _allocskel();
	sp->root = NULL;
	_mpbtop = _mpblock;
	_freeskel(_walk(sp, HANG));
#ifdef CHECKMEM
	if(f_matches+_count!=x_matches) {
		printf(";#m %d %d %d\n", f_matches, _count, x_matches);
		assert(0);
	}
	if(f_partials+pcount!=x_partials) {
		printf(";#p %d %d %d\n", f_partials, pcount, x_partials);
		assert(0);
	}
#endif
}

/* Keutzer  1/88- _mtG now uses varargs for portability to SUN4 etc.*/
NODEPTR _mtG(va_alist)
va_dcl
{
    NODEPTR root;
    int i=0;
    va_list p_var;
    va_start(p_var);
    root = va_arg(p_var,NODEPTR);
    while( (i = va_arg(p_var,int)) != -1){
        root = mtGetNodes(root, i);
    }
    va_end(p_var);
    return(root);
}

/* diagnostic routines */

markIfNoMatch(skp,mark) skeleton *skp; int mark;
{skeleton *child; int i; int found=0;
 for(i=0; i<MAXLABELS; i++)
	 if (skp->succ[i]) found=1;
 if (!found) skp->root->mark=mark;
 for(child=skp->leftchild; child; child=child->sibling)
	markIfNoMatch(child,mark);
}

_nowin(skp) skeleton *skp;
{
  markIfNoMatch(skp,1);
  printf("# No match was found for the nodes printed in lowercase\n");
  printTree(skp->root);
  markIfNoMatch(skp,0);
}
short int mtTable[] = {
3,0,-1,3,3,-1,1,01000,3,-1
,-1,3,6,-1,1,01000,11,-1,-1,1
,010001,6,010002,14,-1,-1,1,01,0,00
,19,-1,-1};
short int mtAccept[] = {
0,0,-1,1,1,-1,1,1,-1};
short int mtStart = 26;
short int mtMap[] = {
00,00};
short int mtPaths[] = {
2,-4,-2,01000,-3,-2,01000,-1,0,0
,0 };
short int mtPathStart[] = {
9,0};
NODEPTR
mtAction (_t, _ll, _s)
int _t; __match **_ll; skeleton *_s;
{ NODEPTR root = _s->root;
switch (_t) {
case 0:{

# line 24 "prefix.mt"
 putchar(root->id); } break;
case 1:{

# line 28 "prefix.mt"

		putchar(root->op);
		tDO(_ll[(1)-1]); tDO(_ll[(2)-1]);
	} break;
} return(_s->root);}
mtEvalCost(_m, _ll, _s)
__match **_ll; __match *_m; skeleton *_s;
{ NODEPTR root = _s->root;
COST cost; cost = DEFAULT_COST;
switch(_m->tree) {
case 1:{

# line 27 "prefix.mt"
 TOPDOWN; } break;
}
_m->cost = cost; return(xDEFER);}

# line 33 "prefix.mt"


mtValue(root)
	NODEPTR root;
{
	if(root==NULL)
		return(0);
	if(root->op==0)
		return(nIdent);
	else
		return(nOp);
}

NODEPTR
mtGetNodes(r,n)
	NODEPTR r;
	int n;
{
	if(r==NULL)
		if(n==1)
			return(Root);
		else
			return(NULL);
	if(n==1)
		return(r->left);
	else if(n==2)
		return(r->right);
	else return(NULL);
}

mtSetNodes(r,n,c)
	NODEPTR r,c;
	int n;
{
	if(r==NULL && n==1) {
		Root = c;
		return;
	}
	if(n==1)
		r->left = c;
	else if(n==2)
		r->right = c;
}

syntax_error()
{
	printf("syntax error\n");
	exit(1);
}

/* allocated a node */
NODEPTR
getnode(op,id)
	char op,id;
{
	NODEPTR n;
	n = (NODEPTR) malloc(sizeof(struct node));
	n->op = op;
	n->left = n->right = NULL;
	n->id = id;
	return(n);
}

NODEPTR
gettree()
{
	char c;
	NODEPTR n = NULL;

	c = getchar();
	if(c==EOF) 
		syntax_error();

	n = getnode(0,0);

	/* build node for left operand */
	if(c=='(')
		n->left = gettree();
	else
		n->left = getnode(0,c);

	c = getchar();
	if(strchr("+-*/",c)==NULL)
		syntax_error();
	n->op = c;

	c = getchar();
	/* build node for right operand */
	if(c=='(')
		n->right = gettree();
	else
		n->right = getnode(0,c);

	c = getchar();
	if(c!=')') 
		syntax_error();

	return(n);
}

main()
{
	char c;
	_matchinit();		/* initialize the matcher */

	/* get the initial ( */
	c = getchar();
	if(c!='(')
		syntax_error();

	Root = gettree();
	_match();
	putchar('\n');
}
printTree(x)
{}


