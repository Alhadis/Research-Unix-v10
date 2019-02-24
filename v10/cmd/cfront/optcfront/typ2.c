/*ident	"@(#)ctrans:src/typ2.c	1.3" */
/**************************************************************************

	C++ source for cfront, the C++ compiler front-end
	written in the computer science research center of Bell Labs

	Copyright (c) 1984 AT&T, Inc. All Rights Reserved
	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T, INC.

typ2.c:

***************************************************************************/
 
#include "cfront.h"
#include "size.h"

extern int chars_in_largest;
int largest_int;

void typ_init()
{	
	chars_in_largest = strlen(LARGEST_INT);
	largest_int = int(str_to_long(LARGEST_INT));

	defa_type = new basetype(INT,0);	// note defa_type!=int_type
	int_type = new basetype(INT,0);		// but they both represent `int'

	PERM(int_type); int_type->defined = DEFINED ;
	PERM(defa_type); defa_type->defined = DEFINED ;

	moe_type = new basetype(INT,0);
	PERM(moe_type); moe_type->defined = DEFINED ;
	moe_type->b_const = 1;
	moe_type->check(0);

	uint_type = new basetype(INT,0);
	PERM(uint_type); uint_type->defined = DEFINED ;
	uint_type->type_adj(UNSIGNED);
	uint_type->check(0);

	long_type = new basetype(LONG,0);
	PERM(long_type); long_type->defined = DEFINED ;
	long_type->check(0);

	ulong_type = new basetype(LONG,0);
	PERM(ulong_type); ulong_type->defined = DEFINED ;
	ulong_type->type_adj(UNSIGNED);
	ulong_type->check(0);

	short_type = new basetype(SHORT,0);
	PERM(short_type); short_type->defined = DEFINED ;
	short_type->check(0);

	ushort_type = new basetype(SHORT,0);
	PERM(ushort_type); ushort_type->defined = DEFINED ;
	ushort_type->type_adj(UNSIGNED);
	ushort_type->check(0);

	float_type = new basetype(FLOAT,0);
	PERM(float_type); float_type->defined = DEFINED ;

	double_type = new basetype(DOUBLE,0);
	PERM(double_type); double_type->defined = DEFINED ;

	ldouble_type = new basetype(LDOUBLE,0);
	PERM(ldouble_type); ldouble_type->defined = DEFINED ;

	zero_type = new basetype(ZTYPE,0);
	PERM(zero_type); zero_type->defined = DEFINED ;
	zero->tp = zero_type;

	void_type = new basetype(VOID,0);
	PERM(void_type); void_type->defined = DEFINED ;

	char_type = new basetype(CHAR,0);
	PERM(char_type); char_type->defined = DEFINED ;

	uchar_type = new basetype(CHAR,0);
	PERM(uchar_type); uchar_type->defined = DEFINED ;
	uchar_type->type_adj(UNSIGNED);
	uchar_type->check(0);

	Pchar_type = char_type->addrof();
	PERM(Pchar_type); Pchar_type->defined = DEFINED ;

	Pint_type = int_type->addrof();
	PERM(Pint_type); Pint_type->defined = DEFINED ;

	Pvoid_type = void_type->addrof();
	PERM(Pvoid_type); Pvoid_type->defined = DEFINED ;

	Pfctvec_type = new fct(int_type,0,0);	// must be last, see basetype::normalize()
	Pfctvec_type = Pfctvec_type->addrof();
	Pfctvec_type = Pfctvec_type->addrof();
	PERM(Pfctvec_type); Pfctvec_type->defined = DEFINED ;

	gtbl = new table(GTBLSIZE,0,0);
	gtbl->t_name = new name("global");
	ptbl = new table(CTBLSIZE,0,0);
	ptbl->t_name = new name("ptbl");
}

bit enum_promote;

