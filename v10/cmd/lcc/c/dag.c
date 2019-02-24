/* C compiler: dag processing */

#include "c.h"
#define NBUCKETS 020

struct code codehead = { Start };
Code codelist = &codehead;	/* list of code nodes for current function */
int nodecount;			/* # of available nodes in hash table */

static struct dag {		/* dags: */
	struct node node;		/* the node itself */
	struct dag *hlink;		/* next dag on hash chain */
} *buckets[NBUCKETS];		/* hash table */
static Node nodelist;		/* node list */
dclproto(static struct dag *dagnode,(int, Node, Node, Symbol));
dclproto(static void fixup,(Node));
dclproto(static int haskid,(Node, Node));
dclproto(static Node labelnode,(int));
dclproto(static Node list,(Node));
dclproto(static void reset,(void));
dclproto(static void remove,(Node));
dclproto(static void trash,(Node));
dclproto(static void typestab,(Symbol, Generic));
dclproto(static Node undag,(Node));
dclproto(static Node undag1,(Node, Node));

/* addlocal - add local p to list of locals for the current block */
void addlocal(p) Symbol p; {
	if (!p->defined) {
		code(Local)->u.var = p;
		p->defined = 1;
		p->scope = level;
	}
}

/* btot - map operator op or suffix op into a representative type */
Type btot(op) int op; {
	switch (optype(op)) {
	case F: return floattype;
	case D: return doubletype;
	case C: return chartype;
	case S: return shorttype;
	case I: return inttype;
	case U: return unsignedtype;
	case P: return voidptype;
	default: assert(0);
	}
	return 0;
}

/* code - allocate a code node of kind and append to the code list */
Code code(kind) {
	Code cp;

	if (kind > Label) {
		for (cp = codelist; cp->kind < Label; )
			cp = cp->prev;
		if (cp->kind == Jump)
			warning("unreachable code\n");
	}
	cp = (Code)talloc(sizeof *cp);
	cp->kind = kind;
	cp->prev = codelist;
	cp->next = 0;
	codelist->next = cp;
	return codelist = cp;
}

/* dagnode - allocate a dag with the given fields */
static struct dag *dagnode(op, l, r, sym) Node l, r; Symbol sym; {
	register struct dag *p = (struct dag *) talloc(sizeof *p);

	BZERO(p, struct dag);
	p->node.op = op;
	if (p->node.kids[0] = l)
		++l->count;
	if (p->node.kids[1] = r)
		++r->count;
	p->node.syms[0] = sym;
	return p;
}

/* emitcode - emit code for the current function */
void emitcode() {
	Code bp, cp;
	Coordinate save;

	save = src;
	for (bp = 0, cp = &codehead; errcnt <= 0 && cp; cp = cp->next)
		switch (cp->kind) {
		case Blockbeg:
			cp->u.block.prev = bp;
			bp = cp;
			if (glevel && IR->stabblock) {
				(*IR->stabblock)('{',  bp->u.block.level - LOCAL, bp->u.block.locals);
				swtoseg(CODE);
			}
			break;
		case Blockend:
			if (glevel && IR->stabblock) {
				foreach(bp->u.block.identifiers, bp->u.block.level, typestab, (Generic)0);
				foreach(bp->u.block.types,       bp->u.block.level, typestab, (Generic)0);
				(*IR->stabblock)('}', bp->u.block.level - LOCAL, bp->u.block.locals);
				swtoseg(CODE);
			}
			bp = bp->u.block.prev;
			break;
		case Local:
			if (glevel && IR->stabsym) {
				(*IR->stabsym)(cp->u.var);
				swtoseg(CODE);
			}
			break;
		case Defpoint:
			src = cp->u.point.src;
			if (glevel && IR->stabline) {
				(*IR->stabline)(&cp->u.point.src);
				swtoseg(CODE);
			}
			break;
		case Jump:
			if (cp->u.node == 0)
				break;
			/* else fall thru */
		case Gen: case Label:
			(*IR->emit)(cp->u.node);
			break;
		case Switch: {
			int i, k = cp->u.swtch.values[0];
			for (i = 0; i < cp->u.swtch.size; i++) {
				while (k++ < cp->u.swtch.values[i])
					(*IR->defaddress)(cp->u.swtch.deflab->u.l.equatedto);
				(*IR->defaddress)(cp->u.swtch.labels[i]->u.l.equatedto);
			}
			break;
			}
		}
	src = save;
}

