/* C compiler: type-checking and tree construction for expressions */

#include "c.h"

dclproto(static Tree addnode,(int, Tree, Tree));
dclproto(static Tree andnode,(int, Tree, Tree));
dclproto(static Type binary,(Type, Type));
dclproto(static Tree cmpnode,(int, Tree, Tree));
dclproto(static int compatible,(Type, Type));
dclproto(static Tree mulnode,(int, Tree, Tree));
dclproto(static Tree subnode,(int, Tree, Tree));

dclproto(Tree (*opnode[]),(int, Tree, Tree)) = {
#define xx(a,b,c,d,e,f,g) e,
#include "token.h"
};

/* addnode - construct tree for l + r */
static Tree addnode(op, l, r) Tree l, r; {
	int n;
	Type ty = inttype;

	if (isarith(l->type) && isarith(r->type)) {
		ty = binary(l->type, r->type);
		l = cast(l, ty);
		r = cast(r, ty);
	} else if (isptr(l->type) && isint(r->type))
		return addnode(ADD, r, l);
	else if (isptr(r->type) && isint(l->type)) {
		ty = unqual(r->type);
		if ((n = ty->type->size) == 0)
			error("unknown size of type `%t'\n", ty->type);
		l = cast(l, promote(l->type));
		if (n > 1)
			l = mulnode(MUL, constnode(n, inttype), l);
		return simplify(ADD+P, ty, l, r);
	} else
		typeerror(op, l, r);
	return simplify(op, ty, l, r);
}

/* andnode - construct tree for l [&& ||] r */
static Tree andnode(op, l, r) Tree l, r; {
	if (!isscalar(l->type) || !isscalar(r->type))
		typeerror(op, l, r);
	return simplify(op, inttype, cond(l), cond(r));
}

/* asgnnode - construct tree for l = r */
Tree asgnnode(op, l, r) Tree l, r; {
	Type aty, ty;

	r = pointer(r);
	if ((ty = assign(l->type, r)) == 0) {
		typeerror(ASGN, l, r);
		if (r->type == voidtype)
			r = retype(r, inttype);
		ty = r->type;
	}
	if (l->op != FIELD)
		l = lvalue(l);
	aty = l->type;
	if (isptr(aty))
		aty = unqual(aty)->type;
	if (isconst(aty) || (isstruct(aty) && unqual(aty)->u.sym->u.s.cfields))
		if (isaddrop(l->op) && !l->u.sym->computed && !l->u.sym->generated)
			error("assignment to const identifier `%s'\n", l->u.sym->name);
		else
			error("assignment to const location\n");
	r = cast(r, ty);
	if (l->op == FIELD && fieldsize(l->u.field) < 8*l->u.field->type->size)
		if (isunsigned(l->u.field->type))
			r = bitnode(BAND, r,
				constnode(fieldmask(l->u.field), unsignedtype));
		else {
			int n = 8*l->u.field->type->size - fieldsize(l->u.field);
			/*
			 * The value of assignment is the value of the left
			 * operand (sec. 3.3.16), which for bit fields is the
			 * same as extracting the field. When r is a constant,
			 * the explicit left shift below avoids overflow that
			 * would occur if shnode(LSH, ...) were used. The
			 * subsequent shnode(RSH, ...) will be signed and thus
			 * extend the sign bit correctly.
			 */
			if (r->op == CNST+I)
				r = constnode(r->u.v.i<<n, inttype);
			else
				r = shnode(LSH, r, constnode(n, inttype));
			r = shnode(RSH, r, constnode(n, inttype));
		}
	if (isstruct(ty)) {
		if (r->op == RIGHT && r->kids[0] && r->kids[0]->op == CALL+B
		&& isaddrop(l->op)) {
			/*
			 * When x=f() where f returns a struct and x is a simple
			 * address, release the temporary generated to hold f's
			 * return value and use x in its place. The RIGHT tree
			 * assigned to r below returns x's address, and
			 * rvalue(r) hangs that tree under an INDIR+B,
			 * thus identifying the result as an lvalue.
			 */
			r = r->kids[0];	/* CALL+B node */
			release(r->kids[1]->u.sym);
			r = tree(RIGHT, l->type,
				tree(CALL+B, r->kids[0]->type, r->kids[0], l),
				retype(isarray(l->u.sym->type) ? idnode(l->u.sym) : addrof(idnode(l->u.sym)), l->type));
			return rvalue(r);
		}
		return tree(ASGN+B, ty, l, r);
	}
	return tree(op + (isunsigned(ty) ? I : ttob(ty)), ty, l, r);
}

/* binary - usual arithmetic conversions, return target type */
static Type binary(xty, yty) Type xty, yty; {
	if (isdouble(xty) || isdouble(yty))
		return doubletype;
	if (xty == floattype || yty == floattype)
		return floattype;
	if (isunsigned(xty) || isunsigned(yty))
		return unsignedtype;
	return inttype;
}

