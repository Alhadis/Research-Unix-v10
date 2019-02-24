#include	"mk.h"

Arc *
newarc(n, r, stem, match)
	Node *n;
	Rule *r;
	char *stem;
	regsubexp *match;
{
	register Arc *a;

	a = (Arc *)Malloc(sizeof(Arc));
	a->n = n;
	a->r = r;
	a->stem = strdup(stem);
	memcpy((char *)a->match, (char *)match, sizeof a->match);
	a->next = 0;
	a->flag = 0;
	a->prog = r->prog;
	return(a);
}

dumpa(s, a)
	char *s;
	register Arc *a;
{
	char buf[1024];

	sprint(buf, "%s    ", (*s == ' ')? s:"");
	Fprint(1, "%sArc@%ld: n=%ld r=%ld flag=0x%x stem='%s'",
		s, a, a->n, a->r, a->flag, a->stem);
	if(a->prog)
		Fprint(1, " prog='%s'", a->prog);
	Fprint(1, "\n");
	if(a->n)
		dumpn(buf, a->n);
}

nrep()
{
	register Symtab *sym;

	if(sym = symlook("NREP", S_VAR, (char *)0))
		nreps = atoi(sym->value);
	if(nreps < 1)
		nreps = 1;
	if(DEBUG(D_GRAPH))
		Fprint(1, "nreps = %d\n", nreps);
}