/* fixup - re-aim equated labels to the true label */
static void fixup(p) Node p; {
	for ( ; p; p = p->link)
		switch (generic(p->op)) {
		case JUMP:
			if (p->kids[0]->op == ADDRG+P)
				p->kids[0]->syms[0] = p->kids[0]->syms[0]->u.l.equatedto;
			break;
		case LABEL:
		case EQ: case GE: case GT: case LE: case LT: case NE:
			assert(p->syms[0]);
			p->syms[0] = p->syms[0]->u.l.equatedto;
		}
}

/* gencode - generate code for the current function */
void gencode(caller, callee) Symbol caller[], callee[]; {
	int i;
	Code bp, cp;
	Symbol p, q;
	Coordinate save;

	save = src;
	cp = codehead.next->next;
	codelist = codehead.next;
	for (i = 0; (p = callee[i]) && (q = caller[i]); i++)
		if (p->sclass != q->sclass || p->type != q->type) {
			walk(asgn(p, idnode(q)), 0, 0);
			if (glevel && IR->stabsym) {
				(*IR->stabsym)(p);
				(*IR->stabsym)(q);
				swtoseg(CODE);
			}
		} else if (glevel && IR->stabsym) {
			(*IR->stabsym)(p);
			swtoseg(CODE);
		}
	codelist->next = cp;
	cp->prev = codelist;
	for (bp = 0, cp = &codehead; errcnt <= 0 && cp; cp = cp->next)
		switch (cp->kind) {
		case Start: case Switch:
			break;
		case Defpoint: 
			src = cp->u.point.src;
			break;
		case Blockbeg: {
			Symbol *p = cp->u.block.locals;
			cp->u.block.prev = bp;
			bp = cp;
			(*IR->blockbeg)(&bp->u.block.x);
			for ( ; *p; p++)
				if ((*p)->ref || glevel)
					(*IR->local)(*p);
			break;
			}
		case Blockend:
			(*IR->blockend)(&bp->u.block.x);
			bp = bp->u.block.prev;
			break;
		case Local:
			assert(cp->u.var->scope == bp->u.block.level);
			(*IR->local)(cp->u.var);
			break;
		case Address:
			(*IR->address)(cp->u.addr.sym, cp->u.addr.base, cp->u.addr.offset);
			break;
		case Jump:
			if (cp->u.node == 0)
				break;
			/* else fall thru */
		case Gen: case Label:
			fixup(cp->u.node);
			if (IR->no_dag)
				cp->u.node = undag(cp->u.node);
			cp->u.node = (*IR->gen)(cp->u.node);
			break;
		default:assert(0);
		}
	src = save;
}

/* haskid - does p appear as an operand in t? */
static int haskid(p, t) Node p, t; {
	if (t == 0)
		return 0;
	else if (p == t)
		return 1;
	else
		return haskid(p, t->kids[0]) || haskid(p, t->kids[1]); 
}

/* labelnode - list and return a LABEL node for label lab */
static Node labelnode(lab) {
	assert(lab);
	if (islabel(nodelist)) {
		equatelab(findlabel(lab), nodelist->syms[0]);
		return nodelist;
	}
	return list(newnode(LABEL+V, 0, 0, findlabel(lab)));
}

/* list - list node p unless it's already listed; return p */
static Node list(p) Node p; {
	if (p && p->link == 0) {
		if (nodelist) {
			p->link = nodelist->link;
			nodelist->link = p;
		} else
			p->link = p;
		nodelist = p;
#ifdef DAGCHECK
		{ extern void type_checker(Node); type_checker(p); }
#endif
	}
	return p;
}

