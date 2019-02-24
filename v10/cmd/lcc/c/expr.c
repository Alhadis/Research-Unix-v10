/* C compiler: expression parsing */

#include "c.h"

static char prec[] = {
#define xx(a,b,c,d,e,f,g) c,
#include "token.h"
};
static int oper[] = {
#define xx(a,b,c,d,e,f,g) d,
#include "token.h"
};
dclproto(static Tree call,(Tree, Type, Coordinate));
dclproto(static Tree expr2,(void));
dclproto(static Tree expr3,(int));
dclproto(static Tree nullcheck,(Tree));
dclproto(static Tree postfix,(Tree));
dclproto(static Tree prefix,(void));
dclproto(static Tree primary,(void));
dclproto(static Tree rightkid,(Tree));
dclproto(static Tree value,(Tree));

/* addrof - address of p */
Tree addrof(p) Tree p; {
	Tree q = p;

	for (;;)
		switch (generic(q->op)) {
		case RIGHT:
			assert(q->kids[0] || q->kids[1]);
			q = q->kids[1] ? q->kids[1] : q->kids[0];
			continue;
		case ASGN:
			q = q->kids[1];
			continue;
		case COND: {
			Symbol t1 = q->u.sym;
			q->u.sym = 0;
			q = idnode(t1);
			/* fall thru */
			}
		case INDIR:
			if (p == q)
				return q->kids[0];
			q = q->kids[0];
			return tree(RIGHT, q->type, root(p), q);
		default:
			error("addressable object required\n");
			return value(p);
		}
}

/* asgn - generate tree for assignment of expr e to symbol p sans qualifiers */
Tree asgn(p, e) Symbol p; Tree e; {
	if (isarray(p->type))
		e = tree(ASGN+B, p->type, idnode(p),
			tree(INDIR+B, e->type, e, 0));
	else {
		Type ty = p->type;
		p->type = unqual(p->type);
		if (isstruct(p->type) && p->type->u.sym->u.s.cfields) {
			p->type->u.sym->u.s.cfields = 0;
			e = asgnnode(ASGN, idnode(p), e);
			p->type->u.sym->u.s.cfields = 1;
		} else
			e = asgnnode(ASGN, idnode(p), e);
		p->type = ty;
	}
	return e;
}

/* assign - perform type-checking of assignment of p to variable of type xty */
Type assign(xty, p) Type xty; Tree p; {
	Type yty = unqual(p->type);

	if (yty->size == 0)
		return 0;
	xty = unqual(xty);
	if (isenum(xty))
		xty = xty->type;
	if (isarith(xty) && isarith(yty))
		return xty;
	if (isstruct(xty) && xty == yty)
		return xty;
	if (isstruct(xty) && isstruct(yty) && extends(yty, xty))
		return xty;
	if (isptr(xty) && isptr(yty)) {
		Type lty = xty->type, rty = yty->type;
		if (isstruct(lty) && isstruct(rty) && extends(rty, lty))
			return xty;
		if (eqtype(lty, rty, 1)
		|| (eqtype(unqual(lty), unqual(rty), 1)
		&& (   !isconst(rty) || isconst(lty))
		&& (!isvolatile(rty) || isvolatile(lty))))
			return yty;
		if (xty == voidptype || unqual(lty) == voidtype
		||  yty == voidptype || unqual(rty) == voidtype)
			return xty;
		if (lty == inttype && isenum(rty) || rty == inttype && isenum(lty)) {
			if (Aflag >= 1)
				warning("assignment between `%t' and `%t' is compiler-dependent\n",
					xty, yty);
			return xty;
		}
	}
	if (isptr(xty) && isint(p->type) && generic(p->op) == CNST
	&& cast(p, unsignedtype)->u.v.u == 0)
		return xty;
	return 0;
}