Pbase basetype::arit_conv(Pbase t)
/*
	perform the "usual arithmetic conversions" C ref Manual 6.6
	on "this" op "t"
	"this" and "t" are integral or floating
	"t" may be 0
*/
{
	while (base == TYPE) this = Pbase(Pbase(this)->b_name->tp);
// error('d', "arit_conv: this: %k %d %t %d", base, base, this, this );

	bit l;
	bit u;
	bit f;
	bit l1 = (base == LONG);
	bit u1 = b_unsigned;
	bit f1 = (base==FLOAT || base==DOUBLE || base==LDOUBLE);
	if (t) {
		while (t->base == TYPE) t = Pbase(Pbase(t)->b_name->tp);
// error('d', "arit_conv: t: %k %d %t %d", t->base, t->base, t, t );

		bit l2 = (t->base == LONG);
		bit u2 = t->b_unsigned;
		bit f2 = (t->base==FLOAT || t->base==DOUBLE || base==LDOUBLE);
		l = l1 || l2;
		u = u1 || u2;
		f = f1 || f2;
	}
	else {
		l = l1;
		u = u1;
		f = f1;
	}

	if (f) {
		if (base==LDOUBLE || (t && t->base==LDOUBLE)) return ldouble_type;
		if (base==DOUBLE || (t && t->base==DOUBLE)) return double_type;
		return float_type;
	}
	if (l & u) return ulong_type;
	if (l & !u) return long_type;
	if (u) {
		if (base==INT || (t && t->base==INT)) return uint_type;
		if (SZ_SHORT==SZ_INT) // ANSIism
			if (base==SHORT || (t && t->base==SHORT)) return uint_type;
		return int_type;
	}

	if (t && t->base == EOBJ && base == EOBJ) enum_promote = 1;
	return int_type;
}

bit vec_const = 0;
bit fct_const = 0;

bit type::tconst()
/*
	is this type a constant
*/
{
	Ptype t = this;
	vec_const = 0;
	fct_const = 0;
//error('d',"tconst %t",t);
xxx:
	switch (t->base) {
	case TYPE:
		if (Pbase(t)->b_const) return 1;
		t = Pbase(t)->b_name->tp;
		goto xxx;
	case VEC:
		vec_const = 1;
		return 1;
	case PTR:
	case RPTR:
		return Pptr(t)->rdo;
	case FCT:
	case OVERLOAD:
		fct_const = 1;
		return 1;
	default:
		return Pbase(t)->b_const;
	}
}

TOK type::set_const(bit mode)
/*
	make someting a constant or variable, return old status
*/
{
	Ptype t = this;
	int m;
xxx:
	switch (t->base) {
	case TYPE:
		m = Pbase(t)->b_const;
		Pbase(t)->b_const = mode;
		t = Pbase(t)->b_name->tp;
		goto xxx;
	case ANY:
	case RPTR:
	case VEC:
		return t->base;		// constant by definition
	case PTR:
		m = Pptr(t)->rdo;
		Pptr(t)->rdo = mode;
		return m;
	default:
		m = Pbase(t)->b_const;
		Pbase(t)->b_const = mode;
		return m;
	}
}

Pptr type::is_ref()
{
	Ptype t = this;
xxx:
	switch (t->base) {
	case TYPE:	t = Pbase(t)->b_name->tp; goto xxx;
	case RPTR:	return Pptr(t);
	default:	return 0;
	}
}

Pclass Mptr;

Pptr type::is_ptr()
{
	Ptype t = this;
xxx:
	switch (t->base) {
	case TYPE:	t = Pbase(t)->b_name->tp; goto xxx;
	case PTR:
	case VEC:	Mptr = Pptr(t)->memof;
			return Pptr(t);
	default:	return 0;
	}
}

Pptr type::is_ptr_or_ref()
{
	Ptype t = this;
xxx:
	switch (t->base) {
	case TYPE:	t = Pbase(t)->b_name->tp; goto xxx;
	case PTR:
	case RPTR:
	case VEC:	Mptr = Pptr(t)->memof;
			return Pptr(t);
	default:	return 0;
	}
}

int type::align()
{
	Ptype t = this;
xx:
/*fprintf(stderr,"align %d %d\n",t,t->base);*/
	switch (t->base) {
	case TYPE:	t = Pbase(t)->b_name->tp; goto xx;
	case COBJ:	t = Pbase(t)->b_name->tp; goto xx;
	case VEC:	t = Pvec(t)->typ; goto xx;
	case ANY:	return 1;
	case CHAR:	return AL_CHAR;
	case SHORT:	return AL_SHORT;
	case INT:	return AL_INT;
	case LONG:	return AL_LONG;
	case FLOAT:	return AL_FLOAT;
	case DOUBLE:	return AL_DOUBLE;
	case LDOUBLE:	return AL_LDOUBLE;
	case PTR:
	case RPTR:	return AL_WPTR;
	case CLASS:	return Pclass(t)->obj_align;
	case ENUM:
	case EOBJ:	return AL_INT;
	case VOID:	error("illegal use of void"); return AL_INT;
	default:	error('i',"(%d,%k)->type::align",t,t->base);
	}
}

