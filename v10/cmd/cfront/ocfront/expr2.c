/*ident	"@(#)ctrans:src/expr2.c	1.1.5.36" */
/***************************************************************************

	C++ source for cfront, the C++ compiler front-end
	written in the computer science research center of Bell Labs

	Copyright (c) 1984 AT&T, Inc. All rigths Reserved
	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T, INC.

expr2.c:

	type check expressions

************************************************************************/

#include "cfront.h"
#include "size.h"
extern Pname conv_dominates(Pname,Pname);
static int const_obj1,const_obj2;

Pname really_dominate(Pname on1, Pname on2, bit tc)
{
	Pfct f1 = on1->tp->base==FCT ? Pfct(on1->tp) :
		Pfct(Pgen(on1->tp)->fct_list->f->tp);
	Pfct f2 = on2->tp->base==FCT ? Pfct(on2->tp) :
		Pfct(Pgen(on2->tp)->fct_list->f->tp);

	// const check
	int c1 = f1->f_const;
	int c2 = f2->f_const;

	if(c1 == c2) ;
	else if(c1 && !c2) return tc ? on1 : on2;
	else if(c2 && !c1) return tc ? on2 : on1;

	// hierarchy check
	Pname on = conv_dominates(on1,on2);
	if(on) return on;
}

void name::assign()
{
	if (n_assigned_to++ == 0) {
		switch (n_scope) {
		case FCT:
			if (n_used && n_addr_taken==0)  {
				Ptype t = tp;
			ll:
				switch (t->base) {
				case TYPE:
					t=Pbase(t)->b_name->tp; goto ll;
				case VEC:
					break;
				default:
					if (curr_loop)
						error('w',&where,"%n may have been used before set",this);
					else
						error('w',&where,"%n used before set",this);
				}
			}
		}
	}
}

void name::take_addr()
{
// error('d', "%n->take_addr tp: %t", this, tp?tp:0 );
// error('d', "%n->take_addr tp: %d %d", this, tp?tp:0, tp?tp->base:0 );
	if ( (warning_opt) && (! n_addr_taken) && (tp) && (tp->base==FCT) && Pfct (tp)->f_inline)
		error('w',"can't take address of inline function %n, %n not inlined", this, this);  
	n_addr_taken++; 
	if ( n_sto == EXTERN && tp ) {
		Ptype t = tp;
		while ( t->base == TYPE )  
			t = Pbase(t)->b_name->tp;
		switch ( t->base ) {
			case COBJ:
				t = Pbase(t)->b_name->tp; // no break
			case CLASS: {
				Pclass cl = Pclass(t);
				if ( cl->c_body == 1 )
					cl->dcl_print(0);
			}
		}
	}
}

int ignore_const;	// for use by ref_init
static is_dataMemPtr(Pexpr);

int expr::lval(TOK oper)
{
	register Pexpr ee = this;
	register Pname n;
	int deref = 0;
	char* es;

//error('d',"%k expr::lval %k",base,oper);

	switch (oper) {
	case ADDROF:
	case G_ADDROF:	es = "address of";	break;
	case DEREF:	es = "dereference of";	break;
	case INCR:	es = "increment of";	goto def;
	case DECR:	es = "decrement of";	goto def;
	default:	es = "assignment to";
	def:
		if (ignore_const==0 && tp->tconst()) {
			if (oper) {
				if (base == NAME) {
					if (vec_const && Pname(this)->n_scope==ARG) break;
					error("%s constant%n",es,this);
				}
				else
					error("%s constant",es);
			}
			return 0;
		}
	}

	for(;;) {
//error('d',"loop %k",ee->base);
		switch (ee->base) {
	//	case G_CALL:
	//	case CALL:
		default:
		defa:
			if (deref == 0) {
				if (oper) error("%s%k (not an lvalue)",es,ee->base);
				return 0;
			}
			return 1;
		case ZERO:
		case CCON:
		case ICON:
		case FCON:
			if (oper) error("%s numeric constant",es);
			return 0;
		case STRING:
			if (oper) error('w',"%s string constant",es);
			return 1;
		case CAST:
		switch( oper ) {
		case 0:
		case ADDROF:
		case G_ADDROF:
		case DEREF:
     			goto defa;
		default:
			if ( ee->tp->base == PTR 
			     && is_dataMemPtr(ee) ) 
			{ // check for const class object
    				Pexpr te;
				te = ee->e1->e1->e1;	 
       				if ( te->base == G_ADDROF )
					te = te->e2;
       				if ( te->base == NAME ) {
					Ptype pt = te->tp;
					if ( pt->base == PTR )
						pt = Pptr(pt)->typ;
					if ( pt->tconst() ) 
						error("%sCMP of const%n",es,te);
					return 0;
				}
			}
			goto defa;
		}

		case DEREF:
		{	
			Pexpr ee1 = ee->e1;
// error( 'd', "ee1: %k", ee1->base );
			switch (ee1->base) {	// *& vanishes
			case ADDROF:	// *&
				return 1;
			case G_CM:
			case CM:	// look for *(a,&b)
				if (ee1->e2->base==G_ADDROF
				|| ee1->e2->base==ADDROF)
					return 1;
				goto defaa;
			case QUEST:	// look for *(q?&a:&b)
				if ((ee1->e1->base==G_ADDROF
					|| ee1->e1->base==ADDROF)
				&& (ee1->e2->base==G_ADDROF
					|| ee1->e2->base==ADDROF))
					return 1;
				// no break
			default:
			defaa:
				ee = ee1;
				deref = 1;
			}
			break;
		}

		case QUEST:
		{	int x1 = ee->e1->lval(deref?0:oper);
			int x2 = ee->e2->lval(deref?0:oper);
			if (ee->e1->tp->check(ee->e2->tp,0)) return 0;
			if (deref) return 1;
			return x1 && x2;
		}

		case INCR:
		case DECR:
			if (e1) goto defa;	// postfix does not preserve lval
		case ASSIGN:
		case ASPLUS:
		case ASMINUS:
		case ASMUL:
		case ASDIV:
		case ASMOD:
		case ASAND:
		case ASOR:
		case ASER:
		case ASLS:
		case ASRS:
			return 1;

		case CM:
		case G_CM:
			if (ee->e2->lval(deref?0:oper)==0) return deref;
			return 1;

		case MEMPTR:
			ee = ee->e2;
			break;

		case MDOT:
			ee = ee->mem;
			break;

		case DOT:
		{
// error('d',"dot %k oper %k",ee->e1->base,oper);
			Pexpr e = 0;
			int e_const = 0; // to catch x.y.val = 1, where x is const

			switch (ee->e1->base) {		// update use counts, etc.
			case NAME:
				switch (oper) {
				case ADDROF:
				case G_ADDROF:	Pname(ee->e1)->take_addr();
				case 0:		break;
				case ASSIGN:	Pname(ee->e1)->n_used--;
				default:	Pname(ee->e1)->assign(); // asop
				}
				break;
			case DOT:
				e = ee->e1;
				do e=e->e1; while (e->base==DOT);
				if (e->base == NAME) {
					e_const = e->tp->tconst();
					switch (oper) {
					case ADDROF:
					case G_ADDROF:	Pname(e)->take_addr();
					case 0:		break;
					case ASSIGN:	Pname(e)->n_used--;
					default:	Pname(e)->assign(); // asop
					}
				}
			}

			n = Pname(ee->mem);
			while (n->base == MDOT) n = Pname(Pref(n)->mem);

			if (deref==0 && 
				(ee->e1->tp->tconst() || e_const)) {

				switch (oper) {
				case 0:
				case ADDROF:
				case G_ADDROF:
				case DEREF:
					break;
				default:
					error("%sM%n of%t",es,n,e_const?e->tp:ee->e1->tp);
				}
				return 0;
			}
		}
		goto xx;

		case REF:
			n = Pname(ee->mem);
			while (n->base == MDOT) n = Pname(Pref(n)->mem);

			if (deref==0 && ee->e1) {  //BR
				Ptype p = ee->e1->tp;
			zxc:
				switch (p->base) {
				case TYPE:	p = Pbase(p)->b_name->tp; goto zxc;
				case PTR:
				case VEC:	break;
				default:	error('i',"expr::lval %t->%n",p,n);
				}
				if (Pptr(p)->typ->tconst()) {
					switch (oper) {
					case 0:
					case G_ADDROF:
					case DEREF:
						break;
					case ADDROF:
						if ( cm_const_save == 0 && const_ptr == 0 ) 
							error(strict_opt?0:'w',"%sM%n of%t",es,n,Pptr(p)->typ);
						break;
					default:
						error("%sM%n of%t",es,n,Pptr(p)->typ);
					}
					return 0;
				}
			}
			goto xx;

		case NAME:
			n = Pname(ee);
		xx:
// error('d',"name xx: %n oper %d lex_level: %d",n,oper,n->lex_level);
			if (deref) return 1;
			if (oper==0) return n->n_stclass != ENUM ;

			if (n->tp->base==FIELD && Pbase(n->tp)->b_bits==0) {
				error("%s 0-length field%n",es,n);
				return 0;
			}

			switch (oper) {
			case ADDROF:
			case G_ADDROF:
			{	Pfct f = (Pfct)n->tp;

				if (n->n_sto == REGISTER) {
					if (warning_opt) error('w',"& register%n",n);
				//	return 0;
					n->n_sto = 0;
					n->n_stclass = AUTO;
				}

				if (f == 0) {
					error("& label%n",n);
					return 0;
				}

				if (n->n_stclass == ENUM) {
					error("& enumerator%n",n);
					return 0;
				}

				if (n->tp->base == FIELD) {
					error("& field%n",n);
					return 0;
				}

				n->n_used--;
				if (n->n_qualifier) { // oops, not the real one
					Pname tn = Pclass(n->n_table->t_name->tp)->memtbl->look(n->string,0);
					n = tn ? tn : n;
				}
				n->take_addr();

				// suppress hoisting of local consts
				int statmem = n->n_scope==0 || n->n_scope==PUBLIC || n->n_scope == FCT;
				if (n->n_evaluated && n->n_scope!=ARG) { // &const
					if (statmem == 0 && n->n_dcl_printed==0) {
						n->n_initializer = new ival(n->n_val);
						n->dcl_print(0);
					}
				}
				else if (f->base==FCT && n->n_dcl_printed==0)
					n->dcl_print(0);
				break;
			}

			case ASSIGN:
//error('d',"ass %n %d",n,n->n_used);
				n->n_used--;
				n->assign();
				break;
			//	goto check_void;
			default:	/* incr ops, and asops */
				if (cc->tot && n==cc->c_this) {
					error("%n%k",n,oper);
					return 0;
				}
		//	check_void:
		//		{	Ptype t = n->tp;
		//			while (t->base==TYPE) t = Pbase(t)->b_name->tp;
		//			if (t==Pvoid_type) {
		//				error("%s%t",es,n->tp);
		//				return 0;
		//			}
		//		}
				n->assign();
			}
			return 1;
		}
	}
}

