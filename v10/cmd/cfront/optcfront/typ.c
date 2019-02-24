/*ident	"@(#)ctrans:src/typ.c	1.5" */
/**************************************************************************

	C++ source for cfront, the C++ compiler front-end
	written in the computer science research center of Bell Labs

	Copyright (c) 1984 AT&T, Inc. All Rights Reserved
	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T, INC.

typ.c:


***************************************************************************/
 
#include "cfront.h"
#include "size.h"

Pbase short_type;
Pbase int_type;
Pbase char_type;
Pbase long_type;

Pbase uchar_type;
Pbase ushort_type;
Pbase uint_type;
Pbase ulong_type;

Pbase zero_type;
Pbase float_type;
Pbase double_type;
Pbase ldouble_type;
Pbase void_type;
Pbase any_type;

Ptype Pint_type;
Ptype Pchar_type;
Ptype Pvoid_type;
Ptype Pfctvec_type;

Ptable gtbl;
Ptable ptbl;

Pname Cdcl;
Pstmt Cstmt;

bit new_type;

void echeck(Ptype t1, Ptype t2)
/*
	t1 is an enum, t2 is assigned to it
*/
{
	if (t1 == t2) return;

//error('d',"echeck(%t,%t) %d %d",t1,t2,t1->base,t2->base);
//error('d',"se %d promote: %d",suppress_error, enum_promote);
	if (t1->base==EOBJ
	&& t2->base==EOBJ
	&& Pbase(t1)->b_name->tp == Pbase(t2)->b_name->tp) return;

	if (enum_promote) return;
	error(strict_opt?0:'w',"%t assigned to %t (anachronism)",t2,t1);
}


Ptype np_promote(TOK oper, TOK r1, TOK r2, Ptype t1, Ptype t2, TOK p)
/*
	an arithmetic operator "oper" is applied to "t1" and "t2",
	types t1 and t2 has been checked and belongs to catagories
	"r1" and "r2", respectively:
		A	ANY
		Z	ZERO
		I	CHAR, SHORT, INT, LONG, FIELD, or EOBJ
		F	FLOAT DOUBLE LDOUBLE
		P	PTR (to something) or VEC (of something)
	test for compatability of the operands,
	if (p) return the promoted result type
*/
{
	if (r2 == 'A') return t1;

//error('d',"promote(%t,%t,%k)",t1,t2,oper);

	switch (r1) {
	case 'A':	return t2;
	case 'Z':
		switch (r2) {
		case 'Z':		return int_type;
		case 'I':
		case 'F':
			if(oper==DEREF) return any_type;
			return (p) ? Pbase(t2)->arit_conv(0) : 0;
		case 'P': switch (oper) {
			case PLUS:
			case ASPLUS:	if(t2!=Pvoid_type) break;
			default:	return any_type;
			}
			return t2;
		case FCT:	error("zero%kF",oper); return any_type;
		default:	error('i',"zero(%d)",r2);
		}
	case 'I':
		switch (r2) {
		case 'Z': t2 = 0;
		case 'I':
		case 'F': 
			if(oper==DEREF) return any_type;
			return (p) ? Pbase(t1)->arit_conv(Pbase(t2)) : 0;
		case 'P': switch (oper) {
			case PLUS:
			case ASPLUS:	if(t2!=Pvoid_type) break;
			default:	error("int%kP",oper); return any_type;
			}
			return t2;
		case FCT: error("int%kF",oper); return any_type;
		default: error('i',"int(%d)",r2); return any_type;
		}
	case 'F':
		switch (r2) {
		case 'Z': t2 = 0;
		case 'I':
		case 'F': 
			if(oper==DEREF) return any_type;
			return (p) ? Pbase(t1)->arit_conv(Pbase(t2)) : 0;
		case 'P': error("float%kP",oper); return any_type;
		case FCT: error("float%kF",oper); return any_type;
		default: error('i',"float(%d)",r2); return any_type;
		}
	case 'P':
		switch (r2) {
		case 'Z': return t1;
		case 'I':
			switch (oper) {
			case PLUS:
			case MINUS:
			case ASPLUS:
			case ASMINUS:
				if (t1->check(Pvoid_type,0)==0) {
					return any_type;
				}
				break;
			default: error("P%k int",oper); return any_type;
			}
			return t1;
		case 'F': error("P%k float",oper); return any_type;
		case 'P':
			if (t1->check(t2,ASSIGN)) {
				switch (oper) {
				case EQ:
				case NE:
				case LE:
				case GE:
				case GT:
				case LT:
				case QUEST:
					if (t2->check(t1,ASSIGN) == 0) goto zz;
				}
				error("T mismatch:%t %k%t",t1,oper,t2);
				return any_type;
			}
			zz:
			switch (oper) {
			case MINUS:	
				return (t2!=Pvoid_type) ? int_type : any_type;
			case ASMINUS:	error("P -=P"); return any_type;
			case PLUS:	error("P +P"); return any_type;
			case ASPLUS:	error("P +=P"); return any_type;
			case LS:
			case RS:	return any_type;
			default:	return (t1!=Pvoid_type) ? t1 : any_type;
			}
		case FCT:	return t1;
		default:	error('i',"P(%d)",r2);
		}
	case FCT:
		if(oper == QUEST) {
			switch (r2) {
			case 'Z':
				return any_type;
			case 'P':
				return t2;
			case 'I':
			case 'F':
				error("F%k%t",oper,t2);
			default:
				return t1;
			}
		}
		error("F%k%t",oper,t2);
		return any_type;
	default:
		error('i',"np_promote(%d,%d)",r1,r2);
	}
}

