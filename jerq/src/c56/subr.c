#include <stdio.h>
#include "struct.h"

/*
 * Initialize the dictionary with one node for each
 * possible character.  To make sure these nodes are
 * never deleted, pretend that each node already
 * has a child and delete it from the LRU queue.
 */	
crunchinit()
{
	register int c;

	nd(ROOTNODE).children++;
	for (c = MINCHAR; c < MAXCHAR; c++) {
		register nodep n;
		register struct node *np;
		n = makenode (ROOTNODE, c);
		np = &nd(n);
		delp (np);
		np->children++;
	}

	setchild (prevnode, ROOTNODE, '\n');
}

extend (p, s)
	register nodep p;
	nodep s;
{
	struct strdesc sd;
	register int i;
	register char *cp;
	nodep oldp = p;
	register struct node *sp = &nd(s);

	sd = nodestr (s);
	i = sd.len;
	cp = sd.ptr;

	/*
	 * Protect the suffix by giving it a ghost child
	 * and deleting it from the LRU queue if needed.
	 */
	if (sp->children == 0) {
		delp (sp);
	}
	++sp->children;

	while (--i >= 0) {
		register nodep newp;
		setchild (newp, p, *cp);
		if (newp == NULLNODE)
			newp = makenode (p, *cp);
		cp++;
		if (newp == NULLNODE)
			break;
		p = newp;
	}

	/* Unprotect the suffix if needed. */
	if (--sp->children == 0)
		inshead (s);
}

/*
 * Make a new node with parent 'node' and branch 'ch'.
 * We are guaranteed that such a node does not already exist.
 * Return a pointer to the newly made node.
 */
nodep
makenode (parent, ch)
	nodep parent;
	unsigned char ch;
{
	static nodep sn = 1;
	register nodep n;
	register int h;
	register struct node *pnode = &nd(parent);

	/* Make sure we can't recycle the parent */
	if (pnode->children == 0)
		delp (pnode);
	pnode->children++;

	/* allocate a new node, recycling if necessary */
	if (sn < MAXNODE)
		n = sn++;
	else {
		/* take a node off the LRU chain */
		n = LRUHEAD.back;
		if (n != NULLNODE) {
			register struct node *np = &nd(n);
			delp (np);

			/* delete the node from its hash chain */
			h = inithash (np->parent, np->ch);
			if (htab[h] == n)
				htab[h] = np->hash;
			else {
				register nodep q, p = htab[h];
				register struct node *pp = &nd(p);
				while ((q = pp->hash) != n) {
					p = q;
					pp = &nd(p);
				}
				pp->hash = np->hash;
			}

			/*
			 * If this node was the last child of its parent,
			 * put the parent on the LRU chain.
			 */
			if (--nd(np->parent).children == 0)
				instail (np->parent);
		}
	}

	if (n != NULLNODE) {
		register struct node *np = &nd(n);

		np->ch = ch;
		np->parent = parent;
		np->children = 0;

		h = inithash (parent, ch);
		np->hash = htab[h];
		htab[h] = n;

		/* The newly made node goes on the LRU queue */
		inshead (n);
	} else {
		/* couldn't get a node, maybe put parent back */
		if (--pnode->children == 0)
			inshead (parent);
	}

	return n;
}

/* return the string represented by a node */
struct strdesc
nodestr (node)
	nodep node;
{
	static char buf[MAXNODE];
	register char *p;
	register nodep n;
	struct strdesc sd;

	n = node;
	p = &buf[MAXNODE];

	while (n != NULLNODE) {
		*--p = nd(n).ch;
		n = nd(n).parent;
	}

	sd.ptr = p;
	sd.len = buf + MAXNODE - p;
	return sd;
}

/* Insert a node at the back of the LRU queue */
instail (n)
	register nodep n;
{
	register struct node *p = &nd(n);
	p->lru.forw = LRUNODE;
	p->lru.back = LRUHEAD.back;
	nd(LRUHEAD.back).lru.forw = n;
	LRUHEAD.back = n;
}
