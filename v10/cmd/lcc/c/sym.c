/* C compiler: symbol table management */
#include "c.h"

struct entry {		/* symbol table entry: */
	struct symbol sym;	/* the symbol */
	List refs;		/* list form of sym.uses (omit) */
	struct entry *link;	/* next entry on hash chain */
};
struct table {		/* symbol tables: */
	int level;		/* scope level for this table */
	struct table *previous;	/* table for previous scope */
	Symbol list;		/* list of entries via up fields */
	struct entry *buckets[HASHSIZE];
};
static struct table
	tconstants =   { CONSTANTS },
	texternals =   { GLOBAL },
	tidentifiers = { GLOBAL },
	ttypes =       { GLOBAL };
Table constants	  = &tconstants;	/* constants */
Table externals	  = &texternals;	/* externals */
Table identifiers = &tidentifiers;	/* identifiers */
Table globals	  = &tidentifiers;	/* globals */
Table labels[2];			/* labels */
Table types	  = &ttypes;		/* types */
static struct temporary {/* temporaries: */
	Symbol sym;		/* pointer to the symbol */
	struct temporary *link;	/* next available temporary */
} *temps;		/* list of available temporaries */
int bnumber;		/* current block number */
int level;		/* current block level */
List symbols;		/* list of all symbols; used only if xref != 0 */

Table table(tp, lev) Table tp; {
	int i;
	Table new = (Table)talloc(sizeof *new);

	assert(lev > GLOBAL || lev == LABELS);
	new->previous = tp;
	new->level = lev;
	new->list = tp ? tp->list : (Symbol)0;	/* (omit) */
	for (i = 0; i < HASHSIZE; i++)
		new->buckets[i] = 0;
	return new;
}
void foreach(tp, lev, apply, cl) Table tp;
dclproto(void (*apply),(Symbol, Generic)); Generic cl; {
	assert(tp);
	while (tp && tp->level > lev)
		tp = tp->previous;
	if (tp && tp->level == lev) {
		Symbol p;
		Coordinate sav;
		sav = src;
		for (p = tp->list; p && p->scope == lev; p = p->up) {
			src = p->src;
			(*apply)(p, cl);
		}
		src = sav;
	}
}
void enterscope() {
	if (++level >= USHRT_MAX)
		error("compound statements nested too deeply\n");
}
void exitscope() {
	rmtypes();
	rmtemps(0, level);
	if (identifiers->level == level) {
		if (Aflag >= 2) {
			int n = 0;
			Symbol p;
			for (p = identifiers->list; p && p->scope == level; p = p->up)
				if (++n > 127) {
					warning("more than 127 identifiers declared in a block\n");
					break;
				}
		}
		if (xref)	/* (omit) */
			setuses(identifiers);	/* (omit) */
		identifiers = identifiers->previous;
	}
	if (types->level == level) {
		if (xref) {	/* (omit) */
			foreach(types, level, fielduses, (Generic)0);	/* (omit) */
			setuses(types);	/* (omit) */
		}	/* (omit) */
		types = types->previous;
	}
	assert(level >= GLOBAL);
	--level;
}
Symbol install(name, tpp, perm) char *name; Table *tpp; {
	struct entry *p;
	unsigned h = (unsigned)name&(HASHSIZE-1);

	if ((tpp == &identifiers || tpp == &types) && (*tpp)->level < level)
		*tpp = table(*tpp, level);
	if (perm)
		p = (struct entry *) alloc(sizeof *p);
	else
		p = (struct entry *) talloc(sizeof *p);
	BZERO(&p->sym, struct symbol);
	p->sym.name = name;
	p->sym.scope = (*tpp)->level;
	p->sym.up = (*tpp)->list;
	(*tpp)->list = &p->sym;
	p->link = (*tpp)->buckets[h];
	(*tpp)->buckets[h] = p;
	p->refs = 0;	/* (omit) */
	return &p->sym;
}
Symbol lookup(name, tp) char *name; Table tp; {
	struct entry *p;
	unsigned h = (unsigned)name&(HASHSIZE-1);

	assert(tp);
	do
		for (p = tp->buckets[h]; p; p = p->link)
			if (name == p->sym.name)
				return &p->sym;
	while (tp = tp->previous);
	return 0;
}
int genlabel(n) {
	static int label = 1;

	label += n;
	return label - n;
}
Symbol findlabel(lab) {
	char *label = stringd(lab);
	Symbol p;

	if (p = lookup(label, labels[1]))
		return p;
	p = install(label, &labels[1], 0);
	p->generated = 1;
	p->u.l.label = lab;
	p->u.l.equatedto = p;
	(*IR->defsymbol)(p);
	return p;
}
Symbol constant(ty, v) Type ty; Value v; {
	struct entry *p;
	unsigned h = v.u&(HASHSIZE-1);

	ty = unqual(ty);
	for (p = constants->buckets[h]; p; p = p->link)
		if (eqtype(ty, p->sym.type, 1))
			switch (ty->op) {
			case CHAR:     if (v.uc == p->sym.u.c.v.uc) return &p->sym; break;
			case SHORT:    if (v.ss == p->sym.u.c.v.ss) return &p->sym; break;
			case INT:      if (v.i  == p->sym.u.c.v.i)  return &p->sym; break;
			case UNSIGNED: if (v.u  == p->sym.u.c.v.u)  return &p->sym; break;
			case FLOAT:    if (v.f  == p->sym.u.c.v.f)  return &p->sym; break;
			case DOUBLE:   if (v.d  == p->sym.u.c.v.d)  return &p->sym; break;
			case ARRAY: case FUNCTION:
			case POINTER:  if (v.p  == p->sym.u.c.v.p)  return &p->sym; break;
			default: assert(0);
			}
	p = (struct entry *) alloc(sizeof *p);
	BZERO(&p->sym, struct symbol);
	p->sym.name = vtoa(ty, v);
	p->sym.scope = CONSTANTS;
	p->sym.type = ty;
	p->sym.sclass = STATIC;
	p->sym.u.c.v = v;
	p->link = constants->buckets[h];
	p->sym.up = constants->list;
	constants->list = &p->sym;
	constants->buckets[h] = p;
	p->refs = 0;	/* (omit) */
	(*IR->defsymbol)(&p->sym);
	return &p->sym;
}
Symbol intconst(n) {
	Value v;

	v.i = n;
	return constant(inttype, v);
}
Symbol genident(sclass, ty, lev) Type ty; {
	Symbol p;

	if (lev <= PARAM)
		p = (Symbol) alloc(sizeof *p);
	else
		p = (Symbol) talloc(sizeof *p);
	BZERO(p, struct symbol);
	p->name = stringd(genlabel(1));
	p->scope = lev;
	p->sclass = sclass;
	p->type = ty;
	p->generated = 1;
	if (lev < PARAM)
		(*IR->defsymbol)(p);
	return p;
}
Symbol temporary(sclass, ty) Type ty; {
	Symbol t1;
	struct temporary *p, **q = &temps;

	for (p = *q; p; q = &p->link, p = *q)
		if (p->sym->sclass == sclass && eqtype(p->sym->type, ty, 1))
		/* &&  p->sym->type->size  == ty->size
		&&  p->sym->type->align >= ty->align)*/ {
			*q = p->link;
			p->sym->type = ty;
			return p->sym;
		}
	t1 = genident(sclass, ty, level);
	t1->temporary = 1;
	return t1;
}
void rmtemps(sclass, level) {
	struct temporary *p, **q = &temps;

	for (p = *q; p; p = *q)
		if (p->sym->scope == level || p->sym->sclass == sclass)
			*q = p->link;
		else
			q = &p->link;
}
void release(t1) Symbol t1; {
	if (0 && t1->ref) {
		struct temporary *p = (struct temporary *) talloc(sizeof *p);
		p->sym = t1;
		p->link = temps;
		temps = p;
		t1->ref = 0;
	}
}
Symbol newtemp(sclass, tc) {
	Symbol t1 = temporary(sclass, btot(tc));

	t1->scope = LOCAL;
	if (t1->defined == 0) {
		(*IR->local)(t1);
		t1->defined = 1;
	}
	return t1;
}
/* fielduses - convert use lists for fields in type p */
void fielduses(p, cl) Symbol p; Generic cl; {
	if (p->type && isstruct(p->type) && p->u.s.ftab)
		setuses(p->u.s.ftab);
}