/* listnodes - walk tree tp, building and listing dag nodes in execution order */
Node listnodes(tp, tlab, flab) Tree tp; {
	Opcode op;
	Node p, l, r;

	if (tp == 0)
		return 0;
	if (tp->node)
		return tp->node;
	op = generic(tp->op);
	switch (op) {
	case AND:
		if (flab) {
			listnodes(tp->kids[0], 0, flab);
			r = listnodes(tp->kids[1], 0, flab);
		} else {
			listnodes(tp->kids[0], 0, flab = genlabel(1));
			listnodes(tp->kids[1], tlab, 0);
			r = labelnode(flab);
		}
		trash(0);
		return r;
	case NOT:
		return listnodes(tp->kids[0], flab, tlab);
	case OR:
		if (tlab) {
			listnodes(tp->kids[0], tlab, 0);
			r = listnodes(tp->kids[1], tlab, 0);
		} else {
			listnodes(tp->kids[0], tlab = genlabel(1), 0);
			listnodes(tp->kids[1], 0, flab);
			r = labelnode(tlab);
		}
		trash(0);
		return r;
	case COND: {
		Tree q;
		assert(tlab == 0 && flab == 0);
		if (tp->u.sym)
			addlocal(tp->u.sym);
		trash(0);
		listnodes(tp->kids[0], 0, flab = genlabel(2));
		trash(0);
		if (q = tp->kids[1]) {
			assert(q->op == RIGHT);
			listnodes(q->kids[0], 0, 0);
			if (islabel(nodelist)) {
				equatelab(nodelist->syms[0], findlabel(flab + 1));
				remove(nodelist);
			}
			trash(0);
		}
		if (q && q->kids[1]) {
			list(jump(flab + 1));
			labelnode(flab);
			listnodes(q->kids[1], 0, 0);
			if (islabel(nodelist)) {
				equatelab(nodelist->syms[0], findlabel(flab + 1));
				remove(nodelist);
			}
		} else
			labelnode(flab);
		p = labelnode(flab + 1);
		trash(0);
		if (tp->u.sym) {
			Tree t = idnode(tp->u.sym);
			tp->u.sym->ref = 0;	/* undo idnode's setting ref */
			p = listnodes(t, 0, 0);
		}
		break;
		}
	case CNST: {
		Type ty = unqual(tp->type);
		assert(op == CNST+S || ty->u.sym);
		if (op == CNST+S || ty->u.sym->addressed)
			p = listnodes(cvtconst(tp), tlab, flab);
		else if (tlab == 0 && flab == 0)
			p = node(tp->op, 0, 0, constant(ty, tp->u.v));
		else {
			assert(ty == inttype);
			if (tlab && tp->u.v.i != 0)
				p = list(jump(tlab));
			else if (flab && tp->u.v.i == 0)
				p = list(jump(flab));
			else
				p = 0;
		}
		break;
		}
	case RIGHT:
		if (tp->kids[0] && generic(tp->kids[0]->op) == INDIR
		&&  tp->kids[1] && generic(tp->kids[1]->op) == ASGN
		&&  tp->kids[0]->kids[0] == tp->kids[1]->kids[0]) {	/* e++ */
			p = listnodes(tp->kids[0], 0, 0);
			if (nodelist) {
				Node t;
				for (t = nodelist; ; t = t->link)
					if (haskid(p, t->link)) {
						p->link = t->link;
						t->link = p;
						break;
					} else if (t->link == nodelist) {
						list(p);
						break;
					}
			} else
				list(p);
			listnodes(tp->kids[1], 0, 0);
		} else if (tp->kids[1]) {
			if (tp->kids[0] && generic(tp->kids[0]->op) == CNST)
				tp->kids[0] = 0;
			listnodes(tp->kids[0], 0, 0);
			p = listnodes(tp->kids[1], tlab, flab);
		} else
			p = listnodes(tp->kids[0], tlab, flab);
		break;
	case JUMP:
		assert(tlab == 0 && flab == 0);
		assert(tp->u.sym == 0);
		assert(tp->kids[0]);
		l = listnodes(tp->kids[0], 0, 0);
		p = newnode(JUMP+V, l, 0, 0);
		trash(0);
		list(p);
		break;
	case CALL:
		assert(tlab == 0 && flab == 0);
		l = listnodes(tp->kids[0], 0, 0);	/* arguments, function name */
		r = listnodes(tp->kids[1], 0, 0);
		p = newnode(tp->op, l, r, 0);
		p->syms[0] = (Symbol)talloc(sizeof *p->syms[0]);
		BZERO(p->syms[0], struct symbol);
		if (isptr(tp->kids[0]->type) && isfunc(tp->kids[0]->type->type))
			p->syms[0]->type = tp->kids[0]->type->type;
		else
			p->syms[0]->type = func(voidtype, 0, 1);
		trash(0);
		list(p);
		cfunc->u.f.ncalls++;
		break;
	case ARG:
		assert(tlab == 0 && flab == 0);
		if (IR->left_to_right)
			listnodes(tp->kids[1], 0, 0);
		l = listnodes(tp->kids[0], 0, 0);
		p = newnode(tp->op, l, 0, 0);
		p->syms[0] = intconst(tp->type->size);
		p->syms[1] = intconst(tp->type->align);
		list(p);
		if (!IR->left_to_right)
			listnodes(tp->kids[1], 0, 0);
		return 0;
	case EQ: case NE: case GT: case GE: case LE: case LT: {
		int lab;
		assert(tp->u.sym == 0);
		assert(errcnt || tlab || flab);
		if (lab = flab) {
			assert(tlab == 0);
			switch (op) {
			case EQ: op = NE + optype(tp->op); break;
			case NE: op = EQ + optype(tp->op); break;
			case GT: op = LE + optype(tp->op); break;
			case LT: op = GE + optype(tp->op); break;
			case GE: op = LT + optype(tp->op); break;
			case LE: op = GT + optype(tp->op); break;
			}
		} else if (lab = tlab)
			op = tp->op;
		l = listnodes(tp->kids[0], 0, 0);
		r = listnodes(tp->kids[1], 0, 0);
		p = newnode(op, l, r, findlabel(lab));
		p->syms[0]->ref++;
		list(p);
		break;
		}
	case ASGN:
		assert(tlab == 0 && flab == 0);
		if (tp->kids[0]->op == FIELD) {
			Tree x = tp->kids[0]->kids[0];
			Field p = tp->kids[0]->u.field;
			assert(generic(x->op) == INDIR);
			trash(0);
			l = listnodes(lvalue(x), 0, 0);
			if (fieldsize(p) < 8*p->type->size) {
				unsigned int fmask = fieldmask(p);
				unsigned int mask = fmask<<fieldright(p);
				Tree q = tp->kids[1];
				if (q->op == CNST+I && q->u.v.i == 0
				||  q->op == CNST+U && q->u.v.u == 0)
					q = bitnode(BAND, x, constnode(~mask, unsignedtype));
				else if (q->op == CNST+I && (q->u.v.i&fmask) == fmask
				||       q->op == CNST+U && (q->u.v.u&fmask) == fmask)
					q = bitnode(BOR, x, constnode(mask, unsignedtype));
				else
					q = bitnode(BOR,
						bitnode(BAND, x, constnode(~mask, unsignedtype)),
						bitnode(BAND, shnode(LSH, cast(q, unsignedtype),
							constnode(fieldright(p), inttype)),
							constnode(mask, unsignedtype)));
				r = listnodes(q, 0, 0);
			} else
				r = listnodes(tp->kids[1], 0, 0);
		} else {
			l = listnodes(tp->kids[0], 0, 0);
			r = listnodes(tp->kids[1], 0, 0);
		}
		trash(isaddrop(tp->kids[0]->op) && !tp->kids[0]->u.sym->computed ? l : 0);
		p = newnode(tp->op, l, r, 0);
		p->syms[0] = intconst(tp->kids[1]->type->size);
		p->syms[1] = intconst(tp->kids[1]->type->align);
		list(p);
		p = listnodes(tp->kids[1], 0, 0);
		break;
	case BAND:
		assert(tlab == 0 && flab == 0);
		l = listnodes(tp->kids[0], 0, 0);
		if (IR->compl_band)
			r = listnodes(cast(
				simplify(BCOM+U, unsignedtype,
					cast(tp->kids[1], unsignedtype), 0),
				tp->kids[1]->type), 0, 0);
		else
			r = listnodes(tp->kids[1], 0, 0);
		p = node(tp->op, l, r, 0);
		break;
	case RSH:
		assert(tlab == 0 && flab == 0);
		l = listnodes(tp->kids[0], 0, 0);
		if (IR->compl_band && tp->op == RSH+I)
			r = listnodes(simplify(NEG+I, inttype, tp->kids[1], 0), 0, 0);
		else
			r = listnodes(tp->kids[1], 0, 0);
		p = node(tp->op, l, r, 0);
		break;
	case ADD: case SUB:  case DIV: case MUL: case MOD:
	case BOR: case BXOR: case LSH:
		assert(tlab == 0 && flab == 0);
		l = listnodes(tp->kids[0], 0, 0);
		r = listnodes(tp->kids[1], 0, 0);
		p = node(tp->op, l, r, 0);
		if (IR->mulops_are_calls &&
		   (p->op == DIV+I || p->op == MOD+I || p->op == MUL+I
		||  p->op == DIV+U || p->op == MOD+U || p->op == MUL+U))
			list(p);
		break;
	case RET:
		assert(tlab == 0 && flab == 0);
		l = listnodes(tp->kids[0], 0, 0);
		p = newnode(tp->op, l, 0, 0);
		list(p);
		break;
	case CVC: case CVD: case CVF: case CVI:
	case CVP: case CVS: case CVU: case NEG: case BCOM:
		assert(tlab == 0 && flab == 0);
		l = listnodes(tp->kids[0], 0, 0);
		p = node(tp->op, l, 0, 0);
		break;
	case INDIR: {
		Type ty = tp->kids[0]->type;
		if (isptr(ty))
			ty = unqual(ty)->type;
		assert(tlab == 0 && flab == 0);
		l = listnodes(tp->kids[0], 0, 0);
		if (isvolatile(ty) || (isstruct(ty) && unqual(ty)->u.sym->u.s.vfields))
			p = newnode(tp->op, l, 0, 0);
		else
			p = node(tp->op, l, 0, 0);
		break;
		}
	case FIELD: {
		Tree q;
		assert(tlab == 0 && flab == 0);
		q = shnode(RSH,
			shnode(LSH, tp->kids[0],
				constnode(fieldleft(tp->u.field), inttype)),
			constnode(8*tp->type->size - fieldsize(tp->u.field), inttype));
		p = listnodes(q, 0, 0);
		break;
		}
	case ADDRL:
		assert(tlab == 0 && flab == 0);
		if (tp->u.sym->temporary) {
			addlocal(tp->u.sym);
			release(tp->u.sym);
		}
		p = node(tp->op, 0, 0, tp->u.sym);
		break;
	case ADDRG: case ADDRF:
		assert(tlab == 0 && flab == 0);
		if (tp->u.sym->scope == LABELS)
			tp->u.sym->ref++;
		p = node(tp->op, 0, 0, tp->u.sym);
		break;
	default:assert(0);
	}
	return tp->node = p;
}