/* bitnode - construct tree for l [& | ^ %] r */
Tree bitnode(op, l, r) Tree l, r; {
	Type ty = inttype;

	if (isint(l->type) && isint(r->type)) {
 		ty = binary(l->type, r->type);
		l = cast(l, ty);
		r = cast(r, ty);
		if (op != MOD) {
			l = cast(l, unsignedtype);
			r = cast(r, unsignedtype);
		}
	} else
		typeerror(op, l, r);
	if (op == MOD)
		return simplify(op, ty, l, r);
	return cast(simplify(op, unsignedtype, l, r), ty);
}

/* callnode - construct call node to f, return type ty, arguments args */
Tree callnode(f, ty, args) Tree f, args; Type ty; {
	Tree p;

	if (args)
		f = tree(RIGHT, f->type, args, f);
	if (isstruct(ty)) {
		Symbol t1 = temporary(AUTO, unqual(ty));
		if (ty->size == 0)
			error("illegal use of incomplete type `%t'\n", ty);
		p = tree(RIGHT, ty, tree(CALL+B, ty, f, lvalue(idnode(t1))),
			idnode(t1));
	} else {
		Type rty = ty;
		if (isenum(ty))
			rty = unqual(ty)->type;
		else if (isptr(ty))
			rty = unsignedtype;
		p = tree(CALL + widen(rty), rty, f, 0);
		if (isptr(ty))
			p = cast(p, ty);
	}
	return p;
}

/* cmpnode - construct tree for l [< <= >= >] r */
static Tree cmpnode(op, l, r) Tree l, r; {
	Type ty = unsignedtype;

	if (isarith(l->type) && isarith(r->type)) {
		ty = binary(l->type, r->type);
		l = cast(l, ty);
		r = cast(r, ty);
	} else if (compatible(l->type, r->type)) {
		l = cast(l, ty);
		r = cast(r, ty);
	} else
		typeerror(op, l, r);
	return simplify(op + ttob(ty), inttype, l, r);
}

/* compatible - are ty1 & ty2 sans qualifiers pointers to compatible object or incomplete types? */
static int compatible(ty1, ty2) Type ty1, ty2; {
	if (isptr(ty1) && !isfunc(ty1->type) && isptr(ty2) && !isfunc(ty2->type)) {
		do {
			ty1 = unqual(ty1->type);
			ty2 = unqual(ty2->type);
		} while (isptr(ty1) && isptr(ty2));
		return eqtype(ty1, ty2, 0);
	}
	return 0;
}

/* condnode - build a tree for e ? l : r */
Tree condnode(e, l, r) Tree e, l, r; {
	Symbol t1 = 0;
	Type ty = 0, lty = l->type, rty = r->type;
	Tree p;
	
	if (isarith(lty) && isarith(rty)) {
		ty = binary(lty, rty);
		l = cast(l, ty);
		r = cast(r, ty);
	} else if (eqtype(lty, rty, 1)
	|| isptr(lty) && isint(rty) && assign(lty, r))
		ty = unqual(lty);
	else if (isptr(rty) && isint(lty) && assign(rty, l))
		ty = unqual(rty);
	else if (isptr(lty) && isptr(rty)) {
		lty = lty->type;
		rty = rty->type;
		if (unqual(lty) == voidtype || unqual(rty) == voidtype)
			ty = voidtype;
		else if (eqtype(unqual(lty), unqual(rty), 1))
			ty = unqual(lty);
		if (ty) {
			if (isconst(lty) || isconst(rty))
				ty = qual(CONST, ty);
			if (isvolatile(lty) || isvolatile(rty))
				ty = qual(VOLATILE, ty);
			ty = ptr(ty);
		}
	}
	if (ty == 0) {
		typeerror(COND, l, r);
		ty = inttype;
	} else if (e->op == CNST+D || e->op == CNST+F) {
		e = cast(e, doubletype);
		return retype(e->u.v.d != 0.0 ? l : r, ty);
	} else if (generic(e->op) == CNST) {
		e = cast(e, unsignedtype);
		return retype(e->u.v.u ? l : r, ty);
	} else if (ty != voidtype && ty->size > 0) {
		Opcode op = generic(e->op);
		t1 = temporary(REGISTER, ty);
		l = root(asgn(t1, l));
		if (op != AND && op != OR && op != NOT && op != EQ && op != NE
		&&  op !=  LE && op != LT && op !=  GE && op != GT && op != RIGHT
		&& r->op == CNST+I && r->u.v.i == 0
		&& assign(e->type, r)) {
			/* e1?e2:0 => ((t1=e1,t1)?(t1=e2): ) */
			e = tree(RIGHT, ty, asgn(t1, cast(e, ty)), idnode(t1));
			r = 0;
		} else
			r = root(asgn(t1, r));
	}
	p = tree(COND, ty, cond(e), tree(RIGHT, ty, l, r));
	p->u.sym = t1;
	return p;
}