/* call - parse function call to f, type fty */
static Tree call(f, fty, src) Tree f; Type fty; Coordinate src; {
	int n = 0;
	Tree args = 0, r = 0;
	Type *proto = fty->u.f.oldstyle ? (Type *)0 : fty->u.f.proto;

	if (hascall(f))
		r = f;
	if (t != ')')
		do {
			Tree q = pointer(expr1(0));
			if (proto && *proto && *proto != voidtype) {
				Type aty;
				q = value(q);
				if (aty = assign(*proto, q)) {
					q = cast(q, aty);
					if ((isint(q->type) || isenum(q->type))
					&& q->type->size != inttype->size)
						q = cast(q, promote(q->type));
				} else
					error("type error in argument %d to %s; found `%t' expected `%t'\n",
						n + 1, funcname(f), q->type, *proto);
				proto++;
			} else {
				if (!fty->u.f.oldstyle && *proto == 0)
					error("too many arguments to %s\n", funcname(f));
				q = value(q);
				if (q->type == floattype)
					q = cast(q, doubletype);
				else if ((isint(q->type) || isenum(q->type))
				&& q->type->size != inttype->size)
					q = cast(q, promote(q->type));
				else if (isarray(q->type) || q->type->size == 0)
					error("type error in argument %d to %s; `%t' is illegal\n",
						n + 1, funcname(f), q->type);
			}
			if (IR->no_argb && isstruct(q->type)) {
				Symbol t1;
				if (q->op == RIGHT && q->kids[0]
				&& q->kids[0]->op == CALL+B) {
					/*
					 * When the argument is f() where f
					 * returns a struct, simply use the
					 * temporary generated to hold f's
					 * return value.
					 */
					assert(q->kids[0]->kids[1]);
					t1 = q->kids[0]->kids[1]->u.sym;
					q = q->kids[0];
				} else {
					t1 = temporary(AUTO, unqual(q->type));
					q = asgn(t1, q);
				}
				q = tree(RIGHT, ptr(q->type), root(q), lvalue(idnode(t1)));
			}
			if (q->type->size == 0)
				q->type = inttype;
			if (hascall(q))
				r = tree(RIGHT, voidtype, r, q);
			args = tree(ARG + widen(q->type), q->type, q, args);
			n++;
			if (Aflag >= 2 && n == 32)
				warning("more than 31 arguments in a call to %s\n", funcname(f));
		} while (t == ',' && (t = gettok()));
	expect(')');
	if (proto && *proto && *proto != voidtype)
		error("insufficient number of arguments to %s\n", funcname(f));
	if (r)
		args = tree(RIGHT, voidtype, r, args);
	if (events.calls)
		apply(events.calls, (Generic)&src, (Generic)&f);
	return callnode(f, freturn(fty), args);
}