int char_to_int(char* s)
/*	assume s points to a string:
		'c'
	or	'\c'
	or	'\0'
	or	'\ddd'
	or multi-character versions of the above
	(hex constants have been converted to octal by the parser)
*/
{
	register int i = 0;
	register char c, d, e;

	switch (*s) {
	default:
		error('i',"char constant store corrupted");
	case '`':
		error("bcd constant");
		return 0;
	case '\'':
		break;
	}

	for(;;)			/* also handle multi-character constants */
	switch (c = *++s) {
	case '\'':
		return i;
	case '\\':			/* special character */
		switch (c = *++s) {
		case '0': case '1': case '2': case '3': case '4':
		case '5': case '6': case '7':	/* octal representation */
			c -= '0';
			switch (d = *++s) {		/* try for 2 */
				
			case '0': case '1': case '2': case '3': case '4':
			case '5': case '6': case '7':
				d -= '0';
				switch (e = *++s) {	/* try for 3 */
					
				case '0': case '1': case '2': case '3': case '4':
				case '5': case '6': case '7':
					c = c*64+d*8+e-'0';
					break;
				default:
					c = c*8+d;
					s--;
				}
				break;
			default:
				s--;
			}
			break;
		case 'a':
			c = '\a';
			break;
		case 'b':
			c = '\b';
			break;
		case 'f':
			c = '\f';
			break;
		case 'n':
			c = '\n';
			break;
		case 'r':
			c = '\r';
			break;
		case 't':
			c = '\t';
			break;
		case 'v':
			c = '\v';
			break;
		case '\\':
			c = '\\';
			break;
		case '\'':
			c = '\'';
			break;
		}
		/* no break */
	default:
		if (i) i <<= BI_IN_BYTE;
		i += c;
	}
}

const A10 = 'A'-10;
const a10 = 'a'-10;

long str_to_long(register const char* p)
{
	register c;
	register unsigned long i= 0;
	const char* pp = p;

// error( 'd', "str_to_long: %s", p );

	if ((c=*p++) == '0') {
		switch (c = *p++) {
		case 0:
			return 0;

		case 'l':
		case 'L':	/* long zero */
			return 0;

		case 'x':
		case 'X':	/* hexadecimal */
			while (c=*p++) {
				switch (c) {
				case 'l':
				case 'L':
				case 'U':
				case 'u':
					return i;
				case 'A':
				case 'B':
				case 'C':
				case 'D':
				case 'E':
				case 'F':
					i = i*16 + c-A10;
					break;
				case 'a':
				case 'b':
				case 'c':
				case 'd':
				case 'e':
				case 'f':
					i = i*16 + c-a10;
					break;
				default:
					i = i*16 + c-'0';
				}
			}
			return i;

		default:	/* octal */
			do 
				switch (c) {
				case 'l':
				case 'L':
				case 'U':
				case 'u':
					return i;
				default:
					i = i*8 + c-'0';
				}
			while (c=*p++);
			return i;
		}
	}	
				/* decimal */
	i = c-'0';
	while (c=*p++)
		switch (c) {
		case 'l':
		case 'L':
		case 'U':
		case 'u':
			return i;
		default:
		{       unsigned long ii = i;
			i = i*10 + c-'0';
			if (i<ii) goto bad;
		}
		}
	return i;
bad:
	error("integer constant %s larger than the largest long",pp);
	return i;
	
		
}

bit type::is_unsigned()
{
	Ptype t = this;
	while (t->base==TYPE) t = Pbase(t)->b_name->tp;
	if (t->base == PTR) return 0;
	return Pbase(t)->b_unsigned;
}

char* Neval;
bit binary_val;

