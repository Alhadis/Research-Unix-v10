/* C compiler: tree management */

#include "c.h"

static struct arena first[] = {
	0, 0, 0, &first[0], 0,
	0, 0, 0, &first[1], 0,
};
Arena permanent = &first[0];	/* permanent storage */
Arena transient = &first[1];	/* transient storage; released at function end */
static Arena freearenas;	/* list of free arenas */

/* allocate - allocate n bytes in arena **p, adding a new arena if necessary */
char *allocate(n, p) Arena *p; {
	Arena ap = *p;

	while (ap->avail + n > ap->limit)
		if (ap->next) {		/* move to next arena */
			ap = ap->next;
			ap->avail = (char *)ap + roundup(sizeof *ap, sizeof (double));
		} else if (ap->next = freearenas) {
			freearenas = freearenas->next;
			ap = ap->next;
			ap->avail = (char *)ap + roundup(sizeof *ap, sizeof (double));
			ap->first = (*p)->first;
			ap->next = 0;
		} else {		/* allocate a new arena */
			int m = n + MEMINCR*1024 + roundup(sizeof *ap, sizeof (double));
			ap->next = (Arena) malloc(m);
			assert(ap->next && (int)ap->next >= 0);
			if ((char *)ap->next == ap->limit) /* extend previous arena? */
				ap->limit = (char *)ap->next + m;
			else {			/* link to a new arena */
				ap = ap->next;
				ap->limit = (char *)ap + m;
				ap->avail = (char *)ap + roundup(sizeof *ap, sizeof (double));
			}
			ap->first = (*p)->first;
			ap->next = 0;
		}
	*p = ap;
	ap->avail += n;
	return ap->avail - n;
}

/* deallocate - release all space in arena *p, except the first arena; reset *p */
void deallocate(p) Arena *p; {
	Arena first = (*p)->first;

	(*p)->next = freearenas;
	freearenas = first->next;
	first->next = 0;
	*p = first;
}


/* cvtconst - convert a constant tree into tree for a global variable */
Tree cvtconst(p) Tree p; {
	Symbol q = constant(p->type, p->u.v);
	Tree e;

	if (q->u.c.loc == 0)
		q->u.c.loc = genident(STATIC, p->type, GLOBAL);
	if (isarray(p->type)) {
		e = tree(ADDRG+P, atop(p->type), 0, 0);
		e->u.sym = q->u.c.loc;
	} else
		e = idnode(q->u.c.loc);
	return e;
}

/* hascall - does tree p contain a CALL? */
int hascall(p) Tree p; {
	if (p == 0)
		return 0;
	if (generic(p->op) == CALL || (IR->mulops_are_calls &&
	  (p->op == DIV+I || p->op == MOD+I || p->op == MUL+I
	|| p->op == DIV+U || p->op == MOD+U || p->op == MUL+U)))
		return 1;
	return hascall(p->kids[0]) || hascall(p->kids[1]);
}

/* opname - return string for operator op */
char *opname(op) {
	static char *opnames[] = {
	"",
#define NEEDNAMES
#include "ops.h"
	};
	char *name;

	switch (op) {
	case AND:  return "AND";
	case NOT:  return "NOT";
	case OR:   return "OR";
	case COND: return "COND";
	case RIGHT:return "RIGHT";
	case FIELD:return "FIELD";
	}
	if (opindex(op) > 0 && opindex(op) < sizeof opnames/sizeof opnames[0])
		name = opnames[opindex(op)];
	else
		name = stringd(opindex(op));
	if (optype(op) > 0 && optype(op) < sizeof (TYPENAMES) - 1)
		return stringf("%s%c", name, TYPENAMES[optype(op)]);
	else
		return stringf("%s+%d", name, optype(op));
}

/* tree - allocate and initialize a tree node */
int ntree = 0;			/* next free tree in trees */
static struct tree trees[100];	/* default allocation area for trees */

Tree tree(op, type, left, right) Type type; Tree left, right; {
	register Tree p;

	if (ntree < sizeof trees/sizeof trees[0])
		p = &trees[ntree++];
	else
		p = (Tree)talloc(sizeof *p);
	BZERO(p, struct tree);
	p->op = op;
	p->type = type;
	p->kids[0] = left;
	p->kids[1] = right;
	return p;
}

/* retype - return a copy of tree p with type field = ty */
Tree retype(p, ty) Tree p; Type ty;{
	Tree q;

	if (p->type == ty)
		return p;
	q = tree(p->op, ty, p->kids[0], p->kids[1]);
	q->u = p->u;
	return q;
}

