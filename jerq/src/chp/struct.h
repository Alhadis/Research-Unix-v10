/*
 * Unless mentioned otherwise, defines whose names start
 * with MAX really represent values one larger than the
 * maximum possible value.  This allows them to be used
 * for array bounds, equality comparisons, and so on.
 */

#define MINCHAR 0
#define MAXCHAR 0x100

#define MAXNODE 4096
#define MAXHASH 4096

#define ROOTNODE 0
#define NULLNODE 0

/* starting hash bucket, given a parent node and a character */
#define inithash(node,ch) (((((int)(unsigned char)ch)<<2)^node)&(MAXHASH-1))

/* given a node and a branch label, find the appropriate child, if any */
#define setchild(h,n,c) {\
	h = htab[inithash (n, c)];\
	while (h != NULLNODE) {\
		register struct node *hn = &nd(h);\
		if (hn->parent == n && hn->ch == c)\
			break;\
		h = hn->hash;\
	}\
}

/* Insert a node at the front of the LRU queue */
#define inshead(n) {\
	register struct node *p = &nd(n);\
	p->lru.back = LRUNODE;\
	p->lru.forw = LRUHEAD.forw;\
	nd(LRUHEAD.forw).lru.back = n;\
	LRUHEAD.forw = n;\
}

/* Delete a node from the LRU queue, given a true pointer to the node */
#define delp(p) {\
	register struct node *f = &nd(p->lru.forw);\
	register struct node *b = &nd(p->lru.back);\
	b->lru.forw = p->lru.forw;\
	f->lru.back = p->lru.back;\
}

typedef short int nodep;

/*
 * This macro maps a nodep into the corresponding struct node.
 * It is written as a macro to allow alternate choices in the
 * future about the time-space tradeoff in representing node
 * pointers as true pointers or array indices.
 */
#define nd(p) nodes[p]

struct qptrs {
	nodep forw;
	nodep back;
};

struct node {
	struct qptrs lru;
	nodep parent;
	nodep hash;
	short children;
	unsigned char ch;
};

struct strdesc {
	char *ptr;
	int len;
};

struct node nodes[MAXNODE];
nodep prevnode;
nodep htab[MAXHASH];

/*
 * The LRU chain is anchored in the root node, which is
 * guaranteed never to be a "legitimate" element of the chain.
 */
#define LRUHEAD nd(ROOTNODE).lru
#define LRUNODE ROOTNODE

nodep makenode();
struct strdesc nodestr();