/* jump - return tree for a jump to lab */
Node jump(lab) {
	Symbol p = findlabel(lab);

	p->ref++;
	return newnode(JUMP+V, node(ADDRG+P, 0, 0, p), 0, 0);
}

/* newnode - allocate a node with the given fields */
Node newnode(op, l, r, sym) Node l, r; Symbol sym; {
	return &dagnode(op, l, r, sym)->node;
}

/* node - search for a node with the given fields, or allocate it */
Node node(op, l, r, sym) Node l, r; Symbol sym; {
	int i = (opindex(op)^((unsigned)sym>>2))&(NBUCKETS-1);
	register struct dag *p;

	for (p = buckets[i]; p; p = p->hlink)
		if (p->node.op == op && p->node.syms[0] == sym
		&& p->node.kids[0] == l && p->node.kids[1] == r)
			return &p->node;
	p = dagnode(op, l, r, sym);
	p->hlink = buckets[i];
	buckets[i] = p;
	++nodecount;
	return &p->node;
}

dclproto(static void printdag1,(Node, int, int));
dclproto(static void printnode,(Node, int, int));

/* printdag - print dag p on fd, or the node list if p == 0 */
void printdag(p, fd) Node p; {
	printed(0);
	if (p == 0) {
		if (p = nodelist)
			do {
				p = p->link;
				printdag1(p, fd, 0);
			} while (p != nodelist);
	} else if (*printed(nodeid((Tree)p)))
		fprint(fd, "node'%d printed above\n", nodeid((Tree)p));
	else
		printdag1(p, fd, 0);
}