unsigned long expr::ueval(long x1, long x2)
{
	unsigned long i1 = (unsigned long) x1;	
	unsigned long i2 = (unsigned long) x2;
//error('d',"ueval %k %ld %ld",base,x1,x2);
	switch (base) {
	case UMINUS:	return -i2;
	case UPLUS:	return i2;
	case NOT:	return !i2;
	case COMPL:	return ~i2;
	case CAST:	return i1;
	case PLUS:	return i1+i2;
	case MINUS:	return i1-i2;
	case MUL:	return i1*i2;
	case LS:	return i1<<i2;
	case RS:	return i1>>i2;
	case NE:	return i1!=i2;
	case EQ:	return i1==i2;
	case LT:	return i1<i2;
	case LE:	return i1<=i2;
	case GT:	return i1>i2;
	case GE:	return i1>=i2;
	case AND:	return i1&i2;
 	case ANDAND:	return i1&&i2;
	case OR:	return i1|i2;
	case OROR:	return i1||i2;
	case ER:	return i1^i2;
	case MOD:	if (i2 == 0) {
				if (Neval == 0) { 
					Neval = "mod zero";
					error("mod zero");
				}
				return 1;
			}
			return i1%i2;
	case QUEST:	return (cond->eval()) ? i1 : i2;
	case DIV:	if (i2 == 0) {
				if (Neval == 0) { 
					Neval = "divide by zero";
					error("divide by zero");
				}
				return 1;
			}
			return i1/i2;
	case CM:
	case G_CM:
		return i2;
	}

	Neval = "unsigned expression";
	return 0;
}

long expr::eval()
{
	if (Neval) return 1;
// error('d',"eval %k",base);

	static int targno=0;
	static int icallflag=0;

	switch (base) {
	case ZERO:	return 0;
	case IVAL:	return i1;
	case ICON:	return str_to_long(string);
	case CCON:	return char_to_int(string);
	case FCON:	Neval = "float in constant expression"; return 1;
	case STRING:	Neval = "string in constant expression"; return 1;
	case EOBJ:	return Pname(this)->n_val;
	case SIZEOF:
		extern no_sizeof;
		if (no_sizeof) Neval = "sizeof";
		return tp2->tsizeof();
		
	case NAME:
	{	Pname n = Pname(this);
// error('d',"eval %n eval %d %d",n,n->n_evaluated,n->n_val);
// error('d',"eval tp->tconst() %d, n->n_initializer: %k", n->tp->tconst(), n->n_initializer?n->n_initializer->base:0 );
		if (n->n_evaluated && n->n_scope!=ARG) return n->n_val;
		if (binary_val && strcmp(string,"_result")==0) return 8888;
		Neval = "cannot evaluate constant";
		return 1;
	}
	case ICALL:
		if (e1) {
			icallflag=1;
			targno=0;
			il->i_next = curr_icall;
			curr_icall = il;
			long i = e1->eval();
			curr_icall = il->i_next;
			icallflag=0;
			return i;
		}
		Neval = "void inlineF";
		return 1;
	case ANAME:
	{	Pname n = (Pname)this;

/*
		int argno;
		if (icallflag) {
		argno=targno;
		targno++;
		}
*/
		int argno = (int) n->n_val;
	
		Pin il;
		for (il=curr_icall; il; il=il->i_next)
			if (il->i_table == n->n_table) goto aok;
		goto bok;
	aok:
		if (il->i_args[argno].local) {
	bok:
			Neval = "inlineF call too complicated for constant expression";
			return 1;
		}
		Pexpr aa = il->i_args[argno].arg;
		return aa->eval();
	}
	case CAST:
	{	if (e1->base==FCON && tp2->base!=FLOAT && tp2->base!=DOUBLE) {
			char* p = e1->string;
			while (*p!='.') p++;
			if (p==e1->string) *p++ = '0';
			*p = 0;
			e1->base = ICON;
		}		
		long i = e1->eval();
		Ptype tt = tp2;
	strip:
		switch (tt->base) {
		default:
			Neval = "cast to non-integral type in constant expression";
			break;
		case TYPE:
			tt = Pbase(tt)->b_name->tp;
			goto strip;
		case EOBJ:
		case LONG:
		case INT:
		case CHAR:
		case SHORT:
			i &= ~(((~(unsigned long)0)<<(BI_IN_BYTE*(tp2->tsizeof()-1)))<<BI_IN_BYTE);
			break;
		}
		return i;
	}
	case UMINUS:
	case UPLUS:
	case NOT:
	case COMPL:
	case PLUS:
	case MINUS:
	case MUL:
	case LS:
	case RS:
	case NE:
	case LT:
	case LE:
	case GT:
	case GE:
	case AND:
	case OR:
	case ER:
	case DIV:
	case MOD:
	case QUEST:
	case EQ:
	case ANDAND:
		break;
	case OROR:
		if (binary_val) {	// a||b, don't evaluate b if a!=0
			long i1 = (e1) ? e1->eval() : 0;
			if (Neval==0 && i1 && e1->tp->is_unsigned()==0) return i1;
		}
		break;
	case CM:
	case G_CM:
		break;
	case G_ADDROF:
	case ADDROF:
		if (binary_val) {	// beware of &*(T*)0
			switch (e2->base) {
			case NAME:
			case DOT:
			case REF:	return 9999;
			}
		}
	default:
		Neval = "bad operator in constant expression";
		return 1;
	}

	long i1 = (e1) ? e1->eval() : 0;
	long i2 = (e2) ? e2->eval() : 0;

	if (binary_val && i1==9999 && i2==9999) {
		Neval = "";
		return 1;
	}
	
	if (Neval==0
	&& ((e1&&e1->tp->is_unsigned()) || (e2&&e2->tp->is_unsigned())))
		return (long) ueval(i1,i2);
	
	switch (base) {
	case UMINUS:	return -i2;
	case UPLUS:	return i2;
	case NOT:	return !i2;
	case COMPL:	return ~i2;
	case CAST:	return i1;
	case PLUS:	return i1+i2;
	case MINUS:	return i1-i2;
	case MUL:	return i1*i2;
	case LS:	return i1<<i2;
	case RS:	return i1>>i2;
	case NE:	return i1!=i2;
	case EQ:	return i1==i2;
	case LT:	return i1<i2;
	case LE:	return i1<=i2;
	case GT:	return i1>i2;
	case GE:	return i1>=i2;
	case AND:	return i1&i2;
 	case ANDAND:	return i1&&i2;
	case OR:	return i1|i2;
	case OROR:	return i1||i2;
	case ER:	return i1^i2;
	case MOD:	if (i2 == 0) {
				if (Neval == 0) { 
					Neval = "mod zero";
					error("mod zero");
				}
				return 1;
			}
			return i1%i2;
	case QUEST:	return (cond->eval()) ? i1 : i2;
	case DIV:	if (i2 == 0) {
				if (Neval == 0) { 
					Neval = "divide by zero";
					error("divide by zero");
				}
				return 1;
			}
			return i1/i2;
	case CM:
	case G_CM:
		return i2;
	}
}
bit classdef::baseof(Pname f)
/*
	is ``this'' class a public base class of "f"'s class
	or its immediate base class
*/
{
	Ptable ctbl = f->n_table;
	Pname b = ctbl->t_name;

	if (b == 0) return 0;
	Pclass cl = Pclass(b->tp);
	if (cl == 0) return 0;
	if (cl == this) return 1;
	ppbase = PUBLIC;
	Pclass bcl = is_base(cl->string);
	return (bcl && ppbase==PUBLIC);
}

bit classdef::baseof(Pclass cl)
/*
	is ``this'' class a public base class of "cl"
*/
{
	if (cl == 0) return 0;
	if (cl == this) return 1;
	ppbase = PUBLIC;
	Pclass bcl = is_base(cl->string);
	return (bcl && ppbase==PUBLIC);
}