/* root - tree p will be a root; remove unnecessary temporaries */
Tree root(p) Tree p; {
	if (p == 0)
		return p;
	switch (generic(p->op)) {
	case COND: {
		Tree q = p->kids[1];
		assert(q && q->op == RIGHT);
		if (p->u.sym && q->kids[0] && generic(q->kids[0]->op) == ASGN)
			q->kids[0] = root(q->kids[0]->kids[1]);
		else
			q->kids[0] = root(q->kids[0]);
		if (p->u.sym && q->kids[1] && generic(q->kids[1]->op) == ASGN)
			q->kids[1] = root(q->kids[1]->kids[1]);
		else
			q->kids[1] = root(q->kids[1]);
		if (p->u.sym)
			release(p->u.sym);
		p->u.sym = 0;
		if (q->kids[0] == 0 && q->kids[1] == 0)
			p = root(p->kids[0]);
		}
		break;
	case AND: case OR:
		if ((p->kids[1] = root(p->kids[1])) == 0)
			p = root(p->kids[0]);
		break;
	case NOT:
		return root(p->kids[0]);
	case RIGHT:
		if (p->kids[1] == 0)
			return root(p->kids[0]);
		if (p->kids[0] && p->kids[0]->op == CALL+B
		&&  p->kids[1] && p->kids[1]->op == INDIR+B)
			/* avoid premature release of the CALL+B temporary */
			return p->kids[0];
		if (p->kids[0] && p->kids[0]->op == RIGHT
		&&  p->kids[1] == p->kids[0]->kids[0])
			/* de-construct e++ construction */
			return p->kids[0]->kids[1];
		/* fall thru */
	case EQ:  case NE:  case GT:   case GE:  case LE:  case LT: 
	case ADD: case SUB: case MUL:  case DIV: case MOD:
	case LSH: case RSH: case BAND: case BOR: case BXOR:
		p = tree(RIGHT, p->type, root(p->kids[0]), root(p->kids[1]));
		return p->kids[0] || p->kids[1] ? p : 0;
	case INDIR:
		if (p->type->size == 0 && unqual(p->type) != voidtype)
			warning("reference to `%t' elided\n", p->type);
		if (isptr(p->kids[0]->type) && isvolatile(p->kids[0]->type->type))
			warning("reference to `volatile %t' elided\n", p->type);
		/* fall thru */
	case CVI: case CVF:  case CVD:   case CVU: case CVC: case CVS: case CVP:
	case NEG: case BCOM: case FIELD:
		return root(p->kids[0]);
	case ADDRL:
		if (p->u.sym->temporary)
			release(p->u.sym);
		/* fall thru */
	case ADDRG: case ADDRF: case CNST:
		return 0;
	case ARG: case ASGN: case CALL: case JUMP: case LABEL:
		break;
	default: assert(0);
	}
	return p;
}

/* texpr - parse an expression via f(tok), allocating trees in transient area */
Tree texpr(f, tok) dclproto(Tree (*f),(int)); {
	int n = ntree;
	Tree p;

	ntree = sizeof trees/sizeof trees[0];
	p = (*f)(tok);
	ntree = n;
	return p;
}

/* tfree - release space in all transient arenas and default tree area */
void tfree() {
	if (glevel < 3 && !xref)
		deallocate(&transient);
	ntree = 0;
}

static int nid = 1;		/* identifies trees & nodes in debugging output */
static struct nodeid {
	int printed;
	Tree node;
} ids[500];			/* if ids[i].node == p, then p's id is i */

dclproto(static void printtree1,(Tree, int, int));

/* nodeid - lookup id for tree or node p */
int nodeid(p) Tree p; {
	int i = 1;

	ids[nid].node = p;
	while (ids[i].node != p)
		i++;
	if (i == nid)
		ids[nid++].printed = 0;
	return i;
}

/* printed - return pointer to ids[id].printed */
int *printed(id) {
	if (id)
		return &ids[id].printed;
	nid = 1;
	return 0;
}

/* printtree - print tree p on fd */
void printtree(p, fd) Tree p; {
	(void)printed(0);
	printtree1(p, fd, 1);
}

/* printtree1 - recursively print tree p */
static void printtree1(p, fd, lev) Tree p; {
	int i;
	static char blanks[] = "                                         ";

	if (p == 0 || *printed(i = nodeid(p)))
		return;
	fprint(fd, "#%d%s%s", i, &"   "[i < 10 ? 0 : i < 100 ? 1 : 2],
		 &blanks[sizeof blanks - lev]);
	fprint(fd, "%s %t", opname(p->op), p->type);
	*printed(i) = 1;
	for (i = 0; i < sizeof p->kids/sizeof p->kids[0]; i++)
		if (p->kids[i])
			fprint(fd, " #%d", nodeid(p->kids[i]));
	if (generic(p->op) == FIELD && p->u.field)
		fprint(fd, " %s %d..%d", p->u.field->name,
			fieldsize(p->u.field) + fieldright(p->u.field), fieldright(p->u.field));
	else if (generic(p->op) == CNST)
		fprint(fd, " %s", vtoa(p->type, p->u.v));
	else if (p->u.sym)
		fprint(fd, " %s", p->u.sym->name);
	if (p->node)
		fprint(fd, " node=0x%x", p->node);
	fprint(fd, "\n");
	for (i = 0; i < sizeof p->kids/sizeof p->kids[0]; i++)
		printtree1(p->kids[i], fd, lev + 1);
}
