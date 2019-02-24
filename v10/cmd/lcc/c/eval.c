/* C compiler & debugger: expression evaluation */

#include "c.h"

typedef struct {
	Tree p;			/* associated tree */
	Value r;		/* the register */
} Register;
static char *errcode;		/* evaluation error string */
static int nextr;
static Register regs[30];

dclproto(static Value *eval,(Tree))
dclproto(static Value *treereg,(Tree))

/* evaluate - evaluate tree p */
Symbol evaluate(p) Tree p; {
	static struct symbol sym;

	nextr = 1;
	errcode = 0;
	sym.u.c.v = *eval(p);
	if (errcode)
		return 0;
	sym.type = p->type;
	return &sym;
}

/* eval - recursively evaluate tree p, return allocated register */
static Value *eval(p) Tree p; {
	Value *r, *r1 = 0, *r2 = 0;
	Tree left, right;

	if (r = treereg(p))
		return r;
	if (nextr > sizeof regs/sizeof regs[0]) {
		error("expression too complicated\n");
		return &regs[0].r;
	}
	regs[nextr].p = p;
	r = &regs[nextr++].r;
	switch (generic(p->op)) {	/* control flow operators */
	case AND:
		r1 = eval(p->kids[0]);
		if (r1->i)
			r1 = eval(p->kids[1]);
		r->i = r1->i;
		return r;
	case COND:
		r = eval(p->kids[0]);
		r1 = eval(p->kids[r->i ? 1 : 2]);
		*r = *r1;
		return r;
	case NOT:
		r1 = eval(p->kids[0]);
		r->i = !r1->i;
		return r;
	case OR:
		r1 = eval(p->kids[0]);
		if (r1->i == 0)
			r1 = eval(p->kids[1]);
		r->i = r1->i;
		return r;
	}
	if (left = p->kids[0])
		r1 = eval(left);
	if (right = p->kids[1])
		r2 = eval(right);
	switch (p->op) {
	case ADD+D: r->d = r1->d + r2->d; break;
	case ADD+F: r->f = r1->f + r2->f; break;
	case ADD+I: r->i = r1->i + r2->i; break;
	case ADD+P:
		if (isptr(left->type))
			r->s = r1->s + r2->i;
		else
			r->s = r1->i + r2->s;
		break;
	case ADD+U: r->u = r1->u + r2->u; break;
	case ADDRF+P:
	case ADDRG+P:
	case ADDRL+P: goto bad;
	case ARG+B:
	case ARG+C:
	case ARG+D:
	case ARG+F:
	case ARG+I:
	case ARG+P:
	case ARG+S: goto bad;
	case ASGN+B:
	case ASGN+C:
	case ASGN+D:
	case ASGN+F:
	case ASGN+I:
	case ASGN+P:
	case ASGN+S: goto bad;
	case BAND+I: r->i = r1->i&r2->i; break;
	case BAND+U: r->u = r1->u&r2->u; break;
	case BCOM+U: r->u = ~r1->u; break;
	case BOR+I:  r->i = r1->i|r2->i; break;
	case BOR+U:  r->u = r1->u|r2->u; break;
	case BXOR+I: r->i = r1->i^r2->i; break;
	case BXOR+U: r->u = r1->u^r2->u; break;
	case CALL+B:
	case CALL+D:
	case CALL+F:
	case CALL+I:
	case CALL+V: goto bad;
	case CNST+C:
	case CNST+D:
	case CNST+F:
	case CNST+I:
	case CNST+P:
	case CNST+S:
	case CNST+U: *r = p->u.v; break;
	case CVC+I: r->i = r1->sc; break;
	case CVC+U: r->u = r1->uc; break;
	case CVD+F: r->f = r1->d; break;
	case CVD+I: r->i = r1->d; break;
	case CVD+U: r->u = r1->d; break;
	case CVF+D: r->d = r1->f; break;
	case CVI+C: r->sc = r1->i; break;
	case CVI+D: r->d = r1->i; break;
	case CVI+S: r->ss = r1->i; break;
	case CVI+U: r->u = r1->i; break;
	case CVP+U: r->u = (unsigned)r1->p; break;
	case CVS+I: r->i = r1->ss; break;
	case CVS+U: r->u = r1->us; break;
	case CVU+C: r->uc = r1->u; break;
	case CVU+D: r->d = r1->u; break;
	case CVU+I: r->i = r1->u; break;
	case CVU+P: r->p = (char *)r1->u; break;
	case CVU+S: r->us = r1->u; break;
	case DIV+D:
		if (r2->d == 0.0)
			errcode = "division by 0.0";
		else
			r->d = r1->d/r2->d;
		break;
	case DIV+F:
		if (r2->f == 0.0)
			errcode = "division by 0";
		else
			r->f = r1->f/r2->f;
		break;
	case DIV+I:
		if (r2->i == 0)
			errcode = "division by 0";
		else
			r->i = r1->i/r2->i;
		break;
	case DIV+U:		
		if (r2->u == 0)
			errcode = "division by 0";
		else
			r->u = r1->u/r2->u;
		break;
	case EQ+D: r->i = r1->d == r2->d; break;
	case EQ+F: r->i = r1->f == r2->f; break;
	case EQ+I: r->i = r1->i == r2->i; break;
	case EQ+U: r->i = r1->u == r2->u; break;
	case FIELD: goto bad;
	case GE+D: r->i = r1->d >= r2->d; break;
	case GE+F: r->i = r1->f >= r2->f; break;
	case GE+I: r->i = r1->i >= r2->i; break;
	case GE+U: r->i = r1->u >= r2->u; break;
	case GT+D: r->i = r1->d >  r2->d; break;
	case GT+F: r->i = r1->f >  r2->f; break;
	case GT+I: r->i = r1->i >  r2->i; break;
	case GT+U: r->i = r1->u >  r2->u; break;
	case INDIR+B:
	case INDIR+C:
	case INDIR+D:
	case INDIR+F:
	case INDIR+I:
	case INDIR+P:
	case INDIR+S: goto bad;
	case JUMP+V:
		fatal("eval", "unexpected %s operator\n", "JUMP+V");
	case LE+D: r->i = r1->d <= r2->d; break;
	case LE+F: r->i = r1->f <= r2->f; break;
	case LE+I: r->i = r1->i <= r2->i; break;
	case LE+U: r->i = r1->u <= r2->u; break;
	case LSH+I: r->i = r1->i<<r2->i; break;
	case LSH+U: r->u = r1->u<<r2->u; break;
	case LT+D:  r->i = r1->d < r2->d; break;
	case LT+F:  r->i = r1->f < r2->f; break;
	case LT+I:  r->i = r1->i < r2->i; break;
	case LT+U:  r->i = r1->u < r2->u; break;
	case MOD+I:
		if (r2->i == 0)
			errcode = "division by 0";
		else
			r->i = r1->i%r2->i;
		break;
	case MOD+U:		
		if (r2->u == 0)
			errcode = "division by 0";
		else
			r->u = r1->u%r2->u;
		break;
	case MUL+D: r->d = r1->d*r2->d; break;
	case MUL+F: r->f = r1->f*r2->f; break;
	case MUL+I: r->i = r1->i*r2->i; break;
	case MUL+U: r->u = r1->u*r2->u; break;
	case NE+D:  r->i = r1->d != r2->d; break;
	case NE+F:  r->i = r1->f != r2->f; break;
	case NE+I:  r->i = r1->i != r2->i; break;
	case NE+U:  r->i = r1->u != r2->i; break;
	case NEG+D: r->d = -r1->d; break;
	case NEG+F: r->f = -r1->f; break;
	case NEG+I: r->i = -r1->i; break;
	case RET+B:
	case RET+D:
	case RET+F:
	case RET+I:
	case RET+V: goto bad;
	case RIGHT: r->d = r2->d; break;
	case RSH+I: r->i = r1->i>>r2->i; break;
	case RSH+U: r->u = r1->u>>r2->u; break;
	case SUB+D: r->d = r1->d - r2->d; break;
	case SUB+F: r->f = r1->f - r2->f; break;
	case SUB+I: r->i = r1->i - r2->i; break;
	case SUB+P: r->i = r1->s - r2->s; break;
	case SUB+U: r->u = r1->u - r2->u; break;
	default: bad:
		fatal("eval", "unknown operator %d\n", p->op);
	}
	return r;
}