static int mem_match(Pfct f1, Pfct f2)
/*
	check class membership.

	For some reason checking f_this==0 works and f_static doesn't
*/
{
//	if (f1->memof) return f2->f_this ?f2->memof==f1->memof : 0;
//	if (f1 && f1->memof) return f2->f_this?f2->memof==f1->memof : 0;
//	return f2->f_this==0;
	if (f1==0 || f2==0) return 0;
	if (f1->memof && f2->f_this && f2->memof!=f1->memof) return 0;
	if (f2->f_this) return 0;
	if (f1->memof && f2->f_static) return 0;
	if (f1->check(f2,ASSIGN)) return 0;
	return 1;
}

int Pchecked;

Pexpr ptof(Pfct ef, Pexpr e, Ptable tbl)
/*
	a kludge: initialize/assign-to pointer to function
*/
{
	Pfct f;
	Pname n = 0;
eee:
//error('d',"ptof %t %t %k",ef,e->tp,e->base);
	switch (e->base) {
	case QUEST:
		e->e1 = ptof(ef,e->e1,tbl);
		e->e2 = ptof(ef,e->e2,tbl);
		return e;
	case CM:
	case G_CM:
		e->e2 = ptof(ef,e->e2,tbl);
		return e;
	case NAME:
		f = Pfct(e->tp);
		n = Pname(e);

		switch (f->base) {
		case OVERLOAD:
			e = Pgen(f)->find(ef,0);
			if (e == 0) {
				error("cannot deduceT for &overloaded%n",n);
				return e;
			}
		//	e = n;
			// no break
		case FCT:
			Pchecked = mem_match(ef,Pfct(e->tp));
			e = new expr(G_ADDROF,0,e);
			return e->typ(tbl);	// handle &B::f
			//e->tp = f;
		}
		goto ad;

	case ZERO:
		if (ef->memof) {
			e = new expr(ELIST,zero,zero);
			e = new expr(ILIST,e,zero);
			e->tp = zero_type;
			return e; 
		}
		break;

	case MDOT:
		// ?? error('s',"P toM of not firstB");
		do e = e->mem; while (e->base == MDOT);
		goto eee;

	case DOT:
	case REF:
		f = Pfct(e->mem->tp);

		switch (f->base) {
		case OVERLOAD:
			n = Pgen(f)->find(ef,0);
			if (n == 0) error("cannot deduceT for &overloaded%n",e->mem);
			else e = n;
			// no break
		case FCT:
			Pchecked = mem_match(ef,Pfct(e->tp));
			e = new expr(G_ADDROF,0,e);
			return e->typ(tbl);	// handle &B::f
		//	n = Pname(e->mem);
		//	e = n->address();
		}
		goto ad;

	case ADDROF:
	case G_ADDROF:
		f = Pfct(e->e2->tp);
	ad:
		if (f->base == OVERLOAD) {
			n = Pgen(f)->find(ef,0);
			if (n == 0) error("cannot deduceT for &overloaded %s()",Pgen(f)->fct_list->f->string);
			Pchecked = mem_match(ef,Pfct(n->tp));
			e->e2 = n;
			e->tp = n->tp;
		}
		if (n) n->lval(ADDROF);
		break;

	case CAST:
	{
		Pexpr te = e->e1;
		if (e->e1->base == G_ADDROF) te = e->e1->e2;
		(void) ptof(ef,te,tbl);
	}
	}
	return e;
}

Pexpr ptr_init(Pptr p, Pexpr init, Ptable tbl)
/*
	check for silly initializers

	char* p = 0L;	 ??	fudge to allow redundant and incorrect `L'
	char* p = 2 - 2; ??	worse
*/
{
// error('d',"ptr_init: p=%t init->tp=%t init->base %k",p,init->tp,init->base);

	Pchecked = 0;

	Ptype it = init->tp;
itl:
	switch (it->base) {
	case TYPE:
		it = Pbase(it)->b_name->tp; goto itl;
	case ZTYPE:
//		if (init == zero) break;
		break;
	case EOBJ:
	case INT:
	case CHAR:
	case SHORT:
	case LONG:
	{	Neval = 0;
		long i = init->eval();
		if (Neval)
			error("badPIr: %s",Neval);
		else
		if (i)
			error("badPIr value %d",i);
		else {
			DEL(init);
			init = zero;
		}
		break;
	}		
	}

	Pclass c1 = p->memof;

	if (c1) {
		if (init==zero)
			;
		else {
			Pclass c2;
//error('d',"it %t %d",it,it->base);
			switch (it->base) {
			case FCT:
				c2 = Pfct(it)->memof;
				break;
			case OVERLOAD:
				c2 = Pfct(Pgen(it)->fct_list->f->tp)->memof;
				break;
			case PTR:
			case RPTR:
				c2 = Pptr(it)->memof;
				break;
			default:
				c2 = 0;
			}

			if (c2 == 0) {
				// initialization by &A::f
//error('d',"curious");
			}
			else if (c1 != c2) {
                                Nptr = 0;
				Noffset = 0;
				vcllist->clear();
				vcllist=0;
				int u1 = is_unique_base(c1,c2->string,0);
//error('d',"c1 %t c2 %t u1 %d off %d",c1,c2,u1,Noffset);
                                if (u1 && (Nptr || Noffset)) {
					// requires offset manipulation
					int bad = 0;
        				if (u1 == 1 && !Nptr) {
						if (init->base==ILIST) {
							// d = d+Noffset;
							switch (init->e1->e1->base) {
								case IVAL:
									init->e1->e1->i1 += Noffset;
									break;
								case ZERO:
									init->e1->e1 = new ival(Noffset);
									break;
								default:
									bad = 1;
								}

							// if (i<0) f = vptroffset
							switch (init->e1->e2->base) {
								case IVAL:
									if (0<init->e1->e2->i1) {
									// extern Ptype Pfct_type;
									// store vptr offset
									//  init->e2=new cast(Pfct_type,zero);
									}
									else
										break;
								default:
									bad = 1;
								}	
						} // end if (init->base==ILIST)
						else
							bad = 1;
					} // end if (u1 == 1 ...
					else
						bad = 1;

				if (bad) error('s',"%t assigned to %t (too complicated)",init->tp,p);
				} // end if (u1 && ...

                                Nptr = 0;
				Noffset = 0;
				vcllist->clear();
				vcllist=0;
				int u2 = is_unique_base(c2,c1->string,0);
//error('d',"c1 %t c2 %t u2 %d off %d",c1,c2,u2,Noffset);
                                if (u2 && (Nptr || Noffset)) {
					// requires offset manipulation
					error('s',"%t assigned to %t",init->tp,p);
				}
			} // end if (c1 != c2
		} // end else
	} // end if (c1)

	Ptype pit = p->typ;
lll:
//error('d',"p %t pit %t",p,pit);
	switch (pit->base) {
	case TYPE:
		pit = Pbase(pit)->b_name->tp;
		goto lll;
	case FCT:
		return ptof(Pfct(pit),init,tbl);
	case COBJ:
	{	Pptr r;
// error('d',"cobj: ptr %t, ref %t",it->is_ptr(),it->is_ref());
		if (r=it->is_ptr_or_ref()) {
			Pchecked = 1;
			TOK b = p->base;		// could be REF
			TOK bb = r->base;
			if (b==RPTR) p->base = PTR;
			if (bb==RPTR) r->base = PTR;
			if (p->check(r,ASSIGN)) {
				if ( cc && cc->nof && 
					Pfct(cc->nof->tp)->f_const &&
        				cc->c_this == init )
						error("%n const: assignment of%n (%t) to%t",cc->nof,init,init->tp,p);
				else
					error("no standard conversion of %t to %t",init->tp,p);
			}
			p->base = b;
			r->base = bb;
			Pexpr cp = cast_cptr(Pclass(Pbase(pit)->b_name->tp),init,tbl,0);
			if (cp != init) {
				PERM(p);	// or else it will be deleted twice!
				return new cast(p,cp);
			}
		}
		// no break
	}
	default:
		return init;
	}
}