/* printdag1 - recursively print dag p */
static void printdag1(p, fd, lev) Node p; {
	int id, i;

	if (p == 0 || *printed(id = nodeid((Tree)p)))
		return;
	*printed(id) = 1;
	for (i = 0; i < MAXKIDS; i++)
		printdag1(p->kids[i], fd, lev + 1);
	printnode(p, fd, lev);
}

/* printnode - print fields of dag p */
static void printnode(p, fd, lev) Node p; {
	if (p) {
		int i, id = nodeid((Tree)p);
		fprint(fd, "%c%d%s", lev == 0 ? '\'' : '#', id,
			&"   "[id < 10 ? 0 : id < 100 ? 1 : 2]);
		fprint(fd, "%s count=%d", opname(p->op), p->count);
		for (i = 0; i < MAXKIDS && p->kids[i]; i++)
			fprint(fd, " #%d", nodeid((Tree)p->kids[i]));
		for (i = 0; i < MAXSYMS && p->syms[i]; i++)
			fprint(fd, " %s", p->syms[i]->name);
		fprint(fd, "\n");
	}
}

/* remove - remove node p from the node list */
static void remove(p) Node p; {
	if (nodelist) {
		Node q = nodelist;
		for ( ; q->link != p && q->link != nodelist; q = q->link)
			;
		assert(q->link == p);
		q->link = p->link;
		if (p == nodelist)
			nodelist = q;
	}
}