/* cast - cast expression p to type */
Tree cast(p, type) Tree p; Type type; {
	Field q;
	Type pty, ty = unqual(type);

	p = value(p);
	if (p->type == type)
		return p;
	pty = unqual(p->type);
	if (pty == ty)
		return retype(p, type);
	if (pty == signedchar || pty == chartype)
		p = simplify(CVC, inttype, p, 0);
	else if (pty == shorttype)
		p = simplify(CVS, inttype, p, 0);
	else if (pty == longtype)
		p = retype(p, inttype);
	else if (pty == floattype)
		p = simplify(CVF, doubletype, p, 0);
	else if (pty == longdouble)
		p = retype(p, doubletype);
	else if (pty == unsignedchar)
		p = simplify(CVC, unsignedtype, p, 0);
	else if (pty == unsignedshort)
		p = simplify(CVS, unsignedtype, p, 0);
	else if (pty == unsignedlong)
		p = retype(p, unsignedtype);
	else if (isptr(pty)) {
		if (isstruct(pty->type) && isptr(ty) && isstruct(ty->type)
		&& (q = extends(pty->type, ty->type)))
			return simplify(ADD+P, ty, p, constnode(q->offset, inttype));
		if (isptr(ty)) {
			if (isfunc(pty->type) && !isfunc(ty->type)
			|| !isfunc(pty->type) &&  isfunc(ty->type))
				warning("conversion from `%t' to `%t' is compiler-dependent\n",
					p->type, ty);
			return retype(p, type);
		}
		p = simplify(CVP, unsignedtype, p, 0);
	}
	else if (isstruct(pty) && isstruct(ty) && (q = extends(pty, ty)))
		return rvalue(simplify(ADD+P, ptr(ty), addrof(p),
			constnode(q->offset, inttype)));
	pty = unqual(p->type);
	if (pty == inttype || isenum(pty)) {
		if (ty == floattype || ty == doubletype || ty == longdouble)
			p = simplify(CVI, doubletype, p, 0);
		else if (ty == unsignedtype  || ty == unsignedchar
		     ||  ty == unsignedshort || ty == unsignedlong || isptr(ty))
			p = simplify(CVI, unsignedtype, p, 0);
	} else if (pty == doubletype) {
		if (ty == signedchar || ty == chartype || isenum(ty)
		||  ty == shorttype  || ty == inttype  || ty == longtype)
			p = simplify(CVD, inttype, p, 0);
		else if (ty == unsignedtype  || ty == unsignedchar
		     ||  ty == unsignedshort || ty == unsignedlong || isptr(ty)) {
			/*
			 * For (unsigned)d for double d, build
			 * d >= INT_MAX+1 ? (unsigned)(int)(d-(INT_MAX+1)) + INT_MAX+1 :
			 *		    (unsigned)(int)d
			 */
			Tree c = tree(CNST+D, doubletype, 0, 0);
			c->u.v.d = (double)INT_MAX + 1;
			p = condnode(
				simplify(GE, doubletype, p, c),
				(*opnode['+'])(ADD,
					cast(cast(simplify(SUB, doubletype, p, c), inttype), unsignedtype),
					constnode((unsigned)INT_MAX + 1, unsignedtype)),
				simplify(CVD, inttype, p, 0));
		}
	} else if (pty == unsignedtype) {
		if (ty == signedchar || ty == chartype || isenum(ty)
		||  ty == shorttype  || ty == inttype  || ty == longtype)
			p = simplify(CVU, inttype, p, 0);
		else if (ty == floattype || ty == doubletype || ty == longdouble) {
			/*
			 * For (double)u for unsigned u, build
			 * (int)u >= 0 ? (double)(int)u : (double)(int)u + UINT_MAX + 1
			 */
			Tree u = simplify(CVU, inttype, p, 0);
			p = tree(CNST+D, doubletype, 0, 0);
			p->u.v.d = utod(UINT_MAX) + 1;
			p = condnode(
				simplify(GE, inttype, u, constnode(0, inttype)),
				simplify(CVI, doubletype, u, 0),
				(*opnode['+'])(ADD, simplify(CVI, doubletype, u, 0), p));
		}
	} else
		assert(0);
	if (ty == signedchar || ty == chartype || ty == shorttype)
		p = simplify(CVI, type, p, 0);
	else if (ty == floattype)
		p = simplify(CVD, type, p, 0);
	else if (ty == unsignedchar || ty == unsignedshort || isptr(ty))
		p = simplify(CVU, type, p, 0);
	else
		p = retype(p, type);
	return p;
}

/* cond - check for conditional operator, add comparison if necessary */
Tree cond(p) Tree p; {
	Opcode op = generic(rightkid(p)->op);

	if (op == AND || op == OR || op == NOT || op == EQ || op == NE
	||  op ==  LE || op == LT || op ==  GE || op == GT)
		return p;
	p = pointer(p);
	p = cast(p, promote(p->type));
	return (*opnode[NEQ])(NE, p, constnode(0, inttype));
}

/* conditional - parse expression and cast to conditional */
Tree conditional(tok) {
	Tree p = expr(tok);

	if (Aflag > 1 && isfunc(p->type))
		warning("%s used in a conditional expression\n", funcname(p));
	return cond(p);
}

/* constexpr - parse a constant expression */
Tree constexpr(tok) {
	Tree p;

	needconst++;
	p = expr1(tok);
	needconst--;
	return p;
}

/* expr0 - parse an expression for side effect */
Tree expr0(tok) {
	return root(expr(tok));
}

/* expr - parse an expression */
Tree expr(tok) {
	Tree p = expr1(0);

	while (t == ',') {
		Tree q;
		t = gettok();
		q = pointer(expr1(0));
		if (generic(p->op) != CNST)
			p = root(value(p));
		p = tree(RIGHT, q->type, p, q);
	}
	if (tok && t == tok)
		t = gettok();
	else if (tok) {
		static char follow[] = { IF, ID, '}', 0 };
		test(tok, follow);
	}
	return p;
}

