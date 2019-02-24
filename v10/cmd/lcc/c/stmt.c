/* C compiler: statement parsing */

#include "c.h"

#define SWITCHSIZE 512		/* initial size of switch arrays */

float density = 0.5;		/* minimum switch density */
float refinc = 1.0;		/* amount by which ref field is incremented */

static struct equate {	/* deferred equates: */
	Symbol new;		/* new label, i.e., label being defined */
	Symbol old;		/* old label */
	struct equate *oldlink;	/* pointer to equate structure for old=x */
	struct equate *link;	/* next equate on list */
} *equlist;			/* list of deferred equates */
struct swtch {		/* switch statement data: */
	Symbol sym;		/* local holding the switch value */
	int lab;		/* label of selection code */
	Symbol deflab;		/* default label */
	int ncases;		/* number of cases */
	int size;		/* size of value & label arrays */
	int    *values;		/* value, label pairs */
	Symbol *labels;
};

dclproto(static void branch,(int));
dclproto(static void caselabel,(struct swtch *, int, int));
dclproto(static void cmp,(int, Symbol, int, int));
dclproto(static void dostmt,(int, struct swtch *, int));
dclproto(static int foldcond,(Tree, Tree));
dclproto(static void forstmt,(int, struct swtch *, int));
dclproto(static void ifstmt,(int, int, struct swtch *, int));
dclproto(static Symbol localaddr,(Tree));
dclproto(static void stmtlabel,(char *));
dclproto(static int swcode,(struct swtch *, int *, int, int, int));
dclproto(static void swgen,(struct swtch *));
dclproto(static void swstmt,(int, int, int));
dclproto(static void visit,(struct equate *));
dclproto(static void whilestmt,(int, struct swtch *, int));

/* branch - jump to lab */
static void branch(lab) {
	Code cp;

	walk(0, 0, 0);
	for (cp = codelist; cp->kind < Label; )
		cp = cp->prev;
	if (lab)
		while (cp->kind == Label && islabel(cp->u.node)
		&& cp->u.node->syms[0]->u.l.label != lab) {
			Symbol old = findlabel(lab);
			equatelab(cp->u.node->syms[0], old);
			old->ref++;
			if (cp->prev->next = cp->next)
				cp->next->prev = cp->prev;
			else
				codelist = cp->prev;
			do
				cp = cp->prev;
			while (cp->kind < Label);
		}
	if (cp->kind != Jump) {
		code(Jump);
		codelist->u.node = lab ? jump(lab) : (Node)0;
	}
	/* else do nothing and thus delete dead branch */
}

/* caselabel - add a label to the current switch list */
static void caselabel(swp, val, lab) struct swtch *swp; {
	int k;

	if (swp->ncases >= swp->size) {
		int    *vals = swp->values;
		Symbol *labs = swp->labels;
		swp->size *= 2;
		swp->values = (int    *) talloc(swp->size*sizeof (int));
		swp->labels = (Symbol *) talloc(swp->size*sizeof (Symbol));
		for (k = 0; k < swp->ncases; k++) {
			swp->values[k] = vals[k];
			swp->labels[k] = labs[k];
		}
	}	
	for (k = swp->ncases; k > 0 && swp->values[k-1] >= val; k--) {
		swp->values[k] = swp->values[k-1];
		swp->labels[k] = swp->labels[k-1];
	}
	if (k < swp->ncases && swp->values[k] == val)
		error("duplicate case label `%d'\n", val);
	swp->values[k] = val;
	swp->labels[k] = findlabel(lab);
	++swp->ncases;
	if (Aflag >= 2 && swp->ncases == 258)
		warning("more than 257 cases in a switch statement\n");
}

/* cmp - generate code for `if (p op n) goto lab' for integer n */
static void cmp(op, p, n, lab) Symbol p; {
	listnodes(eqnode(op, cast(idnode(p), inttype), constnode(n, inttype)), lab, 0);
}