TOK type::kind(TOK oper, TOK v)
/*	v ==	'I'	integral
		'N'	numeric
		'P'	numeric or pointer
*/
{
	Ptype t = this;
	if (this == 0) error('i',"type::kind(): this==0");
xx:
	switch (t->base) {
	case ANY:	return 'A';
	case ZTYPE:	return 'Z';
	case FIELD:
	case CHAR:
	case SHORT:
	case INT:
	case LONG:
	case EOBJ:	return 'I';
	case FLOAT:
	case LDOUBLE:
	case DOUBLE:	if (v == 'I') error("float operand for %k",oper);	return 'F';
	case VEC:
	case PTR:	if (v != 'P') error("P operand for %k",oper);
			switch (oper) {
			case INCR:
			case DECR:
			case MINUS:
			case PLUS:
			case ASMINUS:
			case ASPLUS:
				if (t->base==PTR
				&& (Pptr(t)->memof || Pptr(t)->typ->base==FCT))
					error("%t operand of%k",this,oper);
				else
					Pptr(t)->typ->tsizeof(); // get increment
				break;
			default:
				if (t->base==PTR
				&& (Pptr(t)->memof || Pptr(t)->typ->base==FCT))
					error("%t operand of%k",this,oper);
			case ANDAND:
			case OROR:
			case ASSIGN:
			case NE:
			case EQ:
			case IF:
			case WHILE:
			case DO:
			case FOR:
			case QUEST:
			case NOT:
					break;
			}
			return 'P';
	case RPTR:	error("R operand for %k",oper);			return 'A';
	case TYPE:	t = Pbase(t)->b_name->tp;			goto xx;
	case FCT:	if (v != 'P') error("F operand for %k",oper);	return FCT;
	case OVERLOAD:	error("overloaded operand for %k",oper);	return 'A';
	case CLASS:
	case ENUM:	error("%k operand for %k",base,oper);		return 'A';
	default:	error("%t operand for %k",this,oper);		return 'A';
	}
}

