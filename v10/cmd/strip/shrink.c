#include "strip.h"
#include "hash.h"
#define	DESCSYMBS
#include "stab.h"

struct hn {
	struct nlist *symbol;
	int len;
};

static int nglobs; static struct hn *tabstart, *tabend;

shrink(a)
register struct Adotout *a;
{
	register struct nlist *sym, *ssym;
	register char *str; register int n;

	hashinit();
	nglobs = 0;

	for (sym=a->symtab; sym<a->symend; sym++)
		if (str = sym->n_un.n_name) {
			if (sym->n_type == N_GSYM || sym->n_type == N_BSTR)
				nglobs++;
			if (*(str += (int)a->strtab) == '_')
				sym->n_un.n_name = hash(str);
			else
				sym->n_un.n_name = str;
		}

	for (sym=a->symtab; sym<a->symend; sym++)
		if ((str = sym->n_un.n_name) && *str != '_')
			sym->n_un.n_name = hash(str);

	if (prtflag) prthash();

	Free(a->strtab);
	if ((tabstart = Calloc(struct hn, nglobs)) == 0)
		fatal("cannot malloc hashed symbol table", "");
	tabend = tabstart + nglobs;

	for (sym=ssym=a->symtab; sym<a->symend;) {
		if (gflag && (sym->n_type == N_SLINE ||
			      sym->n_type == N_LBRAC || sym->n_type == N_RBRAC)) {
			sym++;
		} else if (sym->n_type != N_GSYM && sym->n_type != N_BSTR) {
			if (ssym != sym)
				*ssym++ = *sym++;
			else
				ssym++, sym++;
		} else if ((n = lookup(sym, ssym)) < 0) {
			sym -= n;
		} else if (ssym != sym) {
			while (--n >= 0)
				*ssym++ = *sym++;
		} else {
			ssym += n, sym += n;
		}
	}
	Free(tabstart);
	a->symend = ssym;
	a->nsymbols = ssym - a->symtab;
	a->symtab = Realloc(struct nlist, a->symtab, a->nsymbols);
	if (prtflag)
		printf("number of symbols reduced to %d\n", a->nsymbols);
}

lookup(sym, where)
register struct nlist *sym; struct nlist *where;
{
	register struct nlist *t;
	register struct hn *pt; struct hn *ptstart;
	int lsym = symlen(sym);

	pt = ptstart = tabstart +
	    ((unsigned long)(sym->n_un.n_name)) % nglobs;

	do {
		if ((t = pt->symbol) == 0) {
			pt->symbol = where;
			pt->len = lsym;
			return lsym;
		}
		if (t->n_un.n_name != sym->n_un.n_name ||
		    t->n_type != sym->n_type) {
			continue;
		}
		if (pt->len == lsym && nlistcmp(t, sym, lsym) == 0) {
			return -lsym;
		} else {
			return lsym;
		}
	} while ((++pt >= tabend ? pt = tabstart : pt) != ptstart);

	fatal("hashed symbol table full", "");
}

symlen(sym)
register struct nlist *sym;
{
	register len = 1, t, m;
	if (sym[1].n_type == N_TYSIG)
		++len;
	else if (sym->n_type == N_BSTR) {
		do ++len; while ((++sym)->n_type != N_ESTR);
	} else {
		switch (BTYPE(t = sym->n_desc)) {
		case STRTY:
		case UNIONTY:
		case ENUMTY:
			++len;
		}
		for (; m = t&TMASK; t >>= TSHIFT)
			if (m == ARY) ++len;
	}
	return len;
}

nlistcmp(a,b,n)
register int *a, *b, n;
{
	n *= sizeof(struct nlist)/sizeof(int);
	while (--n >= 0)
		if (*a++ != *b++) return 1;
	return 0;
}
