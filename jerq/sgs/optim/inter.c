/*	static char  ID[] = "@(#) inter.c: 1.3 10/5/82";	*/

#include "optim.h"

NODE *lastnode;	/* reference to node being built by Saveop */

	NODE *
Saveop(opn, str, len, op) /* save part of inst */
	register int opn; register char *str; unsigned len; int op; {

	register NODE *p = lastnode;

	if (opn == 0) { /* make a new node and link it in */
		p = lastnode = GETSTR(NODE);
		if ((p->op = op) != GHOST) {
			INSNODE(p, &ntail);
			ninst++;
		}
		for (op = 0; op < MAXOPS; )
			p->ops[++op] = NULL;
#ifdef LIVEDEAD
		p->nlive = p->ndead = 0;
#endif
#ifdef IDVAL
		p->uniqid = IDVAL;
#endif
	}
	if (opn < 0 || opn > MAXOPS)
		fatal("invalid opn field for %s\n", str);
	p->ops[opn] = COPY(str, len);
	return (p);
}

	void
addref(str, len) char *str; unsigned len; { /* add text ref to reference list */

	register REF *r = lastref = lastref->nextref = GETSTR(REF);

	r->lab = COPY(str, len);
	r->nextref = NULL;
}

	void
filter() { /* print FILTER nodes and remove from list */

	register NODE *p;

	for (ALLN(p))
		if (p->op == FILTER) {
			(void) puts(p->ops[0]);
			DELNODE(p);
		}
}

	void
prtext() { /* print text list */

	register NODE *p;

	for (ALLN(p)) {
		prinst(p);
	}
}

	boolean
same(p, q) NODE *p, *q; { /* return true iff nodes are the same */

	register char **pp, **qq;
	register int i;

	if (p->op != q->op)
		return (false);

	/* first check for equal numbers of active operands */

	for (pp = p->ops, qq = q->ops, i = MAXOPS + 1;
	    --i >= 0 && (*pp != NULL || *qq != NULL); pp++, qq++)
		if (*pp == NULL || *qq == NULL)
			return (false);

	/* then check for equality of the active operands */

	while (pp > p->ops)
		if (**--pp != **--qq || strcmp(*pp, *qq))
			return (false);
	return (true);
}

	boolean
sameaddr(p, q) NODE *p, *q; { /* return true iff ops[1...] are the same */

	register char **pp, **qq;
	register int i;

	/* first check for equal numbers of active operands */

	for (pp = p->ops, qq = q->ops, i = MAXOPS + 1;
	    --i >= 0 && (*pp != NULL || *qq != NULL); pp++, qq++)
		if (*pp == NULL || *qq == NULL)
			return (false);

	/* then check for equality of the active operands */

	while (pp > p->ops + 1)
		if (**--pp != **--qq || strcmp(*pp, *qq))
			return (false);
	return (true);
}

	char *
xalloc(n) register unsigned n; { /* allocate space */

	extern char *malloc();
	register char *p;

	if ((p = malloc(n)) == NULL)
		fatal("out of space\n", (char *)NULL);
	return (p);
}

	void
xfree(p) char *p; { /* free up space allocated by xalloc */

	extern void free();

	free(p);			/* return space */
}

	void
fatal(fmt, str) char *fmt, *str; { /* report fatal error and quit */

	extern void exit();

	FPRINTF(stderr, "Optimizer: ");
	FPRINTF(stderr, fmt, str);
	exit(2);
}