/* definelab - define a label */
void definelab(lab) {
	Code cp;

	walk(0, 0, 0);
	code(Label);
	codelist->u.node = newnode(LABEL+V, 0, 0, findlabel(lab));
	for (cp = codelist; cp->kind <= Label; )
		cp = cp->prev;
	while (cp->kind == Jump && cp->u.node && cp->u.node->kids[0]
	&& cp->u.node->kids[0]->op == ADDRG+P
	&& cp->u.node->kids[0]->syms[0]->u.l.label == lab) {
		cp->u.node->kids[0]->syms[0]->ref--;
		assert(cp->next);
		cp->prev->next = cp->next;
		cp->next->prev = cp->prev;
		cp = cp->prev;
	}
}

/* definept - define an execution point n: current token at current pc */
Code definept(p) Coordinate *p; {
	int n;
	Code cp = code(Defpoint);

	cp->u.point.src = p ? *p : src;
	cp->u.point.point = npoints;
	if (ncalled > 0 && (n = findcount(src.file, src.x, src.y)) >= 0)
		refinc = n/ncalled + 1;
	if (glevel > 2)
		locus(identifiers, &cp->u.point.src);
	if (events.points) {
		Tree e = 0;
		apply(events.points, (Generic)&cp->u.point.src, (Generic)&e);
		if (e)
			listnodes(e, 0, 0);
	}
	return cp;
}

/* dostmt - do statement while ( expression ) */
static void dostmt(lab, swp, lev) struct swtch *swp; {
	refinc *= 10;
	t = gettok();
	definelab(lab);
	statement(lab, swp, lev);
	definelab(lab + 1);
	expect(WHILE);
	expect('(');
	definept(0);
	walk(conditional(')'), lab, 0);
	if (findlabel(lab + 2)->ref)
		definelab(lab + 2);
}

/* equatelab - add new=old to list of deferred equates */
void equatelab(new, old) Symbol new, old; {
	struct equate *e = (struct equate *) talloc(sizeof *e);

	e->new = new;
	e->old = old;
	e->oldlink = 0;
	e->link = equlist;
	equlist = e;
	for (e = e->link; e; e = e->link)
		if (e->old == new)
			e->oldlink = equlist;
		else if (e->new == old)
			equlist->oldlink = e;
}
	
/* flushequ - flush deferred equates */
void flushequ() {
	for ( ; equlist; equlist = equlist->link)
		visit(equlist);
}

/* foldcond - check if initial test in for(e1;e2;e3) S is necessary */
static int foldcond(e1, e2) Tree e1, e2; {
	Opcode op = generic(e2->op);
	Symbol v;

	if (e1 == 0 || e2 == 0)
		return 0;
	if (generic(e1->op) == ASGN && isaddrop(e1->kids[0]->op)
	&& generic(e1->kids[1]->op) == CNST) {
		v = e1->kids[0]->u.sym;
		e1 = e1->kids[1];
	} else
		return 0;
	if ((op==LE || op==LT || op==EQ || op==NE || op==GT || op==GE)
	&& generic(e2->kids[0]->op) == INDIR
	&& e2->kids[0]->kids[0]->u.sym == v
	&& e2->kids[1]->op == e1->op) {
		e1 = simplify(op, e2->type, e1, e2->kids[1]);
		if (e1->op == CNST+I)
			return e1->u.v.i;
	}
	return 0;
}