/* findtype - find type ty in identifiers */
Symbol findtype(ty) Type ty; {
	Table tp = identifiers;
	int i;
	struct entry *p;

	assert(tp);
	do
		for (i = 0; i < HASHSIZE; i++)
			for (p = tp->buckets[i]; p; p = p->link)
				if (p->sym.type == ty && p->sym.sclass == TYPEDEF)
					return &p->sym;
	while (tp = tp->previous);
	return 0;
}

/* locus - append (table, cp) to the evolving loci and symbol tables lists */
void locus(tp, cp) Table tp; Coordinate *cp; {
	extern List loci, tables;

	loci = append((Generic)cp, loci);
	tables = append((Generic)tp->list, tables);
}

/* setuses - convert p->refs to p->uses for all p at the current level in *tp */
void setuses(tp) Table tp; {
	if (xref) {
		int i;
		struct entry *p;
		for (i = 0; i < HASHSIZE; i++)
			for (p = tp->buckets[i]; p; p = p->link) {
				if (p->refs)
					p->sym.uses = (Coordinate **)ltoa(p->refs, 0);
				p->refs = 0;
				symbols = append((Generic)&p->sym, symbols);
			}
	}
}

/* use - add src to the list of uses for p */
void use(p, src) Symbol p; Coordinate src; {
	if (xref) {
		Coordinate *cp = (Coordinate *)alloc(sizeof *cp);
		*cp = src;
		((struct entry *)p)->refs = append((Generic)cp, ((struct entry *)p)->refs);
	}
}