/* reset - clear the dag */
static void reset() {
	BZERO(buckets, struct dag *[NBUCKETS]);
	nodecount = 0;
}

/* trash - preclude future links to rvalue of p or all values */
static void trash(p) Node p; {
	if (p) {
		register int i;
		register struct dag *q, **r;
		for (i = 0; i < NBUCKETS; i++)
			for (r = &buckets[i]; q = *r; )
				if (generic(q->node.op) == INDIR && (!isaddrop(q->node.kids[0]->op)
				|| q->node.kids[0]->syms[0] == p->syms[0])) {
					*r = q->hlink;
					--nodecount;
				} else
					r = &q->hlink;
	} else if (nodecount > 0)
		reset();
}

/* typestab - emit stab entries for p */
static void typestab(p, cl) Symbol p; Generic cl; {
	if (!isfunc(p->type) && (p->sclass == EXTERN || p->sclass == STATIC) && IR->stabsym)
		(*IR->stabsym)(p);
	else if ((p->sclass == TYPEDEF || p->sclass == 0) && IR->stabtype)
		(*IR->stabtype)(p);
}

#define iscall(p) (generic((p)->op) == CALL || IR->mulops_are_calls \
	&& ((p)->op == DIV+I || (p)->op == MOD+I || (p)->op == MUL+I \
	||  (p)->op == DIV+U || (p)->op == MOD+U || (p)->op == MUL+U))

/* undag - replace multiply-referenced nodes in nodelist by temporaries, return new nodelist */
static Node undag(nodelist) Node nodelist; {
	Node p, pred;
	struct node head;

	head.link = nodelist;
	for (pred = &head; p = pred->link; ) {
		/*
		 * succ is p's successor,
		 * pred is p's predecessor, and
		 * p->link == pred;
		 * undag1(x, p) adds predecessors and maintains p->link == p->link->link.
		 */
		Node succ = p->link;
		p->link = pred;
		if (generic(p->op) == INDIR) {
			assert(p->count >= 1);
			undag1(p, p);
			/*
			 * remove p from the node list,
			 * setting pred to the last node inserted (if any).
			 */
			pred = p->link;
			pred->link = succ;
			p->link = 0;
		} else if (iscall(p) && p->count >= 1) {
			Node q;
			undag1(p, p);
			/*
			 * p->link is p's predecessor,
			 * which is the ASGN of p to a temporary.
			 */
			assert(p->link && generic(p->link->op) == ASGN && p->link->kids[1] == p);
			/*
			 * remove p from the node list,
			 * setting pred to the ASGN node.
			 */
			pred = p->link;
			pred->link = succ;
			/*
			 * re-insert p into the node list
			 * immediately before its predecessor;
			 * this places the CALL node before the ASGN node.
			 * / 
			for (q = &head; q && q->link != pred; q = q->link)
				;
			assert(q);
			q->link = p;
			p->link = pred; */
			p->link = 0;
		} else {
			assert(p->count == 0);
			undag1(p, p);
			/*
			 * reestablish p's successor, and
			 * advance pred so that pred->link == succ
			 */
			p->link = succ;
			pred = p;
		}
	}
	rmtemps(0, LOCAL);
	return head.link;
}