/* expr1 - parse assignments */
Tree expr1(tok) {
	Tree p = expr2();

	while (t == '=' || (prec[t] >= 6 && prec[t] <= 8)
	|| (prec[t] >= 11 && prec[t] <= 13)) {
		Opcode op = t;
		t = gettok();
		if (oper[op] == ASGN)
			p = asgnnode(ASGN, p, value(expr1(0)));
		else {
			expect('=');
			p = incr(op, p, expr1(0));
		}
	}
	if (tok && t == tok)
		t = gettok();
	else if (tok) {
		static char follow[] = { IF, ID, 0 };
		test(tok, follow);
	}
	return p;
}

/* expr2 - parse conditional expressions */
static Tree expr2() {
	Tree p = expr3(4);

	if (t == '?') {
		Tree l, r;
		if (Aflag > 1 && isfunc(p->type))
			warning("%s used in a conditional expression\n", funcname(p));
		p = pointer(p);
		t = gettok();
		if (events.points) {
			Tree e = 0;
			apply(events.points, (Generic)&src, (Generic)&e);
			l = right(e, pointer(expr(':')));
			e = 0;
			apply(events.points, (Generic)&src, (Generic)&e);
			r = right(e, pointer(expr2()));
		} else {
			l = pointer(expr(':'));
			r = pointer(expr2());
		}
		p = condnode(p, l, r);
	}
	return p;
}

/* expr3 - parse expressions at precedence level k */
static Tree expr3(k) {
	int k1;
	Tree p = prefix();

	for (k1 = prec[t]; k1 >= k; k1--)
		while (prec[t] == k1 && *cp != '=') {
			Tree r;
			Opcode op = t;
			t = gettok();
			p = pointer(p);
			if (events.points && (op == ANDAND || op == OROR)) {
				Tree e = 0;
				apply(events.points, (Generic)&src, (Generic)&e);
				r = right(e, pointer(expr3(k1 + (k1>5))));
			} else
				r = pointer(expr3(k1 + (k1>5)));
			p = (*opnode[op])(oper[op], p, r); 
		}
	return p;
}

/* field - construct tree for reference to field name via p */
Tree field(p, name) Tree p; char *name; {
	Field q;
	Type ty1, ty = p->type;

	if (isptr(ty))
		ty = deref(ty);
	ty1 = ty;
	ty = unqual(ty);
	if (q = fieldref(name, ty)) {
		ty = q->type;
		if (isconst(ty1) && !isconst(ty))
			ty = qual(CONST, ty);
		if (isvolatile(ty1) && !isvolatile(ty))
			ty = qual(VOLATILE, ty);
		if (!isarray(q->type))
			ty = ptr(ty);
		p = simplify(ADD+P, ty, p, constnode(q->offset, inttype));
		if (q->lsb) {
			p = tree(FIELD, q->type, rvalue(p), 0);
			p->u.field = q;
		} else if (!isarray(q->type))
			p = rvalue(p);
	} else {
		error("unknown field `%s' of `%t'\n", name, ty);
		p = rvalue(retype(p, ptr(inttype)));
	}
	return p;
}

/* funcname - return name of function f or `a function' */
char *funcname(f) Tree f; {
	if (isaddrop(f->op))
		return stringf("`%s'", f->u.sym->name);
	return "a function";
}

/* idnode - construct tree for reference to r-value of identifier p */
Tree idnode(p) Symbol p; {
	Opcode op;
	Tree e;
	Type ty = p->type ? unqual(p->type) : voidtype;

	p->ref += refinc;
	if (p->scope == PARAM) {
		if (IR->no_argb && isstruct(p->type)) {
			e = tree(ADDRF+P, ptr(ptr(p->type)), 0, 0);
			e->u.sym = p;
			e = rvalue(e);
		} else {
			e = tree(ADDRF+P, ptr(p->type), 0, 0);
			e->u.sym = p;
		}
		return rvalue(e);
	}
	if (p->scope == GLOBAL || p->sclass == STATIC || p->sclass == EXTERN) {
		op = ADDRG+P;
		if (isfunc(ty))
			p->addressed = 1;
	} else
		op = ADDRL+P;
	if (isarray(ty) || isfunc(ty)) {
		e = tree(op, p->type ? p->type : voidtype, 0, 0);
		e->u.sym = p;
		return e;
	} else {
		e = tree(op, ptr(p->type), 0, 0);
		e->u.sym = p;
		return rvalue(e);
	}
}