/* forstmt - for ( [expr1] ; [expr2] ; [expr3] ) statement */
static void forstmt(lab, swp, lev) struct swtch *swp; {
	Tree e1, e2, e3;
	Coordinate pt2, pt3;
	static char follow[] = { IF, ID, '}', 0 };

	e1 = e2 = e3 = 0;
	t = gettok();
	expect('(');
	definept(0);
	if (kind[t] == ID)
		e1 = expr0(';');
	else
		expect(';');
	walk(e1, 0, 0);
	pt2 = src;
	refinc *= 10;
	if (kind[t] == ID)
		e2 = texpr(conditional, ';');
	else
		expect(';');
	pt3 = src;
	if (kind[t] == ID)
		e3 = texpr(expr0, ')');
	else
		test(')', follow);
	if (glevel == 2) {	/* e1; L: if (!e2) goto L+2; S; L+1: e3; goto L; L+2: */
		definelab(lab);
		definept(&pt2);
		if (e2)
			walk(e2, 0, lab + 2);
		statement(lab, swp, lev);
		definelab(lab + 1);
		definept(&pt3);
		if (e3)
			walk(e3, 0, 0);
		branch(lab);
	} else {	/* e1; goto L+3; L: S; L+1: e3; L+3: if (e2) goto L; L+2: */
		int once = 0;
		if (e2) {
			once = foldcond(e1, e2);
			if (!once)
				branch(lab + 3);
		}
		definelab(lab);
		statement(lab, swp, lev);
		definelab(lab + 1);
		definept(&pt3);
		if (e3)
			walk(e3, 0, 0);
		if (e2) {
			if (!once)
				definelab(lab + 3);
			definept(&pt2);
			walk(e2, lab, 0);
		} else {
			definept(&pt2);
			branch(lab);
		}
	}
	if (findlabel(lab + 2)->ref)
		definelab(lab + 2);
}

/* ifstmt - if ( expression ) statement [ else statement ] */
static void ifstmt(lab, loop, swp, lev) struct swtch *swp; {
	t = gettok();
	expect('(');
	definept(0);
	walk(conditional(')'), 0, lab);
	refinc /= 2;
	statement(loop, swp, lev);
	if (t == ELSE) {
		branch(lab + 1);
		t = gettok();
		definelab(lab);
		statement(loop, swp, lev);
		if (findlabel(lab + 1)->ref)
			definelab(lab + 1);
	} else
		definelab(lab);
}

/* localaddr - returns q if p yields the address of local/parameter q; otherwise returns 0 */
static Symbol localaddr(p) Tree p; {
	if (p == 0)
		return 0;
	switch (generic(p->op)) {
	case INDIR: case CALL: case ARG:
		return 0;
	case ADDRL: case ADDRF:
		return p->u.sym;
	case RIGHT: case ASGN:
		if (p->kids[1])
			return localaddr(p->kids[1]);
		return localaddr(p->kids[0]);
	case COND: {
		Symbol q;
		if (p->kids[1] && (q = localaddr(p->kids[1])))
			return q;
		return localaddr(p->kids[2]);
		}
	default: {
		Symbol q;
		if (p->kids[0] && (q = localaddr(p->kids[0])))
			return q;
		return localaddr(p->kids[1]);
		}
	}
}

/* retcode - return p from the current function */
void retcode(p, lab) Tree p; {
	if (p == 0) {
		if (events.returns)
			apply(events.returns, (Generic)cfunc, (Generic)0);
		p = tree(RET+V, voidtype, 0, 0);
	} else {
		Type ty;
		p = pointer(p);
		if (ty = assign(freturn(cfunc->type), p))
			p = cast(p, ty);
		else
			error("illegal return type; found `%t' expected `%t'\n",
				p->type, freturn(cfunc->type));
		if (retv) {
			if (p->op == RIGHT && p->kids[0]->op == CALL+B
			&& p->kids[1]->op == INDIR+B) {
				p = p->kids[0];
				p->kids[1] = idnode(retv);
			} else
				p = asgnnode(ASGN, rvalue(idnode(retv)), p);
			walk(p, 0, 0);
			if (events.returns)
				apply(events.returns, (Generic)cfunc, (Generic)rvalue(idnode(retv)));
			p = tree(RET+V, voidtype, 0, 0);
		} else {
			if (events.returns) {
				Symbol t1 = genident(AUTO, p->type, level);
				addlocal(t1);
				walk(asgn(t1, p), 0, 0);
				apply(events.returns, (Generic)cfunc, (Generic)idnode(t1));
				p = idnode(t1);
			}
			p = cast(p, promote(p->type));
			if (isptr(p->type)) {
				Symbol q = localaddr(p);
				if (q && (q->computed || q->generated))
					warning("pointer to a %s is an illegal return value\n",
						q->scope == PARAM ? "parameter" : "local");
				else if (q)
					warning("pointer to %s `%s' is an illegal return value\n",
						q->scope == PARAM ? "parameter" : "local", q->name);
				p = cast(p, unsignedtype);
			}
			p = tree(RET + widen(p->type), p->type, p, 0);
		}
	}
	walk(p, 0, 0);
	branch(lab);
}