/* undag1 - return a reference to the temporary holding value of p, or p */
static Node undag1(p, root) Node p, root; {
	Node e;

	if (p == 0)
		;
	else if (p->syms[2]) {
		e = newnode(INDIR + (isunsigned(p->syms[2]->type) ? I : ttob(p->syms[2]->type)),
			newnode(ADDRL+P, 0, 0, p->syms[2]), 0, 0);
		e->count = 1;
		if (--p->count == 1) {
			/* fprint(2, "releasing %s from ", p->syms[2]->name); printnode(p, 2, 1);
			release(p->syms[2]); */
			p->syms[2] = 0;
		}
		p = e;
	} else if (p->count <= 1 && !iscall(p)
	||         p->count == 0 &&  iscall(p)) {
		p->kids[0] = undag1(p->kids[0], root);
		p->kids[1] = undag1(p->kids[1], root);
	} else if (p->op == ADDRL+P || p->op == ADDRF+P) {
		assert(p != root);
		p = newnode(p->op, 0, 0, p->syms[0]);
		p->count = 1;
	} else if (generic(p->op) == INDIR
	&& (p->kids[0]->op == ADDRL+P || p->kids[0]->op == ADDRF+P)
	&& p->kids[0]->syms[0]->sclass == REGISTER && p != root) {
		p = newnode(p->op, 
			newnode(p->kids[0]->op, 0, 0, p->kids[0]->syms[0]), 0, 0);
		p->count = 1;
	} else if (p->op == INDIR+B) {
		--p->count;
		p = newnode(p->op, p->kids[0], 0, 0);
		p->count = 1;
		p->kids[0] = undag1(p->kids[0], root);
	} else {
		p->syms[2] = temporary(REGISTER, btot(p->op));
		/* fprint(2, "allocating %s to ", p->syms[2]->name); printnode(p, 2, 1); */
		if (!p->syms[2]->defined) {
			p->syms[2]->scope = LOCAL;
			p->syms[2]->ref = 1;
			(*IR->local)(p->syms[2]);
			p->syms[2]->defined = 1;
		}
		p->kids[0] = undag1(p->kids[0], root);
		p->kids[1] = undag1(p->kids[1], root);
		e = newnode(ASGN + (isunsigned(p->syms[2]->type) ? I : ttob(p->syms[2]->type)),
			newnode(ADDRL+P, 0, 0, p->syms[2]), p, 0);
		e->syms[0] = intconst(p->syms[2]->type->size);
		e->syms[1] = intconst(p->syms[2]->type->align);
		/*
		 * root->link is the root's predecessor;
		 * point the root's predecessor to the ASGN,
		 * make the ASGN the root's new predecessor,
		 * and make root the ASGN's successor.
		 */
		root->link = root->link->link = e;
		e->link = root;
		if (p != root)
			p = undag1(p, root);	/* returns reference to the temporary */
	}
	return p;
}

/* walk - list tree tp, generate code for current node list, reset node list */
void walk(tp, tlab, flab) Tree tp; {
	listnodes(tp, tlab, flab);
	if (nodelist) {
		trash(0);
		code(Gen);
		codelist->u.node = nodelist->link;
		nodelist->link = 0;
		nodelist = 0;
		rmtemps(REGISTER, 0);
	}
	reset();
	ntree = 0;
}