/* incr - construct tree for e1 op= e2 */
Tree incr(op, e1, e2) Tree e1, e2; {
	return asgnnode(ASGN, e1, (*opnode[op])(oper[op], e1, e2));
}

/* intexpr - parse a constant expression and return int value, default n */
int intexpr(tok, n) {
	Tree p = constexpr(tok);

	needconst++;
	if (generic(p->op) == CNST && isint(p->type))
		n = cast(p, inttype)->u.v.i;
	else
		error("integer expression must be constant\n");
	needconst--;
	return n;
}

/* lvalue - check for lvalue, return pointer to lvalue tree */
Tree lvalue(p) Tree p; {
	if (generic(p->op) == INDIR) {
		if (unqual(p->type) == voidtype)
			warning("`%t' used as an lvalue\n", p->type);
		return p->kids[0];
	}
	error("lvalue required\n");
	return value(p);
}

/* nullcheck - test if p null; build ((!p ? _YYnull(lineno) : ), p) */
static Tree nullcheck(p) Tree p; {
	Tree q, arg;

	if (needconst)
		return p;
	arg = tree(ARG+I, inttype, constnode(lineno, inttype), 0);
	q = tree(COND, voidtype,
		tree(NOT, inttype, cond(p), 0),
		tree(RIGHT, inttype, callnode(pointer(idnode(YYnull)), voidtype, arg), 0));
	return tree(RIGHT, p->type, q, p);
}

/* pointer - re-type `array of T', `T function' to `ptr to T', `ptr to T function', resp. */
Tree pointer(p) Tree p; {
	if (isarray(p->type) && (p->op != RIGHT || p->u.sym == 0))
		p = retype(p, atop(p->type));
	else if (isfunc(p->type))
		p = retype(p, ptr(p->type));
	return p;
}

/* postfix - parse a postfix expresssion; p is the primary dag */
static Tree postfix(p) Tree p; {
	Tree q;
	Type ty;

	for (;;)
		switch (t) {
		case INCR: case DECR:
			p = tree(RIGHT, p->type,
				tree(RIGHT, p->type, p, root(incr(t, p, constnode(1, inttype)))),
				p);
			t = gettok();
			continue;
		case '[':
			t = gettok();
			q = expr(']');
			if (YYnull)
				if (isptr(p->type))
					p = nullcheck(p);
				else if (isptr(q->type))
					q = nullcheck(q);
			q = pointer(q);
			p = (*opnode['+'])(ADD, pointer(p), q);
			if (isptr(p->type) && isarray(p->type->type))
				p = retype(p, p->type->type);
			else
				p = rvalue(p);
			continue;
		case '(': {
			Coordinate pt;
			p = pointer(p);
			if (isptr(p->type) && isfunc(p->type->type))
				ty = p->type->type;
			else {
				error("found `%t' expected a function\n", p->type);
				ty = func(voidtype, 0, 1);
			}
			pt = src;
			t = gettok();
			p = call(p, ty, pt);
			continue;
			}
		case '.':
			t = gettok();
			if (t == ID) {
				if (isstruct(p->type)) {
					q = addrof(p);
					p = field(q, token);	/* refme */
					q = rightkid(q);
					if (isaddrop(q->op) && q->u.sym->temporary) {
						p = tree(RIGHT, p->type, p, 0);
						p->u.sym = q->u.sym;
					}
				} else
					error("left operand of . has incompatible type `%t'\n",
						p->type);
				t = gettok();
			} else
				error("field name expected\n");
			continue;
		case DEREF:
			t = gettok();
			p = pointer(p);
			if (t == ID) {
				if (isptr(p->type))
					ty = deref(p->type);
				if (isptr(p->type) && isstruct(ty)) {
					if (YYnull)
						p = nullcheck(p);
					p = field(p, token);	/* refme */
				} else
					error("left operand of -> has incompatible type `%t'\n",
						p->type);
				t = gettok();
			} else
				error("field name expected\n");
			continue;
		default:
			return p;
		}
}