/* statement - parse statements */
void statement(loop, swp, lev) struct swtch *swp; {
	int lab;
	float ref = refinc;

	if (Aflag >= 2 && lev == 15)
		warning("more than 15 levels of nested statements\n");
	switch (t) {
	case IF:
		ifstmt(genlabel(2), loop, swp, lev + 1);
		break;
	case WHILE:
		whilestmt(genlabel(3), swp, lev + 1);
		break;
	case DO:
		dostmt(genlabel(3), swp, lev + 1);
		expect(';');
		break;
	case FOR:
		forstmt(genlabel(4), swp, lev + 1);
		break;
	case SWITCH:
		swstmt(loop, genlabel(2), lev + 1);
		break;
	case CASE:
		if (swp == 0)
			error("illegal case label\n");
		definelab(lab = genlabel(1));
		while (t == CASE) {
			static char follow[] = { IF, ID, 0 };
			Tree p;
			t = gettok();
			p = constexpr(0);
			if (generic(p->op) == CNST && isint(p->type)) {
				if (swp) {
					needconst++;
					p = cast(p, swp->sym->type);
					needconst--;
					caselabel(swp, p->u.v.i, lab);
				}
			} else
				error("case label must be a constant integer expression\n");
			test(':', follow);
			ntree = 0;
		}
		statement(loop, swp, lev);
		break;
	case DEFAULT:
		if (swp)
			if (swp->deflab)
				error("extra default label\n");
			else {
				swp->deflab = findlabel(swp->lab);
				definelab(swp->deflab->u.l.label);
			}
		else
			error("illegal default label\n");
		t = gettok();
		expect(':');
		statement(loop, swp, lev);
		break;
	case RETURN:
		t = gettok();
		definept(0);
		if (t != ';')
			if (freturn(cfunc->type) == voidtype) {
				error("extraneous return value\n");
				expr(0);
				retcode(0, cfunc->u.f.label);
			} else
				retcode(expr(0), cfunc->u.f.label);
		else {
			if (freturn(cfunc->type) != voidtype
			&& (freturn(cfunc->type) != inttype || Aflag >= 1))
				warning("missing return value\n");
			retcode(0, cfunc->u.f.label);
		}
		expect(';');
		break;
	case BREAK:
		walk(0, 0, 0);
		if (swp && swp->lab > loop) {
			definept(0);
			branch(swp->lab + 1);
			t = gettok();
			expect(';');
			break;
		}
	case CONTINUE:
		walk(0, 0, 0);
		definept(0);
		if (loop)
			branch(loop + (t == BREAK ? 2 : 1));
		else
			error("illegal %k statement\n", t);
		t = gettok();
		expect(';');
		break;
	case '{':
		compound(loop, swp, lev + 1);
		break;
	case ';':
		definept(0);
		t = gettok();
		break;
	case GOTO:
		walk(0, 0, 0);
		definept(0);
		t = gettok();
		if (t == ID) {
			Symbol p = lookup(token, labels[0]);
			if (p == 0) {
				p = install(token, &labels[0], 0);
				p->u.l.label = genlabel(1);
				p->u.l.equatedto = p;
				p->src = src;
			}
			p->ref++;
			use(p, src);
			branch(p->u.l.label);
			t = gettok();
		} else
			error("missing label in goto\n");
		expect(';');
		break;
	case ID:
		if (getchr() == ':') {
			stmtlabel(token);
			statement(loop, swp, lev);
			break;
		}
	default:
		definept(0);
		if (kind[t] != ID) {
			error("unrecognized statement\n");
			t = gettok();
		} else {
			Tree e = expr0(0);
			listnodes(e, 0, 0);
			if (glevel > 1 || nodecount == 0 || nodecount > 200)
				walk(0, 0, 0);
			ntree = 0;
		}
		expect(';');
	}
	if (kind[t] != IF && kind[t] != ID && t != '}' && t != EOI) {
		static char follow[] = { IF, ID, '}', 0 };
		error("illegal statement termination\n");
		skipto(0, follow);
	}
	refinc = ref;
}