bit fake_sizeof;

int type::tsizeof(int ptmc)
/*
	the sizeof type operator
	return the size in bytes of the types representation
*/
{
	Ptype t = this;
zx:
//error('d',"zx %t %d",t,t->base);
	if (t == 0) error('i',"typ.tsizeof(t==0)");
	switch (t->base) {
	case TYPE:
		t = Pbase(t)->b_name->tp;
		goto zx;
	case COBJ:
		t = Pbase(t)->b_name->tp;
		if (t == 0) return 0;	// ``fake'' generated classes: _Sdd
		goto zx;

	case ANY:	return 1;
	case VOID:	return 0;
	case ZTYPE:	return SZ_WPTR;	/* assume pointer */
	case CHAR:	return SZ_CHAR;
	case SHORT:	return SZ_SHORT;
	case INT:	return SZ_INT;
	case LONG:	return SZ_LONG;
	case FLOAT:	return SZ_FLOAT;
	case DOUBLE:	return SZ_DOUBLE;
	case LDOUBLE:	return SZ_LDOUBLE;

	case VEC:
		{	Pvec v = Pvec(t);
			if (v->size == 0) {
				if (fake_sizeof == 0) error('w',"sizeof array with undeclared dimension");
				return SZ_WPTR;	// vector argument has sizeof ptr
			}
			return v->size * v->typ->tsizeof();
		}
	case RPTR:	
	case PTR:
	{	
		int k = (Pptr(t)->memof && !ptmc)?sizeof(short)+sizeof(short):0;
		t = Pptr(t)->typ;
	xxx:
		switch (t->base) {
		default:	return SZ_WPTR;
		case CHAR:	return SZ_BPTR;
		case FCT:	return SZ_WPTR+k;
		case TYPE:	t = Pbase(t)->b_name->tp; goto xxx;
		}
	}
	case FIELD:
		error("sizeof(field)");
		return Pbase(t)->b_bits/BI_IN_BYTE+1;
	case FCT:
		error("sizeof(function)");
		return 0;

	case CLASS:	
	{
                Pclass cl = Pclass(t);
		if ((cl->defined&(DEFINED|SIMPLIFIED)) == 0) {
			error("%tU, size not known",cl);
			return SZ_INT;
		}
	 	if (cl->c_body == 1)	// detect first allocation or sizeof
	 		cl->dcl_print(0);
		return cl->obj_size;
	}

	case EOBJ:
	case ENUM:	return SZ_INT;

	default:	return 0;	// deref can be called for any type
			//error('i',"sizeof(%d)",t->base);
	}
}

bit type::vec_type()
{
	Ptype t = this;
xx:
	switch (t->base) {
	case ANY:
	case VEC:
	case PTR:
	case RPTR:	return 1;
	case TYPE:	t = Pbase(t)->b_name->tp; goto xx;
	default:	return 0;
	}
}

int ref_initializer;

Ptype type::deref()
/*	index==1:	*p
	index==0:	p[expr]
*/
{
//error('d',"%t -> deref() refd %d",this,ref_initializer);
	Ptype t = this;
xx:
	switch (t->base) {
	case TYPE:
		t = Pbase(t)->b_name->tp;
		goto xx;
	case PTR:
	case RPTR:
	case VEC:
	{	if (t == Pvoid_type) error("void* dereferenced");
		Ptype tt = t = Pvec(t)->typ;
		if (ref_initializer == 0) {
			while (tt->base == TYPE) tt = Pbase(tt)->b_name->tp;
			if (tt->base == COBJ) { 
                        	tt = Pbase(tt)->b_name->tp;
                        	if (tt && Pclass(tt)->defined&(DEFINED|SIMPLIFIED))
                                	(void) t->tsizeof();
                        }
		}
		// no break
	}
	case ANY:
		return t;
	default:
		error("nonP dereferenced");
		return any_type;
	}
}

Pfct type::memptr()
// is ``this'' a pointer to member function
{
	Ptype t = this;
	while (t->base == TYPE) t = Pbase(t)->b_name->tp;
	if (t->base != PTR || Pptr(t)->memof==0) return 0;

	t = Pptr(t)->typ;
	while (t->base == TYPE) t = Pbase(t)->b_name->tp;
	return (t->base == FCT) ? Pfct(t) : 0;
}