/* prefix - parse a prefix expression */
static Tree prefix() {
	Tree p;

	switch (t) {
	case '*':
		t = gettok();
		p = pointer(prefix());
		if (isptr(p->type) && (isfunc(p->type->type) || isarray(p->type->type))) {
			p = retype(p, p->type->type);
			break;
		}
		if (YYnull)
			p = nullcheck(p);
		p = rvalue(p);
		break;
	case '&':
		t = gettok();
		p = prefix();
		if (isarray(p->type) || isfunc(p->type))
			p = retype(p, ptr(p->type));
		else
			p = lvalue(p);
		if (isaddrop(p->op))
			if (p->u.sym->sclass == REGISTER)
				error("invalid operand of unary &; `%s' is declared register\n",
					p->u.sym->name);
			else
				p->u.sym->addressed = 1;
		break;
	case '+':
		t = gettok();
		p = pointer(prefix());
		if (isarith(p->type))
			p = cast(p, promote(p->type));
		else
			typeerror(ADD, p, 0); 
		break;
	case '-':
		t = gettok();
		p = pointer(prefix());
		if (isarith(p->type)) {
			p = cast(p, promote(p->type));
			if (isunsigned(p->type)) {
				warning("unsigned operand of unary -\n");
				p = cast(simplify(NEG+I, inttype, cast(p, inttype), 0), unsignedtype);
			} else
				p = simplify(NEG + ttob(p->type), p->type, p, 0);
		} else
			typeerror(SUB, p, 0);
		break;
	case '~':
		t = gettok();
		p = pointer(prefix());
		if (isint(p->type)) {
			Type ty = promote(p->type);
			p = simplify(BCOM, ty, cast(p, ty), 0);
		} else
			typeerror(BCOM, p, 0); 
		break;
	case '!':
		t = gettok();
		p = pointer(prefix());
		if (isscalar(p->type))
			p = simplify(NOT, inttype, cond(p), 0);
		else
			typeerror(NOT, p, 0); 
		break;
	case INCR: case DECR: {
		Opcode op = t;
		t = gettok();
		p = incr(op, pointer(prefix()), constnode(1, inttype));
		break;
		}
	case SIZEOF: {
		Type ty;
		t = gettok();
		p = 0;
		if (t == '(') {
			t = gettok();
			if (kind[t] == CHAR || t == ID
			&& tsym && tsym->sclass == TYPEDEF) {
				ty = typename();
				expect(')');
			} else if (t == SCON) {
				ty = tsym->type;
				t = gettok();
				expect(')');
			} else
				p = postfix(expr(')'));
		} else if (t == SCON) {
			ty = tsym->type;
			t = gettok();
		} else
			p = prefix();
		if (p)
			ty = p->type;
		if (isfunc(ty) || ty->size == 0)
			error("invalid type argument `%t' to `sizeof'\n", ty);
		else if (p && p->op == FIELD)
			error("`sizeof' applied to a bit field\n");
		p = constnode(ty->size, unsignedtype);
		break;
		}
	case '(':
		t = gettok();
		if (kind[t] == CHAR || t == ID
		&& tsym && tsym->sclass == TYPEDEF) {
			Type ty, ty1 = typename();
			ty = unqual(ty1);
			if (isenum(ty)) {
				Type ty2 = ty->type;
				if (isconst(ty1))
					ty2 = qual(CONST, ty2);
				if (isvolatile(ty1))
					ty2 = qual(VOLATILE, ty2);
				ty1 = ty2;
				ty = ty->type;
			}
			expect(')');
			if (isstruct(ty) && t == '{') {
				Symbol t1 = temporary(AUTO, ty);
				if (Aflag >= 2)
					warning("non-ANSI constructor for `%t'\n", ty);
				p = tree(RIGHT, ty1, structexp(ty, t1), idnode(t1));
				break;
			}
			p = pointer(prefix());
			if ((isarith(p->type) || isenum(p->type)) && isarith(ty)
			|| isptr(ty) && (isptr(p->type) || isint(p->type) || isenum(p->type)))
				p = cast(p, ty);
			else if (isint(ty) &&  isptr(p->type)) {
				if (Aflag >= 1 && ty->size < p->type->size)
					warning("conversion from `%t' to `%t' is compiler-dependent\n",
						p->type, ty);
				p = cast(p, ty);
			} else if (ty != voidtype) {
				error("cast from `%t' to `%t' is illegal\n",
					p->type, ty1);
				ty1 = inttype;
			}
			p = retype(p, ty1);
			if (/* ty != ty1 && generic(p->op) == CNST
			|| */ generic(p->op) == INDIR)
				p = tree(RIGHT, ty, 0, p);
		} else
			p = postfix(expr(')'));
		break;
	default:
		p = postfix(primary());
	}
	return p;
}