/* stmtlabel - label : */
static void stmtlabel(label) char *label; {
	Symbol p;

	if ((p = lookup(label, labels[0])) == 0) {
		p = install(label, &labels[0], 0);
		p->u.l.label = genlabel(1);
		p->u.l.equatedto = p;
	}
	if (p->defined)
		error("redefinition of label `%s' previously defined at %w\n",
			label, &p->src);
	p->defined = 1;
	p->src = src;
	definelab(p->u.l.label);
	t = gettok();
	expect(':');
}

/* swcode - generate switch decision code for buckets b[lb..ub] */
static int swcode(swp, b, lb, ub, n) struct swtch *swp; int b[]; {
	int hilab, k, l, lolab, median, u, *v;

	v = swp->values;
	median = ((float)v[b[lb]] + (float)v[b[ub+1]-1] + 1)/2;
	for (l = lb, u = ub; l <= u; ) {
		k = (l + u)/2;
		if (v[b[k]] > median)
			u = k - 1;
		else if (v[b[k+1]-1] < median)
			l = k + 1;
		else
			break;
	}
	if (l > u) {	/* which of two buckets is closest to the median? */
		l = b[k];
		u = b[k+1] - 1;
		if (median < v[l] && k > lb && v[l] - median > median - v[l-1])
			k--;
		else if (median > v[u] && k < ub && median - v[u] > v[u+1] - median)
			k++;
	}
	l = b[k];
	u = b[k+1] - 1;
	lolab = hilab = swp->deflab->u.l.label;
	if (k > lb) {
		lolab = 0;
		if (k < ub)
			hilab = genlabel(1);
	} else if (k < ub)
		hilab = 0;
	if (n == 0)
		lolab = hilab = 0;
	if (u - l + 1 <= 3) {
		int i;
		for (i = l; i <= u; i++)
			cmp(EQ, swp->sym, swp->values[i], swp->labels[i]->u.l.label);
		if (lolab && lolab == hilab)
			branch(lolab);
		else {
			if (lolab)
				cmp(LT, swp->sym, swp->values[l], lolab);
			if (hilab)
				cmp(GT, swp->sym, swp->values[u], hilab);
			walk(0, 0, 0);
		}
	} else {
		int lab = genlabel(2);
		Tree e; 
		cmp(LT, swp->sym, swp->values[l], lolab ? lolab : lab);
		cmp(GT, swp->sym, swp->values[u], hilab ? hilab : lab);
		e = tree(ADDRG+P, ptr(voidptype), 0, 0);
		e->u.sym = findlabel(lab + 1);
		e->u.sym->sclass = STATIC;
		e->u.sym->type = e->type;
		walk(tree(JUMP, voidtype, rvalue((*opnode['+'])(ADD, e,
			(*opnode['-'])(SUB, cast(idnode(swp->sym), inttype),
			constnode(swp->values[l], inttype)))), 0), 0, 0);
		definelab(lab + 1);
		code(Switch);
		codelist->u.swtch.sym = swp->sym;
		codelist->u.swtch.deflab = swp->deflab;
		codelist->u.swtch.size = u - l + 1;
		codelist->u.swtch.values = &swp->values[l];
		codelist->u.swtch.labels = &swp->labels[l];
		branch(0);
		definelab(lab);
		if (swp->values[u] - swp->values[l] + 1 >= 10000)
			warning("switch generates a huge table\n");
	}
	if (k > lb)
		n = swcode(swp, b, lb, k - 1, n - 1);
	if (k < ub) {
		if (hilab)
			definelab(hilab);
		n = swcode(swp, b, k + 1, ub, n - 1);
	}
	return n;
}