void type::dcl(Ptable tbl)
/*
	go through the type (list) and
	(1) evaluate vector dimensions
	(2) evaluate field sizes
	(3) lookup struct tags, etc.
	(4) handle implicit tag declarations
*/
{
static arg_fudge;

	Ptype t = this;

	if (this == 0) error('i',"T::dcl(this==0)");
	if (tbl->base != TABLE) error('i',"T::dcl(%d)",tbl->base);

xx:
//error('d',"type::dcl %k",t->base);
	switch (t->base) {
	case TYPE:
		t = Pbase(t)->b_name->tp;
		goto xx;
	case PTR:
	case RPTR:
	{	Pptr p = Pptr(t);
		t = p->typ;
		if (t->base == TYPE) {
			Ptype tt = Pbase(t)->b_name->tp;
			if (tt->base == FCT) p->typ = tt;
			return;
		}
		goto xx;
	}

	case VEC:
	{	Pvec v = Pvec(t);
		Pexpr e = v->dim;
		if (e) {
			Ptype et;
			v->dim = e = e->typ(tbl);
			et = e->tp;
			if (et->integral(0) == 'A') {
				error("UN in array dimension");
			}
			else {
				long i;
				Neval = 0;
				i = e->eval();
				if (Neval == 0) {
					if (largest_int<i)
						error("array dimension too large");
					v->size = int(i);

				if ( lcl_tbl == 0 )

					DEL(v->dim);
					v->dim = 0;
				}

				if (new_type) {
					if (Neval)
						;
					else if (i == 0)
						v->dim = zero;
					else if (i < 0) {
						error("negative array dimension");
						i = 1;
					}
				}
				else {
					if (Neval)
						error("%s",Neval);
					else if (i == 0)
						error('w',"array dimension == 0");
					else if (i < 0) {
						error("negative array dimension");
						i = 1;
					}
				}
			}
		}
		t = v->typ;
	llx:
		switch (t->base) {
		case TYPE:
			t = Pbase(t)->b_name->tp;
			goto llx;
		case FCT:
			v->typ = t;
			break;
		case VEC:				
			if (Pvec(t)->dim==0 && Pvec(t)->size==0) error("null dimension (something like [][] seen)");
			if (arg_fudge) {
				v->base = PTR;	// X[12][10] ==> X(*)[10]
				Pptr(v)->rdo = 0;
				Pptr(v)->memof = 0;
			}
		}
		goto xx;
	}

	case FCT:
	{	Pfct f = Pfct(t);
void dargs(Pname, Pfct, Ptable);
		if (f->argtype) dargs(0,f,tbl);
		for (Pname n=f->argtype; n; n = n->n_list) {
			arg_fudge++;
			n->tp->dcl(tbl);
			arg_fudge--;
		}
		Pname cn = f->returns->is_cl_obj();
		if (cn && Pclass(cn->tp)->has_itor())
			make_res(f);
		else if (f->f_this == 0)
			f->f_args = f->argtype;
		t = f->returns;
		goto xx;
	}

	case FIELD:
	{	Pbase f = Pbase(t);
		Pexpr e = Pexpr(f->b_name);
		long i;
		Ptype et;
		e = e->typ(tbl);
		f->b_name = Pname(e);
		et = e->tp;
		if (et->integral(0) == 'A') {
			error("UN in field size");
			i = 1;
		}
		else {
			Neval = 0;
			i = e->eval();
			if (Neval)
				error("%s",Neval);
			else if (i < 0) {
				error("negative field size");
				i = 1;
			}
			else if (f->b_fieldtype->tsizeof()*BI_IN_BYTE < i)
				error("field size > sizeof(%t)",f->b_fieldtype);
			DEL(e);
		}
		f->b_bits = int(i);
		f->b_name = 0;
		break;
	}
	}
}

bit vrp_equiv;		// vector == pointer equivalence used in check()
bit const_problem;	// types differs only in const
int Vcheckerror;