/* treereg - return register associated with tree p or 0 */
static Value *treereg(p) Tree p; {
	int i;

	for (i = 0; i < nextr; i++)
		if (regs[i].p == p)
			return &regs[i].r;
	return 0;
}

/* vtoa - return string for the constant v of type ty */
char *vtoa(ty, v) Type ty; Value v; {
	static char buf[MAXLINE];

	ty = unqual(ty);
	switch (ty->op) {
	case CHAR:
		sprint(buf, "%d", v.uc);
		break;
	case SHORT:
		sprint(buf, "%d", v.ss);
		break;
	case INT:
		sprint(buf, "%d", v.i);
		break;
	case UNSIGNED:
		if ((v.u&~0x7fff) == 0)
			sprint(buf, "%d", v.u);
		else
			sprint(buf, "0x%x", v.u);
		break;
	case FLOAT:
		if (v.f == 0.0)
			return "0";
		sprintf(buf, "%.*g", 8, v.f);
		break;
	case DOUBLE:
		if (v.d == 0.0)
			return "0";
		sprintf(buf, "%.*g", 18, v.d);
		break;
	case ARRAY:
		if (ty->type->op == CHAR)
			return v.s;
		/* else fall thru */
	case POINTER: case FUNCTION:
		if (((unsigned)v.p&~0x7fff) == 0)
			sprint(buf, "%d", v.p);
		else
			sprint(buf, "0x%x", v.p);
		break;
	default:
		assert(0);
	}
	return buf;
}