static Pname Lcoerce, Rcoerce;
extern int suppress_error;

int try_to_demote(TOK oper, Ptype t1, Ptype t2)
/*
	look at t1 and t2 and see if there are ``demotions'' of t1 and/or t2
	so that ``t1 oper t2'' can be made legal

	return	0 is not
		1 if there is exactly one way
		>1 if there is more than one way (if in doubt return 2)
*/
{
//error('d',"try_to_demote(%k : %t : %t)",oper,t1,t2);

	Pname n1 = t1 ? t1->is_cl_obj() : 0;
	Pclass c1 = n1 ? Pclass(n1->tp) : 0;
	Pname n2 = t2 ? t2->is_cl_obj() : 0;
	Pclass c2 = n2 ? Pclass(n2->tp) : 0;

	Ptype lt = t1;
	Ptype rt = t2;

	Lcoerce = Rcoerce = 0;

//	if (oper == DOT) return 0;

	if (c1)
		switch (oper) {
		case ASSIGN:
		case ASPLUS:
		case ASMINUS:
		case ASMUL:
		case ASDIV:
		case ASMOD:
		case ASAND:
		case ASOR:
		case ASER:
		case ASLS:
		case ASRS:	// don't coerce left hand side of assignment
		//	c1 = 0;
			if (c1->memtbl->look("__as",0)) return 0;
		}
	else
		switch (oper) {
		case ADDROF:
		case INCR:
		case DECR:	// don't coerce unary requiring an lval
			return 0;
		}

	if (c1) {
//error('d',"c1 %t",c1);
		for (Pname on1 = c1->conv; on1; on1=on1->n_list) {
// error( 'd', "on1: %s tp: %k", on1->string, on1->tp->base );
			Pfct f = Pfct(on1->tp);
			lt = f->base==FCT ? f->returns :
				Pfct(Pgen(on1->tp)->fct_list->f->tp)->returns;
			Pname cn = lt->is_cl_obj();

			if (cn && (Lcoerce==0 || Lcoerce->tp->check(f,0))) {
				Pclass cl = Pclass(cn->tp);
				Pname n = cl->has_oper(oper);
				if (n == 0) continue;
		//		while (n->base==REF || n->base==MDOT) n=Pname(n->mem) ;
				Pfct nf = Pfct(n->tp);
// error( 'd', "nf: %d %k", nf->base, nf->base );
				if (nf->base == FCT) {
					if (nf->nargs==1
					&& t2
					&& (nf->argtype->tp->check(t2,ARG)==0
						|| can_coerce(nf->argtype->tp,t2)==1)
					) {
						if (Lcoerce) return 2;
						Lcoerce = on1;
					}
				}
				else {
					for (Plist gl=Pgen(nf)->fct_list; gl; gl=gl->l) {
						Pfct nf = Pfct(gl->f->tp);
						if (nf->nargs==1
						&& t2
						&& (nf->argtype->tp->check(t2,ARG)==0
							|| can_coerce(nf->argtype->tp,t2)==1)
						) {
							if (Lcoerce) return 2;
							Lcoerce = on1;
						}
					}
				}
				continue;
			}
			//if (lt->is_cl_obj()) continue;
			if (c2) {
//error('d',"c2 %t",c2);
				for (Pname on2 = c2->conv; on2; on2=on2->n_list) {
					Pfct f = Pfct(on2->tp);
					rt = f->base==FCT ? f->returns :
						Pfct(Pgen(on2->tp)->fct_list->f->tp)->returns;
					if (rt->is_cl_obj()) continue;

					suppress_error = 1;
					int r1 = lt->kind(oper,0);
					int r2 = rt->kind(oper,0);
					if (np_promote(oper,r1,r2,lt,rt,1)!=any_type) {
						Pname sn = on1;
						if (Lcoerce) {
						    Pname tn = really_dominate(
							Lcoerce, 
							on1, 
							const_obj1
						    );
				    		    if(!tn) {
							suppress_error = 0;
							return 2;
						    }
						    else sn = tn;
						}
						Lcoerce = sn;
						Rcoerce = on2;
					
					}
					suppress_error = 0;
				}
					
			}
			else if (rt) {
				suppress_error = 1;
				int r1 = lt->kind(oper,0);
				int r2 = rt->kind(oper,0);
				if (np_promote(oper,r1,r2,lt,rt,1)!=any_type) {
					Pname sn = on1;
					if (Lcoerce) {
						Pname tn = really_dominate(
							Lcoerce, 
							on1, 
							const_obj1
						);
				    		if(!tn) {
							suppress_error = 0;
							return 2;
						}
						else sn = tn;
					}
					Lcoerce = sn;
				}
				suppress_error = 0;
			}
			else {
				Pname sn = on1;
				if (Lcoerce) {
					Pname tn = really_dominate(
						Lcoerce, 
						on1, 
						const_obj1
					);
				    	if(!tn) return 2;
					else sn = tn;
				}
				Lcoerce = sn;
			}
		}
	}
	else if (c2) {
//error('d',"c2 %n",c2);
		for (Pname on = c2->conv; on; on=on->n_list) {
			Pfct f = Pfct(on->tp);
			rt = f->base==FCT ? f->returns :
				Pfct(Pgen(on->tp)->fct_list->f->tp)->returns;
			Pname cn = rt->is_cl_obj();
//error('d',"cn %n",cn);
			if (cn && (Rcoerce==0 || Rcoerce->tp->check(f,0))) {
				Pclass cl = Pclass(cn->tp);
				Pname n = cl->has_oper(oper);
				if (n == 0) continue;

		//		while (n->base==REF || n->base==MDOT) n=Pname(n->mem);
				Pfct nf = Pfct(n->tp);
				if (nf->base == FCT) {
					if (nf->nargs == 0) {
						if (Lcoerce || Rcoerce) return 2;
						Rcoerce = on;
					}
				}
				else {
					for (Plist gl=Pgen(nf)->fct_list; gl; gl=gl->l) 
						if (Pfct(gl->f->tp)->nargs == 0) {
							if (Lcoerce || Rcoerce)	return 2;
							Rcoerce = on;
						}
				}
				continue;
			}
			//if (rt->is_cl_obj()) continue;
			if( lt ) {
				suppress_error = 1;
				int r1 = lt->kind(oper,0);
				int r2 = rt->kind(oper,0);

				if (np_promote(oper,r1,r2,lt,rt,1)!=any_type) {
					Pname sn = on;
					if (Lcoerce || Rcoerce) {
						Pname tn = really_dominate(
							Rcoerce, 
							on, 
							const_obj2
						);
				    		if(!tn) {
							suppress_error = 0;
							return 2;
						}
						else sn = tn;
					}
					Rcoerce = sn;
				}
				suppress_error = 0;
			}
		}
	}

//error('d',"->%d || %d",Lcoerce,Rcoerce);
	return (Lcoerce || Rcoerce);
}