/* primary - parse a primary expression */
static Tree primary() {
	Tree p;

	switch (t) {
	case ID:
		if (tsym == 0) {
			Symbol q = install(token, &identifiers, level < LOCAL);
			q->src = src;
			t = gettok();
			if (t == '(') {
				Symbol r;
				q->sclass = EXTERN;
				q->type = func(inttype, (Type *)0, 1);
				if (Aflag >= 1)
					warning("missing prototype\n");
				(*IR->defsymbol)(q);
		 		if (r = lookup(q->name, externals)) {
					q->defined = r->defined;
					q->temporary = r->temporary;
					q->generated = r->generated;
					q->computed = r->computed;
					q->addressed = r->addressed;
				} else {
					r = install(q->name, &externals, 1);
					r->src = q->src;
					r->type = q->type;
					r->sclass = EXTERN;
				}
			} else {
				error("undeclared identifier `%s'\n", q->name);
				q->sclass = AUTO;
				q->type = inttype;
				if (q->scope == GLOBAL)
					(*IR->defsymbol)(q);
				else
					addlocal(q);
			}
			if (xref)
				use(q, src);
			return idnode(q);
		}
		if (xref)
			use(tsym, src);
		if (tsym->sclass == ENUM)
			p = constnode(tsym->u.value, inttype);
		else {
			if (tsym->sclass == TYPEDEF)
				error("illegal use of type name `%s'\n", tsym->name);
			p = idnode(tsym);
		}
		break;
	case ICON: case FCON:
		p = tree(CNST + ttob(tsym->type), tsym->type, 0, 0);
		p->u.v = tsym->u.c.v;
		break;
	case SCON:
		tsym->u.c.v.p = stringn(tsym->u.c.v.p, tsym->type->size);
		tsym = constant(tsym->type, tsym->u.c.v); 
		if (tsym->u.c.loc == 0)
			tsym->u.c.loc = genident(STATIC, tsym->type, GLOBAL);
		p = idnode(tsym->u.c.loc);
		break;
	case '(':
		t = gettok();
		return expr(')');
	default:
		error("illegal expression\n");
		p = constnode(0, inttype);
	}
	t = gettok();
	return p;
}

/* promote - the usual integral promotions */
Type promote(ty) Type ty; {
	ty = unqual(ty);
	if (isunsigned(ty) || ty == longtype)
		return ty;
	else if (isint(ty) || isenum(ty))
		return inttype;
	return ty;
}

/* right - return (RIGHT, q->type, root(p), q) or just root(p/q) if q/p==0 */
Tree right(p, q) Tree p, q; {
	assert(p || q);
	if (p && q)
		return tree(RIGHT, q->type, root(p), q);
	return p ? p : q;
}

/* rightkid - dereference RIGHT nodes to find ultimate offspring */
static Tree rightkid(p) Tree p; {
	while (p && p->op == RIGHT) {
		assert(p->kids[0] || p->kids[1]);
		p = p->kids[1] ? p->kids[1] : p->kids[0];
	}
	assert(p);
	return p;
}

/* rvalue - convert p to an rvalue */
Tree rvalue(p) Tree p; {
	Type ty = deref(p->type);

	ty = unqual(ty);
	return tree(INDIR + (isunsigned(ty) ? I : ttob(ty)), ty, p, 0);
}

/* value - convert p from a conditional to a value, if necessary */
static Tree value(p) Tree p; {
	Opcode op = generic(rightkid(p)->op);

	if (op == AND || op == OR || op == NOT || op == EQ || op == NE
	||  op ==  LE || op == LT || op ==  GE || op == GT)
		p = condnode(p, constnode(1, inttype), constnode(0, inttype));
	return p;
}