/* constnode - return a tree for a constant n of type ty (int or unsigned) */
Tree constnode(n, ty) unsigned n; Type ty; {
	Tree p;

	if (isarray(ty))
		p = tree(CNST+P, atop(ty), 0, 0);
	else
		p = tree(CNST + ttob(ty), ty, 0, 0);
	p->u.v.u = n;
	return p;
}

/* eqnode - construct tree for l [== !=] r */
Tree eqnode(op, l, r) Tree l, r; {
	if (isint(l->type) && isptr(r->type))
		return eqnode(op, r, l);
	if (isptr(l->type) && isint(r->type)) {
		l = cast(l, unsignedtype);
		r = cast(r, unsignedtype);
		if (r->op != CNST+U || r->u.v.u)
			error("operands of %s have incompatible types\n",
				op == EQ ? "==" : "!=");
		return simplify(op + U, inttype, l, r);
	}
	if (isptr(l->type) && isptr(r->type) && eqtype(unqual(l->type), unqual(r->type), 1)
	||  isptr(l->type) && !isfunc(l->type->type)
	&& (isptr(r->type) &&  unqual(r->type->type) == voidtype)
	||  isptr(r->type) && !isfunc(r->type->type)
	&& (isptr(l->type) &&  unqual(l->type->type) == voidtype)) {
		l = cast(l, unsignedtype);
		r = cast(r, unsignedtype);
		return simplify(op + U, inttype, l, r);
	}
	return cmpnode(op, l, r);
}

/* mulnode - construct tree for l [* /] r */
static Tree mulnode(op, l, r) Tree l, r; {
	Type ty = inttype;

	if (isarith(l->type) && isarith(r->type)) {
		ty = binary(l->type, r->type);
		l = cast(l, ty);
		r = cast(r, ty);
	} else
		typeerror(op, l, r);
	return simplify(op, ty, l, r);
}

/* shnode - construct tree for l [>> <<] r */
Tree shnode(op, l, r) Tree l, r; {
	Type ty = inttype;

	if (isint(l->type) && isint(r->type)) {
		ty = promote(l->type);
		l = cast(l, ty);
		r = cast(r, inttype);
	} else
		typeerror(op, l, r);
	return simplify(op, ty, l, r);
}

/* subnode - construct tree for l - r */
static Tree subnode(op, l, r) Tree l, r; {
	int n;
	Type ty = inttype;

	if (isarith(l->type) && isarith(r->type)) {
		ty = binary(l->type, r->type);
		l = cast(l, ty);
		r = cast(r, ty);
	} else if (isptr(l->type) && !isfunc(l->type->type) && isint(r->type)) {
		ty = unqual(l->type);
		if ((n = ty->type->size) == 0)
			error("unknown size of type `%t'\n", ty->type);
		r = cast(r, promote(r->type));
		if (n > 1)
			r = mulnode(MUL, constnode(n, inttype), r);
		return simplify(SUB+P, ty, l, r);
	} else if (compatible(l->type, r->type)) {
		if ((n = deref(l->type)->size) == 0)
			error("unknown size of type `%t'\n", deref(l->type));
		l = simplify(SUB+U, unsignedtype, cast(l, unsignedtype),
			cast(r, unsignedtype));
		return simplify(DIV+I, inttype, cast(l, inttype), constnode(n, inttype));
	} else
		typeerror(op, l, r);
	return simplify(op, ty, l, r);
}

/* typeerror - issue "operands of op have illegal types `l' and `r'" */
void typeerror(op, l, r) Tree l, r; {
	int i;
	static struct { Opcode op; char *name; } ops[] = {
		ASGN, "=",	INDIR, "*",	NEG,  "-",
		ADD,  "+",	SUB,   "-",	LSH,  "<<",
		MOD,  "%",	RSH,   ">>",	BAND, "&",
		BCOM, "~",	BOR,   "|",	BXOR, "^",
		DIV,  "/",	MUL,   "*",	EQ,   "==",
		GE,   ">=",	GT,    ">",	LE,   "<=",
		LT,   "<",	NE,    "!=",	AND,  "&&",
		NOT,  "!",	OR,    "||",	COND, "?:",
		0, 0
	};

	op = generic(op);
	for (i = 0; ops[i].op; i++)
		if (op == ops[i].op)
			break;
	assert(ops[i].name);
	if (r)
		error("operands of %s have illegal types `%t' and `%t'\n",
			ops[i].name, l->type, r->type);
	else
		error("operand of unary %s has illegal type `%t'\n", ops[i].name,
			l->type);
}