Pexpr expr::try_to_overload(Ptable tbl)
{
//	TOK bb = (base==DEREF && e2==0) ? MUL : base;
//error('d',"try_to_overload %k %d",base,base);

	Pname n1 = 0;
	Ptype t1 = 0;
	const_obj1 = 0;
	const_obj2 = 0;

	if (e1) {
		t1 = e1->tp;
		Ptype tpx = t1;
		while (tpx->base == TYPE) tpx = Pbase(tpx)->b_name->tp;
		n1 = tpx->is_cl_obj();
		const_obj1 = t1->tconst();

		Pexpr ee = e1;
                while (ee && (ee->base==DOT || ee->base==REF)) {
                       	Pexpr m = ee->mem;
                       	if ( ee->base==REF && m->tp &&  m->tp->is_ptr())
                               	break;
                       	ee = ee->e1;
                }
                if (ee) {
			int tc;
                       	Ptype ttt = ee->tp;
                       	switch (e1->base) {
                       	case REF:
                       	        Pptr p = ttt?ttt->is_ptr():0;
                               	if (p && p->typ->tconst())
                                       	const_obj1 = 1;
                               	break;
                       	case DOT:
				tc = ttt ? ttt->tconst() : 0;
                               	if (ttt && tc && (!strict_opt || tc!=2))
                                       	const_obj1 = 1;
                       	}
                }
	}

	TOK bb = base;
	switch (bb) {
	case DEREF:
		if (e2 == 0) bb = MUL;
		// no break;
	case CALL:
	case G_CALL:
		if (n1 == 0) return 0;	// ignore type of argument list
	}

	Pname n2 = 0;
	Ptype t2 = 0;

	if (e2 && e2->base!=ELIST) {
		t2 = e2->tp;
		Ptype tpx = t2;
		while (tpx->base == TYPE) tpx = Pbase(tpx)->b_name->tp;
		n2 = tpx->is_cl_obj();
		const_obj2 = t2->tconst();
		Pexpr ee = e2;
                while (ee && (ee->base==DOT || ee->base==REF)) {
                       	Pexpr m = ee->mem;
                       	if ( ee->base==REF && m->tp &&  m->tp->is_ptr())
                               	break;
                       	ee = ee->e1;
                }
                if (ee) {
			int tc;
                       	Ptype ttt = ee->tp;
                       	switch (e2->base) {
                       	case REF:
                       	        Pptr p = ttt?ttt->is_ptr():0;
                               	if (p && p->typ->tconst())
                                       	const_obj2 = 1;
                               	break;
                       	case DOT:
				tc = ttt ? ttt->tconst() : 0;
                               	if (ttt && tc && (!strict_opt || tc!=2))
                                       	const_obj2 = 1;
                       	}
                }
	}

	if (n1==0 && n2==0) return 0;
	if (n1 && n1->tp == 0) return 0;	// make_assign() fudge
// error('d',"e1: %k e2: %k", e1?e1->base:0, e2?e2->base:0 );
// error('d',"t1 %t t2 %t",t1,t2);
// error('d',"n1 %n n2 %n",n1,n2);
	/* first try for non-member function:	op(e1,e2) or op(e2) or op(e1) */
	Pexpr oe2 = e2;
	Pexpr ee2 = (e2 && e2->base!=ELIST) ? e2 = new expr(ELIST,e2,0) : 0;
	Pexpr ee1 = e1 ? new expr(ELIST,e1,e2) : ee2;
	char* obb = oper_name(bb);
	Pname gname = tbl->look(obb,0);
	// if necessary check for ambiguities
	int go = gname ? over_call(gname,ee1) : 0;
//error('d',"go %d",go);
	if (go) gname = Nover;

	if (n1) {
		if (bb == ASSIGN) {
			Pclass c1 = Pclass(n1->tp);
//error('d',"look %k %d",bb,c1->memtbl->look(obb,0));
			if (c1->memtbl->look(obb,0)==0) {
				Pclass bcl = c1->baselist?c1->baselist->bclass:0;
				if (n2==0
				|| (Pclass(n2->tp)!=c1
					&& Pclass(n2->tp)->has_base(c1)==0)) {
					// if legal, a=1 can be optimized to a.ctor(1)
						if (2 < go) goto glob;
						return 0;
					}

				if (bcl
				&& c1->obj_size!=bcl->obj_size
				&& bcl->memtbl->look(obb,0)) {
					// cannot inherit from smaller base class
				//	make_assignment(n1);
				//	return try_to_overload(tbl);
					goto mkas;
				}

				if (c1->c_xref&(C_VBASE|C_VPTR|C_ASS)) {
					// make operator=() if
					//	no base (shouldn't happen
					//	different (smaller) sized base
					//	two bases
				mkas:
					if (2 < go) goto glob;
				//	make_assignment(n1);
				//	return try_to_overload(tbl);
					return make_assignment(n1) ? try_to_overload(tbl) : 0;
				}
// error('d',"n2 %n",n2);
			//	if (n2 && Pclass(n2->tp)==c1)
				return 0;
			}
			// now take care of other assignments, 
		}

		int dbconv = 0;
		Pclass ccl = Pclass(n1->tp);
		// Pexpr mn = Pclass(n1->tp)->memtbl->look(obb,0);
		Pexpr mn = ccl->memtbl->look(obb,0);
// error('d', "tcl %d %t cl %d %t", tcl, tcl, ccl, ccl);
  		if(strcmp(obb,"__as")) {
			tcl = ccl; // ugh!!! 
			if(!mn) dbconv = 2;
		}
//		tcl = ccl; // ugh!!! 
		mn = ccl->find_name(obb,0);

		Pname mname = Pname(mn);
// error('d',"mn %n %d %k %s",mname,mn,mn?mn->base:0,obb);
		if (mname == 0) goto glob;

	zaq:
		switch (mname->base) {
		case REF:
		case MDOT:
			mname = Pname(Pexpr(mname)->mem);
			goto zaq;
		}

		int mo = over_call(mname,e2);
		if(mo && dbconv && mo >= dbconv) mo = dbconv;
//error('d',"mo %d (go %d)",mo,go);
		if (mo==0 || mo<go)
			goto glob;
		else if (mo && mo==go) {
//error('d',"t1 %t t2 %t",t1,t2);
			if (gname->tp->base == OVERLOAD) { // find right version
				for (Plist l = Pgen(gname->tp)->fct_list; l; l=l->l) {
					Pname n = l->f;
					int x = over_call(n,ee1);
					if (x == go) {
						gname = n;
						break;
					}
				}
			}
//error('d',"gname %n: %t",gname,gname->tp);
			Pname aa = Pfct(gname->tp)->argtype;
			Pptr p;
			Ptype gt1 = aa->tp;
			if (p = gt1->is_ref()) gt1 = p->typ;
			Ptype gt2 = aa->n_list->tp;
//error('d',"gt1 %t gt2 %t",gt1,gt2);
			if (mname->tp->base == OVERLOAD) { // find right version
				for (Plist l = Pgen(mname->tp)->fct_list; l; l=l->l) {
					Pname n = l->f;
					int x = over_call(n,e2);
					if (x == mo) {
						mname = n;
						break;
					}
				}
			}
//error('d',"mname %n: %t",mname,mname->tp);
			Ptype mt1 = Pfct(mname->tp)->f_this->tp;
			mt1 = Pptr(mt1)->typ;
			Ptype mt2 = Pfct(mname->tp)->argtype->tp;
//error('d',"mt1 %t mt2 %t",mt1,mt2);
			Pname mm = new name;
			Pname a1 = new name;
			a1->tp = mt1;
			Pname a2 = new name;
			a2->tp = mt2;
			a1->n_list = a2;
			mm->tp = new fct(void_type,a1,2);
			Pname gg = new name;
			Pname a3 = new name;
			a3->tp = gt1;
			Pname a4 = new name;
			a4->tp = gt2;
			a3->n_list = a4;
			gg->tp = new fct(void_type,a3,1);
extern Pname dominate(Pname,Pname,Pexpr,int,int);
			aa = dominate(gg,mm,ee1,0,1);
			delete a1;
			delete a2;
			delete a3;
			delete a4;
			delete gg->tp;
			delete mm->tp;
			if (aa == 0) {
				delete gg;
				delete mm;
				error("ambiguous operandTs%n and%t for%k",n1,t2,bb);
				tp = any_type;
				return this;
			}
			else if (aa == gg) {
				delete gg;
				delete mm;
				goto glob;
			}
			delete gg;
			delete mm;
		}
		else if (mo < 2) {	// user-defined conversion user

			if (try_to_demote(bb,t1,t2))
				error("ambiguous use of overloaded%k",bb);
		}

		base = G_CALL;				// e1.op(e2) or e1.op()
		Pname xx = new name(mname->string);	// do another lookup
							// . suppresses virtual
		e1 = new ref(DOT,e1,xx);
		if (ee1) delete ee1;
		return typ(tbl);
	}
	
	if (n2 && e1==0) {			/* look for unary operator */
		suppress_error++;
		Pexpr mn = Pclass(n2->tp)->find_name(obb,0);
		suppress_error--;
		Pname mname = Pname(mn);
		if (mname == 0) goto glob;
	zaqq:
		switch (mname->base) {
		case REF:
		case MDOT:
			mname = Pname(Pexpr(mname)->mem);
			goto zaqq;
		}	

		switch (mname->n_scope) {
		default:	goto glob;
		case 0:
		case PUBLIC:	break;		// try e2.op()
		}
		
		int mo = over_call(mname,0);
//error('d',"e2 mo %d (go %d)",mo,go);
		if (mo==0 || mo<go)
			goto glob;
		else if (mo==go) {
			error("ambiguous operandT%n for%k",n2,bb);
			tp = any_type;
			return this;
		}
		else if (mo < 2) {	// user-defined conversion user
			if (try_to_demote(bb,t1,t2))
				error("ambiguous use of overloaded%k",bb);
		}
		base = G_CALL;				// e2.op()
		Pname xx = new name(Nover->string);	// do another lookup
							// . suppresses virtual
		e1 = new ref(DOT,oe2,xx);
		e2 = 0;
		if (ee2) delete ee2;
		if (ee1 && ee1!=ee2) delete ee1;
		return typ(tbl);
	}
	
glob:
//error('d',"glob %d",go);

	if (go) {
		if (go < 2) {	// user-defined conversion necessary => binary
			if (try_to_demote(bb,t1,t2))
				error("ambiguous use of overloaded%k: %t and %t",bb,t1,t2);
		}

		base = gname->n_table == gtbl ? G_CALL : CALL;
//error('d',"gname %n %t",gname,gname->tp);
		e1 = new name(gname->string);
		// if global scope, look only for globals
		if(gname->n_table == gtbl) Pname(e1)->n_qualifier = sta_name;
		e2 = ee1;
		return typ(tbl);
	}

	if (ee2) delete ee2;
	if (ee1 && ee1!=ee2) delete ee1;
	e2 = oe2;

//error('d',"bb %d %k",bb,bb);
	switch (bb) {
	case CM:
	case G_CM:
	case G_ADDROF:
		return 0;
	case ASSIGN:
		if (n1
		&& n2
		&& (n1->tp==n2->tp || Pclass(n2->tp)->has_base(Pclass(n1->tp)))) {
			if (make_assignment(n1))
				return try_to_overload(tbl);
			else
				return 0;
		}
	case DEREF:
	case CALL:
		if (n1 == 0) break;

	default:	/* look for conversions to basic types */
		if (n1
		&& Pclass(n1->tp)->conv
		&& (bb==ANDAND || bb==OROR)) {
			e1 = check_cond(e1,bb,tbl);
			return 0;
		}

		if (n2
		&& Pclass(n2->tp)->conv
		&& (bb==ANDAND || bb==OROR || bb==NOT)) {
			e2 = check_cond(e2,bb,tbl);
			return 0;
		}

// error( 'd', "bb: %k t1: %k t2: %k", bb, t1?t1->base:0, t2?t2->base:0 );
		switch (try_to_demote(bb,t1,t2)) {
		default:
			if (Lcoerce) error("ambiguous conversion of%n",n1);
			if (Rcoerce) error("ambiguous conversion of%n",n2);
		case 0:
			break;
		case 1:
			if (Lcoerce) {
				Pname xx = new name(Lcoerce->string);
				Pref r = new ref(DOT,e1,xx);
				e1 = new expr(G_CALL,r,0);
			}

			if (Rcoerce) {
				Pname xx = new name(Rcoerce->string);
				Pref r = new ref(DOT,e2,xx);
				e2 = new expr(G_CALL,r,0);
			}
			return typ(tbl);
		}

		switch (bb) {
		case CM:
		case ADDROF:	// has legal built-in meaning
			return 0;
		}

		if (t1 && t2)
			error("bad operandTs%t%t for%k",t1,t2,bb);
		else
			error("bad operandT%t for%k",t1?t1:t2,bb);

		tp = any_type;
		return this;
	}

	return 0;
}