bit type::check(Ptype t, TOK oper)
/*
	check if "this" can be combined with "t" by the operator "oper"

	used for check of
			assignment types		(oper==ASSIGN)
			declaration compatability	(oper==0)
                        parameterized type formals      (oper==255)
                          as for (oper==0) but
                          special checking for ANY types
			argument types			(oper==ARG)
			return types			(oper==RETURN)
			overloaded function name match	(oper==OVERLOAD)
			overloaded function coercion	(oper==COERCE)
			virtual function match		(oper==VIRTUAL)

	NOT for arithmetic operators

	return 1 if the check failed

	checking of const const* and *const is a mess
*/
{
        const unsigned int strict_any_check = (oper == 255);

	Ptype t1 = this;
	Ptype t2 = t;
	Ptype tt1 = this;
	Ptype tt2 = t;
	int cnst1 = 0;
	int cnst2 = 0;
	TOK b1, b2;
	bit first = 1;
	TOK r;
	int vv;
	int ptr_count = 0;
	int fct_seen = 0;
	int over;
	Pptr p1 = 0;
	Pptr p2 = 0;
	int p_count = 0;

        if (strict_any_check) oper = 0;

//error('d',"check %k %t %t",oper,t1,t2);
	if (t1==0 || t2==0) error('i',"check(%p,%p,%d)",t1,t2,oper);

	if (oper==VIRTUAL) {
		vv = 1;
		Vcheckerror = 0;
		oper = 0;
	}
	else
		vv = 0;

	if (oper == OVERLOAD) {
		over = 1;
		oper = 0;
	}
	else
		over = 0;

	const_problem = 0;

	while (t1 && t2) {
	top:
//error('d',"top: %t (%d) %t (%d)",t1,t1->base,t2,t2->base);
		if (t1 == t2) {
			if (cnst1==cnst2) return 0;
			if (oper) {
//error('d',"oper %d cnst1 %d cnst2 %d ptr %d",oper,cnst1,cnst2,tt1->is_ptr());
				if (tt1 = tt1->is_ptr()) {

					// const* = int*
					if (cnst2<cnst1) return 0;

					// int* = int *const
					if (cnst2==1 && tt2->tconst()) {
						// check for int* = const *const
						tt2 = tt2->is_ptr();
						if (tt2->tconst()) return 1;
						return 0;
					}
				}
				else {	// int = const allowed
					if (oper==ARG || cnst1<cnst2) return 0;
				}
			}
			else {
				if (p_count) {
					int pr1 = p1->rdo ? 1 : 0;
					int pr2 = p2->rdo ? 1 : 0;
					if (pr1+cnst1==pr2+cnst2)
						return 0;
				}
			//	const_problem = 1;
			}

			const_problem = 1;
			return 1;
		}

                if  ((t1->base == ANY || t2->base == ANY)) {
                          if (! strict_any_check) return 0;
                          // Perform the check for strict_any, ie. the
                          return ((t1 == t2) ? 0 : 1) ;
                }

		b1 = t1->base;
		switch (b1) {
		case TYPE:
			if (Pbase(t1)->b_const) cnst1++;
			t1 = Pbase(t1)->b_name->tp;
			goto top;
		}

		b2 = t2->base;
		switch (b2) {
		case TYPE:
			if (Pbase(t2)->b_const) cnst2++;
			t2 = Pbase(t2)->b_name->tp;
			goto top;
		}

//error('d',"oper %k b1 %k b2 %k",oper,b1,b2);
		if (b1 != b2) {
			switch (b1) {
			case PTR:
				switch (b2) {
				case VEC:
					if (ptr_count) return 1;
                                	// ptr/vec equivalence does not
                                	// apply to declaration matching
					t1 = Pptr(t1)->typ;
					t2 = Pvec(t2)->typ;
					if (oper == 0 && over==0) return 1;
					ptr_count++;
					first = 0;
					goto top;
				case FCT:
					t1 = Pptr(t1)->typ;
					if (t1->base!=VOID) 
						if (first==0 || t1->base!=b2) return 1;
					first = 0;
					goto top;
				}
				first = 0;
				break;

                        case FCT:
                                switch( b2 ) {
                                case PTR:
                                        t2 = Pptr(t2)->typ;
                                        if (t1->base!=VOID
					&& (first==0||t2->base!=b1)) return 1;
                                        first = 0;
                                        goto top;
                                }
                                first = 0;
                                break;

			case VEC:
				switch (b2) {
				case PTR:
					if (ptr_count) return 1;
					t1 = Pvec(t1)->typ;
					t2 = Pptr(t2)->typ;
					switch (oper) {
					case ARG:
					case ASSIGN:
					case COERCE:
						break;
					case 0:
						if (over) break;
					default:
						return 1;
					}
					ptr_count++;
					first = 0;
					goto top;
				}
				first = 0;
				break;
			}
			goto base_check; 
		}

		switch (b1) {
		case VEC:
//error('d',"vec %k %d %d",oper,Pvec(t1)->size,Pvec(t2)->size);
			if (first==0 && Pvec(t1)->size!=Pvec(t2)->size) return 1;
			first = 0;
			t1 = Pvec(t1)->typ;
			t2 = Pvec(t2)->typ;
			ptr_count++;
			break;

		case PTR:
		case RPTR:
			first = 0;
			p1 = Pptr(t1);
			p2 = Pptr(t2);
			p_count++;

			if (p1->memof != p2->memof) {

				if(p1->memof!=0 && p2->memof!=0
				    && p1->memof->baseof(p2->memof)==0)
					return 1;

				int flag1=0,flag2=0;
				t1 = p1->typ;
				t2 = p2->typ;
				while (t1->base == TYPE) {
					flag1++;
					t1 = Pbase(t1)->b_name->tp;
				}
				while (t2->base == TYPE) {
					flag2++;
					t2 = Pbase(t2)->b_name->tp;
				}
				if (t1 != t2 || (!flag1 && !flag2)) {
					if (p1->memof==0
					|| p2->memof==0
					|| p1->memof->baseof(p2->memof)==0)
						return 1;
			    		Nstd++;
				}
			}

			t1 = p1->typ;
			t2 = p2->typ;
			ptr_count++;

			if (oper==0) {
				int pr1 = p1->rdo ? 1 : 0;
				int pr2 = p2->rdo ? 1 : 0;
				if (pr1+cnst1!=pr2+cnst2
				&& cnst1+Pbase(t1)->b_const!=cnst2+Pbase(t2)->b_const) {
					// const_problem only if nothing
					// more serious is wrong
					if (t1->check(t2,0) == 0) const_problem = 1;
					return 1;
				}

				if (b1==RPTR && t1->tconst()!=t2->tconst())
					const_problem = 1;
			}
			break;

		case FCT:
			first = 0;
		{	Pfct f1 = Pfct(t1);
			Pfct f2 = Pfct(t2);
			Pname a1 = f1->argtype;
			Pname a2 = f2->argtype;
			TOK k1 = f1->nargs_known;
			TOK k2 = f2->nargs_known;
			int n1 = f1->nargs;
			int n2 = f2->nargs;
//error('d',"f1%t f2%t",f1,f2);
			if (f1->memof != f2->memof) {
				//if (f1->memof==0 && f2->f_this==0)	//SSS
				if (f1->memof==0 && f2->f_static)
					goto sss;
				if (vv == 0)	// match even if private base class
					if (f1->memof==0
					|| f2->memof==0
					|| f1->memof->baseof(f2->memof)==0) return 1;
				Nstd++;
				sss:;	//SSS
			}

			if (k1 != k2) return 1;

			if (n1!=n2 && k1 && k2) {
				goto aaa;
			}
			else if (a1 && a2) {
				int i = 0;
				while (a1 && a2) {
					i++;
					if (a1->tp->check(a2->tp,over?OVERLOAD:0)) return 1;
					a1 = a1->n_list;
					a2 = a2->n_list;
				}
				if (a1 || a2) goto aaa;
			}
			else if (a1 || a2) {
			aaa:
//error('d',"aaa k1 %d k2 %d",k1,k2);
				if (k1 == ELLIPSIS) {
					switch (oper) {
					case 0:
						if (a2 && k2==0) break;
						return 1;
					case ASSIGN:
						if (a2 && k2==0) break;
						return 1;
					case ARG:
						if (a1) return 1;
						break;
				//	case OVERLOAD:
					case COERCE:
						return 1;
					}
				}
				else if (k2 == ELLIPSIS) {
					return 1;
				}
				else if (k1 || k2) {
					return 1;
				}
			}

			t1 = f1->returns;
			t2 = f2->returns;
			fct_seen = 1;

			switch (oper) {	//CCC
			case 0:
				if (f1->f_const!=f2->f_const) {
					if (t1->check(t2,0)==0) const_problem = 1;
					return 1;
				//	if (vv == 0) return 1;
				//	Vcheckerror = 1;
				}
				break;
			default:	// really pointer to function
				if (f1->f_const && f2->f_const==0) return 1;
			}


			if (vv && t1->check(t2,0)) { Vcheckerror = 1; return 1; }
		}
			break;

		case FIELD:
			goto field_check;
		case CHAR:
		case SHORT:
		case INT:
		case LONG:
			goto int_check;
		case FLOAT:
		case DOUBLE:
		case LDOUBLE:
			goto float_check;
		case EOBJ:
			goto enum_check;
		case COBJ:
			goto cla_check;
		case ZTYPE:
		case VOID:
			return 0;
		default:
			error('i',"T::check(o=%d %d %d)",oper,b1,b2);
		}
	}

	if (t1 || t2) {
		const_problem = 0;	// not a problem: the type itself is bad
		return 1;
	}
	return 0;

field_check:
	switch (oper) {
	case 0:
	case ARG:
		error('i',"check field?");
	}
	return 0;

enum_check:
//error('d',"enum check %t %t",t1,t2);
	if (Pbase(t1)->b_name->tp != Pbase(t2)->b_name->tp) goto base_check;
	goto const_check;

float_check:
	if (first==0 && b1!=b2 && b2!=ZTYPE) return 1;
	// no break

int_check:
//error('d',"int_check");
	if (Pbase(t1)->b_unsigned != Pbase(t2)->b_unsigned) {
		if (first == 0) return 1;
		if (oper /*&& oper!=OVERLOAD*/)
			Nstd++;
		else
			return 1;
	}
	// no break

const_check:
//error('d',"const_check %t (%d) %t (%d)",t1,t1->tconst(),t2,t2->tconst());
	if (oper==0) {
//error('d',"oper==0: t1 %t t2 %t cnst1 %d cnst2 %d",t1,t2,cnst1,cnst2);
		if (t1->tconst()+cnst1!=t2->tconst()+cnst2) {
			const_problem = 1;
			return 1;
		}
	}
	else if (first==0) {
		if (t1->tconst()+cnst1==0 && t2->tconst()+cnst2) {
//error('d',"t1 %t t2 %t cnst1 %d cnst2 %d",t1,t2,cnst1,cnst2);
//error('d',"tt1 %t %d cnst1 %d cnst2 %d",tt1,tt1->is_ptr(),cnst1,cnst2);
//error('d',"tt2 %t",tt2);
			if (tt1->is_ptr()) {
				if (fct_const || vec_const) cnst2--;

				// const* = int*
				if (cnst2-tt2->tconst()<cnst1-tt1->tconst()) return 0;

				// int* = *const
				//if (cnst2==1 && tt2->tconst()) return 0;

				// const T* = const T*
				if (t2->tconst()+cnst2==t1->tconst()+cnst1) return 0;
			}
			else {	// int = const allowed
				if (cnst1<cnst2) return 0;
			}
			const_problem = 1;
			return 1;
		}
		else {	// const* vs int *const
/*
 //error('d',"t1 %t cnst1 %d t2 %t cnst2 %d",t1,cnst1,t2,cnst2);
			if (tt1->is_ptr()) {
				int tt1c = tt1->tconst();
				int tt2c = tt2->tconst() - fct_const - vec_const;
 //error('d',"tt1c %d tt2c %d",tt1c,tt2c);
				if (tt1c<tt2c) return 1;
				int t1c = t1->tconst();
				int t2c = t2->tconst() - fct_const - vec_const;
 //error('d',"t1c %d t2c %d",t1c,t2c);
				if (cnst1+t1c<cnst2+t2c) return 1;
				if (tt2c<tt1c	// *const = *
				&& cnst1+t1c>cnst2+t2c)	// T = constT
					return 1;
			}
*/
			
		}
	}
	else {
//error('d',"first t1 %t t2 %t cnst1 %d cnst2 %d",t1,t2,cnst1,cnst2);
	}
//error('d',"return 0");
	return 0;

cla_check:
	{	Pname n1 = Pbase(t1)->b_name;
		Pname n2 = Pbase(t2)->b_name;
//error('d',"cla_check %n %n ptr_count %d",n1,n2,ptr_count);
		if (n1 == n2) goto const_check;

                // once again, a more comprehensive check for classes,
                // since they may be parametrized.
                if ((t1->base == COBJ) && (t2->base == COBJ) &&
                   ((Pclass(n1->tp)->same_class(Pclass(n2->tp)))))
                        goto const_check;

		if (/*first || */1<ptr_count || fct_seen) return 1;

		switch (oper) {
		case ARG:
		case ASSIGN:
		case RETURN:
		case COERCE:
		{
			ppbase = PUBLIC;
			if (Pclass(n2->tp)->is_base(n1->string)) {
				if (ppbase!=PUBLIC) {
					const_problem = 0;
				//	vrp_equiv = 0;
					return 1;	// private or protected base
				}
				Nstd++;
				goto const_check;
			}
		}
			// no break
		case 0:
		case OVERLOAD:
			const_problem = 0;
		//	vrp_equiv = 0;
			return 1;
		}

		goto const_check;
	}

base_check:
//error('d',"base_check t1=%t t2=%t oper=%d %s",t1,t2,oper,first?"first":"");
//error('d',"ptr_count %d",ptr_count);
	if (oper)
	if (first || 1!=ptr_count) {
		if (b1==VOID || b2==VOID) return 1;
	}
	else {
		if (b1 == VOID) {		// check for void* = T*
			register Ptype tpx = this;
		tpxloop:
			switch (tpx->base) {	// t1 == void*
			default:
					const_problem = 0;
					return 1;
			case VOID:	break;
			case PTR:
			case VEC:	tpx = Pvec(tpx)->typ;
					goto tpxloop;
			case TYPE:	tpx = Pbase(tpx)->b_name->tp;
					goto tpxloop;
			}

			tpx = t;
		bloop:
			switch (tpx->base) {	// t2 == T*
			default:
					const_problem = 0;
					return 1;
			case VEC:
			case PTR:
			case FCT:	Nstd++;
				//	return 0;
					goto const_check; // prevent void* = const*
			case TYPE:	tpx = Pbase(tpx)->b_name->tp;
					goto bloop;
			}
		}

		if (b2 != ZTYPE) {
			const_problem = 0;
			return 1;
		}
	}
//error('d',"oper %d b1 %d b2 %d cp %d",oper,b1,b2,const_problem);
	switch (oper) {
	case 0:
		if (b1 != b2) {
			const_problem = 0; // we have a bigger problem
		//	vrp_equiv = 0;
		}
		return 1;
	case COERCE:	// could probably be merged with the cases below
		switch (b1) {
		case EOBJ:
		case ZTYPE:
		case CHAR:
		case SHORT:
		case INT:
			switch (b2) {
				case LONG:
				case FLOAT:
				case DOUBLE:
				case LDOUBLE:
				case EOBJ:
				case ZTYPE:
				case CHAR:
				case SHORT:
				case INT:
				case FIELD:
					Nstd++;
					suppress_error++;
					if (b1 == EOBJ) echeck(t1,t2);
					suppress_error--;
					goto const_check;
			}
			return 1;
		case LONG:	// char, short, and int promotes to long
			switch (b2) {
				case FLOAT:
				case DOUBLE:
				case LDOUBLE:
			case ZTYPE:
			case EOBJ:
			case CHAR:
			case SHORT:
			case INT:
			case FIELD:
				Nstd++;
				goto const_check;
			}
			return 1;
		case FLOAT:
		//	switch (b2) {
		//	case ZTYPE:
		//		Nstd++;
		//	case FLOAT:
		//	case DOUBLE:
		//		goto const_check;
		//	}
		//	return 1;
		case DOUBLE:	// char, short, int, and float promotes to double
		case LDOUBLE:
			switch (b2) {
			case LONG:
			case ZTYPE:
			case EOBJ:
			case CHAR:
			case SHORT:
			case INT:
		//		Nstd++;
			case FLOAT:
			case DOUBLE:
			case LDOUBLE:
Nstd++;
				goto const_check;
			}
			return 1;
		case PTR:
			switch (b2) {
			case ZTYPE:
				Nstd++;
				goto const_check;
			}
		case RPTR:
		case VEC:
		case COBJ:
		case FCT:
			return 1;
		}
	case ARG:
	case ASSIGN:
	case RETURN:
		switch (b1) {
		case COBJ:
			return 1;
		case EOBJ:
		case ZTYPE:
		case CHAR:
		case SHORT:
		case INT:
		case LONG:
			suppress_error++;
			r = t2->num_ptr(ASSIGN);
			suppress_error--;
			switch (r) {
			case 'F':
			//	if (oper!=ARG) error('w',"%t assigned to%t",t2,t1);
				break;
			case 'A':
			case 'P':
			case FCT:	return 1;
			}
			if (b1 == EOBJ) echeck(t1,t2);	
			break;
		case FLOAT:
		case DOUBLE:
		case LDOUBLE:
			suppress_error++;
			r = t2->numeric(ASSIGN);
			suppress_error--;
			switch (r) {	
			case 'A':
			case 'P':
			case FCT:	return 1;
			}
			break;
		case VEC:
			if(oper==ARG && b2==ZTYPE) goto const_check;
			return 1;
		case PTR:
			suppress_error++;
			r = t2->num_ptr(ASSIGN);
			suppress_error--;
			switch (r) {
			case 'A':
			case 'I':
			case 'F':	return 1;
			case FCT:	if (Pptr(t1)->typ->base != FCT) return 1;
			}
			break;
		case RPTR:
			return 1;
		case FCT:
			switch (oper) {
			case ARG:
			case ASSIGN:
				return 1;
			}
		}	
		break;
	}
	goto const_check;
}