/* swgen - partition case labels into buckets, initiate code generation */
#define den(k,n) ((float)(k-buckets[n]+1)/(float)(v[k]-v[buckets[n]]+1))
static void swgen(swp) struct swtch *swp; {
	int *buckets, k, n, *v;

	if (swp->ncases == 0)
		return;
	buckets = (int *) talloc((swp->ncases + 1)*sizeof *buckets);
	v = swp->values;
	n = 0;
	for (k = 0; k < swp->ncases; k++, n++) {
		buckets[n] = k;
		while (n > 0) {
			float d = den(k, n-1);
			if (d < density || k < swp->ncases - 1 && d < den(k+1, n))
				break;
			n--;
		}
	}
	buckets[n--] = swp->ncases;
	swcode(swp, buckets, 0, n, n);
}

/* swstmt - switch ( expression ) statement */
static void swstmt(loop, lab, lev) {
	Tree e;
	struct swtch sw;
	Code head, tail;

	t = gettok();
	expect('(');
	definept(0);
	e = expr(')');
	if (!isint(e->type)) {
		error("illegal type `%t' in switch expression\n", e->type);
		e = retype(e, inttype);
	}
	e = cast(e, promote(e->type));
	if (generic(e->op) == INDIR && isaddrop(e->kids[0]->op)
	&& e->kids[0]->u.sym->type == e->type && !isvolatile(e->kids[0]->u.sym->type)) {
		sw.sym = e->kids[0]->u.sym;
		walk(0, 0, 0);
	} else {
		sw.sym = genident(REGISTER, e->type, level);
		addlocal(sw.sym);
		walk(asgn(sw.sym, e), 0, 0);
	}
	code(Switch);
	head = codelist;
	sw.lab = lab;
	sw.deflab = 0;
	sw.ncases = 0;
	sw.size = SWITCHSIZE;
	sw.values = (int    *) talloc(SWITCHSIZE*sizeof (int));
	sw.labels = (Symbol *) talloc(SWITCHSIZE*sizeof (Symbol));
	refinc /= 10;
	statement(loop, &sw, lev);
	if (sw.deflab == 0) {
		sw.deflab = findlabel(lab);
		definelab(lab);
		if (sw.ncases == 0)
			warning("switch statement with no cases\n");
	}
	if (findlabel(lab + 1)->ref)
		definelab(lab + 1);
	tail = codelist;
	codelist = head->prev;
	swgen(&sw);
	branch(lab);
	head->next->prev = codelist;
	codelist->next = head->next;
	codelist = tail;
}

/* visit - equate p->new=p->old, equating predecessor first */
static void visit(p) struct equate *p; {
	if (p && p->old) {
		visit(p->oldlink);
		p->new->u.l.equatedto = p->old->u.l.equatedto;
		p->old = 0;
	}
}

/* whilestmt - while ( expression ) statement */
static void whilestmt(lab, swp, lev) struct swtch *swp; {
	Coordinate pt;
	Tree e;

	refinc *= 10;
	t = gettok();
	expect('(');
	walk(0, 0, 0);
	pt = src;
	e = texpr(conditional, ')');
	if (generic(e->op) == CNST || glevel == 2) {
			/* L, L+1: if (!e) goto L+2; S; goto L; L+2: */
		definelab(lab);
		definelab(lab + 1);
		definept(&pt);
		walk(e, 0, lab + 2);
		statement(lab, swp, lev);
		branch(lab);
	} else {	/* goto L+1; L: S; L+1: if (E) goto L; L+2: */
		branch(lab + 1);
		definelab(lab);
		statement(lab, swp, lev);
		definelab(lab + 1);
		definept(&pt);
		walk(e, lab, 0);
	}
	if (findlabel(lab + 2)->ref)
		definelab(lab + 2);
}