Pexpr cast_cptr(Pclass ccl, Pexpr ee, Ptable tbl, int real_cast)
/*
	"ee" is being cast to pointer object of class "ccl"
	if necessary modify "ee"
*/
{

//	Ptype etp = ee->tp;
// error('d',"cast_cptr %k ccl %t ee->tp %t",ee->tp->base,ccl,ee->tp);
//	if (etp->base!=PTR && etp->base!=RPTR) return ee;
	Ptype etp = ee->tp->is_ptr_or_ref();
	if (etp == 0) return ee;

	Pname on = Pptr(etp)->typ->is_cl_obj();
	if (on == 0) return ee;

	Pclass ocl = Pclass(on->tp);
	if (ocl==ccl || ccl==0 || ocl==0) return ee;

// error('d',"cast_cptr %t(%t) real %d",ccl,ocl,real_cast);
	int oo = 0;
	Pexpr r = 0;

	if (ocl->baselist
	&& (ocl->baselist->bclass!=ccl || ocl->baselist->base!=NAME)) {
		// casting derived to second or virtual base?
		Nptr = 0;
		Nvis = 0; 
		Nalloc_base = 0;
		vcllist->clear();
		vcllist=0;
		int x = is_unique_base(ocl,ccl->string,0);
		if (Nvis) {
			if (real_cast==0)
				error("cast:%n* ->B%t*; privateBC",on,ccl);
			else if (warning_opt)
				error('w',"cast:%n* ->B%t*; privateBC",on,ccl);
			real_cast = 1;	// suppress further error mesages
			Nvis = 0;
		}

		switch (x) {
		default:
			error("cast:%n* ->B%t*;%t isB more than once",on,ccl,ccl);
		case 0:		// unrelated;
			break;
		case 1:
			oo = Noffset;
			break;
		}

		if (Nptr) {	// => ee?Nptr:0
                        if (ocl->c_body==1) ocl->dcl_print(0);
                        Nptr->mem = ee; // ee->Pbase_class
			if ( Nalloc_base ) {
// error('d', "cast_cptr: nalloc_base: %s", Nalloc_base);
				Nptr->i1 = 5;
				Nptr->string4 = new char[strlen(Nalloc_base)];
				strcpy(Nptr->string4,Nalloc_base);
				Nalloc_base = 0;
			}
			else Nptr->i1 = 3;

                        if (ee->base==ADDROF || ee->base==G_ADDROF)   
                                ee = Nptr;
                        else {
				Pexpr p = new expr(QUEST,Nptr,zero);
				nin = 1;
				if (ee->not_simple()) {	// need temp
					Ptype t = ee->tp;
					Pname pp = make_tmp('N',t,tbl);
					Pname(pp)->n_assigned_to = 1;
					ee = new expr(ASSIGN,pp,ee);
					ee->tp = t;
					Nptr->mem = pp;
				}
				nin = 0;
				p->cond = ee;
				p->tp = ee->tp;
                                ee = p;
			}
		}			
	}

	if (ccl->baselist
	&& (ccl->baselist->bclass!=ocl || ccl->baselist->base!=NAME)) {
		// casting second or virtual base to derived?
		Nptr = 0;
		vcllist->clear();
		vcllist=0;
		int x = is_unique_base(ccl,ocl->string,0);
		switch (x) {
		default:
			error("cast:%n* ->derived%t*;%n isB more than once",on,ccl,on);
		case 0:		// unrelated;
			break;
		case 1:
			oo = -Noffset;
                        if (Nptr)
                                error("cast:%n* ->derived%t*;%n is virtualB",on,ccl,on);
                        break;
		}
		Nvis = 0;	// visibility no concern when converting from base to derived
	}
// error('d',"oo %d ee %k",oo,ee->base);
	if (oo) {	// => ee?ee+offset:0
                if (ee->base==ADDROF || ee->base==G_ADDROF)
                        ee = rptr(ee->tp,ee,oo);
                else {
			Pexpr p;
			nin = 1;
			if (ee->not_simple()) {	// need temp
				Ptype t = ee->base==MDOT?ee->mem->tp:ee->tp;
				Pname pp = make_tmp('M',t,tbl);
				Pname(pp)->n_assigned_to = 1;
				ee = new expr(ASSIGN,pp,ee);
				ee->tp = t;
				p = rptr(t,pp,oo);
			}
			else
				p = rptr(ee->base==MDOT?ee->mem->tp:ee->tp,ee,oo);
			nin = 0;
			Pexpr pp = new expr(QUEST,p,zero);
			pp->tp = ee->tp;
			pp->cond = ee;
                        ee = pp;
		}
	}

	Nvis = 0; // Nvis set by has_base()
	if (ocl->has_base(ccl) && Nvis) {
		if (real_cast==0)
			error("cast:%n* ->B%t*; privateBC",on,ccl);
		else if (warning_opt)
			error('w',"cast:%n* ->B%t*; privateBC",on,ccl);
		Nvis = 0;
	}

// error('d',"return %d %k %t",ee,ee->base,ee->tp);
	return ee;
}

Pexpr expr::donew(Ptable tbl)
{
	Ptype tt = tp2;
	Ptype tpx = tt;
	bit v = 0;
	bit old = new_type;
	int init = 0;	// non-constructor initialization
	new_type = 1;

	tt->dcl(tbl);
	new_type = old;
//error('d',"donew %d %d (%k) tt %t",e1,e2,e2?e2->base:0,tt);
	if (e1) e1 = e1->typ(tbl);
	if (e2) e2 = e2->typ(tbl);
ll:
//error('d',"ll %d",tt->base);
	switch (tt->base) {
	default:
		if ( e1) {
			if (v) {
				error("Ir for array created using \"new\"");
				break;
			}
			init = 1;
		} 
	//	if (e1) {
	//		error("Ir for nonCO created using \"new\"");
	//		e1 = 0;
	//	}
		break;
	case VEC:
		if (v && Pvec(tt)->dim) error("only 1st array dimension can be non-constant");
		if (Pvec(tt)->size==0 && Pvec(tt)->dim==0) error("array dimension missing in `new'");
	//	if (Pvec(tt)->dim==zero) {
	//		Pvec(tt)->size = 0;
	//		Pvec(tt)->dim = 0;
	//	}
		v++;
		tt = Pvec(tt)->typ;
		goto ll;
	case TYPE:
		tt = Pbase(tt)->b_name->tp;
		goto ll;
	case VOID:
		error("badT for `new': void");
		break;
	case COBJ:
	{	Pname cn = Pbase(tt)->b_name;
		Pclass cl = Pclass(cn->tp);
		Pname icn = 0;

		if ( e1 ) { // arguments
			if ( e1->e2 == 0 && e1->base == ELIST ) {
				e1 = e1->e1;
				e1 = e1->typ(tbl);
			}
			icn = (e1->base!=ELIST)?e1->tp->is_cl_obj():0;
		}
		//Pname icn = (e1 && e1->base!=ELIST)?e1->tp->is_cl_obj() : 0;

		Pclass icl = icn ? Pclass(icn->tp) : 0;

		if (cl->c_abstract) {
			error("`new' of abstractC%t",cl);
			break;
		}

		if (v && e1) {
			error("Ir for array ofCO created using \"new\"");
			break;
		}

		if ((cl->defined&(DEFINED|SIMPLIFIED)) == 0) {
			error("new%n;%n isU",cn,cn);
			break;
		}

		Pname ctor = cl->has_ctor();

		if (ctor) {
			if (v) {
				Pname ic;
				if ((ic = cl->has_ictor())==0) {
					error("array ofC%n that does not have aK taking noAs",cn);
					break;
				}
					
				if (Pfct(ic->tp)->nargs) {
					error("defaultAs forK for array ofC%n",cn);
					break;
				}
			}

			if (icl
			&& cl->has_itor()==0	// incomplete:
						// what if X(Y&) exists
						// for class Y : X ?
			&& (icl==cl || icl->has_base(cl))) {
				init = 1;
				break;
			}
			e1 = call_ctor(tbl,0,ctor,e1);
		}
		else if (e1) {
			if (icl==cl || icl->has_base(cl)) 
				init = 1;
			else
				error("new%n(As ); %n does not have aK",cn,cn);
		}
	}
	}

	if (init) {
		Pname tmp = make_tmp('N',tt->addrof(),tbl);
		e1 = e1->typ(tbl);
		if (tt->check(e1->tp,ASSIGN))
			error("badIrT %t for new operator (%t X)",e1->tp,tt);
		e1 = new expr(0,tmp,e1);
		tmp->assign();
	}

//	tp = (v) ? tpx : tpx->addrof();
	switch (v) {
		case 0:
			tp = tpx->addrof();
			break;
		case 1:
			tp = tpx;
			break;
		default:
			tp = tpx;
	}
//error('d',"donew(%d) -> %t",v,tp);
	return this;
}

static is_dataMemPtr( Pexpr ee ) 
/* this is utterly implementation dependent 
 * called by expr::lval to determine 
 * const objects bounds to pointers to data members 
 */
{
	Pexpr te = ee->e1;
	if ( te == 0 ) return 0;
	if ( te->base != PLUS ) return 0;
	if ( (te = te->e2) == 0 ) return 0;
	if ( te->base != MINUS ) return 0;
	if ( (te = te->e1) == 0 ) return 0;
	if ( te->base != CAST ) return 0;
	if ( (te = te->e1) == 0 ) return 0;
	if ( te->tp->base != PTR ) return 0;
        if ( Pptr(te->tp)->memof == 0 ) return 0;
	return 1;
}
